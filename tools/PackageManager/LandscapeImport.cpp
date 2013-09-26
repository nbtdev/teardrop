/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "AssetImport.h"
#include "Metadata.h"
#include "PackageManager.h"
#include "ImportedAsset.h"
#include "Asset/LandscapeAsset.h"
#include "Asset/TextureAsset.h"
#include "Asset/HeightfieldAsset.h"
#include "Util/StringUtil.h"
#include "Util/FileSystem.h"
#include "FreeImage.h"
#include "tinyxml/tinyxml.h"
#include "ThirdParty/LibHFZ/wrapper.h"
#include <tbb/task.h>
#include <tbb/concurrent_vector.h>

namespace Teardrop {
	namespace Tools {

		static void readTexture(TiXmlElement* texture, LandscapeAsset* asset) 
		{
			TiXmlElement* textureParam = texture->FirstChildElement();
			while (textureParam) {
				const char* type = textureParam->Value();
				const char* paramName = textureParam->Attribute("name");
				const char* paramValue = textureParam->GetText();

				if (type && paramName && paramValue) {
					if (!strcmp(type, "int")) {
						if (!strcmp(paramName, "nx")) {
							int nx;
							StringUtil::fromString(paramValue, nx);
							asset->setColormapX(nx);
						}
						if (!strcmp(paramName, "ny")) {
							int ny;
							StringUtil::fromString(paramValue, ny);
							asset->setColormapY(ny);
						}
					}
				}

				textureParam = textureParam->NextSiblingElement();
			}
		}

		static void readTerrain(TiXmlElement* terrain, LandscapeAsset* asset) 
		{
			TiXmlElement* terrainParam = terrain->FirstChildElement();
			while (terrainParam) {
				const char* type = terrainParam->Value();
				const char* paramName = terrainParam->Attribute("name");
				const char* paramValue = terrainParam->GetText();

				if (type && paramName && paramValue) {
					if (!strcmp(type, "int")) {
						if (!strcmp(paramName, "nx")) {
							int nx;
							StringUtil::fromString(paramValue, nx);
							asset->setHeightFieldX(nx);
						}
						if (!strcmp(paramName, "ny")) {
							int ny;
							StringUtil::fromString(paramValue, ny);
							asset->setHeightFieldY(ny);
						}
					}
					else if (!strcmp(type, "float")) {
						if (!strcmp(paramName, "MinAlt")) {
							float MinAlt;
							StringUtil::fromString(paramValue, MinAlt);
							asset->setMinAltitude(MinAlt);
						}
						if (!strcmp(paramName, "MaxAlt")) {
							float MaxAlt;
							StringUtil::fromString(paramValue, MaxAlt);
							asset->setMaxAltitude(MaxAlt);
						}
						if (!strcmp(paramName, "HorizScale")) {
							float HorizScale;
							StringUtil::fromString(paramValue, HorizScale);
							asset->setScale(HorizScale);
						}
					}
				}

				terrainParam = terrainParam->NextSiblingElement();
			}
		}

		static void readMapInfo(TiXmlElement* mapInfo, LandscapeAsset* asset) 
		{
			TiXmlElement* mapInfoChild = mapInfo->FirstChildElement("varlist");
			while (mapInfoChild) {
				const char* name = mapInfoChild->Attribute("name");
				if (!strcmp(name, "Terrain")) {
					readTerrain(mapInfoChild, asset);
				}
				else if (!strcmp(name, "Texture")) {
					readTexture(mapInfoChild, asset);
				}
				mapInfoChild = mapInfoChild->NextSiblingElement("varlist");
			}
		}

		class DummyTask : public tbb::task
		{
		public:
			DummyTask() {}
			tbb::task* execute() { return NULL; }
		};

		struct DepInfo
		{
			String mPathname;
			String mDepname;
			TextureAsset* mAsset;
			
			DepInfo(TextureAsset* texAsset, const String& pathname, const String& depName) : mAsset(texAsset), mPathname(pathname), mDepname(depName) {}
			DepInfo(const DepInfo& other) { *this = other; }
			DepInfo& operator=(const DepInfo& other) { mPathname = other.mPathname; mDepname = other.mDepname; mAsset = other.mAsset; return *this; }
		};

		struct ImportTask : public tbb::task
		{
			LandscapeAsset* mAsset;
			String mPathname;
			String mDepName;
			TextureAssetType mTexType;
			typedef void(LandscapeAsset::*PFN)(TextureAsset*);
			PFN mFP;
			tbb::concurrent_vector<DepInfo>& mDeps;

			tbb::task* execute() {
				TextureAsset* tex = importTexture(mPathname, mTexType);
				(mAsset->*mFP)(tex);
				mDeps.push_back(DepInfo(tex, mPathname, mDepName));
				return NULL;
			}

			ImportTask(tbb::concurrent_vector<DepInfo>& deps, const String& depName) : mDeps(deps), mDepName(depName) {}
		};

		static void readMaps(TiXmlElement* maps, LandscapeAsset* asset, const String& filedir, ImportedAsset& imp) 
		{
			// import all map dependencies in parallel
			tbb::task* dummy = new(tbb::task::allocate_root()) DummyTask;
			dummy->set_ref_count(1);
			tbb::concurrent_vector<DepInfo> deps;

			TiXmlElement* mapsChild = maps->FirstChildElement("varlist");
			while (mapsChild) {
				const char* mapName = mapsChild->Attribute("name");
				TiXmlElement* mapParam = mapsChild->FirstChildElement("string");

				const char* filename = 0;
				const char* mapType = 0;

				while (mapParam) {
					const char* name = mapParam->Attribute("name");
					const char* text = mapParam->GetText();

					if (name && !strcmp(name, "Filename")) {
						filename = text;
					}

					if (name && !strcmp(name, "MapType")) {
						mapType = text;
					}

					if (filename && mapType) {
						// we don't care about the design map, so process only if map is not DM
						if (strcmp(mapName, "DM")) {
							String filepath(filedir);
							filepath.append(FileSystem::PATHSEP);
							filepath.append(filename);

							// heightfield has to be uncompressed first...and then stays uncompressed
							if (!strcmp(mapName, "HF")) {
								// libHFZ to read this one
								float* data = hfzLoadHeightfield(filepath);
								if (data) {
									HeightfieldAsset* tex = new HeightfieldAsset;
									int w = asset->getHeightFieldX();
									int h = asset->getHeightFieldY();
									tex->setWidth(w);
									tex->setHeight(h);
									int nBytes = sizeof(float) * w * h;
									void* hfData = tex->createData(nBytes);
									memcpy(hfData, data, nBytes);
									asset->setHeightField(tex);
									deps.push_back(DepInfo(tex, filepath, "HeightField"));

									hfzFreeHeightfieldData(data);
								}
							}

							// water map
							//if (!strcmp(mapName, "WM")) {
							//	TextureAsset* tex = importTexture(filepath, TEXTUREASSET_TYPE_BCX);
							//	asset->setWaterMap(tex);
							//	imp.addDep(tex);
							//}

							// attr map isn't really an image, it's a pair of shorts per HF sample
							//if (!strcmp(mapName, "AM")) {
							//	TextureAsset* tex = importTexture(filepath, TEXTUREASSET_TYPE_UNCOMPRESSED);
							//	asset->setAttributesMap(tex);
							//	imp.addDep(tex);
							//}

							// terrain normal map
							if (!strcmp(mapName, "TN")) {
								ImportTask* t = new(dummy->allocate_child()) ImportTask(deps, "Normal Map");
								t->mTexType = TEXTUREASSET_TYPE_BCX;
								t->mPathname = filepath;
								t->mAsset = asset;
								t->mFP = &LandscapeAsset::setNormalMap;
								dummy->increment_ref_count();
								dummy->spawn(*t);
							}

							// terrain shadow map, 8 bits (1 byte) per sample
							if (!strcmp(mapName, "SH")) {
								ImportTask* t = new(dummy->allocate_child()) ImportTask(deps, "Shadow Map");
								t->mTexType = TEXTUREASSET_TYPE_BCX;
								t->mPathname = filepath;
								t->mAsset = asset;
								t->mFP = &LandscapeAsset::setShadowMap;
								dummy->increment_ref_count();
								dummy->spawn(*t);
							}

							// terrain light map - raw RGB, same dimensions as heightfield
							if (!strcmp(mapName, "LM")) {
								ImportTask* t = new(dummy->allocate_child()) ImportTask(deps, "Light Map");
								t->mTexType = TEXTUREASSET_TYPE_BCX;
								t->mPathname = filepath;
								t->mAsset = asset;
								t->mFP = &LandscapeAsset::setLightMap;
								dummy->increment_ref_count();
								dummy->spawn(*t);
							}

							// terrain specular light map
							if (!strcmp(mapName, "SpecLM")) {
								ImportTask* t = new(dummy->allocate_child()) ImportTask(deps, "Specular Light Map");
								t->mTexType = TEXTUREASSET_TYPE_BCX;
								t->mPathname = filepath;
								t->mAsset = asset;
								t->mFP = &LandscapeAsset::setSpecularLightMap;
								dummy->increment_ref_count();
								dummy->spawn(*t);
							}

							// terrain color map
							if (!strcmp(mapName, "TX")) {
								ImportTask* t = new(dummy->allocate_child()) ImportTask(deps, "Texture Map");
								t->mTexType = TEXTUREASSET_TYPE_BCX;
								t->mPathname = filepath;
								t->mAsset = asset;
								t->mFP = &LandscapeAsset::setDiffuseMap;
								dummy->increment_ref_count();
								dummy->spawn(*t);
							}
						}
					}

					mapParam = mapParam->NextSiblingElement("string");
				}

				mapsChild = mapsChild->NextSiblingElement("varlist");
			}

			dummy->wait_for_all();

			// collect all dependencies into our return value
			delete [] imp.mDeps;
			delete [] imp.mDepsMetadata;
			delete [] imp.mDepsMetaName;
			delete [] imp.mDepsFilepath;

			imp.mDeps = new Asset*[deps.size()];
			imp.mDepsMetadata = new Metadata*[deps.size()];
			imp.mDepsMetaName = new String[deps.size()];
			imp.mDepsFilepath = new String[deps.size()];
			imp.mNumDeps = deps.size();

			for (int i=0; i<imp.mNumDeps; ++i) {
				imp.mDeps[i] = deps[i].mAsset;
				imp.mDepsFilepath[i] = deps[i].mPathname;
				imp.mDepsMetaName[i] = deps[i].mDepname;
			}
		}

		LandscapeAsset* importLandscape(ImportedAsset& imp, const char* filepath, LandscapeAssetType type)
		{
			LandscapeAsset* asset = 0;

			if (!filepath)
				return 0;

			String pathname(filepath);
			String filedir;
			FileSystem::directoryName(filedir, filepath);

			if (type == LANDSCAPEASSET_TYPE_L3DT) {
				TiXmlDocument doc;
				if (doc.LoadFile(filepath)) {
					TiXmlElement* root = doc.RootElement();

					// sanity checks
					const char* elemName = root->Value();
					const char* nameAttr = root->Attribute("name");

					if (elemName && strcmp(elemName, "varlist")) 
						return 0;
					if (nameAttr && strcmp(nameAttr, "ProjectData")) 
						return 0;

					// we have an L3DT project file (more than likely...)
					asset = new LandscapeAsset;

					TiXmlElement* varlist = root->FirstChildElement("varlist");
					while (varlist) {
						const char* name = varlist->Attribute("name");
						if (name && !strcmp(name, "MapInfo")) {
							readMapInfo(varlist, asset);
						}

						if (name && !strcmp(name, "Maps")) {
							readMaps(varlist, asset, filedir, imp);
						}

						varlist = varlist->NextSiblingElement("varlist");
					}
				}
			}

			return asset;
		}

	} // namespace Tools
} // namespace Teardrop

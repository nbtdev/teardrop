/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "MaterialForm.h"
#include "CoreInterface.h"
#include "MessageBus.h"
#include "UI/UserInterface.h"
#include "SceneEditorFrame.h"
#include "Game/TerrainZone.h"
#include "Game/ZoneObject.h"
#include "Game/Component_Render.h"
#include "Gfx/GfxMeshInstance.h"
#include "Gfx/GfxMesh.h"
#include "Gfx/GfxSubMesh.h"
#include "Gfx/GfxMaterial.h"
#include "Gfx/GfxShader.h"
#include "Gfx/GfxRenderer.h"
#include "Stream/FileStream.h"
#include "Util/Environment.h"
#include "wx/wxScintilla.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
static const char* keywords = "asm asm_fragment bool bool2 bool3 bool4 bool1x1 bool1x2 bool1x3 bool1x4 bool2x1 bool2x2 \
		   bool2x3 bool2x4 bool3x1 bool3x2 bool3x3 bool3x4 bool4x1 bool4x2 bool4x3 bool4x4 \
		   int int2 int3 int4 int1x1 int1x2 int1x3 int1x4 int2x1 int2x2 \
		   int2x3 int2x4 int3x1 int3x2 int3x3 int3x4 int4x1 int4x2 int4x3 int4x4  \
		   float float2 float3 float4 float1x1 float1x2 float1x3 float1x4 float2x1 float2x2 \
		   float2x3 float2x4 float3x1 float3x2 float3x3 float3x4 float4x1 float4x2 float4x3 float4x4  \
		   half half2 half3 half4 half1x1 half1x2 half1x3 half1x4 half2x1 half2x2 \
		   half2x3 half2x4 half3x1 half3x2 half3x3 half3x4 half4x1 half4x2 half4x3 half4x4  \
		   double double2 double3 double4 double1x1 double1x2 double1x3 double1x4 double2x1 double2x2 \
		   double2x3 double2x4 double3x1 double3x2 double3x3 double3x4 double4x1 double4x2 double4x3 double4x4  \
		   class column_major compile \
		   compile_fragment const discard decl do \
		   else extern false float \
		   for if in inline \
		   inout int matrix out pass \
		   pixelfragment return register row_major sampler \
		   sampler1D sampler2D sampler3D samplerCUBE sampler_state \
		   shared stateblock stateblock_state static string \
		   struct technique technique10 technique11 texture texture1D texture2D \
		   texture3D textureCUBE true typedef uniform \
		   vector vertexfragment void volatile while \
		   VertexShader PixelShader compile";
//---------------------------------------------------------------------------
MaterialForm::MaterialForm() 
: MaterialFormFrame(0)
{
	m_pSourceEdit = new wxScintilla(m_editorPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxALL|wxEXPAND);
	wxFont font(
		10, 
		wxFONTFAMILY_MODERN, 
		wxFONTSTYLE_NORMAL, 
		wxFONTWEIGHT_NORMAL, 
		false,
		wxT("Courier New")
		);

	m_pSourceEdit->StyleSetFont(wxSCI_STYLE_DEFAULT, font);
	m_pSourceEdit->SetLexer(wxSCI_LEX_CPP);
	m_pSourceEdit->SetKeyWords(0, keywords);
	m_pSourceEdit->StyleSetForeground(wxSCI_C_COMMENTLINE,	wxColour(0,		0x80,   0));
	m_pSourceEdit->StyleSetForeground(wxSCI_C_COMMENT,		wxColour(0,		0x80,   0));
	m_pSourceEdit->StyleSetForeground(wxSCI_C_COMMENTDOC,	wxColour(0,		0x80,   0));
	m_pSourceEdit->StyleSetForeground(wxSCI_C_NUMBER,		wxColour(0x80,  0,		0));
	m_pSourceEdit->StyleSetForeground(wxSCI_C_STRING,		wxColour(0x80,  0,		0x80));
	m_pSourceEdit->StyleSetForeground(wxSCI_C_WORD,			wxColour(0,		0,		0xff));
	//m_pSourceEdit->StyleSetForeground(wxSCI_C_OPERATOR,		wxColour(0xf0,	0x50,	0x50));
	m_pSourceEdit->StyleSetForeground(wxSCI_C_PREPROCESSOR,	wxColour(0x30,	0,		0));
	m_pSourceEdit->SetTabWidth(4);
	m_pSourceEdit->SetIndent(4);
	m_pSourceEdit->SetWrapMode(wxSCI_WRAP_WORD);
	m_pSourceEdit->SetMarginType(0, wxSCI_MARGIN_NUMBER);
	m_pSourceEdit->SetMarginWidth(0, m_pSourceEdit->TextWidth(wxSCI_STYLE_LINENUMBER, wxT("_99999")));

	this->Connect( wxEVT_SIZE, wxSizeEventHandler( MaterialForm::OnFormResize ) );
	this->Connect( wxID_CMD_APPLY, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MaterialForm::OnApply) );
	this->Connect( wxID_CMD_SAVE, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MaterialForm::OnSave) );

	Layout();
	m_pSourceEdit->SetSize(m_editorPanel->GetSize());
	m_pSourceEdit->Refresh();

	int mainSplitHeight = m_splitter2->GetSize().y;
	m_splitter2->SetSashPosition(mainSplitHeight - 150);

	m_cmdApply->Disable();
	m_cmdSave->Disable();

	m_pLogWindow->SetFont(wxFont(10, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

	CoreInterface::get().pMsgBus->ObjectSelectionChanged.connect(
		this, &MaterialForm::onObjectSelectionChanged);
	CoreInterface::get().pUI->OnInputEvent.connect(
		this, &MaterialForm::onInputEvent);
}
//---------------------------------------------------------------------------
MaterialForm::~MaterialForm()
{
	CoreInterface::get().pUI->OnInputEvent.disconnect(
		this, &MaterialForm::onInputEvent);
	CoreInterface::get().pMsgBus->ObjectSelectionChanged.disconnect(
		this, &MaterialForm::onObjectSelectionChanged);

	this->Disconnect( wxID_CMD_APPLY, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MaterialForm::OnApply) );
	this->Disconnect( wxID_CMD_SAVE, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MaterialForm::OnSave) );
	this->Disconnect( wxEVT_SIZE, wxSizeEventHandler( MaterialForm::OnFormResize ) );
	delete m_pSourceEdit;
}
//---------------------------------------------------------------------------
void MaterialForm::OnClose( wxCloseEvent& event )
{
	m_pSourceEdit->ClearAll();
	m_pSourceEdit->Update();
	Hide();
	event.Skip(false);
}
//---------------------------------------------------------------------------
void MaterialForm::OnApply( wxCommandEvent& evt )
{
	evt;
	if (!m_pShader)
		return;

	m_pLogWindow->Clear();
	wxDateTime now = wxDateTime::Now();
	String header("****************************************\nCompile Begin: ");
	header += now.Format();
	header += "\n****************************************\n\n";
	m_pLogWindow->AppendText((const char*)header);

	// reload the shader
	String source(m_pSourceEdit->GetTextRaw());
	String errs;
	if (m_pShader->compile(source, errs))
		m_pLogWindow->AppendText("Compilation Succeeded");
	else
		m_pLogWindow->AppendText((const char*)errs);
}
//---------------------------------------------------------------------------
void MaterialForm::onInputEvent(const Teardrop::InputEvent& evt, bool& bHandled)
{
	if (evt.evtType.type == InputEvent::KEYBOARD)
	{
		if (evt.key.keyCode == DIK_S && 
			(evt.key.lCtrlDown || evt.key.rCtrlDown))
		{
			wxCommandEvent e;
			OnSave(e);
			bHandled |= true;
		}
	}
}
//---------------------------------------------------------------------------
void MaterialForm::OnSave( wxCommandEvent& evt )
{
	evt;
	if (!m_pShader)
		return;

	// reload the shader
	OnApply(evt);

	// save the shader source
	FileStream fs;
	String dataPath("../"); // because the runtime is looking in the built-data dir
	dataPath += m_pShader->getFullPath();

	if (!fs.open(dataPath, WRITE|TRUNCATE))
		return;

	fs.write(m_pSourceEdit->GetText().c_str(), m_pSourceEdit->GetText().Len());
	fs.close();
}
//---------------------------------------------------------------------------
void MaterialForm::OnFormResize( wxSizeEvent& evt )
{
	evt.Skip(true);

	// resize the things that are too dumb to do it themselves?
	Layout();
	m_pSourceEdit->SetSize(m_editorPanel->GetSize());
	m_pSourceEdit->Refresh();
}
//---------------------------------------------------------------------------
void MaterialForm::onObjectSelectionChanged(
	void* sender, Teardrop::Reflection::Object* pObj)
{
	sender;
	setObject(pObj);
}
//---------------------------------------------------------------------------
void MaterialForm::setObject(Teardrop::Reflection::Object* pObj)
{
	m_pSourceEdit->ClearAll();
	m_pSourceEdit->Update();
	m_cmdApply->Disable();
	m_cmdSave->Disable();

	m_pObject = pObj;
	if (!m_pObject)
	{
		return;
	}

	// else get something to edit
	ZoneObject* pZoneObj = 0;
	RenderComponent* pComp = 0;
	const char* pFilename = 0;

	if (pObj->getDerivedClassDef() == TerrainZone::getClassDef())
	{
		TerrainZone* pZone = static_cast<TerrainZone*>(pObj);
		pFilename = pZone->getShader();

		// first object will be a TerrainPatch/ZoneObject
		pZoneObj = pZone->getZoneObjects()[0];
		pZoneObj->findComponents(RenderComponent::getClassDef(), (Component**)&pComp);
	}
	else
	{
		// make sure it's a regular zone object
		if (pObj->getDerivedClassDef()->isA(ZoneObject::getClassDef()))
		{
			pZoneObj = static_cast<ZoneObject*>(pObj);
			if (pZoneObj->findComponents(RenderComponent::getClassDef(), (Component**)&pComp))
			{
				// get the shader name override, if any
				if (pComp->getShaderName().length())
				{
					pFilename = pComp->getShaderName();
				}
			}
		}
	}

	if (pComp)
	{
		// and then the submesh/material info
		GfxMeshInstance& inst = pComp->getMeshInstance();
		GfxMesh* pMesh = AutoResPtr<GfxMesh>(inst.getMeshHandle());
		if (!pMesh)
			return;

		// go through the submeshes; also, check for material overrides
		size_t numMtlOvr = inst.getMaterialCount();
		size_t numSubMesh = pMesh->getNumSubMeshes();

		if (!numSubMesh)
			return;

		GfxSubMesh* pSM = 0;
		GfxMaterial* pMtl = 0;

		for (size_t i=0; i<numSubMesh; ++i)
		{
			pSM = pMesh->getSubMesh(i);

			if (i < numMtlOvr)
				pMtl = inst.getMaterialByIndex(i);
			else
				pMtl = pSM->getMaterial();
		}

		// temp for now, just post up the last one found
		m_pShader = Environment::get().pRenderer->getShader(pSM, pMtl, pFilename);
		if (m_pShader)
		{
			m_pSourceEdit->SetTextRaw(m_pShader->getSource());
		}
	}

	m_cmdApply->Enable();
	m_cmdSave->Enable();
}

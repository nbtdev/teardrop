/*
-----------------------------------------------------------------------------
This source file is part of the Clash Of Steel Project

For the latest info, see http://www.clashofsteel.net/

Copyright (c) The Clash Of Steel Team
Also see acknowledgements in Readme.txt

This software is provided free-of-charge for non-commercial use, under the 
GNU Lesser General Public License as published by the Free Software Foundation; 
either version 2 of the License, or (at your option) any later version. This 
program is distributed in the hope that it will be useful, but WITHOUT ANY 
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A 
PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License 
along with this program; if not, write to the Free Software Foundation, Inc., 
59 Temple Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.

For commercial applications, contact the Clash Of Steel Team at 
info@clashofsteel.net for commercial licensing terms and information.
-----------------------------------------------------------------------------
*/

#include "stdafx.h"
#include "Runtime.h"
#include "Renderer.h"
#include "RenderWindow.h"
#include <msclr/marshal.h>
#include "Game/include/ObjectLibrary.h"
#include "Animation/include/Animation.h"
#include "Physics/include/Physics.h"
#include "Gfx/include/GfxRenderer.h"
#include "Reflection/include/Reflection.h"
#include "Util/include/Logger.h"
#include "Util/include/Environment.h"
#include "Memory/include/MemoryRegion.h"

// local wrapper classes
#include "Manifest.h"

using namespace CoS;
using namespace msclr::interop;
//---------------------------------------------------------------------------
COS_DEFINE_MEMORY_REGION(DEFAULT, 0, 1024);
COS_ALLOCATOR_IMPL(DEFAULT, CrtAllocator, DEFAULT, -1, -1)
COS_ALLOCATOR_IMPL(CRT, CrtAllocator, DEFAULT, -1, -1)
//---------------------------------------------------------------------------
Runtime::Runtime()
{
	m_rootWindow = nullptr;
}
//---------------------------------------------------------------------------
Runtime::~Runtime()
{
}
//---------------------------------------------------------------------------
Runtime::!Runtime()
{
}
//---------------------------------------------------------------------------
bool Runtime::Initialize()
{
	// init the editor context
	if (!m_logStrm->open("SceneEditor2.log", WRITE|TRUNCATE|TEXT))
	{
		// todo: let the user know
		return false;
	}

	m_timer->reset();
	m_resMgr->initialize();

	Physics::setAllocator(GetCRTAllocator());
	Animation::setAllocator(GetCRTAllocator());
	String::setAllocator(GetCRTAllocator());

	Environment& env = Environment::get();
	env.pDataPath = ".\\data/";
	env.isOffline = false;
	env.pLogger = new CoS::Logger(*m_logStrm);
	env.pRenderer = GfxRenderer::allocate(env, GetCRTAllocator());
	env.pMasterClock = &m_timer;
	env.pResourceMgr = &m_resMgr;
	env.pInput = &m_input;
	env.pManifest = &m_manifest;

	if (m_rootWindow)
	{
		GfxRenderConfig cfg;
		cfg.hWnd = m_rootWindow->Handle.ToPointer();
		cfg.pLogger = env.pLogger;
		env.pRenderer->initialize(cfg, false);
	}

	m_manifest->initialize(env);
	ObjectLibrary::instance()->initialize(env);

	if (!Physics::initialize() && Animation::initialize())
	{
		return false;
	}

	// make .NET versions of all Reflection-based class definitions
	generateClassLibrary();
	return true;
}
//---------------------------------------------------------------------------
void Runtime::Shutdown()
{
	Environment::get().pRenderer->destroy();
	GfxRenderer::deallocate(Environment::get().pRenderer);
	Animation::shutdown();
	Physics::shutdown();

	delete Environment::get().pLogger;
	m_logStrm->close();

	// force this now because if we wait until finalization we will crash
	m_manifest.Tidy();
	m_logStrm.Tidy();
	m_timer.Tidy();
	m_resMgr.Tidy();
	m_input.Tidy();
}
//---------------------------------------------------------------------------
Game::Manifest^ Runtime::Manifest::get()
{
	return gcnew Game::Manifest(&m_manifest);
}
//---------------------------------------------------------------------------
System::Reflection::Assembly^ Runtime::Classdef::get()
{
	return m_assembly;
}
//---------------------------------------------------------------------------
System::String^ Runtime::DataPath::get()
{
	return gcnew System::String(Environment::get().pDataPath);
}
//---------------------------------------------------------------------------
CoS::Gfx::Renderer^ Runtime::Renderer::get()
{
	if (Environment::get().pRenderer)
		return gcnew Gfx::Renderer(Environment::get().pRenderer);
	else
		return nullptr;
}
//---------------------------------------------------------------------------
CoS::Gfx::RenderWindow^ Runtime::RenderWindow::get()
{
	Environment& env = Environment::get();
	if (env.pRenderer && env.pRenderer->getMainWindow())
		return gcnew Gfx::RenderWindow(env.pRenderer->getMainWindow());
	else
		return nullptr;
}
//---------------------------------------------------------------------------
System::Windows::Forms::Control^ Runtime::RootWindow::get()
{
	return m_rootWindow;
}
//---------------------------------------------------------------------------
void Runtime::RootWindow::set(System::Windows::Forms::Control^ window)
{
	m_rootWindow = window;
}
//---------------------------------------------------------------------------
// the idea here is to pull every classdef registered, and make an assembly
// in-memory that contains all of those class definitions, so that we can 
// deal with them natively in .NET as well as leave them be in C++ (and 
// ideally, to have changes to data in instances of the classes pass back
// and forth between the C# version -- which is just properties -- and the C++
// version, which is actual code)
using namespace System;
using namespace System::ComponentModel;
using namespace System::IO;
using namespace System::Text;
using namespace System::Reflection;
using namespace System::CodeDom;
using namespace System::CodeDom::Compiler;
using namespace Microsoft::CSharp;
using namespace CoS::Reflection;
//---------------------------------------------------------------------------
static void setCreatable(CodeTypeDeclaration^ decl, bool creatable)
{
	if (creatable)
	{
		decl->CustomAttributes->Add(gcnew CodeAttributeDeclaration("Creatable",
			gcnew CodeAttributeArgument(gcnew CodePrimitiveExpression(true))
			));
	}
}
//---------------------------------------------------------------------------
static System::String^ convertTypeName(const CoS::String& typeName)
{
	if (typeName == "Real" || typeName == "float")
		return gcnew System::String("System.Single");
	if (typeName == "Int32" || typeName == "int")
		return gcnew System::String("System.Int32");
	if (typeName == "Bool" || typeName == "bool")
		return gcnew System::String("System.Boolean");

	return gcnew System::String(typeName);
}
//---------------------------------------------------------------------------
static void setPropertyType(
	CodeMemberProperty^ prop, const PropertyDef* pProp)
{
	if (pProp->isCollection())
	{
		prop->Type->ArrayElementType = gcnew CodeTypeReference(
			convertTypeName(pProp->getTargetTypeName()));
		prop->Type->ArrayRank = 1;
	}
	else
	{
		prop->Type = gcnew CodeTypeReference(
			convertTypeName(pProp->getTypeName()));
	}
}
//---------------------------------------------------------------------------
static bool isKnownComplexPrimitive(System::String^ typeName)
{
	return (typeName == "Transform"
		|| typeName == "Vector4"
		|| typeName == "Vector2"
		|| typeName == "Quaternion"
		|| typeName == "Matrix44"
		);
}
//---------------------------------------------------------------------------
static CodeMemberField^ addField(
	CodeTypeDeclaration^ decl,
	CodeTypeReference^ typeRef, 
	System::String^ name,
	System::String^ default)
{
	CodeMemberField^ f = gcnew CodeMemberField(typeRef, gcnew System::String("m_") + name);

	// if this is a known complex primitive, make a new empty one by default
    if (typeRef->ArrayRank == 0)
    {
        if (isKnownComplexPrimitive(typeRef->BaseType))
        {
            f->InitExpression = gcnew CodeObjectCreateExpression(typeRef->BaseType);
        }
    }

	decl->Members->Add(f);
    return f;
}
//---------------------------------------------------------------------------
static void addAccessors(CodeMemberProperty^ prop, CodeMemberField^ f)
{
    prop->GetStatements->Add(gcnew CodeMethodReturnStatement(
        gcnew CodeFieldReferenceExpression(gcnew CodeThisReferenceExpression(), f->Name)));
    prop->SetStatements->Add(gcnew CodeAssignStatement(gcnew CodeFieldReferenceExpression(
        gcnew CodeThisReferenceExpression(), f->Name), gcnew CodePropertySetValueReferenceExpression()));

	// invoke our hacky "property changed" notification system...
	CodeMethodReferenceExpression^ method = 
		gcnew CodeMethodReferenceExpression(gcnew CodeThisReferenceExpression, "PropertyChanged");
	array<CodePrimitiveExpression^>^ args = gcnew array<CodePrimitiveExpression^>(1);
	args[0] = gcnew CodePrimitiveExpression(prop->Name);

	prop->SetStatements->Add(gcnew CodeMethodInvokeExpression(method, args));
}
//---------------------------------------------------------------------------
static void addDescription(CodeMemberProperty^ prop, const PropertyDef* pProp)
{
    prop->CustomAttributes->Add(gcnew CodeAttributeDeclaration(
		gcnew CodeTypeReference(DescriptionAttribute::typeid),
		gcnew CodeAttributeArgument(gcnew CodePrimitiveExpression(
			gcnew System::String(pProp->getDescription()))
    )));
}
//---------------------------------------------------------------------------
static void addCategory(CodeMemberProperty^ prop, System::String^ className)
{
	prop->CustomAttributes->Add(gcnew CodeAttributeDeclaration(
		gcnew CodeTypeReference(CategoryAttribute::typeid),
		gcnew CodeAttributeArgument(gcnew CodePrimitiveExpression(className))
	));
}
//---------------------------------------------------------------------------
static void addExpandableAttribute(CodeMemberProperty^ prop)
{
	prop->CustomAttributes->Add(gcnew CodeAttributeDeclaration(
		gcnew CodeTypeReference(TypeConverterAttribute::typeid), 
		gcnew CodeAttributeArgument(gcnew CodeTypeOfExpression(
		ExpandableObjectConverter::typeid))
		));
}
//---------------------------------------------------------------------------
static void addEditor(CodeMemberProperty^ prop, const PropertyDef* pProp)
{
}
//---------------------------------------------------------------------------
static void addPropertyToClass(
	const PropertyDef* pProp, CodeTypeDeclaration^ decl)
{
	CodeMemberProperty^ prop = gcnew CodeMemberProperty();
	prop->Name = gcnew System::String(pProp->getName());

	setPropertyType(prop, pProp);
	CodeMemberField^ f = addField(decl, prop->Type, prop->Name, 
		gcnew System::String(pProp->getDefault()));

	addAccessors(prop, f);
	addDescription(prop, pProp);
	addCategory(prop, decl->Name);
	addEditor(prop, pProp);

	if (isKnownComplexPrimitive(prop->Name))
	{
		addExpandableAttribute(prop);
	}

	prop->Attributes = MemberAttributes::Public;
	decl->Members->Add(prop);
}
//---------------------------------------------------------------------------
static void addClassToNamespace(ClassDef* pClass, CodeNamespace^ ns)
{
	CodeTypeDeclaration^ decl = gcnew CodeTypeDeclaration(
		gcnew System::String(pClass->getName()));
	ns->Types->Add(decl);

	CoS::String className(pClass->getName());
	CoS::String baseName(pClass->getBaseClassName());
	if (baseName.length())
	{
		// two classes get special treatment -- ZoneObject and Component
		// want to inherit from ZoneObjectBase and ComponentBase, which are 
		// defined in the C# class lib "Reflection"
		if (className == "ZoneObject")
			baseName = "ZoneObjectBase";
		if (className == "Component")
			baseName = "ComponentBase";
		decl->BaseTypes->Add(gcnew System::String(baseName));
	}

	setCreatable(decl, pClass->isCreatable());

	decl->IsPartial = true;

	// properties for these are handled in their Base classes
	//if (!(className == "ZoneObject" || className == "Component"))
	//{
		// add properties
		const PropertyDef* pProp = pClass->getProps();
		while (pProp)
		{
			addPropertyToClass(pProp, decl);
			pProp = pProp->m_pNext;
		}
	//}
}
//---------------------------------------------------------------------------
void CoS::Runtime::generateClassLibrary()
{
	CodeNamespace^ ns = gcnew CodeNamespace("CoS");

	// add standard imports
	ns->Imports->Add(gcnew CodeNamespaceImport("System"));
	ns->Imports->Add(gcnew CodeNamespaceImport("Math"));

	ClassDef* pClass = ClassDef::getClasses();
	while (pClass)
	{
		addClassToNamespace(pClass, ns);
		pClass = pClass->m_pNext;
	}

	CodeDomProvider^ prov = gcnew CSharpCodeProvider();
	StringBuilder^ sb = gcnew StringBuilder();
    StringWriter^ sw = gcnew StringWriter(sb);

    CodeGeneratorOptions^ opts = gcnew CodeGeneratorOptions();
    prov->GenerateCodeFromNamespace(ns, sw, opts);
    sw->Close();

	System::String^ source = sb->ToString();
	{
		marshal_context^ ctx = gcnew marshal_context();
		Environment::get().pLogger->logMessage(
			ctx->marshal_as<const char*>(source));
		delete ctx;
	}

	CompilerParameters^ parms = gcnew CompilerParameters();

#if defined(_DEBUG)
    parms->IncludeDebugInformation = true;
#endif

	// add some required assemblies
	parms->ReferencedAssemblies->Add("CoS.Reflection.dll");
	parms->ReferencedAssemblies->Add("CoS.Marshal.dll");
	parms->ReferencedAssemblies->Add("System.dll");

    CompilerResults^ res = prov->CompileAssemblyFromSource(parms, source);
	if (res->Errors->Count == 0)
    {
        m_assembly = res->CompiledAssembly;
    }
    else
    {
		System::String^ errs = "";
        for each (CompilerError^ err in res->Errors)
        {
			errs += err->ErrorText + System::Environment::NewLine;
        }

		marshal_context^ ctx = gcnew marshal_context();
		Environment::get().pLogger->logMessage(
			ctx->marshal_as<const char*>(errs));
		delete ctx;
    }
}

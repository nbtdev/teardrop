/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/


#include "MaterialExpression.h"
#include "Attribute.h"
#include <sstream>

using namespace Teardrop;
using namespace Gfx;

TD_CLASS_IMPL(MaterialExpression);

MaterialExpression::MaterialExpression()
{
}

MaterialExpression::~MaterialExpression()
{
}

Attribute* MaterialExpression::findInputAttribute(const char* name)
{
	for (size_t i=0; i<mInputs.size(); ++i) {
		if (!strcmp(mInputs[i].mName, name))
			return &mInputs[i];
	}

	return 0;
}

Attribute* MaterialExpression::findOutputAttribute(const char* name)
{
	for (size_t i=0; i<mOutputs.size(); ++i) {
		if (!strcmp(mOutputs[i].mName, name))
			return &mOutputs[i];
	}

	return 0;
}

const MaterialExpression::Attributes& MaterialExpression::inputAttributes()
{
	return mInputs;
}

const MaterialExpression::Attributes& MaterialExpression::outputAttributes()
{
	return mOutputs;
}

void MaterialExpression::appendDefinition(Language lang, std::ostream& o)
{
	// it's the same in all languages
	o << "void ";
	o << getDerivedClassDef()->getName();
	o << '(';

	// insert input params
	int arg = 0;
	for (size_t i=0; i<mInputs.size(); ++i) {
		if (arg) 
			o << ", ";

		o << "in " << Attribute::paramTypeToString(mInputs[i].mType) << ' ' << mInputs[i].mName;
		arg++;
	}

	for (size_t i=0; i<mOutputs.size(); ++i) {
		if (arg) 
			o << ", ";

		o << "in " << Attribute::paramTypeToString(mOutputs[i].mType) << ' ' << mOutputs[i].mName;
		arg++;
	}

	o << ') {\n';

	appendBody(lang, o);

	o << '}\n\n';
}

void MaterialExpression::appendCall(Language /*lang*/, int ordinal, const std::vector<std::string>& inputs, std::vector<OutputName>& outputs, std::ostream& o)
{
	// same in all languages

	assert(inputs.size() == mInputs.size());

	// generate output param/var names
	outputs.resize(mOutputs.size());

	// generate output variable names and their decls
	for (size_t i=0; i<mOutputs.size(); ++i) {
		std::stringstream ss;
		ss << getDerivedClassDef()->getName() << '_' << mOutputs[i].mName << '_' << ordinal;
		outputs[i].mOutputVarName = ss.str();
		outputs[i].mOutputAttr = &mOutputs[i];

		o << Attribute::paramTypeToString(mOutputs[i].mType) << ' ' << outputs[i].mOutputVarName << ";\n";
	}

	// then generate the call itself, using the provided input param names and generated output param names
	o << getDerivedClassDef()->getName();
	o << '(';

	// insert input params
	int arg = 0;
	for (size_t i=0; i<inputs.size(); ++i) {
		if (arg) 
			o << ", ";

		o << inputs[i];
		arg++;
	}

	for (size_t i=0; i<outputs.size(); ++i) {
		if (arg) 
			o << ", ";

		o << outputs[i].mOutputVarName;
		arg++;
	}

	o << ');\n';
}

void MaterialExpression::appendBody(Language /*lang*/, std::ostream& /*o*/)
{
}
/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/


#include "MaterialExpression.h"
#include "Attribute.h"
#include <cstring>
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

const ShaderFeatures& MaterialExpression::features()
{
	return mFeatures;
}

void MaterialExpression::appendDefinition(Language lang, std::ostream& o)
{
	// give subclasses an opportunity to insert some internal dependencies
	insertDependencies(lang, o);

	// then our own declaration; it starts the same in all languages
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

		o << "out " << Attribute::paramTypeToString(mOutputs[i].mType) << ' ' << mOutputs[i].mName;
		arg++;
	}

	o << ") {\n";

	appendBody(lang, o);

	o << "}\n\n";
}

void MaterialExpression::appendCall(Language /*lang*/, int ordinal, const std::vector<std::string>& inputs, const std::map<const Attribute*, std::string>& outputs, std::ostream& o)
{
	// same in all languages?

	assert(inputs.size() == mInputs.size());

	// generate output variable names and their decls
	std::vector<std::string> outputNames(mOutputs.size());
	int idx = 0;
	for (size_t i=0; i<mOutputs.size(); ++i) {
		std::map<const Attribute*, std::string>::const_iterator it = outputs.find(&mOutputs[i]);

		if (it != outputs.end()) {
			// then there is already a name generated for this output, so use it
			outputNames[i] = it->second;
		}
		else {
			// this output is unused, so generate a dummy name for it
			std::stringstream ss;
			ss << getDerivedClassDef()->getName() << '_' << ordinal << "_unused_" << idx++;
			outputNames[i] = ss.str();
		}

		o << Attribute::paramTypeToString(mOutputs[i].mType) << ' ' << outputNames[i] << ";\n";
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

	for (size_t i=0; i<outputNames.size(); ++i) {
		if (arg) 
			o << ", ";

		o << outputNames[i];
		arg++;
	}

	o << ");\n";
}

void MaterialExpression::appendBody(Language /*lang*/, std::ostream& /*o*/)
{
}

void MaterialExpression::insertDependencies(Language /*lang*/, std::ostream& /*o*/)
{
}

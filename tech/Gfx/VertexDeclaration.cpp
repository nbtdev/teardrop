/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "VertexDeclaration.h"

namespace Teardrop {
namespace Gfx {

VertexDeclaration::VertexDeclaration(Submesh* parent)
	: mParent(parent)
{
}

VertexDeclaration::~VertexDeclaration()
{
}

} // namespace Gfx
} // namespace Teardrop

/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(DATA_INCLUDED)
#define DATA_INCLUDED

#include "FastDelegate.h"

class Data
{
public:
	Data();
	virtual ~Data();

	// "data changed" event
	func::delegate1<Data*> DataChanged(Data* pData);

	virtual unsigned int getType() const = 0;
	virtual bool load(const char* filepath) = 0;
	virtual bool save(const char* filepath) = 0;
	virtual bool update(float deltaT) = 0;
};

#endif // DATA_INCLUDED

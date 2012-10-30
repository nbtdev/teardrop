/*
-----------------------------------------------------------------------------
This source file is part of the Clash Of Steel Project

For the latest info, see http://www.clashofsteel.net/

Copyright (c) The Clash Of Steel Team
Also see acknowledgments in Readme.txt

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/

#include "stdafx.h"
#include "UIFlashEvents.h"
#include "UIFlashInterfaces.h"
#include <assert.h>

using namespace CoS;
using namespace UI;
//---------------------------------------------------------------------------
FlashEvents::FlashEvents()
{
	m_pListener = 0;
	m_refCount = 0;
	m_pConnectionPoint = 0;
}
//---------------------------------------------------------------------------
FlashEvents::~FlashEvents()
{
}
//---------------------------------------------------------------------------
bool FlashEvents::initialize(
	ShockwaveFlashObjects::IShockwaveFlash* pFlashPlayer,
	IFlashEventListener* pListener)
{
	m_pListener = pListener;

	HRESULT hr;
	LPCONNECTIONPOINTCONTAINER pContainer;

	hr = pFlashPlayer->QueryInterface(IID_IConnectionPointContainer, (void**)&pContainer);
	if (SUCCEEDED(hr))
	{
		hr = pContainer->FindConnectionPoint(__uuidof(ShockwaveFlashObjects::_IShockwaveFlashEvents), &m_pConnectionPoint);
		if (SUCCEEDED(hr))
		{
			IDispatch* pDisp;
			hr = QueryInterface(__uuidof(IDispatch), (void**)&pDisp);
			if(SUCCEEDED(hr))
			{
				m_pConnectionPoint->Advise((IUnknown*)pDisp, &m_cookie);
				pDisp->Release();
			}
		}

		pContainer->Release();
	}

	return true;
}
//---------------------------------------------------------------------------
void FlashEvents::destroy()
{
	if (m_pConnectionPoint)
	{
		m_pConnectionPoint->Unadvise(m_cookie);
		m_cookie = 0;

		m_pConnectionPoint->Release();
		m_pConnectionPoint = 0;
	}
}
//---------------------------------------------------------------------------
STDMETHODIMP FlashEvents::QueryInterface(REFIID riid, LPVOID* ppv)
{
	*ppv = 0;

	if (riid == IID_IUnknown)
	{
		*ppv = (LPUNKNOWN)this;
		AddRef();
		return S_OK;
	}
	else if (riid == IID_IDispatch)
	{
		*ppv = (IDispatch*)this;
		AddRef();
		return S_OK;
	}
	else if (riid == __uuidof(ShockwaveFlashObjects::_IShockwaveFlashEvents))
	{
		*ppv = (ShockwaveFlashObjects::_IShockwaveFlashEvents*) this;
		AddRef();
		return S_OK;
	}
	else
	{   
		return E_NOTIMPL;
	}
}
//---------------------------------------------------------------------------
STDMETHODIMP_(ULONG) FlashEvents::AddRef()
{  
	return ++m_refCount;
}
//---------------------------------------------------------------------------
STDMETHODIMP_(ULONG) FlashEvents::Release()
{
	--m_refCount;

	if(!m_refCount)
		delete this;		

	return m_refCount;
}
//---------------------------------------------------------------------------
STDMETHODIMP FlashEvents::GetTypeInfoCount(UINT* pctinfo)
{
	return E_NOTIMPL;
}
//---------------------------------------------------------------------------
STDMETHODIMP FlashEvents::GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo** ppTInfo)
{
	return E_NOTIMPL; 
}
//---------------------------------------------------------------------------
STDMETHODIMP FlashEvents::GetIDsOfNames(REFIID riid, LPOLESTR* rgszNames, UINT cNames, LCID lcid,DISPID* rgDispId)
{
	return E_NOTIMPL; 
}
//---------------------------------------------------------------------------
STDMETHODIMP FlashEvents::Invoke(DISPID dispIdMember, REFIID riid, LCID lcid,
	WORD wFlags, ::DISPPARAMS __RPC_FAR *pDispParams, VARIANT __RPC_FAR *pVarResult,
	::EXCEPINFO __RPC_FAR *pExcepInfo, UINT __RPC_FAR *puArgErr)
{
	switch(dispIdMember)
	{
	case 0x7a6:			
		break;
	case 0x96:			
		if(m_pListener && (pDispParams->cArgs == 2) && (pDispParams->rgvarg[0].vt == VT_BSTR) && (pDispParams->rgvarg[1].vt == VT_BSTR))
			m_pListener->onFSCommand(
			_bstr_t(pDispParams->rgvarg[1].bstrVal), 
			_bstr_t(pDispParams->rgvarg[0].bstrVal));
		break;
	case 0xC5:
		if(m_pListener && (pDispParams->cArgs == 1) && (pDispParams->rgvarg[0].vt == VT_BSTR))
			m_pListener->onFlashCall(
			_bstr_t(pDispParams->rgvarg[0].bstrVal));
		break;
	case DISPID_READYSTATECHANGE:					
		break;
	default:			
		return DISP_E_MEMBERNOTFOUND;
	} 
	
	return NOERROR;
}

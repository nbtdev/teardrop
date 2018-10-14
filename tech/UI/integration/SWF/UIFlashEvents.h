/******************************************************************************
Copyright (c) 2018 Teardrop Games

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
******************************************************************************/

#if !defined(UIFLASHEVENTS_INCLUDED)
#define UIFLASHEVENTS_INCLUDED

#include "Memory/Memory.h"

namespace Teardrop
{
	namespace UI
	{
		/*
		*/

		class IFlashEventListener;

		class FlashEvents
			: public ShockwaveFlashObjects::_IShockwaveFlashEvents
		{
			IFlashEventListener* m_pListener;
			LPCONNECTIONPOINT m_pConnectionPoint;	
			DWORD m_cookie;
			int m_refCount;

		public:
			FlashEvents();
			~FlashEvents();

			bool initialize(
				ShockwaveFlashObjects::IShockwaveFlash* pFlashPlayer,
				IFlashEventListener* pListener);
			void destroy();

			//! ShockwaveFlashObjects::_IShockwaveFlashEvents implementation
			STDMETHOD(QueryInterface(REFIID riid, LPVOID* ppv));
			STDMETHOD_(ULONG, AddRef());
			STDMETHOD_(ULONG, Release());
			STDMETHOD(GetTypeInfoCount(UINT* pctinfo));
			STDMETHOD(GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo** ppTInfo));
			STDMETHOD(GetIDsOfNames(REFIID riid, LPOLESTR* rgszNames, UINT cNames, LCID lcid,DISPID* rgDispId));
			STDMETHOD(Invoke(DISPID dispIdMember, REFIID riid, LCID lcid,
				WORD wFlags, ::DISPPARAMS __RPC_FAR *pDispParams, VARIANT __RPC_FAR *pVarResult,
				::EXCEPINFO __RPC_FAR *pExcepInfo, UINT __RPC_FAR *puArgErr));

			TD_DECLARE_ALLOCATOR();
		};
	} // namespace UI
} // namespace Teardrop

#endif // UIFLASHEVENTS_INCLUDED

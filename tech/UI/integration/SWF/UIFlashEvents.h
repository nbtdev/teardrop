/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

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

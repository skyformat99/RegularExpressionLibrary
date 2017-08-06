
// Wizard で生成された接続ポイント プロキシ クラス
// 警告: このファイルはウィザードで再生成されることがあります。


#pragma once

template<class T>
class CProxy_IIFRegExpEvents :
	public ATL::IConnectionPointImpl<T, &__uuidof(_IIFRegExpEvents)>
{
public:
	HRESULT Fire_OnSwitchQuery(IUnknown * Sender, LONG Param, SHORT * CaseIndex)
	{
		HRESULT hr = S_OK;
		T * pThis = static_cast<T *>(this);
		int cConnections = m_vec.GetSize();

		for (int iConnection = 0; iConnection < cConnections; iConnection++)
		{
			pThis->Lock();
			CComPtr<IUnknown> punkConnection = m_vec.GetAt(iConnection);
			pThis->Unlock();

			IDispatch * pConnection = static_cast<IDispatch *>(punkConnection.p);

			if (pConnection)
			{
				CComVariant avarParams[3];
				avarParams[2] = Sender;
				avarParams[2].vt = VT_UNKNOWN;
				avarParams[1] = Param;
				avarParams[1].vt = VT_I4;
				avarParams[0].byref = CaseIndex;
				avarParams[0].vt = VT_I2|VT_BYREF;
				CComVariant varResult;

				DISPPARAMS params = { avarParams, NULL, 3, 0 };
				hr = pConnection->Invoke(1, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
			}
		}
		return hr;
	}
	HRESULT Fire_OnSelectFunc(IUnknown * Sender, LONG Param, SHORT CaseIndex, LONG CharIndex, LONG CharCount)
	{
		HRESULT hr = S_OK;
		T * pThis = static_cast<T *>(this);
		int cConnections = m_vec.GetSize();

		for (int iConnection = 0; iConnection < cConnections; iConnection++)
		{
			pThis->Lock();
			CComPtr<IUnknown> punkConnection = m_vec.GetAt(iConnection);
			pThis->Unlock();

			IDispatch * pConnection = static_cast<IDispatch *>(punkConnection.p);

			if (pConnection)
			{
				CComVariant avarParams[5];
				avarParams[4] = Sender;
				avarParams[4].vt = VT_UNKNOWN;
				avarParams[3] = Param;
				avarParams[3].vt = VT_I4;
				avarParams[2] = CaseIndex;
				avarParams[2].vt = VT_I2;
				avarParams[1] = CharIndex;
				avarParams[1].vt = VT_I4;
				avarParams[0] = CharCount;
				avarParams[0].vt = VT_I4;
				CComVariant varResult;

				DISPPARAMS params = { avarParams, NULL, 5, 0 };
				hr = pConnection->Invoke(2, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
			}
		}
		return hr;
	}
	HRESULT Fire_OnIfFunc(IUnknown * Sender, LONG Param, VARIANT_BOOL Success, LONG CharIndex, LONG CharCount)
	{
		HRESULT hr = S_OK;
		T * pThis = static_cast<T *>(this);
		int cConnections = m_vec.GetSize();

		for (int iConnection = 0; iConnection < cConnections; iConnection++)
		{
			pThis->Lock();
			CComPtr<IUnknown> punkConnection = m_vec.GetAt(iConnection);
			pThis->Unlock();

			IDispatch * pConnection = static_cast<IDispatch *>(punkConnection.p);

			if (pConnection)
			{
				CComVariant avarParams[5];
				avarParams[4] = Sender;
				avarParams[4].vt = VT_UNKNOWN;
				avarParams[3] = Param;
				avarParams[3].vt = VT_I4;
				avarParams[2] = Success;
				avarParams[2].vt = VT_BOOL;
				avarParams[1] = CharIndex;
				avarParams[1].vt = VT_I4;
				avarParams[0] = CharCount;
				avarParams[0].vt = VT_I4;
				CComVariant varResult;

				DISPPARAMS params = { avarParams, NULL, 5, 0 };
				hr = pConnection->Invoke(3, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
			}
		}
		return hr;
	}
	HRESULT Fire_OnActionFunc(IUnknown * Sender, LONG Param, LONG CharIndex, LONG CharCount)
	{
		HRESULT hr = S_OK;
		T * pThis = static_cast<T *>(this);
		int cConnections = m_vec.GetSize();

		for (int iConnection = 0; iConnection < cConnections; iConnection++)
		{
			pThis->Lock();
			CComPtr<IUnknown> punkConnection = m_vec.GetAt(iConnection);
			pThis->Unlock();

			IDispatch * pConnection = static_cast<IDispatch *>(punkConnection.p);

			if (pConnection)
			{
				CComVariant avarParams[4];
				avarParams[3] = Sender;
				avarParams[3].vt = VT_UNKNOWN;
				avarParams[2] = Param;
				avarParams[2].vt = VT_I4;
				avarParams[1] = CharIndex;
				avarParams[1].vt = VT_I4;
				avarParams[0] = CharCount;
				avarParams[0].vt = VT_I4;
				CComVariant varResult;

				DISPPARAMS params = { avarParams, NULL, 4, 0 };
				hr = pConnection->Invoke(4, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
			}
		}
		return hr;
	}
	HRESULT Fire_OnForInitializeFunc(IUnknown * Sender, LONG Param, LONG CharIndex)
	{
		HRESULT hr = S_OK;
		T * pThis = static_cast<T *>(this);
		int cConnections = m_vec.GetSize();

		for (int iConnection = 0; iConnection < cConnections; iConnection++)
		{
			pThis->Lock();
			CComPtr<IUnknown> punkConnection = m_vec.GetAt(iConnection);
			pThis->Unlock();

			IDispatch * pConnection = static_cast<IDispatch *>(punkConnection.p);

			if (pConnection)
			{
				CComVariant avarParams[3];
				avarParams[2] = Sender;
				avarParams[2].vt = VT_UNKNOWN;
				avarParams[1] = Param;
				avarParams[1].vt = VT_I4;
				avarParams[0] = CharIndex;
				avarParams[0].vt = VT_I4;
				CComVariant varResult;

				DISPPARAMS params = { avarParams, NULL, 3, 0 };
				hr = pConnection->Invoke(5, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
			}
		}
		return hr;
	}
	HRESULT Fire_OnForReinitializeFunc(IUnknown * Sender, LONG Param, LONG CharIndex)
	{
		HRESULT hr = S_OK;
		T * pThis = static_cast<T *>(this);
		int cConnections = m_vec.GetSize();

		for (int iConnection = 0; iConnection < cConnections; iConnection++)
		{
			pThis->Lock();
			CComPtr<IUnknown> punkConnection = m_vec.GetAt(iConnection);
			pThis->Unlock();

			IDispatch * pConnection = static_cast<IDispatch *>(punkConnection.p);

			if (pConnection)
			{
				CComVariant avarParams[3];
				avarParams[2] = Sender;
				avarParams[2].vt = VT_UNKNOWN;
				avarParams[1] = Param;
				avarParams[1].vt = VT_I4;
				avarParams[0] = CharIndex;
				avarParams[0].vt = VT_I4;
				CComVariant varResult;

				DISPPARAMS params = { avarParams, NULL, 3, 0 };
				hr = pConnection->Invoke(6, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
			}
		}
		return hr;
	}
	HRESULT Fire_OnQueryFunc(IUnknown * Sender, LONG Param, LONG * CharIndex, LONG LastIndex, VARIANT_BOOL * Success)
	{
		HRESULT hr = S_OK;
		T * pThis = static_cast<T *>(this);
		int cConnections = m_vec.GetSize();

		for (int iConnection = 0; iConnection < cConnections; iConnection++)
		{
			pThis->Lock();
			CComPtr<IUnknown> punkConnection = m_vec.GetAt(iConnection);
			pThis->Unlock();

			IDispatch * pConnection = static_cast<IDispatch *>(punkConnection.p);

			if (pConnection)
			{
				CComVariant avarParams[5];
				avarParams[4] = Sender;
				avarParams[4].vt = VT_UNKNOWN;
				avarParams[3] = Param;
				avarParams[3].vt = VT_I4;
				avarParams[2].byref = CharIndex;
				avarParams[2].vt = VT_I4|VT_BYREF;
				avarParams[1] = LastIndex;
				avarParams[1].vt = VT_I4;
				avarParams[0].byref = Success;
				avarParams[0].vt = VT_BOOL|VT_BYREF;
				CComVariant varResult;

				DISPPARAMS params = { avarParams, NULL, 5, 0 };
				hr = pConnection->Invoke(7, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
			}
		}
		return hr;
	}
	HRESULT Fire_OnCallFunc(IUnknown * Sender, LONG Param)
	{
		HRESULT hr = S_OK;
		T * pThis = static_cast<T *>(this);
		int cConnections = m_vec.GetSize();

		for (int iConnection = 0; iConnection < cConnections; iConnection++)
		{
			pThis->Lock();
			CComPtr<IUnknown> punkConnection = m_vec.GetAt(iConnection);
			pThis->Unlock();

			IDispatch * pConnection = static_cast<IDispatch *>(punkConnection.p);

			if (pConnection)
			{
				CComVariant avarParams[2];
				avarParams[1] = Sender;
				avarParams[1].vt = VT_UNKNOWN;
				avarParams[0] = Param;
				avarParams[0].vt = VT_I4;
				CComVariant varResult;

				DISPPARAMS params = { avarParams, NULL, 2, 0 };
				hr = pConnection->Invoke(8, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
			}
		}
		return hr;
	}
	HRESULT Fire_OnMatch(IUnknown * Sender, LONG Begin, LONG End, VARIANT_BOOL * Next)
	{
		HRESULT hr = S_OK;
		T * pThis = static_cast<T *>(this);
		int cConnections = m_vec.GetSize();

		for (int iConnection = 0; iConnection < cConnections; iConnection++)
		{
			pThis->Lock();
			CComPtr<IUnknown> punkConnection = m_vec.GetAt(iConnection);
			pThis->Unlock();

			IDispatch * pConnection = static_cast<IDispatch *>(punkConnection.p);

			if (pConnection)
			{
				CComVariant avarParams[4];
				avarParams[3] = Sender;
				avarParams[3].vt = VT_UNKNOWN;
				avarParams[2] = Begin;
				avarParams[2].vt = VT_I4;
				avarParams[1] = End;
				avarParams[1].vt = VT_I4;
				avarParams[0].byref = Next;
				avarParams[0].vt = VT_BOOL|VT_BYREF;
				CComVariant varResult;

				DISPPARAMS params = { avarParams, NULL, 4, 0 };
				hr = pConnection->Invoke(9, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
			}
		}
		return hr;
	}
};


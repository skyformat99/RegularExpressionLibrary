// RegVender.h : CRegVender の宣言

#pragma once
#include "resource.h"       // メイン シンボル

#include "IF_MDL_i.h"
#include "_IIFRegVenderEvents_CP.H"

#include	"RegExp_Vender.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "DCOM の完全サポートを含んでいない Windows Mobile プラットフォームのような Windows CE プラットフォームでは、単一スレッド COM オブジェクトは正しくサポートされていません。ATL が単一スレッド COM オブジェクトの作成をサポートすること、およびその単一スレッド COM オブジェクトの実装の使用を許可することを強制するには、_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA を定義してください。ご使用の rgs ファイルのスレッド モデルは 'Free' に設定されており、DCOM Windows CE 以外のプラットフォームでサポートされる唯一のスレッド モデルと設定されていました。"
#endif


using namespace ATL;


// CIFRegVender



class ATL_NO_VTABLE CIFRegVender :
	public CComObjectRootEx<CComSingleThreadModel>,
//外部作成させない
//	public CComCoClass<CIFRegVender, &CLSID_IFRegVender>,
	public IConnectionPointContainerImpl<CIFRegVender>,
	public CProxy_IIFRegVenderEvents <CIFRegVender>,
	public IDispatchImpl<IIFRegVender, &IID_IIFRegVender, &LIBID_IF_MDLLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
private:
	iregexp_vender fvender;
public:
	CIFRegVender()
	{
		put_MemorySize(msDoubleByte);
	}

//外部作成させない
//DECLARE_REGISTRY_RESOURCEID(IDR_REGVENDER)

DECLARE_NOT_AGGREGATABLE(CIFRegVender)

BEGIN_COM_MAP(CIFRegVender)
	COM_INTERFACE_ENTRY(IIFRegVender)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CIFRegVender)
	CONNECTION_POINT_ENTRY(__uuidof(_IIFRegVenderEvents))
END_CONNECTION_POINT_MAP()


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

	iregexp_vender& vender(void) { return fvender; }

public:


	STDMETHOD(get_MemorySize)(TRegExpMemorySize* pVal);
	STDMETHOD(put_MemorySize)(TRegExpMemorySize newVal);
	STDMETHOD(SetMemoryParam)(CHAR* Memory, ULONG MemorySize, ULONG DataOffset, ULONG DataLength, ULONG DataGap);
	STDMETHOD(SetLocalMemoryParam)(TRegVenderLocalMemoryType Type, CHAR* Memory, ULONG MemorySize, ULONG DataOffset, ULONG DataLength, ULONG DataGap);
};

static _ATL_FUNC_INFO RegVender_OnChangeInfo = { CC_STDCALL, VT_EMPTY, 0, {}};
static _ATL_FUNC_INFO RegVender_OnGetOwnerDataInfo = { CC_STDCALL, VT_I4, 4, { VT_I4, VT_UNKNOWN, VT_I4, VT_VARIANT | VT_BYREF } };

//外部作成させない
//OBJECT_ENTRY_AUTO(__uuidof(IFRegVender), CIFRegVender)

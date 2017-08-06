// IFRegExp.h : CIFRegExp の宣言

#pragma once
#include "resource.h"       // メイン シンボル

#include "IF_MDL_i.h"
#include "_IIFRegExpEvents_CP.h"

#include "IFRegVender.h"
#include "RegExp_Parser.h"
#include "RegExp_RTF_Parser.h"
//#include "mem_container.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "DCOM の完全サポートを含んでいない Windows Mobile プラットフォームのような Windows CE プラットフォームでは、単一スレッド COM オブジェクトは正しくサポートされていません。ATL が単一スレッド COM オブジェクトの作成をサポートすること、およびその単一スレッド COM オブジェクトの実装の使用を許可することを強制するには、_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA を定義してください。ご使用の rgs ファイルのスレッド モデルは 'Free' に設定されており、DCOM Windows CE 以外のプラットフォームでサポートされる唯一のスレッド モデルと設定されていました。"
#endif

using namespace ATL;


// CIFRegExp

enum TRegExpOption;


class ATL_NO_VTABLE CIFRegExp :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CIFRegExp, &CLSID_IFRegExp>,
	public IConnectionPointContainerImpl<CIFRegExp>,
	public CProxy_IIFRegExpEvents<CIFRegExp>,
	public IDispatchImpl<IIFRegExp, &IID_IIFRegExp, &LIBID_IF_MDLLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IDispEventSimpleImpl<1, CIFRegExp, &__uuidof(_IIFRegVenderEvents)/*DIID__IIFRegVenderEvents*/>,
	public iregexp_callback<mem_container::const_iterator>
{
private:
	CComBSTR fPattern;
	TRegExpOption fOption;
	std::wstring parser_string;
	std::wstring pattern_string;

	iregexp_parser<mem_container::const_iterator, mem_container_compare> fparser;
	CComObject<CIFRegVender> *fvdr;
	CComPtr<IIFRegVender> fvender;
public:
	CIFRegExp()
		: fparser()
	{
		fPattern = L"";
		put_RegOption(optM);

		CComObject<CIFRegVender>::CreateInstance(&fvdr);
		fvdr->QueryInterface(IID_IIFRegVender, (void**)&fvender.p);

		fparser.init(fvdr->vender(), regular_expression_parser());
	}

DECLARE_REGISTRY_RESOURCEID(IDR_IFREGEXP)


BEGIN_COM_MAP(CIFRegExp)
	COM_INTERFACE_ENTRY(IIFRegExp)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CIFRegExp)
	CONNECTION_POINT_ENTRY(__uuidof(_IIFRegExpEvents))
END_CONNECTION_POINT_MAP()

BEGIN_SINK_MAP(CIFRegExp)
	SINK_ENTRY_INFO(1, __uuidof(_IIFRegVenderEvents), /*DISP_ID*/ 1, Vender_OnChange, &RegVender_OnChangeInfo)
END_SINK_MAP()


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		IDispEventSimpleImpl<1, CIFRegExp, &__uuidof(_IIFRegVenderEvents)/*DIID__IIFRegVenderEvents*/>::DispEventAdvise(fvender);
		InternalRelease();
		return S_OK;
	}

	void FinalRelease()
	{
		InternalAddRef();
		IDispEventSimpleImpl<1, CIFRegExp, &__uuidof(_IIFRegVenderEvents)/*DIID__IIFRegVenderEvents*/>::DispEventUnadvise(fvender);
	}

public:
	STDMETHOD(Vender_OnChange)();

public:
	virtual short OnSwitch(long param)
	{
		short ret = -1;
		Fire_OnSwitchQuery(GetUnknown(), param, &ret);
		return ret;
	}

	virtual void OnSelect(long param, short index, const mem_container::const_iterator vb, mem_container::const_iterator& vp, const mem_container::const_iterator ve)
	{
		Fire_OnSelectFunc(GetUnknown(), param, index, vp - vb, ve - vp);
	}

	virtual void OnIfAction(long param, bool success, const mem_container::const_iterator vb, mem_container::const_iterator& vp, const mem_container::const_iterator ve)
	{
		Fire_OnIfFunc(GetUnknown(), param, success ? VARIANT_TRUE : VARIANT_FALSE, vp - vb, ve - vp);
	}

	virtual void OnAction(long param, const mem_container::const_iterator vb, mem_container::const_iterator& vp, const mem_container::const_iterator ve)
	{
		Fire_OnActionFunc(GetUnknown(), param, vp - vb, ve - vp);
	}

	virtual void OnForInit(long param, const mem_container::const_iterator vb, mem_container::const_iterator& vp, const mem_container::const_iterator ve)
	{
		Fire_OnForInitializeFunc(GetUnknown(), param, vp - vb);
	}

	virtual void OnForReinit(long param, const mem_container::const_iterator vb, mem_container::const_iterator& vp, const mem_container::const_iterator ve)
	{
		Fire_OnForReinitializeFunc(GetUnknown(), param, vp - vb);
	}

	virtual bool OnQuery(long param, const mem_container::const_iterator vb, mem_container::const_iterator& vp, const mem_container::const_iterator ve)
	{
		VARIANT_BOOL ret = VARIANT_TRUE;
		long idx = vp - vb;
		Fire_OnQueryFunc(GetUnknown(), param, &idx, ve - vb, &ret);
		if (ret == VARIANT_TRUE && vb + idx <= ve)
		{
			vp = vb + idx;
			return true;
		}
		else
			return false;
	}

	virtual void OnFunc(long param, const mem_container::const_iterator vb, mem_container::const_iterator& vp, const mem_container::const_iterator ve)
	{
		Fire_OnCallFunc(GetUnknown(), param);
	}

public:



	STDMETHOD(MatchPosition)(LONG index, LONG* position);
	STDMETHOD(MatchLength)(LONG index, LONG* length);
	STDMETHOD(get_ParserString)(BSTR* pVal);
	STDMETHOD(get_PatternString)(BSTR* pVal);
	STDMETHOD(get_Pattern)(BSTR* pVal);
	STDMETHOD(put_Pattern)(BSTR newVal);
	STDMETHOD(Match)(BSTR Value, LONG Index, LONG* retVal);
	STDMETHOD(Find)(BSTR Value, LONG Index = 0, VARIANT_BOOL LoopFirst = VARIANT_TRUE);
	STDMETHOD(SetPatternData)(CHAR* Memory, ULONG MemorySize, ULONG DataOffset, ULONG DataLength, ULONG DataGap);
	STDMETHOD(RunMatchData)(CHAR* Memory, ULONG MemorySize, ULONG DataOffset, ULONG DataLength, ULONG DataGap, LONG* Index, VARIANT_BOOL* retVal);
	STDMETHOD(RunFindData)(CHAR* Memory, ULONG MemorySize, ULONG DataOffset, ULONG DataLength, ULONG DataGap, LONG Index = 0, VARIANT_BOOL LoopFirst = VARIANT_TRUE);
	STDMETHOD(get_RegOption)(TRegExpOption* pVal);
	STDMETHOD(put_RegOption)(TRegExpOption newVal);
	STDMETHOD(get_Vender)(IIFRegVender** pVal);
	STDMETHOD(rtf_text)(BSTR rtf, ULONG size, BSTR* text);
};

OBJECT_ENTRY_AUTO(__uuidof(IFRegExp), CIFRegExp)

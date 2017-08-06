// RegVender.h : CRegVender �̐錾

#pragma once
#include "resource.h"       // ���C�� �V���{��

#include "IF_MDL_i.h"
#include "_IIFRegVenderEvents_CP.H"

#include	"RegExp_Vender.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "DCOM �̊��S�T�|�[�g���܂�ł��Ȃ� Windows Mobile �v���b�g�t�H�[���̂悤�� Windows CE �v���b�g�t�H�[���ł́A�P��X���b�h COM �I�u�W�F�N�g�͐������T�|�[�g����Ă��܂���BATL ���P��X���b�h COM �I�u�W�F�N�g�̍쐬���T�|�[�g���邱�ƁA����т��̒P��X���b�h COM �I�u�W�F�N�g�̎����̎g�p�������邱�Ƃ���������ɂ́A_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ���`���Ă��������B���g�p�� rgs �t�@�C���̃X���b�h ���f���� 'Free' �ɐݒ肳��Ă���ADCOM Windows CE �ȊO�̃v���b�g�t�H�[���ŃT�|�[�g�����B��̃X���b�h ���f���Ɛݒ肳��Ă��܂����B"
#endif


using namespace ATL;


// CIFRegVender



class ATL_NO_VTABLE CIFRegVender :
	public CComObjectRootEx<CComSingleThreadModel>,
//�O���쐬�����Ȃ�
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

//�O���쐬�����Ȃ�
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

//�O���쐬�����Ȃ�
//OBJECT_ENTRY_AUTO(__uuidof(IFRegVender), CIFRegVender)

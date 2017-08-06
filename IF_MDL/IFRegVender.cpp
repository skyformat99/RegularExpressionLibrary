// RegVender.cpp : CRegVender ‚ÌŽÀ‘•

#include "stdafx.h"
#include "IFRegVender.h"


// CRegVender



STDMETHODIMP CIFRegVender::get_MemorySize(TRegExpMemorySize* pVal)
{
	switch (fvender.length())
	{
	case 1:		*pVal = msSingleByte;		break;
	case 2:		*pVal = msDoubleByte;		break;
	case 4:		*pVal = msQuadrupleByte;	break;
	case 8:		*pVal = msOctupleByte;		break;
	default:	*pVal = msCustom;			break;
	}
	return S_OK;
}


STDMETHODIMP CIFRegVender::put_MemorySize(TRegExpMemorySize newVal)
{
	HRESULT ret = S_OK;
	if (newVal != fvender.length())
	{
		switch (newVal)
		{
		case msSingleByte:		fvender = iregexp_vender(1);	Fire_OnChange();	break;
		case msDoubleByte:		fvender = iregexp_vender(2);	Fire_OnChange();	break;
		case msQuadrupleByte:	fvender = iregexp_vender(4);	Fire_OnChange();	break;
		case msOctupleByte:		fvender = iregexp_vender(8);	Fire_OnChange();	break;
		default:				ret = E_FAIL;					break;
		}
	}
	return ret;
}


STDMETHODIMP CIFRegVender::SetMemoryParam(CHAR* Memory, ULONG MemorySize, ULONG DataOffset, ULONG DataLength, ULONG DataGap)
{
	size_t size = MemorySize / (DataOffset + DataLength + DataGap);
	if (size != 128)
		return E_FAIL;
	fvender = iregexp_vender(DataLength, Memory, size, DataOffset, DataGap);
	Fire_OnChange();
	return S_OK;
}


STDMETHODIMP CIFRegVender::SetLocalMemoryParam(TRegVenderLocalMemoryType Type, CHAR* Memory, ULONG MemorySize, ULONG DataOffset, ULONG DataLength, ULONG DataGap)
{
	HRESULT ret = S_OK;
	//mem_container mem;
	//size_t size = MemorySize / (DataOffset + DataLength + DataGap);
	//mem.atatch(DataLength, Memory, size, DataOffset, DataGap);
	//switch (Type)
	//{
	//case lmtCtrl:
	//	if (size != 32)
	//		ret = E_FAIL;
	//	else
	//		fvender.ctrl_char() = mem;
	//	Fire_OnChange();
	//	break;
	//case lmtWord:	fvender.word_char() = mem;	Fire_OnChange();	break;
	//case lmtSpace:	fvender.space_char() = mem;	Fire_OnChange();	break;
	//case lmtBlanc:	fvender.blanc_char() = mem;	Fire_OnChange();	break;
	//case lmtUpper:	fvender.upper_char() = mem;	Fire_OnChange();	break;
	//case lmtLower:	fvender.lower_char() = mem;	Fire_OnChange();	break;
	//case lmtAlpha:	fvender.alpha_char() = mem;	Fire_OnChange();	break;
	//case lmtAlnum:	fvender.alnum_char() = mem;	Fire_OnChange();	break;
	//}
	return ret;
}

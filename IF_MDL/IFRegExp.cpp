// IFRegExp.cpp : CIFRegExp ‚ÌŽÀ‘•

#include "stdafx.h"
#include "IFRegExp.h"

// CIFRegExp


STDMETHODIMP CIFRegExp::Vender_OnChange()
{
	fparser.init(fvdr->vender(), regular_expression_parser());
	return S_OK;
}

STDMETHODIMP CIFRegExp::Match(BSTR Value, LONG Index, LONG* retVal)
{
	VARIANT_BOOL ret;
	LONG idx = Index;
	RunMatchData((CHAR*)Value, SysStringByteLen(Value), 0, fvdr->vender().length(), 0, &idx, &ret);
	*retVal = ret == VARIANT_TRUE ? idx - Index : 0;
	return S_OK;
}


STDMETHODIMP CIFRegExp::RunMatchData(CHAR* Memory, ULONG MemorySize, ULONG DataOffset, ULONG DataLength, ULONG DataGap, LONG* Index, VARIANT_BOOL* retVal)
{
	if (fvdr->vender().length() != DataLength)
		return E_FAIL;
	mem_container pat;
	size_t size = MemorySize / (DataOffset + DataLength + DataGap);
	pat.atatch(DataLength, Memory, size, DataOffset, DataGap);
	mem_container::const_iterator it = pat.begin();
	it += *Index;

	if (fparser.match(pat.begin(), it, pat.end()))
	{
		*Index = it - pat.begin();
		*retVal = VARIANT_TRUE;
	}
	else
		*retVal = VARIANT_FALSE;
	return S_OK;
}


STDMETHODIMP CIFRegExp::Find(BSTR Value, LONG Index, VARIANT_BOOL LoopFirst)
{
	RunFindData((CHAR*)Value, SysStringByteLen(Value), 0, fvdr->vender().length(), 0, Index, LoopFirst);
	return S_OK;
}


STDMETHODIMP CIFRegExp::RunFindData(CHAR* Memory, ULONG MemorySize, ULONG DataOffset, ULONG DataLength, ULONG DataGap, LONG Index, VARIANT_BOOL LoopFirst)
{
	if (fvdr->vender().length() != DataLength)
		return E_FAIL;
	mem_container pat;
	size_t size = MemorySize / (DataOffset + DataLength + DataGap);
	pat.atatch(DataLength, Memory, size, DataOffset, DataGap);
	VARIANT_BOOL next = VARIANT_TRUE;
	mem_container::const_iterator it = pat.begin() + Index;
	for (size_t pos = 0; pos < size && next == VARIANT_TRUE;)
	{
		mem_container::const_iterator i = it;
		if (fparser.match(pat.begin(), it, pat.end()))
		{
			Fire_OnMatch(GetUnknown(), i - pat.begin(), it - pat.begin(), &next);
			pos += it - i;
		}
		else
		{
			++it;
			++pos;
		}
		if (it == pat.end() && pos < size)
		{
			if (LoopFirst == VARIANT_TRUE)
				it = pat.begin();
			else
				break;
		}
	}
	return S_OK;
}


STDMETHODIMP CIFRegExp::get_Pattern(BSTR* pVal)
{
	*pVal = fPattern.Copy();
	return S_OK;
}


STDMETHODIMP CIFRegExp::put_Pattern(BSTR newVal)
{
	fPattern = L"";
	mem_container pat;
	pat.atatch(fvdr->vender().length(), newVal, SysStringByteLen(newVal) / fvdr->vender().length(), 0, 0);

	if (fparser.parse((fOption & optI) == optI, (fOption & optS) == optS, (fOption & optM) == optM, (fOption & optX) == optX, pat.begin(), pat.end(), this))
	{
		fPattern = newVal;
		return S_OK;
	}
	else
		return E_FAIL;
}


STDMETHODIMP CIFRegExp::SetPatternData(CHAR* Memory, ULONG MemorySize, ULONG DataOffset, ULONG DataLength, ULONG DataGap)
{
	fPattern = L"";
	mem_container pat;
	pat.atatch(DataLength, Memory, MemorySize / (DataOffset + DataLength + DataGap), DataOffset, DataGap);

	if (fparser.parse((fOption & optI) == optI, (fOption & optS) == optS, (fOption & optM) == optM, (fOption & optX) == optX, pat.begin(), pat.end(), this))
	{
		fPattern = SysAllocStringByteLen(Memory, MemorySize);
		return S_OK;
	}
	else
		return E_FAIL;
}


STDMETHODIMP CIFRegExp::MatchPosition(LONG index, LONG* position)
{
	*position = fparser.matchs(index).first;
	return S_OK;
}


STDMETHODIMP CIFRegExp::MatchLength(LONG index, LONG* length)
{
	*length = fparser.matchs(index).second - fparser.matchs(index).first;
	return S_OK;
}


STDMETHODIMP CIFRegExp::get_ParserString(BSTR* pVal)
{
	*pVal = SysAllocString(fparser.get_parser_str().c_str());
	return S_OK;
}


STDMETHODIMP CIFRegExp::get_PatternString(BSTR* pVal)
{
	*pVal = SysAllocString(fparser.get_pattern_str().c_str());
	return S_OK;
}


STDMETHODIMP CIFRegExp::get_RegOption(TRegExpOption* pVal)
{
	*pVal = fOption;
	return S_OK;
}


STDMETHODIMP CIFRegExp::put_RegOption(TRegExpOption newVal)
{
	if (fOption != newVal)
	{
		fOption = newVal;
	}
	return S_OK;
}


STDMETHODIMP CIFRegExp::get_Vender(IIFRegVender** pVal)
{
	return fvender->QueryInterface(pVal);
}


STDMETHODIMP CIFRegExp::rtf_text(BSTR rtf, ULONG size, BSTR* text)
{

	iregexp_vender vdr(size);
	iregexp_rtf_parser<mem_container::const_iterator, mem_container_compare> psr(vdr, reg_rtf_parser());

	mem_container mem;
	mem.atatch(size, rtf, SysStringByteLen(rtf) / size, 0, 0);
	bool ret = psr.parse(mem.begin(), mem.end());

	if (!ret)
		return  E_FAIL;

	SysFreeString(*text);
	*text = SysAllocStringLen(psr.get_string().c_str(), psr.get_string().length());

	return S_OK;
}

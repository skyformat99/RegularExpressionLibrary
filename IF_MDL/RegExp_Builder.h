#pragma once

#include	"tree.h"
#include	"RegExp.h"
#include	"RegExp_Vender.h"

class iregexp_builder_base
{
public:
	enum reg_command
	{
		rcNull,
		rcDefine,
		rcCh,
		rcNotC,
		rcRange,
		rcSet,
		rcNot,
		rcStr,
		rcXch,
		rcCall,
		rcPrevCall,
		rcNextCall,
		rcNameCall,
		rcUse,
		rcPrevUse,
		rcNextUse,
		rcNameUse,
		rcIsMatch,
		rcSignIsMatch,
		rcNameIsMatch,
		rcAny,
		rcDigit,
		rcXDigit,
		rcAlpha,
		rcWord,
		rcUpper,
		rcLower,
		rcSpace,
		rcCtrl,
		rcBegin,
		rcEnd,
		rcWhile,
		rcDoWhile,
		rcTrue,
		rcClasster,
		rcCapture,
		rcNameCapture,
		rcOrTie,
		rcSelect,
		rcMapSelect,
		rcListSelect,
		rcListSelectDeltaPrev,
		rcListSelectDeltaNext,
		rcListSelectName,
		rcSwitch,
		rcMapSwitch,
		rcSetMap,
		rcSetMapDeltaPrev,
		rcSetMapDeltaNext,
		rcSetMapName,
		rcMapPush,
		rcMapPop,
		rcSetMapValue,
		rcCompMapValueEqual,
		rcCompMapValueNot,
		rcCompMapValueLarge,
		rcCompMapValueSmall,
		rcCompMapValueMore,
		rcCompMapValueLess,
		rcIf,
		rcNotIf,
		rcIfAction,
		rcAction,
		rcNotAction,
		rcQuery,
		rcFunc,
		rcAfterEqual,
		rcAfterNot,
		rcBeforEqual,
		rcBeforNot,
	};
	struct reg_match_option
	{
		enum reg_repeat_type
		{
			rtBinary,
			rtAny,
			rtOver,
		};
		enum reg_repeat_option
		{
			roLimited,
			roForce,
			roWeak,
		};
		int fmin;
		int fmax;
		reg_repeat_option type;
		reg_match_option() : fmin(1), fmax(1), type(roLimited) {}
		reg_match_option(reg_repeat_type type) : type(roLimited) { type == rtBinary ? fmin = 0, fmax = 1 : type == rtAny ? fmin = 0, fmax = -1 : fmin = 1, fmax = -1; }
		reg_match_option(int min, int max) : fmin(min), fmax(max), type(roLimited) {}
		reg_match_option& setLimited(void) { fmin = 1, fmax = 1; type = roLimited; return *this; }
		reg_match_option& setForce(void) { type = roForce; return *this; }
		reg_match_option& setWeak(void) { type = roWeak; return *this; }
		reg_match_option& setBinary(void) { fmin = 0, fmax = 1; return *this; }
		reg_match_option& setAny(void) { fmin = 0, fmax = -1; return *this; }
		reg_match_option& setOver(void) { fmin = 1, fmax = -1; return *this; }
		reg_match_option& setLimit(int min) { fmin = min, fmax = -1; return *this; }
		reg_match_option& setLimit(int min, int max) { fmin = min, fmax = max; return *this; }
	};
	struct reg_range_ch
	{
		char sch;
		char ech;
	};
	union rag_content
	{
		char ch;
		reg_range_ch range;
		char* str;
		int i;
	};
	class reg_item
	{
	public:
		reg_command cmd;
		rag_content cnt;
		rag_content cnt2;
		int value;
		reg_match_option opt;
		char* map;
		reg_item() : cmd(rcNull) {}
		reg_item(char c, bool t = false) { cmd = t ? rcNotC: rcCh; cnt.ch = c; }
		reg_item(char s, char e) { cmd = rcRange; cnt.range.sch = s; cnt.range.ech = e; }
		reg_item(char* s) { cmd = rcSet; cnt.str = s; }
		reg_item(char* s, bool t) { cmd = (t ? rcStr : rcNot); cnt.str = s; }
		reg_item(int i, char* s, char* m)
		{
			cmd = rcSetMap;
			cnt.str = s;
			cnt2.i = i;
			map = m;
		}
		reg_item(int i, bool prev, char* s, char* m)
		{
			cmd = prev ? rcSetMapDeltaPrev : rcSetMapDeltaNext;
			cnt.str = s;
			cnt2.i = i;
			map = m;
		}
		reg_item(char* n, char* s, char* m)
		{
			cmd = rcSetMapName;
			cnt.str = s;
			cnt2.str = n;
			map = m;
		}
		reg_item(int i, char* m, int val)
		{
			cmd = rcListSelect;
			cnt.i = i;
			cnt2.i = val;
			map = m;
		}
		reg_item(char* m, int val)
		{
			cmd = rcSetMapValue;
			cnt.i = val;
			map = m;
		}
		reg_item(char* m, int val, bool)
		{
			cmd = rcCompMapValueEqual;
			cnt.i = val;
			map = m;
		}
		reg_item(char* m, int val, bool, bool)
		{
			cmd = rcCompMapValueNot;
			cnt.i = val;
			map = m;
		}
		reg_item(char* m, int val, bool, bool, bool)
		{
			cmd = rcCompMapValueLarge;
			cnt.i = val;
			map = m;
		}
		reg_item(char* m, int val, bool, bool, bool, bool)
		{
			cmd = rcCompMapValueSmall;
			cnt.i = val;
			map = m;
		}
		reg_item(char* m, int val, bool, bool, bool, bool, bool)
		{
			cmd = rcCompMapValueMore;
			cnt.i = val;
			map = m;
		}
		reg_item(char* m, int val, bool, bool, bool, bool, bool, bool)
		{
			cmd = rcCompMapValueLess;
			cnt.i = val;
			map = m;
		}
		reg_item(int i, bool prev, char* m, int val)
		{
			cmd = prev ? rcListSelectDeltaPrev : rcListSelectDeltaNext;
			cnt.i = i;
			cnt2.i = val;
			map = m;
		}
		reg_item(char* n, char* m, int val)
		{
			cmd = rcListSelectName;
			cnt.str = n;
			cnt2.i = val;
			map = m;
		}
		reg_item(reg_command c) { cmd = c; }
		reg_item(reg_command c, int i) { cmd = c; cnt.i = i; }
		reg_item(reg_command c, char* s) { cmd = c; cnt.str = s; }
		reg_item(int index, bool t) { cmd = (t ? rcCall : rcUse); cnt.i = index; }
		reg_item(int index, bool t, bool f) { cmd = f ? (t ? rcPrevCall : rcPrevUse) : (t ? rcNextCall : rcNextUse); cnt.i = index; }
		reg_item& operator + () { opt.setOver(); return *this; }
		reg_item& operator * () { opt.setAny(); return *this; }
		reg_item& operator ! () { opt.setBinary(); return *this; }
		reg_item& operator ++ () { opt.setForce(); return *this; }
		reg_item& operator -- () { opt.setWeak(); return *this; }
		reg_item& set_limit(int min) { opt.setLimit(min); return *this; }
		reg_item& set_limit(int min, int max) { opt.setLimit(min, max); return *this; }
	};
	typedef tree<reg_item> bld_content;
private:
protected:
	template<typename target_iterator, typename target_compare = default_compare_class>
	iregexp_base<target_iterator, target_compare> if_build(iregexp_vender& vdr, iregexp_callback<target_iterator>* callback, bld_content::const_iterator b, bld_content::const_iterator e, bool not = false) const
	{
		if_regexp<target_iterator, target_compare> ret(build<target_iterator, target_compare>(vdr, callback, b.first_child(), e), not);
		if (b.move_next_sibling() != e)
			ret[build<target_iterator, target_compare>(vdr, callback, b.first_child(), e)];
		if (b.move_next_sibling() != e)
			ret.else_p(build<target_iterator, target_compare>(vdr, callback, b.first_child(), e));
		return ret;
	}
	template<typename target_iterator, typename target_compare = default_compare_class>
	iregexp_base<target_iterator, target_compare> switch_build(iregexp_vender& vdr, int index, iregexp_callback<target_iterator>* callback, bld_content::const_iterator b, bld_content::const_iterator e) const
	{
		switch_regexp<target_iterator, target_compare> ret(index, callback);
		ret.default_p(build<target_iterator, target_compare>(vdr, callback, b.first_child(), e));
		while (b.move_next_sibling() != e)
			ret[build<target_iterator, target_compare>(vdr, callback, b.first_child(), e)];
		return ret;
	}
	template<typename target_iterator, typename target_compare = default_compare_class>
	iregexp_base<target_iterator, target_compare> select_build(iregexp_vender& vdr, int index, iregexp_callback<target_iterator>* callback, bld_content::const_iterator b, bld_content::const_iterator e) const
	{
		select_regexp<target_iterator, target_compare> ret(index, callback);
		do
			ret[build<target_iterator, target_compare>(vdr, callback, b.first_child(), e)];
		while (b.move_next_sibling() != e);
		return ret;
	}
	template<typename target_iterator, typename target_compare = default_compare_class>
	iregexp_base<target_iterator, target_compare> if_action_build(iregexp_vender& vdr, int index, iregexp_callback<target_iterator>* callback, bld_content::const_iterator b, bld_content::const_iterator e) const
	{
		if_action_regexp<target_iterator, target_compare> ret(index, callback);
		ret[build<target_iterator, target_compare>(vdr, callback, b.first_child(), e)];
		return ret;
	}
	template<typename target_iterator, typename target_compare = default_compare_class>
	iregexp_base<target_iterator, target_compare> action_build(iregexp_vender& vdr, int index, iregexp_callback<target_iterator>* callback, bld_content::const_iterator b, bld_content::const_iterator e) const
	{
		action_regexp<target_iterator, target_compare> ret(index, callback);
		ret[build<target_iterator, target_compare>(vdr, callback, b, e)];
		return ret;
	}
	template<typename target_iterator, typename target_compare = default_compare_class>
	iregexp_base<target_iterator, target_compare> not_action_build(iregexp_vender& vdr, int index, iregexp_callback<target_iterator>* callback, bld_content::const_iterator b, bld_content::const_iterator e) const
	{
		action_regexp<target_iterator, target_compare> ret(index, callback, true);
		ret[build<target_iterator, target_compare>(vdr, callback, b, e)];
		return ret;
	}
	template<typename target_iterator, typename target_compare = default_compare_class>
	iregexp_base<target_iterator, target_compare> classter_build(iregexp_vender& vdr, iregexp_callback<target_iterator>* callback, bld_content::const_iterator b, bld_content::const_iterator e) const
	{
		iregexp_base<target_iterator, target_compare> ret(build<target_iterator, target_compare>(vdr, callback, b, e));
		ret.classter_items();
		return ret;
	}
	template<typename target_iterator, typename target_compare = default_compare_class>
	iregexp_base<target_iterator, target_compare> capture_build(iregexp_vender& vdr, iregexp_callback<target_iterator>* callback, bld_content::const_iterator b, bld_content::const_iterator e) const
	{
		capture_regexp<target_iterator, target_compare> ret(build<target_iterator, target_compare>(vdr, callback, b, e));
		return ret;
	}
	template<typename target_iterator, typename target_compare = default_compare_class>
	iregexp_base<target_iterator, target_compare> name_capture_build(iregexp_vender& vdr, iregexp_callback<target_iterator>* callback, char* s, bld_content::const_iterator b, bld_content::const_iterator e) const
	{
		capture_regexp<target_iterator, target_compare> ret(vdr.str<target_iterator>(s));
		ret[build<target_iterator, target_compare>(vdr, callback, b, e)];
		return ret;
	}
	template<typename target_iterator, typename target_compare = default_compare_class>
	iregexp_base<target_iterator, target_compare> define_build(iregexp_vender& vdr, iregexp_callback<target_iterator>* callback, bld_content::const_iterator b, bld_content::const_iterator e) const
	{
		null_regexp<target_iterator, target_compare> ret;
		ret[build<target_iterator, target_compare>(vdr, callback, b, e)];
		return ret;
	}
	template<typename target_iterator, typename target_compare = default_compare_class>
	iregexp_base<target_iterator, target_compare> null_build(iregexp_vender& vdr, iregexp_callback<target_iterator>* callback, bld_content::const_iterator b, bld_content::const_iterator e) const
	{
		null_regexp<target_iterator, target_compare> ret(build<target_iterator, target_compare>(vdr, callback, b, e));
		return ret;
	}
	template<typename target_iterator, typename target_compare = default_compare_class>
	iregexp_base<target_iterator, target_compare> or_build(iregexp_vender& vdr, iregexp_callback<target_iterator>* callback, bld_content::const_iterator b, bld_content::const_iterator e) const
	{
		iregexp_base<target_iterator, target_compare> ret(build<target_iterator, target_compare>(vdr, callback, b.first_child(), e));
		while (b.move_next_sibling() != e)
			ret |= build<target_iterator, target_compare>(vdr, callback, b.first_child(), e);
		return ret;
	}
	template<typename target_iterator, typename target_compare = default_compare_class>
	iregexp_base<target_iterator, target_compare> while_build(iregexp_vender& vdr, iregexp_callback<target_iterator>* callback, bld_content::const_iterator b, bld_content::const_iterator e) const
	{
		while_regexp<target_iterator, target_compare> ret(build<target_iterator, target_compare>(vdr, callback, b.first_child(), e));
		if (b.move_next_sibling() != e)
			ret[build<target_iterator, target_compare>(vdr, callback, b.first_child(), e)];
		return ret;
	}
	template<typename target_iterator, typename target_compare = default_compare_class>
	iregexp_base<target_iterator, target_compare> do_while_build(iregexp_vender& vdr, iregexp_callback<target_iterator>* callback, bld_content::const_iterator b, bld_content::const_iterator e) const
	{
		do_while_regexp<target_iterator, target_compare> ret(build<target_iterator, target_compare>(vdr, callback, b.first_child(), e));
		if (b.move_next_sibling() != e)
			ret.while_p(build<target_iterator, target_compare>(vdr, callback, b.first_child(), e));
		return ret;
	}
	template<typename target_iterator, typename target_compare = default_compare_class>
	iregexp_base<target_iterator, target_compare> map_switch_build(iregexp_vender& vdr, const std::pair<target_iterator, target_iterator>& name, iregexp_callback<target_iterator>* callback, bld_content::const_iterator b, bld_content::const_iterator e) const
	{
		map_switch_regexp<target_iterator, target_compare> ret(name);
		ret.default_p(build<target_iterator, target_compare>(vdr, callback, b.first_child(), e));
		while (b.move_next_sibling() != e)
			ret[build<target_iterator, target_compare>(vdr, callback, b.first_child(), e)];
		//do
		//	ret[build<target_iterator, target_compare>(vdr, callback, b.first_child(), e)];
		//while (b.move_next_sibling() != e);
		return ret;
	}
	template<typename target_iterator, typename target_compare = default_compare_class>
	iregexp_base<target_iterator, target_compare> map_select_build(iregexp_vender& vdr, const std::pair<target_iterator, target_iterator>& name, iregexp_callback<target_iterator>* callback, bld_content::const_iterator b, bld_content::const_iterator e) const
	{
		map_select_regexp<target_iterator, target_compare> ret(name);
		do
			ret[build<target_iterator, target_compare>(vdr, callback, b.first_child(), e)];
		while (b.move_next_sibling() != e);
		return ret;
	}
	template<typename target_iterator, typename target_compare = default_compare_class>
	iregexp_base<target_iterator, target_compare> build(iregexp_vender& vdr, iregexp_callback<target_iterator>* callback, bld_content::const_iterator b, bld_content::const_iterator e) const
	{
		iregexp_base<target_iterator, target_compare> ret;
		while (b != e)
		{
			iregexp_base<target_iterator, target_compare> reg;
			bld_content::const_iterator i = b.first_child();
			bool f = i != e;
			switch (b->cmd)
			{
			case rcNull:		reg = null_build<target_iterator, target_compare>(vdr, callback, i, e); break;
			case rcDefine:		reg = define_build<target_iterator, target_compare>(vdr, callback, i, e); break;
			case rcCh:			reg = character_parallel_regexp<target_iterator, target_compare>(vdr.ch<target_iterator>(b->cnt.ch)); break;
			case rcNotC:		reg = character_parallel_regexp<target_iterator, target_compare>(vdr.ch<target_iterator>(b->cnt.ch), true); break;
//			case rcRange:		reg = character_parallel_regexp<target_iterator, target_compare>(vdr.range_set<target_iterator>(b->cnt.range.sch, b->cnt.range.ech)); break;
			case rcSet:			reg = character_parallel_regexp<target_iterator, target_compare>(vdr.str<target_iterator>(b->cnt.str)); break;
			case rcNot:			reg = character_parallel_regexp<target_iterator, target_compare>(vdr.str<target_iterator>(b->cnt.str), true); break;
			case rcStr:			reg = character_serial_regexp<target_iterator, target_compare>(vdr.str<target_iterator>(b->cnt.str)); break;
			case rcXch:			reg = null_regexp<target_iterator, target_compare>(); break;
			case rcUse:			reg = use_match_capture_regexp<target_iterator, target_compare>(b->cnt.i); break;
			case rcCall:		reg = call_capture_regexp<target_iterator, target_compare>(b->cnt.i); break;
			case rcPrevCall:	reg = call_capture_regexp<target_iterator, target_compare>(b->cnt.i, true); break;
			case rcNextCall:	reg = call_capture_regexp<target_iterator, target_compare>(b->cnt.i, false); break;
			case rcNameCall:	reg = call_capture_regexp<target_iterator, target_compare>(vdr.str<target_iterator>(b->cnt.str)); break;
			case rcIsMatch:		reg = is_match_capture_regexp<target_iterator, target_compare>(b->cnt.i); break;
			case rcAny:			reg = any_char_regexp<target_iterator, target_compare>(); break;
			case rcDigit:		reg = character_parallel_regexp<target_iterator, target_compare>(vdr.digit_char<target_iterator>()); break;
			case rcXDigit:		reg = character_parallel_regexp<target_iterator, target_compare>(vdr.xdigit_char<target_iterator>()); break;
			case rcAlpha:		reg = character_parallel_regexp<target_iterator, target_compare>(vdr.alpha_char<target_iterator>()); break;
			case rcWord:		reg = character_parallel_regexp<target_iterator, target_compare>(vdr.word_char<target_iterator>()); break;
			case rcUpper:		reg = character_parallel_regexp<target_iterator, target_compare>(vdr.upper_char<target_iterator>()); break;
			case rcLower:		reg = character_parallel_regexp<target_iterator, target_compare>(vdr.lower_char<target_iterator>()); break;
			case rcSpace:		reg = character_parallel_regexp<target_iterator, target_compare>(vdr.space_char<target_iterator>()); break;
			case rcCtrl:		reg = character_parallel_regexp<target_iterator, target_compare>(vdr.ctrl_char<target_iterator>()); break;
			case rcBegin:		reg = begin_regexp<target_iterator, target_compare>(); break;
			case rcEnd:			reg = end_regexp<target_iterator, target_compare>(); break;
			case rcTrue:		reg = null_regexp<target_iterator, target_compare>(); break;
			case rcClasster:	reg = classter_build<target_iterator, target_compare>(vdr, callback, i, e); break;
			case rcCapture:		reg = capture_build<target_iterator, target_compare>(vdr, callback, i, e); break;
			case rcNameCapture:	reg = name_capture_build<target_iterator, target_compare>(vdr, callback, b->cnt.str, i, e); break;
			case rcOrTie:		reg = or_build<target_iterator, target_compare>(vdr, callback, i, e); break;
			case rcIf:			reg = if_build<target_iterator, target_compare>(vdr, callback, i, e); break;
			case rcNotIf:		reg = if_build<target_iterator, target_compare>(vdr, callback, i, e, true); break;
			case rcSelect:		reg = select_build<target_iterator, target_compare>(vdr, b->cnt.i, callback, i, e); break;
			case rcSwitch:		reg = switch_build<target_iterator, target_compare>(vdr, b->cnt.i, callback, i, e); break;
			case rcIfAction:	reg = if_action_build<target_iterator, target_compare>(vdr, b->cnt.i, callback, i, e); break;
			case rcAction:		reg = action_build<target_iterator, target_compare>(vdr, b->cnt.i, callback, i, e); break;
			case rcNotAction:	reg = not_action_build<target_iterator, target_compare>(vdr, b->cnt.i, callback, i, e); break;
			case rcFunc:		reg = func_regexp<target_iterator, target_compare>(b->cnt.i, callback); break;
			case rcQuery:		reg = query_regexp<target_iterator, target_compare>(b->cnt.i, callback); break;
			case rcAfterEqual:	reg = after_assertion_regexp<target_iterator, target_compare>(build<target_iterator, target_compare>(vdr, callback, i, e)); break;
			case rcAfterNot:	reg = after_assertion_regexp<target_iterator, target_compare>(build<target_iterator, target_compare>(vdr, callback, i, e), true); break;
			case rcBeforEqual:	reg = befor_assertion_regexp<target_iterator, target_compare>(build<target_iterator, target_compare>(vdr, callback, i, e)); break;
			case rcBeforNot:	reg = befor_assertion_regexp<target_iterator, target_compare>(build<target_iterator, target_compare>(vdr, callback, i, e), true); break;
			case rcWhile:		reg = while_build<target_iterator, target_compare>(vdr, callback, i, e); break;
			case rcDoWhile:		reg = do_while_build<target_iterator, target_compare>(vdr, callback, i, e); break;
			case rcSetMap:				reg = map_variable_regexp<target_iterator, target_compare>(b->cnt2.i, vdr.str<target_iterator>(b->cnt.str), vdr.str<target_iterator>(b->map)); break;
			case rcSetMapName:			reg = map_variable_regexp<target_iterator, target_compare>(vdr.str<target_iterator>(b->cnt2.str), vdr.str<target_iterator>(b->cnt.str), vdr.str<target_iterator>(b->map)); break;
			case rcSetMapDeltaPrev:		reg = map_variable_regexp<target_iterator, target_compare>(b->cnt2.i, true, vdr.str<target_iterator>(b->cnt.str), vdr.str<target_iterator>(b->map)); break;
			case rcSetMapDeltaNext:		reg = map_variable_regexp<target_iterator, target_compare>(b->cnt2.i, false, vdr.str<target_iterator>(b->cnt.str), vdr.str<target_iterator>(b->map)); break;
			case rcMapPush:				reg = map_variable_regexp<target_iterator, target_compare>(true); break;
			case rcMapPop:				reg = map_variable_regexp<target_iterator, target_compare>(false); break;
			case rcMapSwitch:			reg = map_switch_build<target_iterator, target_compare>(vdr, vdr.str<target_iterator>(b->cnt.str), callback, i, e); break;
			case rcSetMapValue:			reg = map_variable_regexp<target_iterator, target_compare>(b->cnt.i, vdr.str<target_iterator>(b->map)); break;
			case rcCompMapValueEqual:	reg = map_variable_regexp<target_iterator, target_compare>(map_variable_regexp<target_iterator, target_compare>::ctEqual, b->cnt.i, vdr.str<target_iterator>(b->map)); break;
			case rcCompMapValueNot:		reg = map_variable_regexp<target_iterator, target_compare>(map_variable_regexp<target_iterator, target_compare>::ctNot, b->cnt.i, vdr.str<target_iterator>(b->map)); break;
			case rcCompMapValueLarge:	reg = map_variable_regexp<target_iterator, target_compare>(map_variable_regexp<target_iterator, target_compare>::ctLarge, b->cnt.i, vdr.str<target_iterator>(b->map)); break;
			case rcCompMapValueSmall:	reg = map_variable_regexp<target_iterator, target_compare>(map_variable_regexp<target_iterator, target_compare>::ctSmall, b->cnt.i, vdr.str<target_iterator>(b->map)); break;
			case rcCompMapValueMore:	reg = map_variable_regexp<target_iterator, target_compare>(map_variable_regexp<target_iterator, target_compare>::ctMore, b->cnt.i, vdr.str<target_iterator>(b->map)); break;
			case rcCompMapValueLess:	reg = map_variable_regexp<target_iterator, target_compare>(map_variable_regexp<target_iterator, target_compare>::ctLess, b->cnt.i, vdr.str<target_iterator>(b->map)); break;
			case rcListSelect:			reg = list_select_regexp<target_iterator, target_compare>(b->cnt.i, vdr.str<target_iterator>(b->map), b->cnt2.i, callback); break;
			case rcListSelectName:		reg = list_select_regexp<target_iterator, target_compare>(vdr.str<target_iterator>(b->cnt.str), vdr.str<target_iterator>(b->map), b->cnt2.i, callback); break;
			case rcListSelectDeltaPrev:	reg = list_select_regexp<target_iterator, target_compare>(b->cnt.i, true, vdr.str<target_iterator>(b->map), b->cnt2.i, callback); break;
			case rcListSelectDeltaNext:	reg = list_select_regexp<target_iterator, target_compare>(b->cnt.i, false, vdr.str<target_iterator>(b->map), b->cnt2.i, callback); break;
			case rcMapSelect:			reg = map_select_build<target_iterator, target_compare>(vdr, vdr.str<target_iterator>(b->cnt.str), callback, i, e); break;
			}
			reg.limited_match(b->opt.fmin, b->opt.fmax);
			if (b->opt.type == reg_match_option::roForce)
				reg.force_match();
			if (b->opt.type == reg_match_option::roWeak)
				reg.weak_match();
			ret >>= reg;
			b.move_next_sibling();
		}
		return ret;
	}
};

class iregexp_builder
	: public iregexp_builder_base
{
private:
	bld_content fcontent;
protected:
	void init(void)
	{
		fcontent.clear();
		fcontent.add(fcontent.end(), reg_item(rcClasster));
	}
	bld_content& content(void) { return fcontent; }
	const bld_content& content(void) const { return fcontent; }
	bld_content::iterator begin(void) { return content().begin(); }
	bld_content::const_iterator begin(void) const { return content().begin(); }
	bld_content::iterator end(void) { return content().end(); }
	bld_content::const_iterator end(void) const { return content().end(); }
	bld_content::iterator classter(bool force = false)
	{
		bld_content::iterator ret = fcontent.begin().first_child();
		if (fcontent.size() > 1 && (force || fcontent.begin().count_child() > 1))
		{
			ret = fcontent.insert(fcontent.begin().first_child(), reg_item(rcClasster));
			bld_content::iterator i = ret.next_sibling();
			while (i != fcontent.end())
			{
				fcontent.inc_indent(i);
				i = ret.next_sibling();
			}
		}
		return ret;
	}
	iregexp_builder& and_tie(const iregexp_builder& v)
	{
		if (v.fcontent.size() == 1)
			return *this;
		if (fcontent.size() == 1)
			return (*this = v);
		fcontent.add(fcontent.begin(), v.fcontent.begin().first_child(), v.fcontent.end());
		return *this;
	}
	iregexp_builder& or_tie(const iregexp_builder& v)
	{
		if (v.fcontent.size() == 1)
			return *this;
		if (fcontent.size() == 1)
			return (*this = v);

		bld_content::iterator it;
		if (fcontent.begin().first_child()->cmd == rcOrTie)
		{
			it = fcontent.begin().first_child();
		}
		else
		{
			it = fcontent.insert(fcontent.begin().first_child(), reg_item(rcOrTie));
			bld_content::iterator i = it.next_sibling();
			while (i != fcontent.end())
			{
				fcontent.inc_indent(i);
				i = it.next_sibling();
			}
			i = fcontent.insert(it.first_child(), reg_item(rcClasster));
			bld_content::iterator ii = i.next_sibling();
			while (ii != fcontent.end())
			{
				fcontent.inc_indent(ii);
				ii = i.next_sibling();
			}
		}
		if (v.fcontent.begin().first_child()->cmd == rcOrTie && v.fcontent.begin().first_child().count_child() > 0)
		{
			fcontent.add(it, v.fcontent.begin().first_child().first_child(), v.fcontent.end());
		}
		else
		{
			bld_content::iterator i = fcontent.add(it, reg_item(rcClasster));
			fcontent.add(i, v.fcontent.begin().first_child(), v.fcontent.end());
		}
		return *this;
	}
public:
	iregexp_builder() { init(); }
	iregexp_builder(char c, bool t = false) { init(); fcontent.add(fcontent.begin(), reg_item(c, t)); }
	iregexp_builder(char s, char e) { init(); fcontent.add(fcontent.begin(), reg_item(s, e)); }
	iregexp_builder(char* s) { init(); fcontent.add(fcontent.begin(), reg_item(s)); }
	iregexp_builder(char* s, bool t) { init(); fcontent.add(fcontent.begin(), reg_item(s, t)); }
//	iregexp_builder(char* s, int i) { init(); fcontent.add(fcontent.begin(), reg_item(s, i)); }
	iregexp_builder(reg_command c) { init(); fcontent.add(fcontent.begin(), reg_item(c)); }
	iregexp_builder(reg_command c, int index) { init(); fcontent.add(fcontent.begin(), reg_item(c, index)); }
	iregexp_builder(reg_command c, char* s) { init(); fcontent.add(fcontent.begin(), reg_item(c, s)); }
	iregexp_builder(int index, bool t) { init(); fcontent.add(fcontent.begin(), reg_item(index, t)); }
	iregexp_builder(int index, bool t, bool f) { init(); fcontent.add(fcontent.begin(), reg_item(index, t, f)); }
	iregexp_builder(reg_command c, char* s, const iregexp_builder& v)
	{
		init();
		bld_content::iterator it = content().add(content().begin(), reg_item(c, s));
		content().add(it, v.content().begin().first_child(), v.content().end());
	}
	iregexp_builder(int i, char* s, char* m)
	{
		init();
		bld_content::iterator it = content().add(content().begin(), reg_item(i, s, m));
	}
	iregexp_builder(int i, bool prev, char* s, char* m)
	{
		init();
		bld_content::iterator it = content().add(content().begin(), reg_item(i, prev, s, m));
	}
	iregexp_builder(char* n, char* s, char* m)
	{
		init();
		bld_content::iterator it = content().add(content().begin(), reg_item(n, s, m));
	}
	iregexp_builder(int i, char* m, int idx)
	{
		init();
		bld_content::iterator it = content().add(content().begin(), reg_item(i, m, idx));
	}
	iregexp_builder(int i, bool prev, char* m, int idx)
	{
		init();
		bld_content::iterator it = content().add(content().begin(), reg_item(i, prev, m, idx));
	}
	iregexp_builder(char* n, char* m, int idx)
	{
		init();
		bld_content::iterator it = content().add(content().begin(), reg_item(n, m, idx));
	}
	iregexp_builder(int i, char* m)
	{
		init();
		bld_content::iterator it = content().add(content().begin(), reg_item(m, i));
	}
	iregexp_builder(int i, char* m, bool f1)
	{
		init();
		bld_content::iterator it = content().add(content().begin(), reg_item(m, i, f1));
	}
	iregexp_builder(int i, char* m, bool f1, bool f2)
	{
		init();
		bld_content::iterator it = content().add(content().begin(), reg_item(m, i, f1, f2));
	}
	iregexp_builder(int i, char* m, bool f1, bool f2, bool f3)
	{
		init();
		bld_content::iterator it = content().add(content().begin(), reg_item(m, i, f1, f2, f3));
	}
	iregexp_builder(int i, char* m, bool f1, bool f2, bool f3, bool f4)
	{
		init();
		bld_content::iterator it = content().add(content().begin(), reg_item(m, i, f1, f2, f3, f4));
	}
	iregexp_builder(int i, char* m, bool f1, bool f2, bool f3, bool f4, bool f5)
	{
		init();
		bld_content::iterator it = content().add(content().begin(), reg_item(m, i, f1, f2, f3, f4, f5));
	}
	iregexp_builder(int i, char* m, bool f1, bool f2, bool f3, bool f4, bool f5, bool f6)
	{
		init();
		bld_content::iterator it = content().add(content().begin(), reg_item(m, i, f1, f2, f3, f4, f5, f6));
	}
	iregexp_builder(reg_command c, const iregexp_builder& v)
	{
		init();
		bld_content::iterator it = content().add(content().begin(), reg_item(c));
		content().add(it, v.content().begin().first_child(), v.content().end());
	}
	iregexp_builder(int index, reg_command c)
	{
		init();
		bld_content::iterator it = content().add(content().begin(), reg_item(c, index));
		bld_content::iterator i = content().add(it, reg_item(rcClasster));
	}
	iregexp_builder(char* s, reg_command c)
	{
		init();
		bld_content::iterator it = content().add(content().begin(), reg_item(c, s));
		bld_content::iterator i = content().add(it, reg_item(rcClasster));
	}
	iregexp_builder(int index, reg_command c, const iregexp_builder& v)
	{
		init();
		bld_content::iterator it = content().add(content().begin(), reg_item(c, index));
		bld_content::iterator i = content().add(it, reg_item(rcClasster));
		content().add(i, v.content().begin().first_child(), v.content().end());
	}
	iregexp_builder(bool, int index, reg_command c, const iregexp_builder& v)
	{
		init();
		bld_content::iterator it = content().add(content().begin(), reg_item(c, index));
		content().add(it, v.content().begin().first_child(), v.content().end());
	}
	iregexp_builder(reg_command c, const iregexp_builder& v, const iregexp_builder& v2)
	{
		init();
		bld_content::iterator it = content().add(content().begin(), reg_item(c));
		bld_content::iterator i = content().add(it, reg_item(rcClasster));
		content().add(i, v.content().begin().first_child(), v.content().end());
		i = content().add(it, reg_item(rcClasster));
		content().add(i, v2.content().begin().first_child(), v2.content().end());
	}
	iregexp_builder& operator + () { +*classter(); return *this; }
	iregexp_builder& operator * () { **classter(); return *this; }
	iregexp_builder& operator ! () { !*classter(); return *this; }
	iregexp_builder& operator ++ () { ++ *classter(); return *this; }
	iregexp_builder& operator -- () { -- *classter(); return *this; }
	iregexp_builder& set_limit(int min) { classter()->set_limit(min); return *this; }
	iregexp_builder& set_limit(int min, int max) { classter()->set_limit(min, max); return *this; }
	iregexp_builder operator >> (const iregexp_builder& v) const
	{
		iregexp_builder ret = *this;
		ret.and_tie(v);
		return ret;
	}
	iregexp_builder operator | (const iregexp_builder& v) const
	{
		iregexp_builder ret = *this;
		ret.or_tie(v);
		return ret;
	}
	iregexp_builder& else_p(const iregexp_builder& v)
	{
		if (content().begin().count_child() != 1 || content().begin().first_child()->cmd != rcIf)
			throw;
		bld_content::iterator it = content().add(content().begin().first_child(), reg_item(rcClasster));
		content().add(it, v.content().begin().first_child(), v.content().end());
		return *this;
	}
	iregexp_builder& default_p(const iregexp_builder& v)
	{
		if (content().begin().count_child() != 1 || (content().begin().first_child()->cmd != rcSwitch && content().begin().first_child()->cmd != rcMapSwitch))
			throw;
		bld_content::iterator it = content().begin().first_child().first_child();
		content().add(it, v.content().begin().first_child(), v.content().end());
		return *this;
	}
	iregexp_builder& case_p(const iregexp_builder& v)
	{
		if (content().begin().count_child() != 1 || (content().begin().first_child()->cmd != rcSelect && content().begin().first_child()->cmd != rcSwitch
			&& content().begin().first_child()->cmd != rcMapSwitch && content().begin().first_child()->cmd != rcMapSelect))
			throw;
		bld_content::iterator it = content().add(content().begin().first_child(), reg_item(rcClasster));
		content().add(it, v.content().begin().first_child(), v.content().end());
		return *this;
	}
	iregexp_builder& optI(bool f)
	{
		return *this;
	}
	iregexp_builder& optM(bool f)
	{
		return *this;
	}
	iregexp_builder& optS(bool f)
	{
		return *this;
	}
	iregexp_builder& optX(bool f)
	{
		return *this;
	}
	template<typename target_iterator, typename target_compare = default_compare_class>
	iregexp_base<target_iterator, target_compare> build(iregexp_vender& vdr, iregexp_callback<target_iterator>* callback) const
	{
		return iregexp_builder_base::build<target_iterator, target_compare>(vdr, callback, begin().first_child(), end());
	}
};

static iregexp_builder ch_p(char c) { return iregexp_builder(c); }
static iregexp_builder ch_p(char* c) { return iregexp_builder(c); }
static iregexp_builder not_p(char c) { return iregexp_builder(c, true); }
static iregexp_builder not_p(char* c) { return iregexp_builder(c, false); }
static iregexp_builder str_p(char* c) { return iregexp_builder(c, true); }
static iregexp_builder range_p(char cs, char ce) { return iregexp_builder(cs, ce); }
static iregexp_builder xch_p(char* c) { return iregexp_builder(iregexp_builder::reg_command::rcXch, c); }
static iregexp_builder call_p(char* c) { return iregexp_builder(iregexp_builder::reg_command::rcNameCall, c); }
static iregexp_builder digit_p(void) { return iregexp_builder(iregexp_builder::reg_command::rcDigit); }
static iregexp_builder xdigit_p(void) { return iregexp_builder(iregexp_builder::reg_command::rcXDigit); }
static iregexp_builder alpha_p(void) { return iregexp_builder(iregexp_builder::reg_command::rcAlpha); }
static iregexp_builder lower_p(void) { return iregexp_builder(iregexp_builder::reg_command::rcLower); }
static iregexp_builder upper_p(void) { return iregexp_builder(iregexp_builder::reg_command::rcUpper); }
static iregexp_builder space_p(void) { return iregexp_builder(iregexp_builder::reg_command::rcSpace); }
static iregexp_builder ctrl_p(void) { return iregexp_builder(iregexp_builder::reg_command::rcCtrl); }
static iregexp_builder any_p(void) { return iregexp_builder(iregexp_builder::reg_command::rcAny); }
static iregexp_builder begin_p(void) { return iregexp_builder(iregexp_builder::reg_command::rcBegin); }
static iregexp_builder end_p(void) { return iregexp_builder(iregexp_builder::reg_command::rcEnd); }
static iregexp_builder true_p(void) { return iregexp_builder(iregexp_builder::reg_command::rcTrue); }
static iregexp_builder call_p(int i) { return iregexp_builder(i, true); }
static iregexp_builder use_p(int i) { return iregexp_builder(i, false); }
static iregexp_builder prev_call_p(int i) { return iregexp_builder(i, true, true); }
static iregexp_builder prev_use_p(int i) { return iregexp_builder(i, false, true); }
static iregexp_builder next_call_p(int i) { return iregexp_builder(i, true, false); }
static iregexp_builder next_use_p(int i) { return iregexp_builder(i, false, false); }
static iregexp_builder null_p(const iregexp_builder& v) { return iregexp_builder(iregexp_builder::reg_command::rcNull, v); }
static iregexp_builder define_p(const iregexp_builder& v) { return iregexp_builder(iregexp_builder::reg_command::rcDefine, v); }
static iregexp_builder if_p(const iregexp_builder& v, const iregexp_builder& v2) { return iregexp_builder(iregexp_builder::reg_command::rcIf, v, v2); }
static iregexp_builder not_if_p(const iregexp_builder& v, const iregexp_builder& v2) { return iregexp_builder(iregexp_builder::reg_command::rcNotIf, v, v2); }
static iregexp_builder select_p(int index) { return iregexp_builder(iregexp_builder::reg_command::rcSelect, index); }
static iregexp_builder map_select_p(char* c) { return iregexp_builder(iregexp_builder::reg_command::rcMapSelect, c); }
static iregexp_builder switch_p(int index) { return iregexp_builder(index, iregexp_builder::reg_command::rcSwitch); }
static iregexp_builder if_action_p(int index, const iregexp_builder& v) { return iregexp_builder(index, iregexp_builder::reg_command::rcIfAction, v); }
static iregexp_builder action_p(int index, const iregexp_builder& v) { return iregexp_builder(true, index, iregexp_builder::reg_command::rcAction, v); }
static iregexp_builder not_action_p(int index, const iregexp_builder& v) { return iregexp_builder(true, index, iregexp_builder::reg_command::rcNotAction, v); }
static iregexp_builder capture_p(const iregexp_builder& v) { return iregexp_builder(iregexp_builder::reg_command::rcCapture, v); }
static iregexp_builder capture_p(char* c, const iregexp_builder& v) { return iregexp_builder(iregexp_builder::reg_command::rcNameCapture, c, v); }
static iregexp_builder classter_p(const iregexp_builder& v) { return iregexp_builder(iregexp_builder::reg_command::rcClasster, v); }
static iregexp_builder func_p(int i) { return iregexp_builder(iregexp_builder::reg_command::rcFunc, i); }
static iregexp_builder query_p(int i) { return iregexp_builder(iregexp_builder::reg_command::rcQuery, i); }
static iregexp_builder after_equal_p(const iregexp_builder& v) { return iregexp_builder(iregexp_builder::reg_command::rcAfterEqual, v); }
static iregexp_builder after_not_p(const iregexp_builder& v) { return iregexp_builder(iregexp_builder::reg_command::rcAfterNot, v); }
static iregexp_builder befor_equal_p(const iregexp_builder& v) { return iregexp_builder(iregexp_builder::reg_command::rcBeforEqual, v); }
static iregexp_builder befor_not_p(const iregexp_builder& v) { return iregexp_builder(iregexp_builder::reg_command::rcBeforNot, v); }
static iregexp_builder while_p(const iregexp_builder& v, const iregexp_builder& v2) { return iregexp_builder(iregexp_builder::reg_command::rcWhile, v, v2); }
static iregexp_builder do_while_p(const iregexp_builder& v, const iregexp_builder& v2) { return iregexp_builder(iregexp_builder::reg_command::rcDoWhile, v, v2); }
static iregexp_builder set_map_p(int i, char* c, char* map) { return iregexp_builder(i, c, map); }
static iregexp_builder set_map_p(int i, bool prev, char* c, char* map) { return iregexp_builder(i, prev, c, map); }
static iregexp_builder set_map_p(char* n, char* c, char* map) { return iregexp_builder(n, c, map); }
static iregexp_builder set_map_p(int i, char* map) { return iregexp_builder(i, map); }
static iregexp_builder comp_map_equal_p(int i, char* map) { return iregexp_builder(i, map, false); }
static iregexp_builder comp_map_not_p(int i, char* map) { return iregexp_builder(i, map, false, false); }
static iregexp_builder comp_map_larghe_p(int i, char* map) { return iregexp_builder(i, map, false, false, false); }
static iregexp_builder comp_map_small_p(int i, char* map) { return iregexp_builder(i, map, false, false, false, false); }
static iregexp_builder comp_map_more_p(int i, char* map) { return iregexp_builder(i, map, false, false, false, false, false); }
static iregexp_builder comp_map_less_p(int i, char* map) { return iregexp_builder(i, map, false, false, false, false, false, false); }
static iregexp_builder map_push_p(void) { return iregexp_builder(iregexp_builder::reg_command::rcMapPush); }
static iregexp_builder map_pop_p(void) { return iregexp_builder(iregexp_builder::reg_command::rcMapPop); }
static iregexp_builder map_switch_p(char* map) { return iregexp_builder(map, iregexp_builder::reg_command::rcMapSwitch); }
static iregexp_builder list_select_p(int index, int i, char* map) { return iregexp_builder(i, map, index); }
static iregexp_builder list_select_p(int index, int i, bool prev, char* map) { return iregexp_builder(i, prev, map, index); }
static iregexp_builder list_select_p(int index, char* n, char* map) { return iregexp_builder(n, map, index); }

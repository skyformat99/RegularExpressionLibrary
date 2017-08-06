#pragma once

#include	"RegExp.h"

enum reg_ctrl_ch
{
	rcc_ctrl_00,		// @
	rcc_ctrl_01,		// A
	rcc_ctrl_02,		// B
	rcc_ctrl_03,		// C
	rcc_ctrl_04,		// D
	rcc_ctrl_05,		// E
	rcc_ctrl_06,		// F
	rcc_ctrl_07,		// G
	rcc_ctrl_08,		// H
	rcc_ctrl_09,		// I
	rcc_ctrl_0a,		// J
	rcc_ctrl_0b,		// K
	rcc_ctrl_0c,		// L
	rcc_ctrl_0d,		// M
	rcc_ctrl_0e,		// N
	rcc_ctrl_0f,		// O
	rcc_ctrl_10,		// P
	rcc_ctrl_11,		// Q
	rcc_ctrl_12,		// R
	rcc_ctrl_13,		// S
	rcc_ctrl_14,		// T
	rcc_ctrl_15,		// U
	rcc_ctrl_16,		// V
	rcc_ctrl_17,		// W
	rcc_ctrl_18,		// X
	rcc_ctrl_19,		// Y
	rcc_ctrl_1a,		// Z
	rcc_ctrl_1b,		// [
	rcc_ctrl_1c,		// \ 
	rcc_ctrl_1d,		// ]
	rcc_ctrl_1e,		// ^
	rcc_ctrl_1f,		// _
};
enum reg_parse_ch
{
	rpc_esc,		// \ 
	rpc_char,		// .
	rpc_any,		// . option s 
	rpc_esc_a,
	rpc_esc_e,
	rpc_esc_f,
	rpc_esc_n,
	rpc_esc_r,
	rpc_esc_t,
	rpc_esc_b,
	rpc_esc_d,
	rpc_esc_D,
	rpc_esc_w,
	rpc_esc_W,
	rpc_esc_s,
	rpc_esc_S,
	rpc_esc_h,
	rpc_esc_H,
	rpc_esc_v,
	rpc_esc_V,
	rpc_esc_R,
	rpc_blank,
	rpc_space,
	rpc_alpha,
	rpc_alnum,
	rpc_upper,
	rpc_lower,
	rpc_cntrl,
	rpc_sign,
	rpc_word,
	rpc_digit,
	rpc_xdigit,
	rpc_word_gap,		// b
	rpc_not_word_gap,	// B
	rpc_begin,			// ^
	rpc_end,			// $
	rpc_line_begin,		// A
	rpc_line_end,		// Z
	rpc_line_done,		// z
};
enum reg_syntax_ch
{
	rsc_esc,		// \ 
	rsc_group_s,	// {
	rsc_group_e,	// }
	rsc_class_s,	// (
	rsc_class_e,	// )
	rsc_set_s,		// [
	rsc_set_e,		// ]
	rsc_category_s,	// <
	rsc_category_e,	// >
	rsc_repeat,		// ,
	rsc_delimiter,	// :
	rsc_quoter,		// '
	rsc_set_tie,	// -
	rsc_no_set,		// ^
	rsc_char,		// .
	rsc_end,		// $
	rsc_name,		// &
	rsc_comment,	// #
	rsc_binary,		// ?
	rsc_any,		// *
	rsc_over_one,	// +
	rsc_equal,		// =
	rsc_not,		// !
	rsc_or,			// |
	rsc_opt_m,			// m
	rsc_opt_s,			// s
	rsc_opt_i,			// i
	rsc_opt_x,			// x
	rsc_quort_s,		// Q
	rsc_property_e,		// E
	rsc_refarence,		// R
	rsc_refarence_0,	// 0
	rsc_refarence_name,	// k
	rsc_pre_ctrl,		// c
	rsc_define_d,	// D
	//	rsc_define_e,	// E
	rsc_define_f,	// F
	rsc_define_i,	// I
	rsc_define_n,	// N
};

template<typename target_container, typename target_iterator, typename target_function>
class iregexp_vendor
{
public:
	virtual long to_num(target_iterator vp, target_iterator ve, size_t base = 10) { return 0; }
	virtual target_container hash_to(long) { return target_container(); }
	virtual long to_hash(target_container) { return 0; }
	virtual target_container get_charcter(reg_syntax_ch type) { return 0; }
	virtual target_container get_esc_char(reg_parse_ch type) { return 0; }
	virtual target_container get_ctrl_char(reg_ctrl_ch type) { return 0; }
	virtual iregexp_base<target_container, target_iterator, target_function> get_ctrl_parser(reg_ctrl_ch type) { return 0; }
	virtual iregexp_base<target_container, target_iterator, target_function> get_parser(reg_parse_ch type, bool not = false) { return iregexp_base<target_container, target_iterator, target_function>(); }
	virtual iregexp_base<target_container, target_iterator, target_function> get_command(reg_parse_ch type) { return iregexp_base<target_container, target_iterator, target_function>(); }
	virtual iregexp_base<target_container, target_iterator, target_function> get_range_p(const target_container, const target_container) { return iregexp_base<target_container, target_iterator, target_function>(); }
};

template<typename target_container, typename target_iterator, typename target_function>
class regexp_parser_base
{
public:
	iregexp_base<target_container, target_iterator, target_function> anychar_p(void) { any_char_regexp<target_container, target_iterator, target_function> ret; return ret; }
	character_serial_regexp<target_container, target_iterator, target_function> str_p(const target_container& v) { character_serial_regexp<target_container, target_iterator, target_function> ret(v); return ret; }
	character_serial_regexp<target_container, target_iterator, target_function> str_p(const target_iterator& begin, const target_iterator& end) { character_serial_regexp<target_container, target_iterator, target_function> ret(begin, end); return ret; }
	character_parallel_regexp<target_container, target_iterator, target_function> ch_p(const target_container& v, bool not = false) { character_parallel_regexp<target_container, target_iterator, target_function> ret(v, not); return ret; }
	character_parallel_regexp<target_container, target_iterator, target_function> not_ch_p(const target_container& v) { return ch_p<target_container, target_iterator, target_function>(v, true); }
	character_parallel_regexp<target_container, target_iterator, target_function> ch_p(const target_iterator& vb, const target_iterator& ve, bool not = false) { character_parallel_regexp<target_container, target_iterator, target_function> ret(vp, ve, not); return ret; }
	character_parallel_regexp<target_container, target_iterator, target_function> not_ch_p(const target_iterator& vb, const target_iterator& ve) { return ch_p<target_container, target_iterator, target_function>(vp, ve, true); }
	character_parallel_regexp<target_container, target_iterator, target_function> ch_p(const target_iterator& v, bool not = false) { character_parallel_regexp<target_container, target_iterator, target_function> ret(v, not); return ret; }
	character_parallel_regexp<target_container, target_iterator, target_function> not_ch_p(const target_iterator& v) { return ch_p<target_container, target_iterator, target_function>(v, true); }
	repeat_regexp<target_container, target_iterator, target_function> repeat_p(size_t n) { repeat_regexp<target_container, target_iterator, target_function> ret(n); return ret; }
	repeat_regexp<target_container, target_iterator, target_function> repeat_p(size_t min, size_t max) { repeat_regexp<target_container, target_iterator, target_function> ret(min, max); return ret; }
	iregexp_base<target_container, target_iterator, target_function> no_backtrack_p(const iregexp_base<target_container, target_iterator, target_function>& v) { no_backtrack_regexp<target_container, target_iterator, target_function> ret(v); return ret; }
	null_regexp<target_container, target_iterator, target_function> null_p(void) { null_regexp<target_container, target_iterator, target_function> ret; return ret; }
	null_regexp<target_container, target_iterator, target_function> null_p(const iregexp_base<target_container, target_iterator, target_function>& v) { null_regexp<target_container, target_iterator, target_function> ret(v); return ret; }
	iregexp_base<target_container, target_iterator, target_function> begin_p(void) { begin_regexp<target_container, target_iterator, target_function> ret; return ret; }
	iregexp_base<target_container, target_iterator, target_function> end_p(void) { end_regexp<target_container, target_iterator, target_function> ret; return ret; }
	iregexp_base<target_container, target_iterator, target_function> befor_assertion_p(const iregexp_base<target_container, target_iterator, target_function>& v) { assertion_regexp<target_container, target_iterator, target_function> ret(v, assertion_regexp<target_container, target_iterator, target_function>::at_befor_assertion); return ret; }
	iregexp_base<target_container, target_iterator, target_function> not_befor_assertion_p(const iregexp_base<target_container, target_iterator, target_function>& v) { assertion_regexp<target_container, target_iterator, target_function> ret(v, assertion_regexp<target_container, target_iterator, target_function>::at_not_befor_assertion); return ret; }
	iregexp_base<target_container, target_iterator, target_function> after_assertion_p(const iregexp_base<target_container, target_iterator, target_function>& v) { assertion_regexp<target_container, target_iterator, target_function> ret(v, assertion_regexp<target_container, target_iterator, target_function>::at_after_assertion); return ret; }
	iregexp_base<target_container, target_iterator, target_function> not_after_assertion_p(const iregexp_base<target_container, target_iterator, target_function>& v) { assertion_regexp<target_container, target_iterator, target_function> ret(v, assertion_regexp<target_container, target_iterator, target_function>::at_not_after_assertion); return ret; }
	iregexp_base<target_container, target_iterator, target_function> bound_assertion_p(const iregexp_base<target_container, target_iterator, target_function>& v) { assertion_regexp<target_container, target_iterator, target_function> ret(v, assertion_regexp<target_container, target_iterator, target_function>::at_boundary_assertion); return ret; }
	iregexp_base<target_container, target_iterator, target_function> not_bound_assertion_p(const iregexp_base<target_container, target_iterator, target_function>& v) { assertion_regexp<target_container, target_iterator, target_function> ret(v, assertion_regexp<target_container, target_iterator, target_function>::at_not_boundary_assertion); return ret; }
	for_regexp<target_container, target_iterator, target_function> for_p(const iregexp_base<target_container, target_iterator, target_function>& v, void* param, long param2, void(*init_func)(void* param, long param2, const target_iterator vb, target_iterator &vp, const target_iterator ve), void(*loop_func)(void* param, long param2, const target_iterator vb, target_iterator &vp, const target_iterator ve)) { for_regexp<target_container, target_iterator, target_function> ret(v, param, param2, init_func, loop_func); return ret; }
	while_regexp<target_container, target_iterator, target_function> while_p(const iregexp_base<target_container, target_iterator, target_function>& v) { while_regexp<target_container, target_iterator, target_function> ret(v); return ret; }
	do_while_regexp<target_container, target_iterator, target_function> do_p(const iregexp_base<target_container, target_iterator, target_function>& v) { do_while_regexp<target_container, target_iterator, target_function> ret(v); return ret; }
	if_regexp<target_container, target_iterator, target_function> if_p(const iregexp_base<target_container, target_iterator, target_function>& v) { if_regexp<target_container, target_iterator, target_function> ret(v); return ret; }
	if_action_regexp<target_container, target_iterator, target_function> if_action_p(void* param, long param2, void(*func)(void* param, long param2, bool success, const target_iterator vb, target_iterator &vp, const target_iterator ve)) { if_action_regexp<target_container, target_iterator, target_function> ret(param, param2, func); return ret; }
	action_regexp<target_container, target_iterator, target_function> action_p(void* param, long param2, void(*func)(void* param, long param2, const target_iterator vb, target_iterator &vp, const target_iterator ve)) { action_regexp<target_container, target_iterator, target_function> ret(param, param2, func); return ret; }
	action_regexp<target_container, target_iterator, target_function> not_action_p(void* param, long param2, void(*func)(void* param, long param2, const target_iterator vb, target_iterator &vp, const target_iterator ve)) { action_regexp<target_container, target_iterator, target_function> ret(param, param2, func, true); return ret; }
	switch_regexp<target_container, target_iterator, target_function> switch_p(void* param, long param2, short(*switch_func)(void* param, long param2)) { switch_regexp<target_container, target_iterator, target_function> ret(param, param2, switch_func); return ret; }
	select_regexp<target_container, target_iterator, target_function> select_p(void* param, long param2, void(*switch_func)(void* param, long param2, short index, const target_iterator vb, target_iterator &vp, const target_iterator ve), bool call_default = false) { select_regexp<target_container, target_iterator, target_function> ret(param, param2, switch_func, call_default); return ret; }
	iregexp_base<target_container, target_iterator, target_function> query_p(void* param, long param2, bool(*func)(void* param, long param2, const target_iterator vb, target_iterator &vp, const target_iterator ve)) { query_regexp<target_container, target_iterator, target_function> ret(param, param2, func); return ret; }
	iregexp_base<target_container, target_iterator, target_function> capture_p(const iregexp_base<target_container, target_iterator, target_function>& v) { capture_regexp<target_container, target_iterator, target_function> ret(v); return ret; }
	capture_regexp<target_container, target_iterator, target_function> capture_p(const target_container name) { capture_regexp<target_container, target_iterator, target_function> ret(name); return ret; }
	iregexp_base<target_container, target_iterator, target_function> call_p(void) { capture_regexp<target_container, target_iterator, target_function> ret(0); return ret; }
	iregexp_base<target_container, target_iterator, target_function> call_p(size_t n) { capture_regexp<target_container, target_iterator, target_function> ret(n); return ret; }
	iregexp_base<target_container, target_iterator, target_function> call_p(size_t n, bool prev) { capture_regexp<target_container, target_iterator, target_function> ret(n, prev); return ret; }
	iregexp_base<target_container, target_iterator, target_function> call_p(const target_container name) { capture_regexp<target_container, target_iterator, target_function> ret(name, true); return ret; }
	iregexp_base<target_container, target_iterator, target_function> use_p(size_t n) { capture_regexp<target_container, target_iterator, target_function> ret(true, n); return ret; }
	iregexp_base<target_container, target_iterator, target_function> use_p(size_t n, bool prev) { capture_regexp<target_container, target_iterator, target_function> ret(true, n, prev); return ret; }
	iregexp_base<target_container, target_iterator, target_function> use_p(const target_container name) { capture_regexp<target_container, target_iterator, target_function> ret(true, name); return ret; }
	iregexp_base<target_container, target_iterator, target_function> match_p(size_t n) { capture_regexp<target_container, target_iterator, target_function> ret(true, true, n); return ret; }
	iregexp_base<target_container, target_iterator, target_function> match_p(size_t n, bool prev) { capture_regexp<target_container, target_iterator, target_function> ret(true, true, n, prev); return ret; }
	iregexp_base<target_container, target_iterator, target_function> match_p(const target_container name) { capture_regexp<target_container, target_iterator, target_function> ret(true, true, name); return ret; }
	iregexp_base<target_container, target_iterator, target_function> recursion_p(void) { capture_regexp<target_container, target_iterator, target_function> ret(true, true, true, true); return ret; }
	iregexp_base<target_container, target_iterator, target_function> recursion_p(size_t n) { capture_regexp<target_container, target_iterator, target_function> ret(true, true, true, n); return ret; }
	iregexp_base<target_container, target_iterator, target_function> recursion_p(const target_container name) { capture_regexp<target_container, target_iterator, target_function> ret(true, true, true, name); return ret; }
	iregexp_base<target_container, target_iterator, target_function> func_p(void* param, long param2, void(*func)(void* param, long param2, const target_iterator vb, target_iterator &vp, const target_iterator ve)) { func_regexp<target_container, target_iterator, target_function> ret(param, param2, func); return ret; }
	struct regexp_parser_option
	{
		bool opt_s;
		bool opt_m;
		bool opt_i;
		bool opt_x;
		bool if_condition;
		regexp_parser_option() { clear(); }
		regexp_parser_option& s(void) { opt_s = true; return *this; }
		regexp_parser_option& m(void) { opt_m = true; return *this; }
		regexp_parser_option& i(void) { opt_i = true; return *this; }
		regexp_parser_option& x(void) { opt_x = true; return *this; }
		regexp_parser_option& S(void) { opt_s = false; return *this; }
		regexp_parser_option& M(void) { opt_m = false; return *this; }
		regexp_parser_option& I(void) { opt_i = false; return *this; }
		regexp_parser_option& X(void) { opt_x = false; return *this; }
		regexp_parser_option& set_if_condition(void) { if_condition = true; return *this; }
		regexp_parser_option& reset_if_condition(void) { if_condition = false; return *this; }
		void clear(void)
		{
			opt_s = false;
			opt_m = false;
			opt_i = false;
			opt_x = false;
			if_condition = false;
		}
	};

	size_t fmin;
	size_t fmax;
	bool fnot_char_class;
	bool fnot_char_set;
	bool fopt_s;
	bool fopt_m;
	bool fopt_i;
	bool fopt_x;
	bool fopt_S;
	bool fopt_M;
	bool fopt_I;
	bool fopt_X;
	bool frelative_call_sign;
	target_container fxchar;
	target_container ftiechar_b;
	target_container ftiechar_l;
	iregexp_base<target_container, target_iterator, target_function> fchar_class;
	iregexp_base<target_container, target_iterator, target_function> fchar_set;
	iregexp_base<target_container, target_iterator, target_function> fregexp;
	regexp_parser_option foptions;
	regexp_parser_base()
	{
		clear();
	}
	void clear(void)
	{
		fmin = 0;
		fmax = 0;
		fnot_char_class = false;
		fnot_char_set = false;
		fopt_s = false;
		fopt_m = false;
		fopt_i = false;
		fopt_x = false;
		fopt_S = false;
		fopt_M = false;
		fopt_I = false;
		fopt_X = false;
		fxchar.clear();
		ftiechar_b.clear();
		ftiechar_l.clear();
		fchar_class.clear();
		fchar_set.clear();
		fregexp.clear();
		//foptions.clear();
	}
	void set_class(const iregexp_base<target_container, target_iterator, target_function>& reg)
	{
		fchar_class = reg;
	}
	void set_charset(void)
	{
		if (fchar_set.is_null())
			fchar_set = fchar_class;
		else
			fchar_set |= fchar_class;
		fchar_class.clear();
		fnot_char_class = false;
	}
	void set_charset(const iregexp_base<target_container, target_iterator, target_function>& reg)
	{
		if (fchar_set.is_null())
			fchar_set = reg;
		else
			fchar_set |= reg;
	}
	void add_class(void)
	{
		fregexp >>= fchar_class;
		fchar_class.clear();
		fnot_char_class = false;
	}
	void add_char(void)
	{
		fregexp >>= ch_p(fxchar);
		fchar_set.clear();
		fnot_char_set = false;
	}
	void add_charset(void)
	{
		fregexp >>= fchar_set;
		fchar_set.clear();
		fnot_char_set = false;
	}
	void add_(const iregexp_base<target_container, target_iterator, target_function>& reg)
	{
		fregexp >>= reg;
	}
	void set_math(size_t min, size_t max)
	{
		(*fregexp.begin_item().last_child())->set_limited(min, max);
	}
	void set_force_match(void)
	{
		(*fregexp.begin_item().last_child())->set_force_match();
	}
	void set_weak_match(void)
	{
		(*fregexp.begin_item().last_child())->set_weak_match();
	}
};

template<typename target_container, typename target_iterator, typename target_function>
class regexp_parser_or_base
	: public regexp_parser_base<target_container, target_iterator, target_function>
{
public:
	std::vector<iregexp_base<target_container, target_iterator, target_function> > for_stack;
	void push_or(void)
	{
		for_stack.push_back(fregexp);
		fregexp.clear();
	}
	void back_or(void)
	{
		if (for_stack.size())
		{
			std::vector<iregexp_base<target_container, target_iterator, target_function> >::iterator it = for_stack.begin();
			iregexp_base<target_container, target_iterator, target_function> ret = *it;
			while (++it != for_stack.end())
			{
				ret |= *it;
			}
			for_stack.clear();
			ret |= fregexp;
			fregexp = ret;
		}
	}
};

template<typename target_container, typename target_iterator, typename target_function>
class regexp_parser_classter_base
	: public regexp_parser_or_base<target_container, target_iterator, target_function>
{
public:
	regexp_parser_classter_base() : freg_param(NULL), foption_set(false) {}
	virtual ~regexp_parser_classter_base()
	{
		delete freg_param;
	}
	bool foption_set;
	void* ffunc_param;
	long faction_param;
	iregexp_base<target_container, target_iterator, target_function> *freg_param;
	short(*fswitch_func)(void* param, long param2);
	void(*fselect_func)(void* param, long param2, short index, const target_iterator vb, target_iterator &vp, const target_iterator ve);
	void(*fif_func)(void* param, long param2, bool success, const target_iterator vb, target_iterator &vp, const target_iterator ve);
	void(*faction_func)(void* param, long param2, const target_iterator vb, target_iterator &vp, const target_iterator ve);
	void(*ffor_initialize_func)(void* param, long param2, const target_iterator vb, target_iterator &vp, const target_iterator ve);
	void(*ffor_reinitialize_func)(void* param, long param2, const target_iterator vb, target_iterator &vp, const target_iterator ve);
	bool(*fquery_func)(void* param, long param2, const target_iterator vb, target_iterator &vp, const target_iterator ve);
	void(*fcall_func)(void* param, long param2, const target_iterator vb, target_iterator &vp, const target_iterator ve);
	enum capture_type
	{
		ct_null,
		ct_capture,
		ct_name_capture,
		ct_befor_assertion,
		ct_not_befor_assertion,
		ct_after_assertion,
		ct_not_after_assertion,
		ct_no_backtrack,
		ct_branch_reset,
		ct_switch,
		ct_select,
		ct_if,
		ct_if_action,
		ct_while,
		ct_do_while,
		ct_action,
		ct_not_action,
		ct_for,
		ct_query,
		ct_func,
		ct_define,
	};
	struct capture
	{
		capture_type type;
		target_container name;
		capture() : type(ct_capture) {}
	};
	std::vector<std::pair<std::pair<std::vector<iregexp_base<target_container, target_iterator, target_function> >, capture>, regexp_parser_option> > fclasster_stack;
	void set_classter_type(capture_type type)
	{
		fclasster_stack.back().first.second.type = type;
	}
	void set_classter_name(target_container name)
	{
		fclasster_stack.back().first.second.name = name;
	}
	void push_classter(void)
	{
		for_stack.push_back(fregexp);
		fregexp.clear();
		fclasster_stack.push_back(std::pair<std::pair<std::vector<iregexp_base<target_container, target_iterator, target_function> >, capture>, regexp_parser_option>(std::pair<std::vector<iregexp_base<target_container, target_iterator, target_function> >, capture>(for_stack, capture()), foptions));
		for_stack.clear();
		//foptions.clear();
	}
	void back_classter(void)
	{
		back_or();
		iregexp_base<target_container, target_iterator, target_function> ret = fregexp;
		for_stack = fclasster_stack.back().first.first;
		capture_type type = fclasster_stack.back().first.second.type;
		target_container name = fclasster_stack.back().first.second.name;
		foptions = fclasster_stack.back().second;
		fclasster_stack.pop_back();
		fregexp = for_stack.back();
		for_stack.pop_back();
		switch (type)
		{
		case ct_befor_assertion:		fregexp >>= befor_assertion_p(ret);						break;
		case ct_not_befor_assertion:	fregexp >>= not_befor_assertion_p(ret);					break;
		case ct_after_assertion:		fregexp >>= after_assertion_p(ret);						break;
		case ct_not_after_assertion:	fregexp >>= not_after_assertion_p(ret);					break;
		case ct_no_backtrack:			fregexp >>= no_backtrack_p(ret);						break;
		case ct_branch_reset:			ret.set_branch_reset();	fregexp >>= ret;				break;
		case ct_capture:				fregexp >>= capture_p(ret);								break;
		case ct_name_capture:			fregexp >>= capture_p(name)[ret];						break;
		case ct_switch:					fregexp >>= switch_p(ffunc_param, faction_param, fswitch_func).set_items(ret);	break;
		case ct_select:					fregexp >>= select_p(ffunc_param, faction_param, fselect_func).set_items(ret);	break;
		case ct_if_action:				fregexp >>= if_action_p(ffunc_param, faction_param, fif_func)[ret];		break;
		case ct_action:					fregexp >>= action_p(ffunc_param, faction_param, faction_func)[ret];	break;
		case ct_not_action:				fregexp >>= not_action_p(ffunc_param, faction_param, faction_func)[ret];	break;
		case ct_query:					fregexp >>= query_p(ffunc_param, faction_param, fquery_func);			break;
		case ct_func:					fregexp >>= func_p(ffunc_param, faction_param, fcall_func);				break;
		case ct_if:
			(*dynamic_cast<if_regexp<target_container, target_iterator, target_function>*>(freg_param)).set_items(ret);
			fregexp >>= *freg_param;
			delete freg_param;
			freg_param = NULL;
			break;
		case ct_while:
			(*dynamic_cast<while_regexp<target_container, target_iterator, target_function>*>(freg_param))[ret];
			fregexp >>= *freg_param;
			delete freg_param;
			freg_param = NULL;
			break;
		case ct_do_while:
			(*dynamic_cast<do_while_regexp<target_container, target_iterator, target_function>*>(freg_param)).while_p(ret);
			fregexp >>= *freg_param;
			delete freg_param;
			freg_param = NULL;
			break;
		case ct_for:
			(*dynamic_cast<for_regexp<target_container, target_iterator, target_function>*>(freg_param))[ret];
			fregexp >>= *freg_param;
			delete freg_param;
			freg_param = NULL;
			break;
		case ct_define:
			(*dynamic_cast<null_regexp<target_container, target_iterator, target_function>*>(freg_param))[ret];
			fregexp >>= *freg_param;
			delete freg_param;
			freg_param = NULL;
			break;
		default:						fregexp >>= ret;
		}
	}
	void set_classter(long idx)
	{
		back_or();
		iregexp_base<target_container, target_iterator, target_function> ret = fregexp;
		for_stack = fclasster_stack.back().first.first;
		capture_type type = fclasster_stack.back().first.second.type;
		target_container name = fclasster_stack.back().first.second.name;
		foptions = fclasster_stack.back().second;
		fclasster_stack.pop_back();
		fregexp = for_stack.back();
		for_stack.pop_back();
		if (freg_param != NULL)
			delete freg_param;
		switch (idx)
		{
		case ct_if:
			freg_param = new if_regexp<target_container, target_iterator, target_function>(ret);
			break;
		case ct_while:
			freg_param = new while_regexp<target_container, target_iterator, target_function>(ret);
			break;
		case ct_do_while:
			freg_param = new do_while_regexp<target_container, target_iterator, target_function>(ret);
			break;
		case ct_for:
			freg_param = new for_regexp<target_container, target_iterator, target_function>(ret, ffunc_param, faction_param, ffor_initialize_func, ffor_reinitialize_func);
			break;
		case ct_define:
			freg_param = new null_regexp<target_container, target_iterator, target_function>();
			break;
		}
	}
};

template<typename target_container, typename target_iterator, typename target_function>
class regexp_parser
	: public regexp_parser_classter_base<target_container, target_iterator, target_function>
{
private:
	iregexp_base<target_container, target_iterator, target_function> ps_ESC()			{ return fvendor->get_command(rpc_esc); }
	iregexp_base<target_container, target_iterator, target_function> ps_Any()			{ return fvendor->get_command(rpc_any); }
	iregexp_base<target_container, target_iterator, target_function> ps_Char()			{ return fvendor->get_command(rpc_char); }
	iregexp_base<target_container, target_iterator, target_function> ps_d()			{ return fvendor->get_command(rpc_esc_d); }
	iregexp_base<target_container, target_iterator, target_function> ps_D()			{ return fvendor->get_command(rpc_esc_D); }
	iregexp_base<target_container, target_iterator, target_function> ps_w()			{ return fvendor->get_command(rpc_esc_w); }
	iregexp_base<target_container, target_iterator, target_function> ps_W()			{ return fvendor->get_command(rpc_esc_W); }
	iregexp_base<target_container, target_iterator, target_function> ps_s()			{ return fvendor->get_command(rpc_esc_s); }
	iregexp_base<target_container, target_iterator, target_function> ps_S()			{ return fvendor->get_command(rpc_esc_S); }
	iregexp_base<target_container, target_iterator, target_function> ps_h()			{ return fvendor->get_command(rpc_esc_h); }
	iregexp_base<target_container, target_iterator, target_function> ps_H()			{ return fvendor->get_command(rpc_esc_H); }
	iregexp_base<target_container, target_iterator, target_function> ps_v()			{ return fvendor->get_command(rpc_esc_v); }
	iregexp_base<target_container, target_iterator, target_function> ps_V()			{ return fvendor->get_command(rpc_esc_V); }
	iregexp_base<target_container, target_iterator, target_function> ps_a()			{ return fvendor->get_command(rpc_esc_a); }
	iregexp_base<target_container, target_iterator, target_function> ps_e()			{ return fvendor->get_command(rpc_esc_e); }
	iregexp_base<target_container, target_iterator, target_function> ps_f()			{ return fvendor->get_command(rpc_esc_f); }
	iregexp_base<target_container, target_iterator, target_function> ps_n()			{ return fvendor->get_command(rpc_esc_n); }
	iregexp_base<target_container, target_iterator, target_function> ps_r()			{ return fvendor->get_command(rpc_esc_r); }
	iregexp_base<target_container, target_iterator, target_function> ps_t()			{ return fvendor->get_command(rpc_esc_t); }
	iregexp_base<target_container, target_iterator, target_function> ps_b()			{ return fvendor->get_command(rpc_esc_b); }
	iregexp_base<target_container, target_iterator, target_function> ps_R()			{ return fvendor->get_command(rpc_esc_R); }
	iregexp_base<target_container, target_iterator, target_function> ps_blank()		{ return fvendor->get_command(rpc_blank); }
	iregexp_base<target_container, target_iterator, target_function> ps_space()		{ return fvendor->get_command(rpc_space); }
	iregexp_base<target_container, target_iterator, target_function> ps_alpha()		{ return fvendor->get_command(rpc_alpha); }
	iregexp_base<target_container, target_iterator, target_function> ps_alnum()		{ return fvendor->get_command(rpc_alnum); }
	iregexp_base<target_container, target_iterator, target_function> ps_upper()		{ return fvendor->get_command(rpc_upper); }
	iregexp_base<target_container, target_iterator, target_function> ps_lower()		{ return fvendor->get_command(rpc_lower); }
	iregexp_base<target_container, target_iterator, target_function> ps_cntrl()		{ return fvendor->get_command(rpc_cntrl); }
	iregexp_base<target_container, target_iterator, target_function> ps_sign()			{ return fvendor->get_command(rpc_sign); }
	iregexp_base<target_container, target_iterator, target_function> ps_word()			{ return fvendor->get_command(rpc_word); }
	iregexp_base<target_container, target_iterator, target_function> ps_digit()		{ return fvendor->get_command(rpc_digit); }
	iregexp_base<target_container, target_iterator, target_function> ps_xdigit()		{ return fvendor->get_command(rpc_xdigit); }
	iregexp_base<target_container, target_iterator, target_function> ps_word_gap()		{ return fvendor->get_command(rpc_word_gap); }
	iregexp_base<target_container, target_iterator, target_function> ps_not_word_gap()	{ return fvendor->get_command(rpc_not_word_gap); }
	iregexp_base<target_container, target_iterator, target_function> ps_begin()		{ return fvendor->get_command(rpc_begin); }
	iregexp_base<target_container, target_iterator, target_function> ps_end()			{ return fvendor->get_command(rpc_end); }
	iregexp_base<target_container, target_iterator, target_function> ps_line_begin()	{ return fvendor->get_command(rpc_line_begin); }
	iregexp_base<target_container, target_iterator, target_function> ps_line_end()		{ return fvendor->get_command(rpc_line_end); }
	iregexp_base<target_container, target_iterator, target_function> ps_line_done()	{ return fvendor->get_command(rpc_line_done); }
	iregexp_base<target_container, target_iterator, target_function> ps_refarence()	{ return fvendor->get_command(rpc_refarence); }
	iregexp_base<target_container, target_iterator, target_function> ps_refarence_0()	{ return fvendor->get_command(rpc_refarence_0); }
	iregexp_base<target_container, target_iterator, target_function> ps_quort_s()		{ return fvendor->get_command(rpc_quort_s); }
	iregexp_base<target_container, target_iterator, target_function> ps_property_e()	{ return fvendor->get_command(rpc_property_e); }
	iregexp_base<target_container, target_iterator, target_function> ESC_p()			{ return fvendor->get_parser(rpc_esc); }
	iregexp_base<target_container, target_iterator, target_function> Any_p()			{ return fvendor->get_parser(rpc_any); }
	iregexp_base<target_container, target_iterator, target_function> Char_p()			{ return fvendor->get_parser(rpc_char); }
	iregexp_base<target_container, target_iterator, target_function> d_p()				{ return fvendor->get_parser(rpc_esc_d); }
	iregexp_base<target_container, target_iterator, target_function> D_p()				{ return fvendor->get_parser(rpc_esc_D); }
	iregexp_base<target_container, target_iterator, target_function> w_p()				{ return fvendor->get_parser(rpc_esc_w); }
	iregexp_base<target_container, target_iterator, target_function> W_p()				{ return fvendor->get_parser(rpc_esc_W); }
	iregexp_base<target_container, target_iterator, target_function> s_p()				{ return fvendor->get_parser(rpc_esc_s); }
	iregexp_base<target_container, target_iterator, target_function> S_p()				{ return fvendor->get_parser(rpc_esc_S); }
	iregexp_base<target_container, target_iterator, target_function> h_p()				{ return fvendor->get_parser(rpc_esc_h); }
	iregexp_base<target_container, target_iterator, target_function> H_p()				{ return fvendor->get_parser(rpc_esc_H); }
	iregexp_base<target_container, target_iterator, target_function> v_p()				{ return fvendor->get_parser(rpc_esc_v); }
	iregexp_base<target_container, target_iterator, target_function> V_p()				{ return fvendor->get_parser(rpc_esc_V); }
	iregexp_base<target_container, target_iterator, target_function> a_p()				{ return fvendor->get_parser(rpc_esc_a); }
	iregexp_base<target_container, target_iterator, target_function> e_p()				{ return fvendor->get_parser(rpc_esc_e); }
	iregexp_base<target_container, target_iterator, target_function> f_p()				{ return fvendor->get_parser(rpc_esc_f); }
	iregexp_base<target_container, target_iterator, target_function> n_p()				{ return fvendor->get_parser(rpc_esc_n); }
	iregexp_base<target_container, target_iterator, target_function> r_p()				{ return fvendor->get_parser(rpc_esc_r); }
	iregexp_base<target_container, target_iterator, target_function> t_p()				{ return fvendor->get_parser(rpc_esc_t); }
	iregexp_base<target_container, target_iterator, target_function> R_p()				{ return fvendor->get_parser(rpc_esc_R); }
	iregexp_base<target_container, target_iterator, target_function> blank_p(bool not = false)	{ return fvendor->get_parser(rpc_blank, not); }
	iregexp_base<target_container, target_iterator, target_function> space_p(bool not = false)	{ return fvendor->get_parser(rpc_space, not); }
	iregexp_base<target_container, target_iterator, target_function> alpha_p(bool not = false)	{ return fvendor->get_parser(rpc_alpha, not); }
	iregexp_base<target_container, target_iterator, target_function> alnum_p(bool not = false)	{ return fvendor->get_parser(rpc_alnum, not); }
	iregexp_base<target_container, target_iterator, target_function> upper_p(bool not = false)	{ return fvendor->get_parser(rpc_upper, not); }
	iregexp_base<target_container, target_iterator, target_function> lower_p(bool not = false)	{ return fvendor->get_parser(rpc_lower, not); }
	iregexp_base<target_container, target_iterator, target_function> cntrl_p(bool not = false)	{ return fvendor->get_parser(rpc_cntrl, not); }
	iregexp_base<target_container, target_iterator, target_function> sign_p(bool not = false)	{ return fvendor->get_parser(rpc_sign, not); }
	iregexp_base<target_container, target_iterator, target_function> word_p(bool not = false)	{ return fvendor->get_parser(rpc_word, not); }
	iregexp_base<target_container, target_iterator, target_function> digit_p(bool not = false)	{ return fvendor->get_parser(rpc_digit, not); }
	iregexp_base<target_container, target_iterator, target_function> xdigit_p(bool not = false){ return fvendor->get_parser(rpc_xdigit, not); }
	iregexp_base<target_container, target_iterator, target_function> word_gap_p()		{ return fvendor->get_parser(rpc_word_gap); }
	iregexp_base<target_container, target_iterator, target_function> not_word_gap_p()	{ return fvendor->get_parser(rpc_not_word_gap); }
	iregexp_base<target_container, target_iterator, target_function> line_begin_p()	{ return fvendor->get_parser(rpc_line_begin); }
	iregexp_base<target_container, target_iterator, target_function> line_end_p()		{ return fvendor->get_parser(rpc_line_end); }
	iregexp_base<target_container, target_iterator, target_function> line_done_p()		{ return fvendor->get_parser(rpc_line_done); }
	iregexp_base<target_container, target_iterator, target_function> pc_control(char c)
	{
		switch (c)
		{
		case '@':	return character_parallel_regexp<target_container, target_iterator, target_function>(fvendor->get_ctrl_char(rcc_ctrl_00));
		case 'A':	return character_parallel_regexp<target_container, target_iterator, target_function>(fvendor->get_ctrl_char(rcc_ctrl_01));
		case 'B':	return character_parallel_regexp<target_container, target_iterator, target_function>(fvendor->get_ctrl_char(rcc_ctrl_02));
		case 'C':	return character_parallel_regexp<target_container, target_iterator, target_function>(fvendor->get_ctrl_char(rcc_ctrl_03));
		case 'D':	return character_parallel_regexp<target_container, target_iterator, target_function>(fvendor->get_ctrl_char(rcc_ctrl_04));
		case 'E':	return character_parallel_regexp<target_container, target_iterator, target_function>(fvendor->get_ctrl_char(rcc_ctrl_05));
		case 'F':	return character_parallel_regexp<target_container, target_iterator, target_function>(fvendor->get_ctrl_char(rcc_ctrl_06));
		case 'G':	return character_parallel_regexp<target_container, target_iterator, target_function>(fvendor->get_ctrl_char(rcc_ctrl_07));
		case 'H':	return character_parallel_regexp<target_container, target_iterator, target_function>(fvendor->get_ctrl_char(rcc_ctrl_08));
		case 'I':	return character_parallel_regexp<target_container, target_iterator, target_function>(fvendor->get_ctrl_char(rcc_ctrl_09));
		case 'J':	return character_parallel_regexp<target_container, target_iterator, target_function>(fvendor->get_ctrl_char(rcc_ctrl_0a));
		case 'K':	return character_parallel_regexp<target_container, target_iterator, target_function>(fvendor->get_ctrl_char(rcc_ctrl_0b));
		case 'L':	return character_parallel_regexp<target_container, target_iterator, target_function>(fvendor->get_ctrl_char(rcc_ctrl_0c));
		case 'M':	return character_parallel_regexp<target_container, target_iterator, target_function>(fvendor->get_ctrl_char(rcc_ctrl_0d));
		case 'N':	return character_parallel_regexp<target_container, target_iterator, target_function>(fvendor->get_ctrl_char(rcc_ctrl_0e));
		case 'O':	return character_parallel_regexp<target_container, target_iterator, target_function>(fvendor->get_ctrl_char(rcc_ctrl_0f));
		case 'P':	return character_parallel_regexp<target_container, target_iterator, target_function>(fvendor->get_ctrl_char(rcc_ctrl_10));
		case 'Q':	return character_parallel_regexp<target_container, target_iterator, target_function>(fvendor->get_ctrl_char(rcc_ctrl_11));
		case 'R':	return character_parallel_regexp<target_container, target_iterator, target_function>(fvendor->get_ctrl_char(rcc_ctrl_12));
		case 'S':	return character_parallel_regexp<target_container, target_iterator, target_function>(fvendor->get_ctrl_char(rcc_ctrl_13));
		case 'T':	return character_parallel_regexp<target_container, target_iterator, target_function>(fvendor->get_ctrl_char(rcc_ctrl_14));
		case 'U':	return character_parallel_regexp<target_container, target_iterator, target_function>(fvendor->get_ctrl_char(rcc_ctrl_15));
		case 'V':	return character_parallel_regexp<target_container, target_iterator, target_function>(fvendor->get_ctrl_char(rcc_ctrl_16));
		case 'W':	return character_parallel_regexp<target_container, target_iterator, target_function>(fvendor->get_ctrl_char(rcc_ctrl_17));
		case 'X':	return character_parallel_regexp<target_container, target_iterator, target_function>(fvendor->get_ctrl_char(rcc_ctrl_18));
		case 'Y':	return character_parallel_regexp<target_container, target_iterator, target_function>(fvendor->get_ctrl_char(rcc_ctrl_19));
		case 'Z':	return character_parallel_regexp<target_container, target_iterator, target_function>(fvendor->get_ctrl_char(rcc_ctrl_1a));
		case '[':	return character_parallel_regexp<target_container, target_iterator, target_function>(fvendor->get_ctrl_char(rcc_ctrl_1b));
		case '\\':	return character_parallel_regexp<target_container, target_iterator, target_function>(fvendor->get_ctrl_char(rcc_ctrl_1c));
		case ']':	return character_parallel_regexp<target_container, target_iterator, target_function>(fvendor->get_ctrl_char(rcc_ctrl_1d));
		case '^':	return character_parallel_regexp<target_container, target_iterator, target_function>(fvendor->get_ctrl_char(rcc_ctrl_1e));
		case '_':	return character_parallel_regexp<target_container, target_iterator, target_function>(fvendor->get_ctrl_char(rcc_ctrl_1f));
		default:	throw;
		}
	}
	target_container syntax_ch(char c)
	{
		switch (c)
		{
		case '\\':	return fvendor->get_charcter(rsc_esc);
		case '{':	return fvendor->get_charcter(rsc_group_s);
		case '}':	return fvendor->get_charcter(rsc_group_e);
		case '(':	return fvendor->get_charcter(rsc_class_s);
		case ')':	return fvendor->get_charcter(rsc_class_e);
		case '[':	return fvendor->get_charcter(rsc_set_s);
		case ']':	return fvendor->get_charcter(rsc_set_e);
		case '<':	return fvendor->get_charcter(rsc_category_s);
		case '>':	return fvendor->get_charcter(rsc_category_e);
		case ':':	return fvendor->get_charcter(rsc_delimiter);
		case '\'':	return fvendor->get_charcter(rsc_quoter);
		case '-':	return fvendor->get_charcter(rsc_set_tie);
		case ',':	return fvendor->get_charcter(rsc_repeat);
		case '^':	return fvendor->get_charcter(rsc_no_set);
		case '.':	return fvendor->get_charcter(rsc_char);
		case '$':	return fvendor->get_charcter(rsc_end);
		case '&':	return fvendor->get_charcter(rsc_name);
		case '#':	return fvendor->get_charcter(rsc_comment);
		case '?':	return fvendor->get_charcter(rsc_binary);
		case '*':	return fvendor->get_charcter(rsc_any);
		case '+':	return fvendor->get_charcter(rsc_over_one);
		case '=':	return fvendor->get_charcter(rsc_equal);
		case '!':	return fvendor->get_charcter(rsc_not);
		case '|':	return fvendor->get_charcter(rsc_or);
		case 'm':	return fvendor->get_charcter(rsc_opt_m);
		case 's':	return fvendor->get_charcter(rsc_opt_s);
		case 'i':	return fvendor->get_charcter(rsc_opt_i);
		case 'x':	return fvendor->get_charcter(rsc_opt_x);
		case 'Q':	return fvendor->get_charcter(rsc_quort_s);
		case 'E':	return fvendor->get_charcter(rsc_property_e);
		case 'R':	return fvendor->get_charcter(rsc_refarence);
		case '0':	return fvendor->get_charcter(rsc_refarence_0);
		case 'k':	return fvendor->get_charcter(rsc_refarence_name);
		case 'c':	return fvendor->get_charcter(rsc_pre_ctrl);
		case 'D':	return fvendor->get_charcter(rsc_define_d);
		case 'F':	return fvendor->get_charcter(rsc_define_f);
		case 'I':	return fvendor->get_charcter(rsc_define_i);
		case 'N':	return fvendor->get_charcter(rsc_define_n);
		default:	throw;
		}
	}
	target_container syntax_ch(char* c, size_t sz)
	{
		target_container ret;
		for (size_t l = 0; l < sz; l++)
			ret += syntax_ch(c[l]);
		return ret;
	}
	iregexp_base<target_container, target_iterator, target_function> pc_not_syntax(char c)
	{
		return pc_syntax(c, true);
	}
	iregexp_base<target_container, target_iterator, target_function> pc_syntax(char c, bool not = false)
	{
		return character_parallel_regexp<target_container, target_iterator, target_function>(syntax_ch(c), not);
	}
	iregexp_base<target_container, target_iterator, target_function> pc_not_syntax(char* c)
	{
		return pc_syntax(c, true);
	}
	iregexp_base<target_container, target_iterator, target_function> pc_syntax(char* c, bool not = false)
	{
		target_container v = syntax_ch(c, strlen(c));
		return character_parallel_regexp<target_container, target_iterator, target_function>(v, not);
	}
	iregexp_base<target_container, target_iterator, target_function> ps_syntax(char* c)
	{
		target_container v = syntax_ch(c, strlen(c));
		return character_serial_regexp<target_container, target_iterator, target_function>(v);
	}
	iregexp_vendor<target_container, target_iterator, target_function>* fvendor;
	iregexp_base<target_container, target_iterator, target_function> fchar_parser;
	iregexp_base<target_container, target_iterator, target_function> fclass_parser;
	iregexp_base<target_container, target_iterator, target_function> fclass_parser2;
	iregexp_base<target_container, target_iterator, target_function> fcondition_parser;
	iregexp_base<target_container, target_iterator, target_function> fcondition_parser2;
	iregexp_base<target_container, target_iterator, target_function> foption_parser;
	iregexp_base<target_container, target_iterator, target_function> fregexp_parser;
	static void char_class(void* param, long param2, short index, const target_iterator vb, target_iterator &vp, const target_iterator ve)
	{
		regexp_parser *parser = (regexp_parser*)param;
		int type = (int)param2;
		switch (type)
		{
		case 0:
			switch (index)
			{
			case  0: 	parser->set_class(parser->d_p());	break;	//[ch_p(L'd')]	// 10 進数字にマッチング。 
			case  1: 	parser->set_class(parser->D_p());	break;	//[ch_p(L'D')]	// 非 10 進数字にマッチング。 
			case  2: 	parser->set_class(parser->w_p());	break;	//[ch_p(L'w')]	// 「単語」文字にマッチング。 
			case  3: 	parser->set_class(parser->W_p());	break;	//[ch_p(L'W')]	// 非「単語」文字にマッチング。 
			case  4: 	parser->set_class(parser->s_p());	break;	//[ch_p(L's')]	// 空白文字にマッチング。 
			case  5: 	parser->set_class(parser->S_p());	break;	//[ch_p(L'S')]	// 非空白文字にマッチング。 
			case  6: 	parser->set_class(parser->h_p());	break;	//[ch_p(L'h')]	// 水平空白文字にマッチング。 
			case  7: 	parser->set_class(parser->H_p());	break;	//[ch_p(L'H')]	// 水平空白でない文字にマッチング。 
			case  8: 	parser->set_class(parser->v_p());	break;	//[ch_p(L'v')]	// 垂直空白文字にマッチング。 
			case  9: 	parser->set_class(parser->V_p());	break;	//[ch_p(L'V')]	// 垂直空白でない文字にマッチング。
			case 10: 	parser->set_class(parser->a_p());	break;	//[ch_p(L'a')]
			case 11: 	parser->set_class(parser->e_p());	break;	//[ch_p(L'e')]
			case 12: 	parser->set_class(parser->f_p());	break;	//[ch_p(L'f')]
			case 13: 	parser->set_class(parser->n_p());	break;	//[ch_p(L'n')]
			case 14: 	parser->set_class(parser->r_p());	break;	//[ch_p(L'r')]
			case 15: 	parser->set_class(parser->t_p());	break;	//[ch_p(L't')]
			case 16: 	parser->set_class(parser->R_p());	break;	//[ch_p(L'R')]
			}
			break;
		case 1:
			switch (index)
			{
			case  0:	parser->set_class(parser->alpha_p(parser->fnot_char_class ^ parser->fnot_char_set));	break;	//[str_p(L"alpha")]	// 任意の英字 ("[A-Za-z]")。 
			case  1:	parser->set_class(parser->alnum_p(parser->fnot_char_class ^ parser->fnot_char_set));	break;	//[str_p(L"alnum")]	// 任意の英数字。("[A-Za-z0-9]") 
			case  2:	parser->set_class(parser->blank_p(parser->fnot_char_class ^ parser->fnot_char_set));	break;	//[str_p(L"blank")]	// GNU 拡張; スペースまたは水平タブ ("\t") と同じ。 
			case  3:	parser->set_class(parser->cntrl_p(parser->fnot_char_class ^ parser->fnot_char_set));	break;	//[str_p(L"cntrl")]	// 任意の制御文字。後述の [2] 参照。 
			case  4:	parser->set_class(parser->digit_p(parser->fnot_char_class ^ parser->fnot_char_set));	break;	//[str_p(L"digit")]	// 任意の 10 進数字 ("[0-9]"); "\d" と等価。 
			case  5:	parser->set_class(parser->lower_p(parser->fnot_char_class ^ parser->fnot_char_set));	break;	//[str_p(L"lower")]	// 任意の小文字 ("[a-z]")。 
			case  6:	parser->set_class(parser->space_p(parser->fnot_char_class ^ parser->fnot_char_set));	break;	//[str_p(L"space")]	// 任意の空白文字。"\s" に加えて水平タブ ("\cK")。 
			case  7:	parser->set_class(parser->upper_p(parser->fnot_char_class ^ parser->fnot_char_set));	break;	//[str_p(L"upper")]	// 任意の大文字 ("[A-Z]")。 
			case  8:	parser->set_class(parser->word_p(parser->fnot_char_class ^ parser->fnot_char_set));		break;	//[str_p(L"word")]	// Perl 拡張 ("[A-Za-z0-9_]"); "\w" と等価。 
			case  9:	parser->set_class(parser->xdigit_p(parser->fnot_char_class ^ parser->fnot_char_set));	break;	//[str_p(L"xdigit")]	// 任意の 16 進文字 ("[0-9a-fA-F]")。
				//case 10:	parser->set_class(parser->ascii_p(parser->fnot_char_class ^ parser->fnot_char_set));	break;	//[str_p(L"ascii")]	// 任意の ASCII 文字集合の文字。 
				//case 11:	parser->set_class(parser->graph_p(parser->fnot_char_class ^ parser->fnot_char_set));	break;	//[str_p(L"graph")]	// 任意の表示文字; スペースを除く。後述の [3] 参照。 
				//case 12:	parser->set_class(parser->print_p(parser->fnot_char_class ^ parser->fnot_char_set));	break;	//[str_p(L"print")]	// 任意の表示文字; スペースを含む。後述の [4] 参照。 
				//case 13:	parser->set_class(parser->punct_p(parser->fnot_char_class ^ parser->fnot_char_set));	break;	//[str_p(L"punct")]	// 任意の「単語」文字を除く表示文字。[5] 参照。 
			}
			break;
		case 2:
			parser->set_class(parser->fvendor->get_ctrl_parser((reg_ctrl_ch)index));
			break;
		}
	}
	static void not_char_class(void* param, long param2, bool success, const target_iterator vb, target_iterator &vp, const target_iterator ve)
	{
		regexp_parser *parser = (regexp_parser*)param;
		parser->fnot_char_class = success;
	}
	static void not_char_set(void* param, long param2, bool success, const target_iterator vb, target_iterator &vp, const target_iterator ve)
	{
		regexp_parser *parser = (regexp_parser*)param;
		parser->fnot_char_set = success;
	}
	static void char_set(void* param, long param2, short index, const target_iterator vb, target_iterator &vp, const target_iterator ve)
	{
		regexp_parser *parser = (regexp_parser*)param;
		switch (index)
		{
		case 0:	parser->set_charset(parser->fvendor->get_range_p(parser->ftiechar_b, parser->ftiechar_l));	break;	//[anychar_p() >> ch_p(L'-') >> anychar_p()]
		case 1:	parser->set_charset(parser->ch_p(vp + 1));										break;	//[ch_p(L'\\') >> ch_p(L"\\]R")]
		case 2:	parser->set_charset(parser->ch_p(parser->fvendor->get_esc_char(rpc_esc_b)));	break;	//[ch_p(L'\b')]
		case 3:	parser->set_charset(parser->ch_p(parser->fxchar));								break;	//[xchar_class]
		case 4:	parser->set_charset();															break;	//[char_class]
		case 5:	parser->set_charset();															break;	//[char_class]
		case 6:	parser->set_charset(parser->ch_p(vp));											break;	//[not_ch_p(L']')]
		}
	}
	static void classter_condition(void* param, long param2, const target_iterator vb, target_iterator &vp, const target_iterator ve)
	{
		regexp_parser *parser = (regexp_parser*)param;
		parser->set_classter(param2);
		parser->push_classter();
	}
	static void classter_opt2(void* param, long param2, short index, const target_iterator vb, target_iterator &vp, const target_iterator ve)
	{
		regexp_parser *parser = (regexp_parser*)param;
		switch (param2)
		{
		case 0:
			switch (index)
			{
			case 0: parser->set_classter_type(ct_switch);		break;
			case 1: parser->set_classter_type(ct_select);		break;
			case 2: parser->set_classter_type(ct_for);			break;
			case 3: parser->set_classter_type(ct_if_action);	break;
			case 4: parser->set_classter_type(ct_action);		break;
			case 5: parser->set_classter_type(ct_not_action);	break;
			case 6: parser->set_classter_type(ct_query);		break;
			case 7: parser->set_classter_type(ct_func);			break;
			case 8: parser->set_classter_type(ct_while);		break;
			case 9: parser->set_classter_type(ct_do_while);		break;
			case 10: parser->set_classter_type(ct_define);		break;
			case 11:
				parser->set_classter_type(ct_if);
				//parser->fif_condition = false;
				break;
			default: parser->set_classter_type(ct_null);
			}
			break;
		}
	}
	static void condition_param(void* param, long param2, short index, const target_iterator vb, target_iterator &vp, const target_iterator ve)
	{
		regexp_parser *parser = (regexp_parser*)param;
		switch (index)
		{
		case 0: parser->add_(parser->match_p(parser->fvendor->to_num(vp, ve)));			break;
		case 1: parser->add_(parser->match_p(target_container(vp + 1, ve - 1)));		break;
		case 2: parser->add_(parser->match_p(target_container(vp + 1, ve - 1)));		break;
		case 3: parser->add_(parser->recursion_p(parser->fvendor->to_num(vp + 1, ve)));		break;
		case 4: parser->add_(parser->recursion_p(target_container(vp + 2, ve)));		break;
		case 5:	parser->add_(parser->recursion_p());									break;
		}
	}
	static void classter_opt(void* param, long param2, short index, const target_iterator vb, target_iterator &vp, const target_iterator ve)
	{
		regexp_parser *parser = (regexp_parser*)param;
		parser->set_classter_type(ct_null);
		int type = (int)param2;
		if (type == 0)
		{
			switch (index)
			{
			case 0:
				if (parser->fopt_s) parser->foptions.s();
				if (parser->fopt_m) parser->foptions.m();
				if (parser->fopt_i) parser->foptions.i();
				if (parser->fopt_x) parser->foptions.x();
				if (parser->fopt_S) parser->foptions.S();
				if (parser->fopt_M) parser->foptions.M();
				if (parser->fopt_I) parser->foptions.I();
				if (parser->fopt_X) parser->foptions.X();
				break;	//[ch_p(L":")]
			case 1: parser->set_classter_type(ct_befor_assertion);		break;	//[ch_p(L"=")]
			case 2: parser->set_classter_type(ct_not_befor_assertion);	break;	//[ch_p(L"!")]
			case 3: parser->set_classter_type(ct_after_assertion);		break;	//[str_p(L"<=")]
			case 4: parser->set_classter_type(ct_not_after_assertion);	break;	//[str_p(L"<!")]
			case 5: parser->set_classter_type(ct_no_backtrack);			break;	//[ch_p(L">")]
			case 6: parser->set_classter_type(ct_branch_reset);			break;	//[ch_p(L"|")]
			case 7: parser->set_classter_type(ct_name_capture); parser->set_classter_name(target_container(vp + 1, ve - 1));	break;	//[ch_p(L"<") >> + not_ch_p(L">") >> ch_p(L">")]
			case 8: parser->set_classter_type(ct_name_capture); parser->set_classter_name(target_container(vp + 1, ve - 1));	break;	//[ch_p(L"'") >> + not_ch_p(L"'") >> ch_p(L"'")]
			}
		}
		else
		{
			switch (index)
			{
			case 0: parser->add_(parser->null_p(parser->str_p(vp, ve)));								break;	//[ch_p(L"#") >> * not_ch_p(L")")]
			case 1: parser->add_(parser->call_p());														break;	//[ch_p(L"R")]
			case 2: parser->add_(parser->call_p());														break;	//[ch_p(L"0")]
			case 3: parser->add_(parser->call_p(parser->fvendor->to_num(vp, ve)));						break;	//[+ digit_p()]
			case 4: parser->add_(parser->call_p(parser->fvendor->to_num(vp + 1, ve), parser->frelative_call_sign));	break;	//[ch_p(L"+-") >> + digit_p()]
			case 5: parser->add_(parser->call_p(target_container(vp + 1, ve)));							break;	//[ch_p(L"&") >> + not_ch_p(L")")]
			case 6:
				if (parser->fopt_s) parser->fclasster_stack.back().second.s();
				if (parser->fopt_m) parser->fclasster_stack.back().second.m();
				if (parser->fopt_i) parser->fclasster_stack.back().second.i();
				if (parser->fopt_x) parser->fclasster_stack.back().second.x();
				if (parser->fopt_S) parser->fclasster_stack.back().second.S();
				if (parser->fopt_M) parser->fclasster_stack.back().second.M();
				if (parser->fopt_I) parser->fclasster_stack.back().second.I();
				if (parser->fopt_X) parser->fclasster_stack.back().second.X();
				break;
			}
		}
	}
	static void relative_call_sign(void* param, long param2, short index, const target_iterator vb, target_iterator &vp, const target_iterator ve)
	{
		regexp_parser *parser = (regexp_parser*)param;
		parser->frelative_call_sign = index == 1;
	}
	static void classter(void* param, long param2, const target_iterator vb, target_iterator &vp, const target_iterator ve)
	{
		regexp_parser *parser = (regexp_parser*)param;
		parser->push_classter();
	}
	static void char_parse(void* param, long param2, short index, const target_iterator vb, target_iterator &vp, const target_iterator ve)
	{
		regexp_parser *parser = (regexp_parser*)param;
		switch (index)
		{
		case  0:	parser->add_char();																	break;	//[xchar_class]
		case  1:	parser->add_class();																break;	//[char_class]
		case  2:	parser->add_charset();																break;	//[char_set]
		case  3:	parser->add_(parser->ESC_p());														break;	//[ch_p(L"\\\\")]
		case  4:	parser->add_(parser->foptions.opt_s ? parser->Any_p() : parser->Char_p());			break;	//[ch_p(L'.')]
		case  5:	parser->add_(parser->foptions.opt_m ? parser->line_begin_p() : parser->begin_p());	break;	//[ch_p(L'^')]
		case  6:	parser->add_(parser->foptions.opt_m ? parser->line_end_p() : parser->end_p());		break;	//[ch_p(L'$')]
		case  7:	parser->add_(parser->word_gap_p());													break;	//[str_p(L"\\b")]
		case  8:	parser->add_(parser->not_word_gap_p());												break;	//[str_p(L"\\B")]
		case  9:	parser->add_(parser->line_begin_p());												break;	//[str_p(L"\\A")]
		case 10:	parser->add_(parser->line_done_p());												break;	//[str_p(L"\\z")]
		case 11:	parser->add_(parser->line_end_p());													break;	//[str_p(L"\\Z")]
		case 12:	parser->back_classter();															break;	//[ch_p(L'(') >> call_p((size_t)0) >> ch_p(L')')]
		case 13:	parser->add_(parser->use_p(parser->fvendor->to_num(vp + 1, ve)));					break;	//[ch_p(L'\\') >> + digit_p()]
		case 14:	parser->add_(parser->use_p(target_container(vp + 3, ve - 1)));						break;	//[str_p("\\k<") >> +not_ch_p('>') >> ch_p('>')]
		case 15:	parser->add_(parser->use_p(target_container(vp + 3, ve - 1)));						break;	//[str_p("\\k\'") >> +not_ch_p('\'') >> ch_p('\'')]
		case 16:	parser->add_(parser->str_p(vp + 2, ve - 2));										break;	//[str_p(L"\\Q") >> * (anychar_p() - str_p(L"\\E")) >> str_p(L"\\E")]
		case 17:	parser->add_(parser->ch_p(vp + 1));													break;	//[ch_p(L'\\') >> ch_p(L"\\?*+.^$|([")]
		case 18:	parser->add_(parser->str_p(vp, ve));													break;	//[not_ch_p(L"\\?*+.^$|([")]
		}
	}
	static void option_reset(void* param, long param2, const target_iterator vb, target_iterator &vp, const target_iterator ve)
	{
		regexp_parser *parser = (regexp_parser*)param;
		parser->fopt_s = false;
		parser->fopt_m = false;
		parser->fopt_i = false;
		parser->fopt_x = false;
		parser->fopt_S = false;
		parser->fopt_M = false;
		parser->fopt_I = false;
		parser->fopt_X = false;
	}
	static void option_set(void* param, long param2, short index, const target_iterator vb, target_iterator &vp, const target_iterator ve)
	{
		regexp_parser *parser = (regexp_parser*)param;
		int type = (int)param2;
		switch (index)
		{
		case 0:	type == 0 ? parser->fopt_m = true : parser->fopt_M = true;	break;
		case 1:	type == 0 ? parser->fopt_s = true : parser->fopt_S = true;	break;
		case 2:	type == 0 ? parser->fopt_i = true : parser->fopt_I = true;	break;
		case 3:	type == 0 ? parser->fopt_x = true : parser->fopt_X = true;	break;
		}
	}
	static void option_action(void* param, long param2, bool success, const target_iterator vb, target_iterator &vp, const target_iterator ve)
	{
		regexp_parser *parser = (regexp_parser*)param;
		parser->foption_set = success;
	}
	static bool check_option_action(void* param, long param2, const target_iterator vb, target_iterator &vp, const target_iterator ve)
	{
		regexp_parser *parser = (regexp_parser*)param;
		return parser->foption_set;
	}
	static void math_parse(void* param, long param2, short index, const target_iterator vb, target_iterator &vp, const target_iterator ve)
	{
		regexp_parser *parser = (regexp_parser*)param;
		switch (index)
		{
		case 0:	parser->set_math(0, 1);		break;	//?
		case 1:	parser->set_math(0, -1);	break;	//*
		case 2:	parser->set_math(1, -1);	break;	//+
		case 3:	parser->set_math(parser->fmin, parser->fmax);	break;	//{}
		}
	}
	static void math_opt(void* param, long param2, short index, const target_iterator vb, target_iterator &vp, const target_iterator ve)
	{
		regexp_parser *parser = (regexp_parser*)param;
		switch (index)
		{
		case 0:	parser->set_weak_match();	break;	//?
		case 1:	parser->set_force_match();	break;	//+
		}
	}
	static void action_param(void* param, long param2, bool success, const target_iterator vb, target_iterator &vp, const target_iterator ve)
	{
		regexp_parser *parser = (regexp_parser*)param;
		if (success)
			parser->faction_param = parser->fvendor->to_num(vp, ve);
		else
			parser->faction_param = 0;
	}
	static void action_xparam(void* param, long param2, bool success, const target_iterator vb, target_iterator &vp, const target_iterator ve)
	{
		regexp_parser *parser = (regexp_parser*)param;
		if (success)
			parser->faction_param = parser->fvendor->to_num(vp, ve, 16);
		else
			parser->faction_param = 0;
	}
	static void math_repeat(void* param, long param2, const target_iterator vb, target_iterator &vp, const target_iterator ve)
	{
		regexp_parser *parser = (regexp_parser*)param;
		int type = (int)param2;
		switch (type)
		{
		case 0:	parser->fmin = parser->fmax = parser->fvendor->to_num(vp, ve); break;
		case 1:	parser->fmax = -1;				break;
		case 2: parser->fmax = parser->fvendor->to_num(vp, ve);	break;
		}
	}
	static void xchar_class(void* param, long param2, const target_iterator vb, target_iterator &vp, const target_iterator ve)
	{
		regexp_parser *parser = (regexp_parser*)param;
		parser->fxchar = parser->fvendor->hash_to(parser->fvendor->to_num(vp, ve, 16));
	}
	static void xchar_set(void* param, long param2, const target_iterator vb, target_iterator &vp, const target_iterator ve)
	{
		regexp_parser *parser = (regexp_parser*)param;
		int type = (int)param2;
		if (type == 0)
			parser->ftiechar_b = parser->fxchar;
		else
			parser->ftiechar_l = parser->fxchar;
	}
	static void anychar_set(void* param, long param2, const target_iterator vb, target_iterator &vp, const target_iterator ve)
	{
		regexp_parser *parser = (regexp_parser*)param;
		int type = (int)param2;
		if (type == 0)
			parser->ftiechar_b = target_container(vp, vp + 1);
		else
			parser->ftiechar_l = target_container(vp, vp + 1);
	}
	static void escchar_set(void* param, long param2, const target_iterator vb, target_iterator &vp, const target_iterator ve)
	{
		regexp_parser *parser = (regexp_parser*)param;
		int type = (int)param2;
		if (type == 0)
			parser->ftiechar_b = target_container(vp + 1, vp + 2);
		else
			parser->ftiechar_l = target_container(vp + 1, vp + 2);
	}
	static void escchar_b_set(void* param, long param2, const target_iterator vb, target_iterator &vp, const target_iterator ve)
	{
		regexp_parser *parser = (regexp_parser*)param;
		int type = (int)param2;
		if (type == 0)
			parser->ftiechar_b = parser->fvendor->get_esc_char(rpc_esc_b);
		else
			parser->ftiechar_l = parser->fvendor->get_esc_char(rpc_esc_b);
	}
	static void or_tie(void* param, long param2, const target_iterator vb, target_iterator &vp, const target_iterator ve)
	{
		regexp_parser *parser = (regexp_parser*)param;
		parser->push_or();
	}
	static void parse_end(void* param, long param2, const target_iterator vb, target_iterator &vp, const target_iterator ve)
	{
		regexp_parser *parser = (regexp_parser*)param;
		parser->back_or();
	}
	static bool check_opt_x(void* param, long param2, const target_iterator vb, target_iterator &vp, const target_iterator ve)
	{
		regexp_parser *parser = (regexp_parser*)param;
		return parser->foptions.opt_x;
	}
	regexp_parser(const regexp_parser&) {}
public:
	~regexp_parser() { delete fvendor; }
	regexp_parser(long obectsize)
	{
		fvendor = new iregexp_vendor<target_container, target_iterator, target_function>(obectsize);
		fchar_parser
			= no_backtrack_p(ps_syntax("\\x")
			>> (
			action_p(this, 0, &regexp_parser::xchar_class)[repeat_p(2)[xdigit_p()]]
			| pc_syntax('{') >> action_p(this, 0, &regexp_parser::xchar_class)[repeat_p(4)[xdigit_p()]] >> pc_syntax('}')
			))
			;
		fclass_parser
			= no_backtrack_p(befor_assertion_p(pc_syntax('\\'))
			>> select_p(this, 0, &regexp_parser::char_class)
			[ps_d()]	// 10 進数字にマッチング。 
		[ps_D()]	// 非 10 進数字にマッチング。 
		[ps_w()]	// 「単語」文字にマッチング。 
		[ps_W()]	// 非「単語」文字にマッチング。 
		[ps_s()]	// 空白文字にマッチング。 
		[ps_S()]	// 非空白文字にマッチング。 
		[ps_h()]	// 水平空白文字にマッチング。 
		[ps_H()]	// 水平空白でない文字にマッチング。 
		[ps_v()]	// 垂直空白文字にマッチング。 
		[ps_V()]	// 垂直空白でない文字にマッチング。
		[ps_a()]
		[ps_e()]
		[ps_f()]
		[ps_n()]
		[ps_r()]
		[ps_t()]
		[ps_R()]
		[ps_syntax("\\c") >> select_p(this, 2, &regexp_parser::char_class)
			[pc_control('@')][pc_control('A')][pc_control('B')][pc_control('C')][pc_control('D')][pc_control('E')][pc_control('F')][pc_control('G')]
			[pc_control('H')][pc_control('I')][pc_control('J')][pc_control('K')][pc_control('L')][pc_control('M')][pc_control('N')][pc_control('O')]
			[pc_control('P')][pc_control('Q')][pc_control('R')][pc_control('S')][pc_control('T')][pc_control('U')][pc_control('V')][pc_control('W')]
			[pc_control('X')][pc_control('Y')][pc_control('Z')][pc_control('[')][pc_control('\\')][pc_control(']')][pc_control('^')][pc_control('_')]
		])
		;
		fclass_parser2
			= no_backtrack_p(ps_syntax("[:")
			>> !if_action_p(this, 0, &regexp_parser::not_char_class)[pc_syntax('^')]
			>> select_p(this, 1, &regexp_parser::char_class)
			[ps_alpha()]	// 任意の英字 ("[A - Za - z]")。 
		[ps_alnum()]	// 任意の英数字。("[A - Za - z0 - 9]") 
		[ps_blank()]	// GNU 拡張; スペースまたは水平タブ ("\t") と同じ。 
		[ps_cntrl()]	// 任意の制御文字。後述の [2] 参照。 
		[ps_digit()]	// 任意の 10 進数字 ("[0 - 9]"); "\d" と等価。 
		[ps_lower()]	// 任意の小文字 ("[a - z]")。 
		[ps_space()]	// 任意の空白文字。"\s" に加えて水平タブ ("\cK")。 
		[ps_upper()]	// 任意の大文字 ("[A - Z]")。 
		[ps_word()]		// Perl 拡張 ("[A - Za - z0 - 9_]"); "\w" と等価。 
		[ps_xdigit()]	// 任意の 16 進文字 ("[0 - 9a - fA - F]")。
		//[ps_ascii")]	// 任意の ASCII 文字集合の文字。 
		//[ps_graph")]	// 任意の表示文字; スペースを除く。後述の [3] 参照。 
		//[ps_print")]	// 任意の表示文字; スペースを含む。後述の [4] 参照。 
		//[ps_punct")]	// 任意の「単語」文字を除く表示文字。[5] 参照。 
		>> ps_syntax(":]"))
			;
		fcondition_parser = no_backtrack_p(if_action_p(this, 0, &regexp_parser::action_param)[repeat_p(1, 10)[digit_p()]] | ps_syntax("\\x") >> if_action_p(this, 0, &regexp_parser::action_xparam)[repeat_p(1, 8)[xdigit_p()]]);
		fcondition_parser2 = select_p(this, 0, &regexp_parser::condition_param)
			[+digit_p()]
		[pc_syntax('<') >> +pc_not_syntax('>') >> pc_syntax('>')]
		[pc_syntax('\'') >> +pc_not_syntax('\'') >> pc_syntax('\'')]
		[pc_syntax('R') >> +digit_p()]
		[ps_syntax("R&") >> +pc_not_syntax(')')]
		[pc_syntax('R')]
		[call_p()]
		;
		foption_parser = !(func_p(this, 0, &regexp_parser::option_reset)
			>> !repeat_p(1, 4)[
				select_p(this, 0, &regexp_parser::option_set)
					[pc_syntax('m')]
				[pc_syntax('s')]
				[pc_syntax('i')]
				[pc_syntax('x')]
			]
				>> !(pc_syntax('-')
				>> repeat_p(1, 4)[
					select_p(this, 1, &regexp_parser::option_set)
						[pc_syntax('m')]
					[pc_syntax('s')]
					[pc_syntax('i')]
					[pc_syntax('x')]
				]
					)
					);
				fregexp_parser
					= null_p()[fchar_parser]
					>> null_p()[fclass_parser]
					>> null_p()[fclass_parser2]
					>> null_p()[pc_syntax('\\') >> pc_syntax("\\]R")]
					>> null_p()[fcondition_parser]
					>> null_p()[fcondition_parser2]
					>> null_p()[if_p(query_p(this, 0, &regexp_parser::check_opt_x))[*space_p()].else_p(null_p())]
					>> null_p()[foption_parser]
					>> call_p(7)
					>> select_p(this, 0, &regexp_parser::char_parse)
					[call_p(1)]
				[call_p(2)]
				[pc_syntax('[')
					>> !if_action_p(this, 0, &regexp_parser::not_char_set)[pc_syntax('^')]
					>> +select_p(this, 0, &regexp_parser::char_set)
					[(action_p(this, 0, &regexp_parser::xchar_set)[call_p(1)]
					| action_p(this, 0, &regexp_parser::escchar_set)[call_p(4)]
					| action_p(this, 0, &regexp_parser::escchar_b_set)[ps_b()]
					| action_p(this, 0, &regexp_parser::anychar_set)[pc_not_syntax(']')])
					>> pc_syntax('-')
					>> (
					action_p(this, 1, &regexp_parser::xchar_set)[call_p(1)]
					| action_p(this, 1, &regexp_parser::escchar_set)[call_p(4)]
					| action_p(this, 1, &regexp_parser::escchar_b_set)[ps_b()]
					| action_p(this, 1, &regexp_parser::anychar_set)[pc_not_syntax(']')]
					)
					]
				[call_p(4)]
				[ps_b()]
				[call_p(1)]
				[call_p(2)]
				[call_p(3)]
				[pc_not_syntax(']')]
				>> pc_syntax(']')
				]
				[ps_ESC()]
				[ps_Any()]
				[ps_begin()]
				[ps_end()]
				[ps_word_gap()]
				[ps_not_word_gap()]
				[ps_line_begin()]
				[ps_line_done()]
				[ps_line_end()]
				[action_p(this, 0, &regexp_parser::classter)[pc_syntax('(')]
					>> if_p(pc_syntax('?'))[
						if_p(pc_syntax('('))[
							select_p(this, 0, &regexp_parser::classter_opt2)
								/*switch*/[pc_syntax('<') >> !call_p(5) >> pc_syntax(')') >> call_p()]
							/*select*/[pc_syntax('>') >> !call_p(5) >> pc_syntax(')') >> call_p()]
							/*for*/[pc_syntax('#') >> !call_p(5) >> pc_syntax(':') >> action_p(this, ct_for, &regexp_parser::classter_condition)[call_p(6)] >> pc_syntax(')') >> call_p()]
							/*if_action*/[pc_syntax('|') >> !call_p(5) >> pc_syntax(')') >> call_p()]
							/*action*/[pc_syntax('=') >> !call_p(5) >> pc_syntax(')') >> call_p()]
							/*not_action*/[pc_syntax('!') >> !call_p(5) >> pc_syntax(')') >> call_p()]
							/*query*/[pc_syntax('?') >> !call_p(5) >> pc_syntax(')')]
							/*func*/[pc_syntax(':') >> !call_p(5) >> pc_syntax(')')]
							/*while*/[pc_syntax('*') >> action_p(this, ct_while, &regexp_parser::classter_condition)[call_p(6)] >> pc_syntax(')') >> call_p()]
							/*do_while*/[pc_syntax('+') >> action_p(this, ct_do_while, &regexp_parser::classter_condition)[call_p(6)] >> pc_syntax(')') >> call_p()]
							[action_p(this, ct_define, &regexp_parser::classter_condition)[ps_syntax("DEFINE")] >> pc_syntax(')') >> call_p()]
							[action_p(this, ct_if, &regexp_parser::classter_condition)[call_p(6)] >> pc_syntax(')') >> call_p()]
						].else_p(
							if_p(
							select_p(this, 0, &regexp_parser::classter_opt)
							[call_p(8) >> pc_syntax(':')]
						[pc_syntax('=')]
						[pc_syntax('!')]
						[ps_syntax("<=")]
						[ps_syntax("<!")]
						[pc_syntax('>')]
						[pc_syntax('|')]
						[pc_syntax('<') >> +pc_not_syntax('>') >> pc_syntax('>')]
						[pc_syntax('\'') >> +pc_not_syntax('\'') >> pc_syntax('\'')]
						)[call_p()].else_p(
							select_p(this, 1, &regexp_parser::classter_opt)
							[pc_syntax('#') >> *pc_not_syntax(')')]
						[pc_syntax('R')]
						[pc_syntax('0')]
						[+digit_p()]
						[
							select_p(this, 0, &regexp_parser::relative_call_sign)
							[pc_syntax('+')]
							[pc_syntax('-')]
							>> +digit_p()
						]
						[pc_syntax('&') >> +pc_not_syntax(')')]
						[call_p(8) /*>> befor_assertion_p(pc_syntax(')'))*/]
						)
							)
					].else_p(call_p())
					>> pc_syntax(')')
				]
				[pc_syntax('\\') >> +digit_p()]
				[ps_syntax("\\k<") >> +pc_not_syntax('>') >> pc_syntax('>')]
				[ps_syntax("\\k\'") >> +pc_not_syntax('\'') >> pc_syntax('\'')]
				[ps_syntax("\\Q") >> *(anychar_p() - ps_syntax("\\E")) >> ps_syntax("\\E")]
				[pc_syntax('\\') >> pc_syntax("?*+.^$|[{()")]
				[pc_not_syntax("\\?*+.^$|[{()")]
				>> !select_p(this, 0, &regexp_parser::math_parse)
					[pc_syntax('?')]
				[pc_syntax('*')]
				[pc_syntax('+')]
				[pc_syntax('{')
					>> action_p(this, 0, &regexp_parser::math_repeat)[repeat_p(1, 3)[digit_p()]]
					>> !(action_p(this, 1, &regexp_parser::math_repeat)[pc_syntax(',')]
					>> !action_p(this, 2, &regexp_parser::math_repeat)[repeat_p(1, 3)[digit_p()]])
					>> pc_syntax('}')
				]
				>> !(select_p(this, 0, &regexp_parser::math_opt)
					[pc_syntax("?")]
				[pc_syntax("+")]
				)
					>> !(call_p(7)
					>> !action_p(this, 0, &regexp_parser::or_tie)[pc_syntax('|')] >> not_action_p(this, 0, &regexp_parser::parse_end)[call_p()]
					)
					;
	}
	bool parse(target_iterator vb, target_iterator ve)
	{
		clear();
		target_iterator vp = vb;
		return fregexp_parser.match(vb, vp, ve) && vp == ve;
	}
	bool match(const target_iterator vb, target_iterator &vp, const target_iterator ve)
	{
		target_iterator v = vp;
		return fregexp.match(vb, vp, ve) && vp > v;
	}
	typename iregexp_base<target_container, target_iterator, target_function>::iregexp_item<target_container, target_iterator, target_function>::iregexp_item_point matchs(size_t index)
	{
		return fregexp.matchs(index);
	}
	void get_parser_string(std::wstring& str)
	{
		fregexp_parser.get_type_string(str);
	}
	void get_pattern_string(std::wstring& str)
	{
		fregexp.get_type_string(str);
	}
};

template<>
class iregexp_vendor<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >
{
private:
	int fmem_size;
	mem_container& get_container(mem_container& ret, long val)
	{
		char v1;
		wchar_t v2;
		long v4;
		switch (fmem_size)
		{
		case 1:
			v1 = (char)val;
			ret.append(&v1, 1, 0, fmem_size, 0);
			break;
		case 2:
			v2 = (wchar_t)val;
			ret.append(&v2, 1, 0, fmem_size, 0);
			break;
		case 4:
			v4 = val;
			ret.append(&v4, 1, 0, fmem_size, 0);
			break;
		default:
			throw;
		}
		return ret;
	}
	mem_container get_container(long val)
	{
		mem_container ret;
		return get_container(ret, val);
	}
	mem_container get_container(long b, long e)
	{
		mem_container ret;
		if (b > e)
		{
			e += b;
			b = e - b;
			e -= b;
		}
		for (long val = b; val <= e; ++val)
			get_container(ret, val);
		return ret;
	}
	mem_container get_container(void* val)
	{
		mem_container ret;
		size_t size = 0;
		switch (fmem_size)
		{
		case 1:
			size = strlen((char*)val);
			ret.append(val, size, 0, fmem_size, 0);
			break;
		case 2:
			size = wcslen((wchar_t*)val);
			ret.append(val, size, 0, fmem_size, 0);
			break;
		case 4:
			for (long* v = (long*)val; *v != 0; v++, size++);
			ret.append(val, size, 0, fmem_size, 0);
			break;
		default:
			throw;
		}
		return ret;
	}
public:
	iregexp_vendor(int size) : fmem_size(size) {}
	long to_num(mem_container::const_iterator vp, mem_container::const_iterator ve, size_t base = 10)
	{
		long ret = 0;
		while (vp != ve)
		{
			ret *= base;
			long val;
			switch (fmem_size)
			{
			case 1:
				val = *((char*)&*vp);
				break;
			case 2:
				val = *((wchar_t*)&*vp);
				break;
			case 4:
				val = *((long*)&*vp);
				break;
			default:
				throw;
			}
			ret += val - (val < 0x40 ? 0x30 : (val < 0x60 ? 0x41 : 0x61));
			++vp;
		}
		return ret;
	}
	long to_hash(mem_container v)
	{
		long ret = 0;
		switch (v.length())
		{
		case 1:
			ret = *((char*)&*v.begin().content());
			break;
		case 2:
			ret = *((wchar_t*)&*v.begin().content());
			break;
		case 4:
			ret = *((long*)&*v.begin().content());
			break;
		default:
			throw;
		}
		return ret;
	}
	mem_container hash_to(long v)
	{
		return get_container(v);
	}
	mem_container get_esc_char(reg_parse_ch type)
	{
		long ret = 0;
		switch (type)
		{
		case rpc_esc_a:		ret = '\a';		break;
		case rpc_esc_e:		ret = '\x1b';	break;
		case rpc_esc_f:		ret = '\f';		break;
		case rpc_esc_n:		ret = '\n';		break;
		case rpc_esc_r:		ret = '\r';		break;
		case rpc_esc_t:		ret = '\t';		break;
		case rpc_esc_b:		ret = '\b';		break;
		default:	throw;
		}
		return get_container(ret);
	}
	mem_container get_ctrl_char(reg_ctrl_ch type)
	{
		long ret = 0;
		switch (type)
		{
		case rcc_ctrl_00:		ret = '@';		break;
		case rcc_ctrl_01:		ret = 'A';		break;
		case rcc_ctrl_02:		ret = 'B';		break;
		case rcc_ctrl_03:		ret = 'C';		break;
		case rcc_ctrl_04:		ret = 'D';		break;
		case rcc_ctrl_05:		ret = 'E';		break;
		case rcc_ctrl_06:		ret = 'F';		break;
		case rcc_ctrl_07:		ret = 'G';		break;
		case rcc_ctrl_08:		ret = 'H';		break;
		case rcc_ctrl_09:		ret = 'I';		break;
		case rcc_ctrl_0a:		ret = 'J';		break;
		case rcc_ctrl_0b:		ret = 'K';		break;
		case rcc_ctrl_0c:		ret = 'L';		break;
		case rcc_ctrl_0d:		ret = 'M';		break;
		case rcc_ctrl_0e:		ret = 'N';		break;
		case rcc_ctrl_0f:		ret = 'O';		break;
		case rcc_ctrl_10:		ret = 'P';		break;
		case rcc_ctrl_11:		ret = 'Q';		break;
		case rcc_ctrl_12:		ret = 'R';		break;
		case rcc_ctrl_13:		ret = 'S';		break;
		case rcc_ctrl_14:		ret = 'T';		break;
		case rcc_ctrl_15:		ret = 'U';		break;
		case rcc_ctrl_16:		ret = 'V';		break;
		case rcc_ctrl_17:		ret = 'W';		break;
		case rcc_ctrl_18:		ret = 'X';		break;
		case rcc_ctrl_19:		ret = 'Y';		break;
		case rcc_ctrl_1a:		ret = 'Z';		break;
		case rcc_ctrl_1b:		ret = '[';		break;
		case rcc_ctrl_1c:		ret = '\\';		break;
		case rcc_ctrl_1d:		ret = ']';		break;
		case rcc_ctrl_1e:		ret = '^';		break;
		case rcc_ctrl_1f:		ret = '_';		break;
		default:	throw;
		}
		return get_container(ret);
	}
	mem_container get_charcter(reg_syntax_ch type)
	{
		long ret = 0;
		switch (type)
		{
		case rsc_esc:			ret = '\\';		break;
		case rsc_group_s:		ret = '{';		break;
		case rsc_group_e:		ret = '}';		break;
		case rsc_class_s:		ret = '(';		break;
		case rsc_class_e:		ret = ')';		break;
		case rsc_set_s:			ret = '[';		break;
		case rsc_set_e:			ret = ']';		break;
		case rsc_category_s:	ret = '<';		break;
		case rsc_category_e:	ret = '>';		break;
		case rsc_repeat:		ret = ',';		break;
		case rsc_delimiter:		ret = ':';		break;
		case rsc_quoter:		ret = '\'';		break;
		case rsc_set_tie:		ret = '-';		break;
		case rsc_no_set:		ret = '^';		break;
		case rsc_char:			ret = '.';		break;
		case rsc_end:			ret = '$';		break;
		case rsc_name:			ret = '&';		break;
		case rsc_binary:		ret = '?';		break;
		case rsc_comment:		ret = '#';		break;
		case rsc_any:			ret = '*';		break;
		case rsc_over_one:		ret = '+';		break;
		case rsc_equal:			ret = '=';		break;
		case rsc_not:			ret = '!';		break;
		case rsc_or:			ret = '|';		break;
		case rsc_quort_s:		ret = 'Q';		break;
		case rsc_property_e:	ret = 'E';		break;
		case rsc_opt_m:			ret = 'm';		break;
		case rsc_opt_s:			ret = 's';		break;
		case rsc_opt_i:			ret = 'i';		break;
		case rsc_opt_x:			ret = 'x';		break;
		case rsc_pre_ctrl:		ret = 'c';		break;
		case rsc_refarence:		ret = 'R';		break;
		case rsc_refarence_0:	ret = '0';		break;
		case rsc_refarence_name:	ret = 'k';		break;
		case rsc_define_d:		ret = 'D';		break;
		case rsc_define_f:		ret = 'F';		break;
		case rsc_define_i:		ret = 'I';		break;
		case rsc_define_n:		ret = 'N';		break;
		default:	throw;
		}
		return get_container(ret);
	}
	iregexp_base<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> > get_range_p(const mem_container b, const mem_container l)
	{
		return character_parallel_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(get_container(to_hash(b), to_hash(l)));
	}
	iregexp_base<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> > get_ctrl_parser(reg_ctrl_ch type)
	{
		long ret = 0;
		switch (type)
		{
		case rcc_ctrl_00:		ret = '\x00';		break;
		case rcc_ctrl_02:		ret = 'x02';		break;
		case rcc_ctrl_03:		ret = '\x03';		break;
		case rcc_ctrl_04:		ret = '\x04';		break;
		case rcc_ctrl_05:		ret = '\x05';		break;
		case rcc_ctrl_06:		ret = '\x06';		break;
		case rcc_ctrl_07:		ret = '\x07';		break;
		case rcc_ctrl_08:		ret = '\x08';		break;
		case rcc_ctrl_09:		ret = '\x09';		break;
		case rcc_ctrl_0a:		ret = '\x0a';		break;
		case rcc_ctrl_0b:		ret = '\x0b';		break;
		case rcc_ctrl_0c:		ret = '\x0c';		break;
		case rcc_ctrl_0d:		ret = '\x0d';		break;
		case rcc_ctrl_0e:		ret = '\x0e';		break;
		case rcc_ctrl_0f:		ret = '\x0f';		break;
		case rcc_ctrl_10:		ret = '\x10';		break;
		case rcc_ctrl_11:		ret = '\x11';		break;
		case rcc_ctrl_12:		ret = '\x12';		break;
		case rcc_ctrl_13:		ret = '\x13';		break;
		case rcc_ctrl_14:		ret = '\x14';		break;
		case rcc_ctrl_15:		ret = '\x15';		break;
		case rcc_ctrl_16:		ret = '\x16';		break;
		case rcc_ctrl_17:		ret = '\x17';		break;
		case rcc_ctrl_18:		ret = '\x18';		break;
		case rcc_ctrl_19:		ret = '\x19';		break;
		case rcc_ctrl_1a:		ret = '\x1a';		break;
		case rcc_ctrl_1b:		ret = '\x1b';		break;
		case rcc_ctrl_1c:		ret = '\x1c';		break;
		case rcc_ctrl_1d:		ret = '\x1d';		break;
		case rcc_ctrl_1e:		ret = '\x1e';		break;
		case rcc_ctrl_1f:		ret = '\x1f';		break;
		default:	throw;
		}
		return character_parallel_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(get_container(ret));
	}
	iregexp_base<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> > get_parser(reg_parse_ch type, bool not = false)
	{
		iregexp_base<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> > ret;
		switch (type)
		{
		case rpc_esc:	ret = character_parallel_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(get_container(L'\\'));	break;
		case rpc_any:	ret = any_char_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >();	break;
		case rpc_char:	ret = character_parallel_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(get_container(L"\x0a\x0b\x0c\x0d\x85\x2028\u2029"), true); break;
		case rpc_esc_d: ret = get_parser(rpc_digit); break;
		case rpc_esc_D: ret = get_parser(rpc_digit, true); break;
		case rpc_esc_w: ret = get_parser(rpc_word); break;
		case rpc_esc_W: ret = get_parser(rpc_word, true); break;
		case rpc_esc_s: ret = character_parallel_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(get_container(L"\x09\x0a\x0c\x0d\x20\x85\xa0\u1680\u180e\u2000\u2001\u2002\u2003\u2004\u2005\u2006\u2007\u2008\u2009\u200a\u2028\u2029\u202f\u205f\u3000"), not); break;
		case rpc_esc_S: ret = get_parser(rpc_esc_s, true); break;
		case rpc_esc_h: ret = character_parallel_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(get_container(L"\x09\x20\xa0\u1680\u180e\u2000\u2001\u2002\u2003\u2004\u2005\u2006\u2007\u2008\u2009\u200a\u202f\u205f\u3000"), not); break;
		case rpc_esc_H: ret = get_parser(rpc_esc_h, true); break;
		case rpc_esc_v: ret = character_parallel_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(get_container(L"\x0a\x0b\x0c\x0d\x85\u2028\u2029"), not); break;
		case rpc_esc_V: ret = get_parser(rpc_esc_v, true); break;
		case rpc_esc_a: ret = character_parallel_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(get_container(L'\x07')); break;
		case rpc_esc_e: ret = character_parallel_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(get_container(L'\x1b')); break;
		case rpc_esc_f: ret = character_parallel_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(get_container(L'\x0c')); break;
		case rpc_esc_n: ret = character_parallel_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(get_container(L'\x0a')); break;
		case rpc_esc_r: ret = character_parallel_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(get_container(L'\x0d')); break;
		case rpc_esc_t: ret = character_parallel_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(get_container(L'\x09')); break;
		case rpc_esc_R: ret = no_backtrack_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(character_parallel_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(get_container(L"\x0a\x0b\x0c\x0d\x85\x2028\u2029")) | character_serial_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(get_container(L"\r\n"))); break;
		case rpc_blank: ret = character_parallel_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(get_container(L" \t"), not); break;
		case rpc_space: ret = character_parallel_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(get_container(L"\x09\x0a\x0b\x0c\x0d\x20\x85\xa0\u1680\u180e\u2000\u2001\u2002\u2003\u2004\u2005\u2006\u2007\u2008\u2009\u200a\u2028\u2029\u202f\u205f\u3000"), not); break;
		case rpc_alpha: ret = character_parallel_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(get_container(L"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"), not); break;
		case rpc_alnum: ret = character_parallel_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(get_container(L"0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"), not); break;
		case rpc_upper: ret = character_parallel_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(get_container(L"ABCDEFGHIJKLMNOPQRSTUVWXYZ"), not); break;
		case rpc_lower: ret = character_parallel_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(get_container(L"abcdefghijklmnopqrstuvwxyz"), not); break;
		case rpc_cntrl: ret = character_parallel_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(get_container(0x00, 0x1f)) | character_parallel_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(get_container(128, 139)); break;
		case rpc_sign: ret = character_parallel_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(get_container(L"-+"), not); break;
		case rpc_word: ret = character_parallel_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(get_container(L"_0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"), not); break;
		case rpc_digit: ret = character_parallel_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(get_container(L"0123456789"), not); break;
		case rpc_xdigit: ret = character_parallel_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(get_container(L"0123456789abcdefABCDEF"), not); break;
		case rpc_word_gap: ret = assertion_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(get_parser(rpc_word), assertion_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >::at_boundary_assertion); break; // b
		case rpc_not_word_gap: ret = assertion_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(get_parser(rpc_word), assertion_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >::at_not_boundary_assertion); break; // B
		case rpc_begin: ret = ::begin_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(); break; // ^
		case rpc_end: ret = ::end_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(); break; // $
		case rpc_line_begin: ret = assertion_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(get_parser(rpc_esc_R) | ::begin_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(), assertion_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >::at_after_assertion); break; // A
		case rpc_line_end: ret = assertion_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(get_parser(rpc_esc_R) | ::end_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(), assertion_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >::at_befor_assertion); break; // Z
		case rpc_line_done: ret = iregexp_base<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(get_parser(rpc_esc_R) | ::end_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >()); break; // z
		default:	throw;
		}
		return ret;
	}
	iregexp_base<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> > get_command(reg_parse_ch type)
	{
		iregexp_base<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> > ret;
		switch (type)
		{
		case rpc_esc:			ret = character_serial_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(get_container(L"\\\\"));	break;
		case rpc_any:			ret = character_parallel_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(get_container(L'.'));		break;
		case rpc_char:			ret = character_parallel_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(get_container(L'.'));		break;
		case rpc_esc_d:			ret = character_serial_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(get_container(L"\\d"));		break;
		case rpc_esc_D:			ret = character_serial_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(get_container(L"\\D"));		break;
		case rpc_esc_w:			ret = character_serial_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(get_container(L"\\w"));		break;
		case rpc_esc_W:			ret = character_serial_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(get_container(L"\\W"));		break;
		case rpc_esc_s:			ret = character_serial_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(get_container(L"\\s"));		break;
		case rpc_esc_S:			ret = character_serial_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(get_container(L"\\S"));		break;
		case rpc_esc_h:			ret = character_serial_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(get_container(L"\\h"));		break;
		case rpc_esc_H:			ret = character_serial_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(get_container(L"\\H"));		break;
		case rpc_esc_v:			ret = character_serial_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(get_container(L"\\v"));		break;
		case rpc_esc_V:			ret = character_serial_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(get_container(L"\\V"));		break;
		case rpc_esc_a:			ret = character_serial_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(get_container(L"\\a"));		break;
		case rpc_esc_e:			ret = character_serial_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(get_container(L"\\e"));		break;
		case rpc_esc_f:			ret = character_serial_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(get_container(L"\\f"));		break;
		case rpc_esc_n:			ret = character_serial_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(get_container(L"\\n"));		break;
		case rpc_esc_r:			ret = character_serial_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(get_container(L"\\r"));		break;
		case rpc_esc_t:			ret = character_serial_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(get_container(L"\\t"));		break;
		case rpc_esc_b:			ret = character_serial_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(get_container(L"\\b"));		break;
		case rpc_esc_R:			ret = character_serial_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(get_container(L"\\R"));		break;
		case rpc_blank:			ret = character_serial_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(get_container(L"blank"));	break;
		case rpc_space:			ret = character_serial_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(get_container(L"space"));	break;
		case rpc_alpha:			ret = character_serial_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(get_container(L"alpha"));	break;
		case rpc_alnum:			ret = character_serial_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(get_container(L"alnum"));	break;
		case rpc_upper:			ret = character_serial_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(get_container(L"upper"));	break;
		case rpc_lower:			ret = character_serial_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(get_container(L"lower"));	break;
		case rpc_cntrl:			ret = character_serial_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(get_container(L"cntrl"));	break;
		case rpc_sign:			ret = character_serial_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(get_container(L"sign"));	break;
		case rpc_word:			ret = character_serial_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(get_container(L"word"));	break;
		case rpc_digit:			ret = character_serial_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(get_container(L"digit"));	break;
		case rpc_xdigit:		ret = character_serial_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(get_container(L"xdigit"));	break;
		case rpc_word_gap:		ret = character_serial_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(get_container(L"\\b"));		break; // b
		case rpc_not_word_gap:	ret = character_serial_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(get_container(L"\\B"));		break; // B
		case rpc_begin:			ret = character_parallel_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(get_container(L'^'));		break; // ^
		case rpc_end:			ret = character_parallel_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(get_container(L'$'));		break; // $
		case rpc_line_begin:	ret = character_serial_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(get_container(L"\\A"));		break; // A
		case rpc_line_end:		ret = character_serial_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(get_container(L"\\Z"));		break; // Z
		case rpc_line_done:		ret = character_serial_regexp<mem_container, mem_container::const_iterator, container_function<mem_container, mem_container::const_iterator> >(get_container(L"\\z"));		break; // z
		default:	throw;
		}
		return ret;
	}
};

#pragma once

#include	"tree.h"
#include	"RegExp.h"
#include	"RegExp_Builder.h"
#include	"RegExp_Vender.h"

enum reg_parse_state
{
	rpsMain,
	rpsMainMetaChar,
	rpsMainCharClass,
	rpsMainRegAssertion,
	rpsMainRepert,
	rpsMainEscChar,
	rpsMainEscKeywordChar,
	rpsMainEscMetaChar,
	rpsMainEscCtrlChar,
	rpsMainRepertOpt,
	rpsMainOrTie,
	rpsMainClasster,
	rpsMainDone,
	rpsExClass,
	rpsExClassEx,
	rpsRegEx,
	rpsRegAssertion,
	rpsCharSet,
	rpsCharSetDef,
	rpsCharSetBegin,
	rpsCharDone,
	rpsNumber,
	rpsCall,
	rpsRangeS,
	rpsRangeE,
	rpsRepertBegin,
	rpsRepertTie,
	rpsRepertEnd,
	rpsComment,
	rpsDeltaCall,
	rpsDeltaFlag,
	rpsName,
	rpsMapList,
	rpsMapName,
	rpsOptionX,
	rpsSetOption,
	rpsResetOption,
};

static iregexp_builder regular_expression_parser(void)
{
	iregexp_builder reg =
		call_p(6)
		>> select_p(rpsMain)
			.case_p(ch_p('^'))
			.case_p(ch_p('$'))
			.case_p(ch_p('\\') >> select_p(rpsMainRegAssertion).case_p(ch_p('A')).case_p(ch_p('Z')).case_p(ch_p('z')).case_p(ch_p('b')).case_p(ch_p('B')))
			.case_p(str_p("\\Q") >> *any_p() >> str_p("\\E"))
			.case_p(
				select_p(rpsMainMetaChar)
					.case_p(ch_p('.'))
					.case_p(ch_p('\\') >> select_p(rpsMainEscChar)
						.case_p(select_p(rpsMainEscKeywordChar).case_p(ch_p('\\')).case_p(ch_p('(')).case_p(ch_p('[')).case_p(ch_p(')')).case_p(ch_p('.')).case_p(ch_p('^')).case_p(ch_p('$')).case_p(ch_p('|')).case_p(ch_p('?')).case_p(ch_p('*')).case_p(ch_p('+')))
						.case_p(select_p(rpsMainCharClass).case_p(ch_p('s')).case_p(ch_p('S')).case_p(ch_p('d')).case_p(ch_p('D')).case_p(ch_p('w')).case_p(ch_p('W')))
						.case_p(call_p(3))
						.case_p(str_p("k'") >> action_p(rpsName, ++ + not_p('\'')) >> ch_p('\''))
						.case_p(str_p("k<") >> action_p(rpsName, ++ + not_p('>')) >> ch_p('>'))
						.case_p(select_p(rpsMainEscMetaChar).case_p(ch_p('n')).case_p(ch_p('r')).case_p(ch_p('t')).case_p(ch_p('f')).case_p(ch_p('e')))
						.case_p(ch_p('c') >> select_p(rpsMainEscCtrlChar).case_p(ch_p('@')).case_p(ch_p('A')).case_p(ch_p('B')).case_p(ch_p('C')).case_p(ch_p('D')).case_p(ch_p('E')).case_p(ch_p('F')).case_p(ch_p('G')).case_p(ch_p('H')).case_p(ch_p('I')).case_p(ch_p('J')).case_p(ch_p('K')).case_p(ch_p('L')).case_p(ch_p('M')).case_p(ch_p('N')).case_p(ch_p('O')).case_p(ch_p('P')).case_p(ch_p('Q')).case_p(ch_p('R')).case_p(ch_p('S')).case_p(ch_p('T')).case_p(ch_p('U')).case_p(ch_p('V')).case_p(ch_p('W')).case_p(ch_p('X')).case_p(ch_p('Y')).case_p(ch_p('Z')).case_p(ch_p('[')).case_p(ch_p('\\')).case_p(ch_p(']')).case_p(ch_p('^')).case_p(ch_p('_'))))
					.case_p(action_p(rpsMainClasster, ch_p('(')) >> call_p(1) >> call_p(6) >> ch_p(')'))
					.case_p(ch_p('[') >> call_p(2) >> ch_p(']'))
					.case_p(not_p("?*+)"))
				>> !if_action_p(rpsCharDone,
					select_p(rpsMainRepert)
						.case_p(ch_p('?'))
						.case_p(ch_p('*'))
						.case_p(ch_p('+'))
						.case_p(ch_p('{') >> call_p(6) >> action_p(rpsRepertBegin, call_p(3)) >> !(call_p(6) >> action_p(rpsRepertTie, ch_p(',') >> call_p(6)) >> !action_p(rpsRepertEnd, call_p(3))) >> call_p(6) >> ch_p('}'))
						>> !select_p(rpsMainRepertOpt).case_p(ch_p('?')).case_p(ch_p('+'))
					)
				) >> call_p(6) >> !action_p(rpsMainOrTie, ch_p('|')) >> !not_action_p(rpsMainDone, call_p(0))
		>> define_p(
			if_p(ch_p('?'),
				if_p(ch_p('('),
					select_p(rpsExClass)
						.case_p(ch_p('?') >> call_p(6) >> !call_p(3) >> call_p(6) >> ch_p(')'))					/*query*/
						.case_p(ch_p('#') >> call_p(6) >> !call_p(3) >> call_p(6) >> ch_p(')'))					/*func*/
						.case_p(ch_p('%') >> select_p(rpsDeltaFlag).case_p(ch_p('+')).case_p(ch_p('-')) >> ch_p(')'))	/*stack_map*/
						.case_p(ch_p('%') >> (!if_action_p(rpsDeltaCall, select_p(rpsDeltaFlag).case_p(ch_p('+')).case_p(ch_p('-'))) >> call_p(3)) >> str_p(":'") >> action_p(rpsMapList, ++ + not_p('\'')) >> str_p("')")
							>> (ch_p('\'') >> action_p(rpsMapName, ++ + not_p('\'')) >> ch_p('\'') | ch_p('<') >> action_p(rpsMapName, ++ + not_p('>')) >> ch_p('>')))	/*set_map*/
						.case_p(ch_p('%') >> call_p(6) >> ch_p('&') >> action_p(rpsName, ++ + not_p(':')) >> str_p(":'") >> action_p(rpsMapList, ++ + not_p('\'')) >> str_p("')")
							>> (ch_p('\'') >> action_p(rpsMapName, ++ + not_p('\'')) >> ch_p('\'') | ch_p('<') >> action_p(rpsMapName, ++ + not_p('>')) >> ch_p('>')))	/*set_map*/
						.case_p(select_p(rpsExClassEx)
							.case_p(ch_p('<') >> call_p(6) >> !call_p(3) >> call_p(6) >> ch_p(':') >> call_p(6) >> !call_p(0) >> call_p(6) >> ch_p(')'))	/*switch*/
							.case_p(str_p("%<") >> call_p(6) >> ch_p('&') >> (ch_p('\'') >> action_p(rpsMapName, ++ + not_p('\'')) >> ch_p('\'') | ch_p('<') >> action_p(rpsMapName, ++ + not_p('>')) >> ch_p('>')) >> ch_p(')'))	/*map_switch*/
							.case_p(ch_p('>') >> call_p(6) >> !call_p(3) >> call_p(6) >> ch_p(')'))						/*select*/
							.case_p(ch_p('@') >> call_p(6) >> !call_p(3) >> call_p(6) >> ch_p(':') >> call_p(6) >> call_p(4) >> call_p(6) >> ch_p(')'))	/*for*/
							.case_p(ch_p('|') >> call_p(6) >> !call_p(3) >> call_p(6) >> ch_p(')'))						/*if_action*/
							.case_p(ch_p('=') >> call_p(6) >> !call_p(3) >> call_p(6) >> ch_p(')'))						/*action*/
							.case_p(ch_p('!') >> call_p(6) >> !call_p(3) >> call_p(6) >> ch_p(')'))						/*not_action*/
							.case_p(ch_p('*') >> call_p(6) >> call_p(4) >> call_p(6) >> ch_p(')'))						/*while*/
							.case_p(ch_p('+') >> call_p(6) >> call_p(4) >> call_p(6) >> ch_p(')'))						/*do_while*/
							.case_p(str_p("DEFINE") >> ch_p(')'))								/*define*/
							.case_p(call_p(6) >> call_p(4) >> call_p(6) >> ch_p(')'))										/*if*/
					>> call_p(6) >> call_p(0))
				).else_p(
					select_p(rpsRegEx)
						.case_p(ch_p('#') >> action_p(rpsComment, ++ *not_p(')')))
						.case_p(ch_p('&') >> action_p(rpsName, ++ + not_p(')')))
						.case_p(!if_action_p(rpsDeltaCall, select_p(rpsDeltaFlag).case_p(ch_p('+')).case_p(ch_p('-'))) >> call_p(3))
						.case_p(str_p("R0"))
						.case_p(call_p(5) >> befor_equal_p(ch_p(')')))
						.case_p(select_p(rpsRegAssertion)
							.case_p(!call_p(5) >> ch_p(':'))
							.case_p(ch_p('|'))
							.case_p(ch_p('>'))
							.case_p(ch_p('='))
							.case_p(ch_p('!'))
							.case_p(str_p("<="))
							.case_p(str_p("<!"))
							.case_p(ch_p('<') >> action_p(rpsName, ++ + not_p('>')) >> ch_p('>'))
							.case_p(ch_p('\'') >> action_p(rpsName, ++ + not_p('\'')) >> ch_p('\''))
							>> call_p(0)
						)
				)
			).else_p(
				call_p(0)
			))
		>> define_p(
			!if_action_p(rpsCharSetBegin, ch_p('^'))
				>> +select_p(rpsCharSet)
				.case_p(
					str_p("[:")
					>> select_p(rpsCharSetDef)
						.case_p(str_p("alpha"))
						.case_p(str_p("alnum"))
						.case_p(str_p("blanc"))
						.case_p(str_p("digit"))
						.case_p(str_p("xdigit"))
						.case_p(str_p("word"))
						.case_p(str_p("upper"))
						.case_p(str_p("lower"))
						.case_p(str_p("space"))
						.case_p(str_p("ctrl"))
						>> str_p(":]"))
					.case_p(action_p(rpsRangeS, range_p('a', 'z')) >> ch_p('-') >> action_p(rpsRangeE, range_p('a', 'z')))
					.case_p(action_p(rpsRangeS, range_p('A', 'Z')) >> ch_p('-') >> action_p(rpsRangeE, range_p('A', 'Z')))
					.case_p(action_p(rpsRangeS, range_p('0', '9')) >> ch_p('-') >> action_p(rpsRangeE, range_p('0', '9')))
					.case_p(not_p(']'))
			)
		>> define_p(
			select_p(rpsNumber)
				.case_p(str_p("0x") >> ++ + xdigit_p())
				.case_p(++ + digit_p()))
		>> define_p(
			select_p(rpsCall)
				.case_p(call_p(3))
				.case_p(ch_p('<') >> action_p(rpsName, ++ + not_p('>')) >> ch_p('>'))
				.case_p(ch_p('\'') >> action_p(rpsName, ++ + not_p('\'')) >> ch_p('\''))
				.case_p(ch_p('R') >> call_p(3))
				.case_p(str_p("R&") >> ++ + not_p(')'))
				.case_p(ch_p('R'))
				.case_p(call_p(0)))
		>> define_p(
			(+select_p(rpsSetOption).case_p(ch_p('i')).case_p(ch_p('m')).case_p(ch_p('s')).case_p(ch_p('x')) >> !ch_p('-') | ch_p('-'))
			>> if_p(after_equal_p(ch_p('-')), +select_p(rpsResetOption).case_p(ch_p('i')).case_p(ch_p('m')).case_p(ch_p('s')).case_p(ch_p('x'))).else_p(true_p()))
		>> define_p(if_p(query_p(rpsOptionX), ++ *space_p()).else_p(true_p()))
		;
	return std::move(reg);
}

template<typename target_iterator, typename target_compare = default_compare_class>
class iregexp_parser_helper
	: public iregexp_callback<target_iterator>
{
public:
	typedef std::pair<target_iterator, target_iterator> target_range;
	struct regexp_syntax_option
	{
		enum opt_state
		{
			osSet,
			osReset,
		};
		bool opt_s;
		bool opt_m;
		bool opt_i;
		bool opt_x;
		regexp_syntax_option() { clear(); }
		regexp_syntax_option& s(void) { opt_s = true; return *this; }
		regexp_syntax_option& m(void) { opt_m = true; return *this; }
		regexp_syntax_option& i(void) { opt_i = true; return *this; }
		regexp_syntax_option& x(void) { opt_x = true; return *this; }
		regexp_syntax_option& S(void) { opt_s = false; return *this; }
		regexp_syntax_option& M(void) { opt_m = false; return *this; }
		regexp_syntax_option& I(void) { opt_i = false; return *this; }
		regexp_syntax_option& X(void) { opt_x = false; return *this; }
		bool is_s(void) { return opt_s; }
		bool is_m(void) { return opt_m; }
		bool is_i(void) { return opt_i; }
		bool is_x(void) { return opt_x; }
		void clear(void)
		{
			opt_s = false;
			opt_m = false;
			opt_i = false;
			opt_x = false;
		}
	};
private:
	iregexp_vender fvender;
	regexp_syntax_option fsyntax_option;
	iregexp_base<target_iterator, target_compare> fparser;
	iregexp_callback<target_iterator> *fparser_callback;
protected:
	regexp_syntax_option& syntax_option(void) { return fsyntax_option; }
	iregexp_vender& vender(void) { return fvender; }
	iregexp_callback<target_iterator>* parser_callback(void) const { return fparser_callback; }
public:
	virtual void init_parser(void) {}
	virtual void parse_done(bool f) {}
	void init(iregexp_vender& vdr, const iregexp_builder& reg)
	{
		fvender = vdr;
		fparser = reg.build<target_iterator, target_compare>(vdr, this);
	}
	bool parse(bool i, bool s, bool m, bool x, target_iterator vb, target_iterator ve, iregexp_callback<target_iterator> *parser_callback)
	{
		init_parser();
		fsyntax_option.clear();
		i ? fsyntax_option.i() : fsyntax_option.I();
		s ? fsyntax_option.s() : fsyntax_option.S();
		m ? fsyntax_option.m() : fsyntax_option.M();
		x ? fsyntax_option.x() : fsyntax_option.X();
		target_iterator vp = vb;
		fparser_callback = parser_callback;
		bool ret = fparser.match(vb, vp, ve) && vp == ve;
		parse_done(ret);
		return ret;
	}
	std::wstring get_parser_str(void)
	{
		std::wstring ret;
		fparser.get_type_string(ret);
		return ret;

	}
};

template<typename target_iterator, typename target_compare = default_compare_class>
class iregexp_parser_builder
	: public iregexp_parser_helper<target_iterator, target_compare>
{
public:
	enum capture_type
	{
		ct_null,
		ct_capture,
		ct_classter,
		ct_name_capture,
		ct_num_call,
		ct_dlt_capture,
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
		ct_option,
		ct_set_map,
		ct_map_switch,
	};
private:
	bool fnot_char_set;
	capture_type fcapture_type;
	target_iterator fchar_b;
	target_iterator fchar_e;
	target_range fchar;
	target_range fname;
	target_range fmap_name;
	target_range fmap_list;
	iregexp_base<target_iterator, target_compare> fregexp;
	std::vector<iregexp_base<target_iterator, target_compare>> for_stack;
	std::shared_ptr<iregexp_base<target_iterator, target_compare>> fclass_reg;
	struct reg_or_struct
	{
		typename iregexp_parser_helper<target_iterator, target_compare>::regexp_syntax_option syntax_option;
		capture_type cls_type;
		iregexp_base<target_iterator, target_compare> regexp;
		std::vector<iregexp_base<target_iterator, target_compare>> or_stack;
		std::shared_ptr<iregexp_base<target_iterator, target_compare>> class_reg;
		reg_or_struct(typename iregexp_parser_helper<target_iterator, target_compare>::regexp_syntax_option syntax_option, capture_type cls_type, iregexp_base<target_iterator, target_compare> regexp, std::vector<iregexp_base<target_iterator, target_compare>> or_stack, std::shared_ptr<iregexp_base<target_iterator, target_compare>> class_reg)
			: syntax_option(syntax_option), cls_type(cls_type), regexp(regexp), or_stack(or_stack), class_reg(class_reg){}
	};
	std::vector<reg_or_struct> fclasster_stack;
protected:
	iregexp_base<target_iterator, target_compare> regexp(void) { return fregexp; }
public:
	void parser_builder_clear(void)
	{
		fregexp.clear();
		fnot_char_set = false;
		for_stack.clear();
		fclass_reg = nullptr;
	}
	bool match(target_iterator vb, target_iterator& vp, target_iterator ve)
	{
		return fregexp.match(vb, vp, ve);
	}
	std::pair<size_t, size_t> matchs(size_t index) const
	{
		return fregexp.matchs(index);
	}
	std::wstring get_pattern_str(void)
	{
		std::wstring ret;
		fregexp.get_type_string(ret);
		return ret;

	}
	target_range get_name(void) const { return fname; }
	void set_name(typename const target_iterator& vp, typename const target_iterator& ve)
	{
		fname = vender().str(vp, ve);
	}
	void set_map_name(typename const target_iterator& vp, typename const target_iterator& ve)
	{
		fmap_name = vender().str(vp, ve);
	}
	void set_map_list(typename const target_iterator& vp, typename const target_iterator& ve)
	{
		fmap_list = vender().str(vp, ve);
	}
	void clear_range_char(typename const target_iterator& vp)
	{
		fchar.clear();
	}
	void set_range_char_begin(typename const target_iterator& vp)
	{
		fchar_b = vp;
	}
	void set_range_char_end(typename const target_iterator& vp)
	{
		fchar_e = vp;
	}
	void set_range_char_logic(bool f)
	{
		fnot_char_set = f;
	}
	void set_range_char(typename const target_range& v)
	{
		fchar = vender().range_set<target_iterator>(fchar_b, fchar_e);
	}
	void set_char(typename const target_iterator& vp)
	{
		fchar = vender().ch(vp);
	}
	void set_char(typename const target_range& v)
	{
		fchar = vender().str(v.first, v.second);
	}
	void set_math(size_t min, size_t max)
	{
		(*fregexp.last_item())->set_limited(min, max);
	}
	void set_force_match(void)
	{
		(*fregexp.last_item())->set_force_match();
	}
	void set_weak_match(void)
	{
		(*fregexp.last_item())->set_weak_match();
	}
	void add_range_char(void)
	{
		fregexp >>= character_parallel_regexp<target_iterator, target_compare>(fchar, fnot_char_set);
	}
	void add_char(typename const target_iterator& vp)
	{
		fregexp >>= character_parallel_regexp<target_iterator, target_compare>(vender().ch(vp));
	}
	void add_str(typename const target_iterator& vp, typename const target_iterator& ve)
	{
		fregexp >>= character_serial_regexp<target_iterator, target_compare>(vender().str(vp, ve));
	}
	void add_reg(typename const iregexp_base<target_iterator, target_compare>& v)
	{
		if (fregexp.last_item() != fregexp.end_item() && (*fregexp.last_item())->get_single_char_match() && (*v.last_item())->get_single_char_match())
			(*fregexp.last_item())->add_single_char(**v.last_item());
		else
			fregexp >>= v;
	}
	void push_or(void)
	{
		for_stack.push_back(fregexp);
		fregexp.clear();
	}
	void back_or(void)
	{
		if (for_stack.size())
		{
			push_or();
			std::vector<iregexp_base<target_iterator, target_compare>>::iterator it = for_stack.begin();
			fregexp = *it;
			while (++it != for_stack.end())
				fregexp |= *it;
			for_stack.clear();
		}
	}
	void push_classter(void)
	{
		fclasster_stack.push_back(reg_or_struct(iregexp_parser_helper<target_iterator, target_compare>::syntax_option(), fcapture_type, fregexp, for_stack, fclass_reg));
		fregexp.clear();
		for_stack.clear();
		fclass_reg = nullptr;
		fcapture_type = ct_capture;
	}
	void back_classter(void)
	{
		back_or();
		regexp_syntax_option option = syntax_option();
		std::shared_ptr<iregexp_base<target_iterator, target_compare>> fclass = fclass_reg;
		iregexp_base<target_iterator, target_compare> freg = fregexp;
		capture_type cls_type = fcapture_type;
		syntax_option() = fclasster_stack.back().syntax_option;
		fregexp = fclasster_stack.back().regexp;
		for_stack = fclasster_stack.back().or_stack;
		fcapture_type = fclasster_stack.back().cls_type;
		fclass_reg = fclasster_stack.back().class_reg;
		fclasster_stack.pop_back();
		switch (cls_type)
		{
		case ct_option:		syntax_option() = option; break;
		case ct_classter:	freg.classter_items(); fregexp >>= freg; break;
		case ct_capture:	fregexp >>= capture_regexp<target_iterator, target_compare>(freg); break;
		case ct_func:		fregexp >>= *fclass; break;
		case ct_query:		fregexp >>= *fclass; break;
		case ct_switch:
			dynamic_cast<switch_regexp<target_iterator, target_compare>*>(&*fclass)->set_items(freg);
			fregexp >>= *fclass;
			break;
		case ct_select:
			dynamic_cast<select_regexp<target_iterator, target_compare>*>(&*fclass)->set_items(freg);
			fregexp >>= *fclass;
			break;
		case ct_for:
			(*dynamic_cast<for_regexp<target_iterator, target_compare>*>(&*fclass))[freg];
			fregexp >>= *fclass;
			break;
		case ct_if_action:	fregexp >>= *fclass; break;
		case ct_action:		fregexp >>= *fclass; break;
		case ct_not_action:	fregexp >>= *fclass; break;
		case ct_while:
			(*dynamic_cast<while_regexp<target_iterator, target_compare>*>(&*fclass))[freg];
			fregexp >>= *fclass;
			break;
		case ct_do_while:
			dynamic_cast<do_while_regexp<target_iterator, target_compare>*>(&*fclass)->while_p(freg);
			fregexp >>= *fclass;
			break;
		case ct_if:
			dynamic_cast<if_regexp<target_iterator, target_compare>*>(&*fclass)->set_items(freg);
			fregexp >>= *fclass;
			break;
		case ct_define:
			(*dynamic_cast<null_regexp<target_iterator, target_compare>*>(&*fclass))[freg];
			fregexp >>= *fclass;
			break;
		case ct_null:	fregexp >>= *fclass; break;
		case ct_branch_reset:
			dynamic_cast<branch_reset_regexp<target_iterator, target_compare>*>(&*fclass)->set_items(freg);
			fregexp >>= *fclass;
			break;
		case ct_no_backtrack:
			fclass = std::shared_ptr<iregexp_base<target_iterator, target_compare>>(new no_backtrack_regexp<target_iterator, target_compare>(freg));
			fregexp >>= *fclass;
			break;
		case ct_befor_assertion:
			fclass = std::shared_ptr<iregexp_base<target_iterator, target_compare>>(new befor_assertion_regexp<target_iterator, target_compare>(freg));
			fregexp >>= *fclass;
			break;
		case ct_not_befor_assertion:
			fclass = std::shared_ptr<iregexp_base<target_iterator, target_compare>>(new befor_assertion_regexp<target_iterator, target_compare>(freg, true));
			fregexp >>= *fclass;
			break;
		case ct_after_assertion:
			fclass = std::shared_ptr<iregexp_base<target_iterator, target_compare>>(new after_assertion_regexp<target_iterator, target_compare>(freg));
			fregexp >>= *fclass;
			break;
		case ct_not_after_assertion:
			fclass = std::shared_ptr<iregexp_base<target_iterator, target_compare>>(new after_assertion_regexp<target_iterator, target_compare>(freg, true));
			fregexp >>= *fclass;
			break;
		case ct_name_capture:
			(*dynamic_cast<capture_regexp<target_iterator, target_compare>*>(&*fclass))[freg];
			fregexp >>= *fclass;
			break;
		case ct_set_map:	fregexp >>= *fclass; break;
		case ct_map_switch:
			dynamic_cast<map_switch_regexp<target_iterator, target_compare>*>(&*fclass)->set_items(freg);
			fregexp >>= *fclass;
			break;
		}
	}
	void ex_classter_set_map(capture_type type, bool push)
	{
		fcapture_type = type;
		fclass_reg = std::shared_ptr<iregexp_base<target_iterator, target_compare>>(new map_variable_regexp<target_iterator, target_compare>(push));
		fregexp.clear();
	}
	void ex_classter_set_map(capture_type type, bool dlt, int index, bool prev)
	{
		fcapture_type = type;
		if (dlt)
			fclass_reg = std::shared_ptr<iregexp_base<target_iterator, target_compare>>(new map_variable_regexp<target_iterator, target_compare>(index, prev, fmap_list, fmap_name));
		else
			fclass_reg = std::shared_ptr<iregexp_base<target_iterator, target_compare>>(new map_variable_regexp<target_iterator, target_compare>(index, fmap_list, fmap_name));
		fregexp.clear();
	}
	void ex_classter_set_map(capture_type type)
	{
		fcapture_type = type;
		fclass_reg = std::shared_ptr<iregexp_base<target_iterator, target_compare>>(new map_variable_regexp<target_iterator, target_compare>(fname, fmap_list, fmap_name));
		fregexp.clear();
	}
	void ex_classter(capture_type type, long index = 0, iregexp_callback<target_iterator> *parser_callback = nullptr)
	{
		fcapture_type = type;
		switch (type)
		{
		case ct_func:
			fclass_reg = std::shared_ptr<iregexp_base<target_iterator, target_compare>>(new func_regexp<target_iterator, target_compare>(index, parser_callback));
			break;
		case ct_query:
			fclass_reg = std::shared_ptr<iregexp_base<target_iterator, target_compare>>(new query_regexp<target_iterator, target_compare>(index, parser_callback));
			break;
		case ct_switch:
			fclass_reg = std::shared_ptr<iregexp_base<target_iterator, target_compare>>(new switch_regexp<target_iterator, target_compare>(index, parser_callback));
			dynamic_cast<switch_regexp<target_iterator, target_compare>*>(&*fclass_reg)->default_p(fregexp);
			break;
		case ct_select:
			fclass_reg = std::shared_ptr<iregexp_base<target_iterator, target_compare>>(new select_regexp<target_iterator, target_compare>(index, parser_callback));
			break;
		case ct_for:
			fclass_reg = std::shared_ptr<iregexp_base<target_iterator, target_compare>>(new for_regexp<target_iterator, target_compare>(fregexp, index, parser_callback));
			break;
		case ct_if_action:
			fclass_reg = std::shared_ptr<iregexp_base<target_iterator, target_compare>>(new if_action_regexp<target_iterator, target_compare>(fregexp, index, parser_callback));
			break;
		case ct_action:
			fclass_reg = std::shared_ptr<iregexp_base<target_iterator, target_compare>>(new action_regexp<target_iterator, target_compare>(fregexp, index, parser_callback));
			break;
		case ct_not_action:
			fclass_reg = std::shared_ptr<iregexp_base<target_iterator, target_compare>>(new action_regexp<target_iterator, target_compare>(fregexp, index, parser_callback, true));
			break;
		case ct_while:
			fclass_reg = std::shared_ptr<iregexp_base<target_iterator, target_compare>>(new while_regexp<target_iterator, target_compare>(fregexp));
			break;
		case ct_do_while:
			fclass_reg = std::shared_ptr<iregexp_base<target_iterator, target_compare>>(new do_while_regexp<target_iterator, target_compare>(fregexp));
			break;
		case ct_if:
			fclass_reg = std::shared_ptr<iregexp_base<target_iterator, target_compare>>(new if_regexp<target_iterator, target_compare>(fregexp));
			break;
		case ct_define:
			fclass_reg = std::shared_ptr<iregexp_base<target_iterator, target_compare>>(new null_regexp<target_iterator, target_compare>());
			break;
		case ct_null:
			fclass_reg = std::shared_ptr<iregexp_base<target_iterator, target_compare>>(new null_regexp<target_iterator, target_compare>(fregexp));
			break;
		case ct_branch_reset:
			fclass_reg = std::shared_ptr<iregexp_base<target_iterator, target_compare>>(new branch_reset_regexp<target_iterator, target_compare>());
			break;
		case ct_no_backtrack:
		case ct_befor_assertion:
		case ct_not_befor_assertion:
		case ct_after_assertion:
		case ct_not_after_assertion:
			break;
		case ct_name_capture:
			fclass_reg = std::shared_ptr<iregexp_base<target_iterator, target_compare>>(new capture_regexp<target_iterator, target_compare>(fname));
			break;
		case ct_map_switch:
			fclass_reg = std::shared_ptr<iregexp_base<target_iterator, target_compare>>(new map_switch_regexp<target_iterator, target_compare>(fmap_name));
			break;
		}
		fregexp.clear();
	}
};

template<typename target_iterator, typename target_compare = default_compare_class>
class iregexp_parser
	: public iregexp_parser_builder<target_iterator, target_compare>
{
private:
	iregexp_base<target_iterator, target_compare> fchar;
	long fparse_digit;
	bool fdelta_call;
	bool fdelta_flag;
	size_t fmin;
	size_t fmax;
protected:
	void set_math(void)
	{
		if (fmin > fmax)
		{
			long t = fmin;
			fmin = fmax;
			fmax = t;
		}
		set_math(fmin, fmax);
	}
	void set_math_begin(long v)
	{
		fmin = v;
		fmax = v;
	}
	void set_math_tie(void)
	{
		fmax = -1;
	}
	void set_math_end(long v)
	{
		fmax = v;
	}
	void set_math(size_t min, size_t max)
	{
		if (fchar.last_item() == fchar.end_item())
			iregexp_parser_builder<target_iterator, target_compare>::set_math(min, max);
		else
			(*fchar.last_item())->set_limited(min, max);
	}
	void set_force_match(void)
	{
		if (fchar.last_item() == fchar.end_item())
			iregexp_parser_builder<target_iterator, target_compare>::set_force_match();
		else
			(*fchar.last_item())->set_force_match();
	}
	void set_weak_match(void)
	{
		if (fchar.last_item() == fchar.end_item())
			iregexp_parser_builder<target_iterator, target_compare>::set_weak_match();
		else
			(*fchar.last_item())->set_weak_match();
	}
public:
	virtual void init_parser(void)
	{
		parser_builder_clear();
		fparse_digit = 0;
		fdelta_call = false;
		fdelta_flag = false;
		fmin = 1;
		fmax = 1;
	}
	virtual void OnFunc(long param, const target_iterator vb, target_iterator &vp, const target_iterator ve)
	{
	}
	virtual bool OnQuery(long param, const target_iterator vb, target_iterator &vp, const target_iterator ve)
	{
		bool ret = false;
		switch (param)
		{
		case rpsOptionX:
			ret = syntax_option().is_x();
			break;
		}
		return ret;
	}
	virtual void OnIfAction(long param, bool success, const target_iterator vb, target_iterator &vp, const target_iterator ve)
	{
		switch (param)
		{
		case rpsCharSetBegin:
			set_range_char_logic(success);
			break;
		case rpsDeltaCall:
			fdelta_call = success;
			break;
		case rpsCharDone:
			if (fchar.begin_item().count_child() > 0)
				add_reg(fchar);
			fchar.clear();
			break;
		}
	}
	virtual void OnAction(long param, const target_iterator vb, target_iterator &vp, const target_iterator ve)
	{
		switch (param)
		{
		case rpsMainClasster:
			push_classter();
			break;
		case rpsMainOrTie:
			push_or();
			break;
		case rpsMainDone:
			back_or();
			break;
		case rpsRangeS:
			set_range_char_begin(vp);
			break;
		case rpsRangeE:
			set_range_char_end(vp);
			break;
		case rpsRepertBegin:
			set_math_begin(fparse_digit);
			break;
		case rpsRepertTie:
			set_math_tie();
			break;
		case rpsRepertEnd:
			set_math_end(fparse_digit);
			break;
		case rpsComment:
			add_str(vp, ve);
			break;
		case rpsName:
			set_name(vp, ve);
			break;
		case rpsMapName:
			set_map_name(vp, ve);
			break;
		case rpsMapList:
			set_map_list(vp, ve);
			break;
		}
	}
	virtual void OnSelect(long param, short index, const target_iterator vb, target_iterator &vp, const target_iterator ve)
	{
		switch (param)
		{
		case rpsMain:
			switch (index)
			{
			case 0:	// ^
				syntax_option().is_m()
					? add_reg(begin_regexp<target_iterator, target_compare>() | after_assertion_regexp<target_iterator, target_compare>(character_parallel_regexp<target_iterator, target_compare>(vender().str<target_iterator>("\r\n"))))
					: add_reg(begin_regexp<target_iterator, target_compare>());
				break;
			case 1:	// $
				syntax_option().is_m()
					? add_reg(end_regexp<target_iterator, target_compare>() | (!character_parallel_regexp<target_iterator, target_compare>(vender().ch<target_iterator>('\r')) >> character_parallel_regexp<target_iterator, target_compare>(vender().ch<target_iterator>('\n'))))
					: add_reg(end_regexp<target_iterator, target_compare>());
				break;
			case 2:	/* rpsMainRegAssertion */	break;
			case 3: /* \\Q - \\E */				add_str(vp + 2, ve - 2); break;
			case 4:	/* rpsMainMetaChar */	break;
			}
			break;
		case rpsMainRegAssertion:
			switch (index)
			{
			case 0:	/* \\A */		add_reg(begin_regexp<target_iterator, target_compare>()); break;
			case 1:	/* \\Z */		add_reg(end_regexp<target_iterator, target_compare>() | (!character_parallel_regexp<target_iterator, target_compare>(vender().ch<target_iterator>('\r')) >> character_parallel_regexp<target_iterator, target_compare>(vender().ch<target_iterator>('\n')))); break;
			case 2:	/* \\z */		add_reg(end_regexp<target_iterator, target_compare>()); break;
			case 3:	/* \\b */		add_reg(boundary_assertion_regexp<target_iterator, target_compare>(character_parallel_regexp<target_iterator, target_compare>(vender().word_char<target_iterator>()))); break;
			case 4:	/* \\B */		add_reg(boundary_assertion_regexp<target_iterator, target_compare>(character_parallel_regexp<target_iterator, target_compare>(vender().word_char<target_iterator>()), true)); break;
			}
			break;
		case rpsMainMetaChar:
			switch (index)
			{
			case 0:	/* . */
				if (syntax_option().is_s())
					fchar = any_char_regexp<target_iterator, target_compare>();
				else
					fchar = character_parallel_regexp<target_iterator, target_compare>(vender().str<target_iterator>("\r\n"), true);
				break;
			case 1:	/* rpsMainEscChar */	break;
			case 2:					back_classter(); break;
			case 3:					add_range_char(); break;
			case 4:
				//if (syntax_option().is_i())
				//	fchar = character_parallel_regexp<target_iterator, target_compare>(vender().char_ignore(vp));
				//else
					fchar = character_parallel_regexp<target_iterator, target_compare>(vender().ch<target_iterator>(vp));
				break;
			}
			break;
		case rpsMainEscChar:
			switch (index)
			{
			case 0:	/* rpsMainEscKeywordChar */	break;
			case 1:	/* rpsMainCharClass */		break;
			case 2:	/* digit */					fchar = use_match_capture_regexp<target_iterator, target_compare>(vender().number(target_range(vp, ve))); break;
			case 3: /* k'' */
			case 4:	/* k<> */					fchar = use_match_capture_regexp<target_iterator, target_compare>(get_name()); break;
			case 5:	/* rpsMainEscMetaChar */	break;
			case 6:	/* rpsMainEscCtrlChar */	break;
			}
			break;
		case rpsMainEscKeywordChar:
			switch (index)
			{
			case  0: /* \\ */	fchar = character_parallel_regexp<target_iterator, target_compare>(vender().base_char<target_iterator>('\\')); break;
			case  1: /* ( */	fchar = character_parallel_regexp<target_iterator, target_compare>(vender().base_char<target_iterator>('(')); break;
			case  2: /* [ */	fchar = character_parallel_regexp<target_iterator, target_compare>(vender().base_char<target_iterator>('[')); break;
			case  3: /* ) */	fchar = character_parallel_regexp<target_iterator, target_compare>(vender().base_char<target_iterator>(')')); break;
			case  4: /* . */	fchar = character_parallel_regexp<target_iterator, target_compare>(vender().base_char<target_iterator>('.')); break;
			case  5: /* ^ */	fchar = character_parallel_regexp<target_iterator, target_compare>(vender().base_char<target_iterator>('^')); break;
			case  6: /* $ */	fchar = character_parallel_regexp<target_iterator, target_compare>(vender().base_char<target_iterator>('$')); break;
			case  7: /* | */	fchar = character_parallel_regexp<target_iterator, target_compare>(vender().base_char<target_iterator>('|')); break;
			case  8: /* ? */	fchar = character_parallel_regexp<target_iterator, target_compare>(vender().base_char<target_iterator>('?')); break;
			case  9: /* * */	fchar = character_parallel_regexp<target_iterator, target_compare>(vender().base_char<target_iterator>('*')); break;
			case 10: /* + */	fchar = character_parallel_regexp<target_iterator, target_compare>(vender().base_char<target_iterator>('+')); break;
			}
			break;
		case rpsMainEscCtrlChar:
			switch (index)
			{
			case 0:  case 1:  case 2:  case 3:  case 4:  case 5:  case 6:  case 7: 
			case 8:  case 9:  case 10: case 11: case 12: case 13: case 14: case 15:
			case 16: case 17: case 18: case 19: case 20: case 21: case 22: case 23:
			case 24: case 25: case 26: case 27: case 28: case 29: case 30: case 31:
				fchar = character_parallel_regexp<target_iterator, target_compare>(vender().ctrl_char<target_iterator>(index));
				break;
			}
			break;
		case rpsMainEscMetaChar:
			switch (index)
			{
			case 0:	/* n */		fchar = character_parallel_regexp<target_iterator, target_compare>(vender().base_char<target_iterator>('\n')); break;
			case 1:	/* r */		fchar = character_parallel_regexp<target_iterator, target_compare>(vender().base_char<target_iterator>('\r')); break;
			case 2:	/* t */		fchar = character_parallel_regexp<target_iterator, target_compare>(vender().base_char<target_iterator>('\t')); break;
			case 3:	/* f */		fchar = character_parallel_regexp<target_iterator, target_compare>(vender().base_char<target_iterator>('\f')); break;
			case 4:	/* e */		fchar = character_parallel_regexp<target_iterator, target_compare>(vender().base_char<target_iterator>(0x1b)); break;
			}
			break;
		case rpsMainCharClass:
			switch (index)
			{
			case 0:	/* s */		fchar = character_parallel_regexp<target_iterator, target_compare>(vender().space_char<target_iterator>()); break;
			case 1:	/* S */		fchar = character_parallel_regexp<target_iterator, target_compare>(vender().space_char<target_iterator>(), true); break;
			case 2:	/* d */		fchar = character_parallel_regexp<target_iterator, target_compare>(vender().digit_char<target_iterator>()); break;
			case 3:	/* D */		fchar = character_parallel_regexp<target_iterator, target_compare>(vender().digit_char<target_iterator>(), true); break;
			case 4:	/* w */		fchar = character_parallel_regexp<target_iterator, target_compare>(vender().word_char<target_iterator>()); break;
			case 5:	/* W */		fchar = character_parallel_regexp<target_iterator, target_compare>(vender().word_char<target_iterator>(), true); break;
			}
			break;
		case rpsMainRepert:
			switch (index)
			{
			case 0:	/* ? */		set_math(0, 1); break;
			case 1:	/* * */		set_math(0, -1); break;
			case 2:	/* + */		set_math(1, -1); break;
			case 3:	/* {} */	set_math(); break;
			}
			break;
		case rpsMainRepertOpt:
			switch (index)
			{
			case 0:	/* ? */		set_weak_match(); break;
			case 1:	/* + */		set_force_match(); break;
			}
			break;
		case rpsCharSet:
			switch (index)
			{
			case 0:	/* rpsCharSetDef */	break;
			case 1:	/* a-z */		set_range_char(vender().lower_char<target_iterator>()); break;
			case 2:	/* A-Z */		set_range_char(vender().upper_char<target_iterator>()); break;
			case 3:	/* 0-9 */		set_range_char(vender().digit_char<target_iterator>()); break;
			case 4:					set_char(vp);
				break;
			}
			break;
		case rpsCharSetDef:
			switch (index)
			{
			case 0:	/* alpha */		set_char(vender().alpha_char<target_iterator>()); break;
			case 1:	/* alnum */		set_char(vender().alnum_char<target_iterator>()); break;
			case 2:	/* blanc */		set_char(vender().blanc_char<target_iterator>()); break;
			case 3:	/* digit */		set_char(vender().digit_char<target_iterator>()); break;
			case 4:	/* xdigit */	set_char(vender().xdigit_char<target_iterator>()); break;
			case 5:	/* word */		set_char(vender().word_char<target_iterator>()); break;
			case 6:	/* upper */		set_char(vender().upper_char<target_iterator>()); break;
			case 7:	/* lower */		set_char(vender().lower_char<target_iterator>()); break;
			case 8:	/* space */		set_char(vender().space_char<target_iterator>()); break;
			case 9:	/* ctrl */		set_char(vender().ctrl_char<target_iterator>()); break;
			}
			break;
		case rpsExClass:
			switch (index)
			{
			case 0:	/* query */		ex_classter(ct_query); break;
			case 1:	/* func */		ex_classter(ct_func); break;
			case 2:	/* set map */	ex_classter_set_map(ct_set_map, !fdelta_flag); break;
			case 3:	/* set map */	ex_classter_set_map(ct_set_map, fdelta_call, fparse_digit, fdelta_flag); break;
			case 4:	/* set map */	ex_classter_set_map(ct_set_map); break;
			case 5:	/* rpsExClassEx */	break;
			}
			break;
		case rpsExClassEx:
			switch (index)
			{
			case 0:	/* switch */		ex_classter(ct_switch, fparse_digit, parser_callback()); break;
			case 1:	/* map_switch */	ex_classter(ct_map_switch); break;
			case 2:	/* select */		ex_classter(ct_select, fparse_digit, parser_callback()); break;
			case 3:	/* for */			ex_classter(ct_for, fparse_digit, parser_callback()); break;
			case 4:	/* if_action */		ex_classter(ct_if_action, fparse_digit, parser_callback()); break;
			case 5:	/* action */		ex_classter(ct_action, fparse_digit, parser_callback()); break;
			case 6:	/* not_action */	ex_classter(ct_not_action, fparse_digit, parser_callback()); break;
			case 7:	/* while */			ex_classter(ct_while); break;
			case 8:	/* do_while */		ex_classter(ct_do_while); break;
			case 9:	/* if */			ex_classter(ct_if); break;
			case 10:	/* define */		ex_classter(ct_define); break;
			}
			break;
		case rpsRegEx:
			switch (index)
			{
			case 0:	/* # */			ex_classter(ct_null); break;
			case 1:	/* &name */		add_reg(call_capture_regexp<target_iterator, target_compare>(get_name())); break;
			case 2:	/* +-num */		add_reg(fdelta_call ? call_capture_regexp<target_iterator, target_compare>(fparse_digit, fdelta_flag) : call_capture_regexp<target_iterator, target_compare>(fparse_digit)); break;
			case 3:	/* R,0 */		add_reg(call_capture_regexp<target_iterator, target_compare>(0)); break;
			case 4:					ex_classter(ct_option); break;
			case 5: /* rpsRegAssertion */ break;
			}
			break;
		case rpsRegAssertion:
			switch (index)
			{
			case 0:	/* ?--: */			ex_classter(ct_classter); break;
			case 1:	/* ?| */			ex_classter(ct_branch_reset); break;
			case 2:	/* ?> */			ex_classter(ct_no_backtrack); break;
			case 3:	/* ?= */			ex_classter(ct_befor_assertion); break;
			case 4:	/* ?! */			ex_classter(ct_not_befor_assertion); break;
			case 5:	/* ?<= */			ex_classter(ct_after_assertion); break;
			case 6:	/* ?<! */			ex_classter(ct_not_after_assertion); break;
			case 7:	/* 'name' */		ex_classter(ct_name_capture); break;
			case 8:	/* "name" */		ex_classter(ct_name_capture); break;
			}
			break;
		case rpsNumber:
			switch (index)
			{
			case 0: fparse_digit = vender().xnumber(target_range(vp, ve)); break;
			case 1: fparse_digit = vender().number(target_range(vp, ve)); break;
			}
			break;
		case rpsCall:
			switch (index)
			{
			case 0:	/* (N) */			add_reg(is_match_capture_regexp<target_iterator, target_compare>(fparse_digit)); break;
			case 1: /* (<name>) */
			case 2: /* ('name') */		add_reg(is_match_capture_regexp<target_iterator, target_compare>(get_name())); break;
			case 3: /* (RN) */			add_reg(recursion_capture_regexp<target_iterator, target_compare>(fparse_digit)); break;
			case 4: /* (R&name) */		add_reg(recursion_capture_regexp<target_iterator, target_compare>(get_name())); break;
			case 5: /* (R) */			add_reg(recursion_capture_regexp<target_iterator, target_compare>(0)); break;
			}
			break;
		case rpsDeltaFlag:
			switch (index)
			{
			case 0:	fdelta_flag = false;	break;
			case 1:	fdelta_flag = true;		break;
			}
			break;
		case rpsSetOption:
			switch (index)
			{
			case 0: /* i */		syntax_option().i(); break;
			case 1: /* m */		syntax_option().m(); break;
			case 2: /* s */		syntax_option().s(); break;
			case 3: /* x */		syntax_option().x(); break;
			}
			break;
		case rpsResetOption:
			switch (index)
			{
			case 0: /* i */		syntax_option().I(); break;
			case 1: /* m */		syntax_option().M(); break;
			case 2: /* s */		syntax_option().S(); break;
			case 3: /* x */		syntax_option().X(); break;
			}
			break;
		}
	}
};

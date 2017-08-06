#pragma once

#include	"tree.h"
#include	"RegExp.h"
#include	"RegExp_Builder.h"
#include	"RegExp_Vender.h"
#include	"mem_container.h"
#include	<string>

enum reg_rtf_parse_state
{
	rrpsDefault,
	rrpsDefParam,
	rrpsParam,
	rrpsXParam,
	rrpsParamSign,
	rrpsMakeColor,
	rrpsSetColor,
	rrpsSetChar,
	rrpsSetXChar,
	rrpsSetEscChar,
	rrpsParParam,
};

static iregexp_builder reg_rtf_parser(void)
{
	iregexp_builder reg =
		str_p("{\\rtf1")
			>> capture_p(while_p(
				map_select_p("main")
					.case_p(ch_p('\\') >> capture_p("control", ++ + alpha_p()) >> !if_action_p(rrpsParam, !if_action_p(rrpsParamSign, ch_p('-')) >> ++ + digit_p()) >> !ch_p(' '))
					.case_p(str_p("\\*"))
					.case_p(ch_p('\\') >> capture_p("esc_char", ch_p("\\{}")))
					.case_p(ch_p('\\') >> ch_p("|~-_:"))
					.case_p(str_p("\\'") >> action_p(rrpsXParam, ++xdigit_p().set_limit(2, 2)))
					.case_p(ch_p('{') >> map_push_p() >> *ch_p("\r\n") >> call_p(1) >> ch_p('}') >> map_pop_p())
					.case_p(capture_p("char", not_p("\\{}\r\n")))
					>> *ch_p("\r\n")
				, map_switch_p("main")
					.case_p(map_switch_p("state")
						.case_p(true_p())
						.case_p(list_select_p(rrpsMakeColor, "control", "red,green,blue,") | true_p())
						.case_p(true_p())
						.case_p(true_p())
						.case_p(true_p())
						.case_p(true_p())
						.case_p(true_p())
						.case_p(true_p())
						.default_p(select_p(rrpsDefault)
							.case_p(set_map_p("control", "fonttbl,colortbl,stylesheet,footnote,header,footer,pict,info,", "state"))
							.case_p(list_select_p(rrpsDefParam, "control", "ansicpg,deff,deflang,"))
							.case_p(comp_map_not_p(0, "text_state") >> list_select_p(rrpsParParam, "control", "par,"))
							.case_p(true_p())))
					.case_p(set_map_p(0, "text_state"))
					.case_p(comp_map_not_p(0, "text_state") >> list_select_p(rrpsSetEscChar, "esc_char", "\\,{,},") | true_p())
					.case_p(true_p())
					.case_p(comp_map_not_p(0, "text_state") >> comp_map_equal_p(-1, "state") >> func_p(rrpsSetXChar) | true_p())
					.case_p(true_p())
					.case_p(map_switch_p("state")
						.case_p(/*comp_map_not_p(0, "text_state") >> func_p(rrpsSetChar) |*/ true_p())
						.case_p(list_select_p(rrpsSetColor, "char", ";,"))
						.case_p(true_p())
						.case_p(true_p())
						.case_p(true_p())
						.case_p(true_p())
						.case_p(true_p())
						.case_p(true_p())
						.default_p(comp_map_not_p(0, "text_state") >> func_p(rrpsSetChar) | true_p())
				)))
				>> ch_p('}')
		;
	return std::move(reg);
}

template<typename target_iterator>
class iregexp_rtf_parser
	: public iregexp_callback<target_iterator>
{
public:
	struct rt_color
	{
		long red;
		long green;
		long blue;
		long color;
		rt_color() { reset(); }
		void set()
		{
			color = (blue << 16) + (green << 8) + red;
		}
		void reset()
		{
			red = green = blue = color = 0;
		}
	};
	class rt_char
	{
	private:
		bool fmulti_byte_state;
		bool fparagraph_state;
		int fcpg;
		std::vector<std::wstring> fline;
		char fstr[2];
	public:
		rt_char(int cpg = CP_ACP) : fmulti_byte_state(false), fparagraph_state(false), fcpg(cpg){}
		void set_cpg(int v) { fcpg = v; }
		void set_paragraph_state(void)
		{
			if (!fparagraph_state)
				fline.push_back(std::wstring());
			fparagraph_state = false;
		}
		void set_string(wchar_t c)
		{
			if (!fparagraph_state)
				fline.push_back(std::wstring());
			fparagraph_state = true;
			fline.back() += c;
		}
		std::wstring get_string()
		{
			std::wstring ret;
			std::vector<std::wstring>::const_iterator i = fline.begin();
			ret += *(i++);
			while (i != fline.end())
			{
				ret += L"\r\n";
				ret += *(i++);
			}
			return ret;
		}
		void set_char(wchar_t c)
		{
			if (c >= 0x80 || fmulti_byte_state)
			{
				if (!fmulti_byte_state)
				{
					fstr[0] = (char)c;
					fmulti_byte_state = true;
				}
				else
				{
					fstr[1] = (char)c;
					wchar_t wstr;
					MultiByteToWideChar(932, MB_PRECOMPOSED, &fstr[0], 2, &wstr, 1);
					set_string(wstr);
					fmulti_byte_state = false;
				}
			}
			else
				set_string(c);
		}
	};
	class rt_param
	{

	};
private:
	bool fsign_minus;
	long fparam;
	rt_color fcolor;
	std::vector<rt_color> fcolor_tbl;
	rt_char fstring;
public:
	virtual void OnSelect(long param, short index, const target_iterator vb, target_iterator &vp, const target_iterator ve)
	{
		switch (param)
		{
		case rrpsDefault:
			switch (index)
			{
			case 0:
				break;
			}
			break;
		case rrpsDefParam:
			switch (index)
			{
			case 0:	fstring.set_cpg(fparam); break;
			}
			break;
		case rrpsParParam:
			switch (index)
			{
			case 0:	fstring.set_paragraph_state(); break;
			}
			break;
		case rrpsMakeColor:
			switch (index)
			{
			case 0: fcolor.red = fparam;  break;
			case 1: fcolor.green = fparam;  break;
			case 2: fcolor.blue = fparam;  break;
			}
			break;
		case rrpsSetColor:
			fcolor.set();
			fcolor_tbl.push_back(fcolor);
			fcolor.reset();
			break;
		case rrpsSetEscChar:
			switch (index)
			{
			case 0: fstring.set_char('\\');  break;
			case 1: fstring.set_char('{');  break;
			case 2: fstring.set_char('}');  break;
			}
			break;
		}
	}
	virtual void OnIfAction(long param, bool success, const target_iterator vb, target_iterator &vp, const target_iterator ve)
	{
		switch (param)
		{
		case rrpsParamSign:	fsign_minus = success; break;
		case rrpsParam:		fparam = success ? fvender.number(vender().create(vp, ve)) * (fsign_minus ? -1 : 1) : 0; break;
		}
	}
	virtual void OnAction(long param, const target_iterator vb, target_iterator &vp, const target_iterator ve)
	{
		switch (param)
		{
		case rrpsXParam:		fparam = fvender.xnumber(vender().create(vp, ve)); break;
		}
	}
	virtual void OnFunc(long param, const target_iterator vb, target_iterator &vp, const target_iterator ve)
	{
		switch (param)
		{
		case rrpsSetChar:
		{
			std::pair<size_t, size_t> pt = fparser.matchs(fvender.str("char"));
			fstring.set_char(*((wchar_t*)&*(vb + pt.first)));
			break;
		}
		case rrpsSetXChar:
			fstring.set_char(fparam);
			break;
		}
	}
	virtual short OnSwitch(long param) { return -1; }
	virtual bool OnQuery(long param, const target_iterator vb, target_iterator &vp, const target_iterator ve) { return false; }
	virtual void OnForInit(long param, const target_iterator vb, target_iterator &vp, const target_iterator ve) {}
	virtual void OnForReinit(long param, const target_iterator vb, target_iterator &vp, const target_iterator ve) {}
private:
	const iregexp_vender& fvender;
	iregexp_base<target_iterator> fparser;
protected:
	const iregexp_vender& vender(void) { return fvender; }
public:
	iregexp_rtf_parser(const iregexp_vender& vdr, const iregexp_builder& reg)
		: fvender(vdr)
	{
		fparser = reg.build<target_iterator>(vdr, this);
	}
	bool parse(target_iterator vb, target_iterator ve)
	{
		target_iterator vp = vb;
		return fparser.match(vb, vp, ve);
	}
	std::wstring get_parser_str(void)
	{
		std::wstring ret;
		fparser.get_type_string(ret);
		return ret;

	}
	std::wstring get_string()
	{
		return fstring.get_string();
	}
};

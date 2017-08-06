#pragma once

#include	"tree.h"

template<class exp_item>
class and_or_expression_object
{
private:
	struct exp_obj
		: public exp_item
	{
		bool type;
		exp_obj() : type(false) {}
	};
public:
	typedef tree<and_or_expression_object::exp_obj> expression_tree;
private:
	void init(void)
	{
		fcontent.clear();
		fcontent.add(fcontent.end(), exp_obj());
	}
	expression_tree fcontent;
protected:
	expression_tree& content(void) { return fcontent; }
	expression_tree& content(void) const { return fcontent; }
public:
	and_or_expression_object()
	{
		init();
	}
	and_or_expression_object(const exp_item& v)
	{
		init();
		fcontent.add(fcontainer().begin(), v);
	}
	expression_tree::iterator begin(void) const { return fcontainer().begin(); }
	expression_tree::const_iterator begin(void) { return fcontainer.begin(); }
	expression_tree::iterator end(void) const { return fcontainer.end(); }
	expression_tree::const_iterator end(void) { return fcontainer.end(); }
	expression_tree::iterator classter(bool force = true)
	{
		typename expression_tree::iterator ret;
		if (fcontent.begin().count_child() <= 1)
		{
			if (force)
				ret = fcontent.add(fcontent.begin(), exp_obj());
			else
				ret = fcontent.begin().first_child();
		}
		else
		{
			ret = fcontent.insert(fcontent.begin().first_child(), exp_obj());
			typename expression_tree::iterator i = ret.next_sibling();
			while (i != fcontent.end())
			{
				fcontent.inc_indent(i);
				i = ret.next_sibling();
			}
		}
		return ret;
	}
	and_or_expression_object& and_tie(const and_or_expression_object& v)
	{
		if (v.fcontent.size() == 1)
			return *this;
		if (fcontent.size() == 1)
			return (*this = v);
		fcontent.add(fcontent.begin(), v.fcontent.begin().first_child(), v.fcontent.end());
		return *this;
	}
	and_or_expression_object& or_tie(const and_or_expression_object& v)
	{
		if (v.fcontent.size() == 1)
			return *this;
		if (fcontent.size() == 1)
			return (*this = v);

		typename expression_tree::iterator it;
		if (fcontent.begin().first_child()->type == true)
		{
			it = fcontent.begin().first_child();
		}
		else
		{
			it = fcontent.insert(fcontent.begin().first_child(), v);
			it->type = true;
			typename expression_tree::iterator i = it.next_sibling();
			while (i != fcontent.end())
			{
				fcontent.inc_indent(i);
				i = it.next_sibling();
			}
			i = fcontent.insert(it.first_child(), v);
			typename expression_tree::iterator ii = i.next_sibling();
			while (ii != fcontent.end())
			{
				fcontent.inc_indent(ii);
				ii = i.next_sibling();
			}
		}
		if (v.fcontent.begin().first_child()->type == true && v.fcontent.begin().first_child().count_child() > 0)
		{
			fcontent.add(it, v.fcontent.begin().first_child().first_child(), v.fcontent.end());
		}
		else
		{
			typename expression_tree::iterator i = fcontent.add(it, exp_obj());
			fcontent.add(i, v.fcontent.begin().first_child(), v.fcontent.end());
		}
		return *this;
	}
};

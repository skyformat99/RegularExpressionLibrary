#pragma once

#include	<vector>
#include	<string>
#include	<memory>
#include	<algorithm>
#include	"tree.h"

extern int SYNTAX_DEBUG;

struct default_compare_class
{
	template<typename target_iterator>
	int operator () (const target_iterator& l, const target_iterator& r) const
	{
		if (*l > *r)
			return 1;
		else if (*l < *r)
			return -1;
		else
			return 0;
	}
};

template<typename target_iterator, typename target_compare = default_compare_class>
class iregexp_base
{
public:
	class iregexp_item;
	typedef std::shared_ptr<iregexp_item> iregexp_item_ptr;
	typedef tree<iregexp_item_ptr> iregexp_item_tree;
	typedef std::pair<size_t, size_t> iregexp_item_point;
	typedef std::pair<target_iterator, target_iterator> target_range;


	struct target_equal_
	{
		template<class _InIt1,
		class _InIt2,
		class _Pr> inline
			bool operator () (_InIt1 _First1, _InIt1 _Last1,
			_InIt2 _First2, _Pr _Pred)
		{	// compare [_First1, _Last1) to [_First2, ...) using _Pred
			for (; _First1 != _Last1; ++_First1, ++_First2)
				if (!_Pred(_First1, _First2))
					return (false);
			return (true);
		}
	};
	struct target_equal
	{
		bool operator () (const target_iterator& l, const target_iterator& r) const
		{
			return target_compare()(l, r) == 0;
		}
	};
	struct target_range_compare
	{
		int operator () (const target_iterator& lb, const target_iterator& le, const target_iterator& rb, const target_iterator& re) const
		{
			if (le - lb > re - rb)
				return 1;
			else if (le - lb < re - rb)
				return -1;
			else
			{
				int ret = 0;
				for (target_iterator l = lb, r = rb; ret == 0 && l < le; ++l, ++r)
					ret = target_compare()(l, r);
				return ret;
			}
		}
	};
	struct target_range_equal
	{
		bool operator () (const target_iterator& lb, const target_iterator& le, const target_iterator& r) const
		{
			return target_equal_()(lb, le, r, target_equal());
		}
		bool operator () (const target_iterator& lb, const target_iterator& le, const target_iterator& rb, const target_iterator& re) const
		{
			return le - lb == re - rb && target_equal_()(lb, le, rb, target_equal());
		}
	};
	struct target_range_find
	{
		bool operator () (const target_iterator& lb, const target_iterator& le, const target_iterator& r) const
		{
			bool ret = false;
			for (target_iterator l = lb; !ret && l < le; ++l)
				ret = target_equal()(l, r);
			return ret;
		}
	};
	struct target_range_size
	{
		size_t operator () (const target_range& v) const
		{
			return v.second - v.first;
		}
	};

	class iregexp_item_match_base
	{
	private:
		std::vector<iregexp_item_point> fmatchs;
		std::vector<std::vector<iregexp_item_point>> fmatch_stack;
	protected:
		size_t matchs_count(void) const { return fmatchs.size(); }
		void stack_clear(void) { fmatch_stack.clear(); }
		void matchs_clear(void) { fmatchs.clear(); }
		void matchs_push(iregexp_item_point point) { fmatchs.push_back(point); }
		iregexp_item_point matchs_back(void)
		{
			iregexp_item_point ret = fmatchs.back();
			fmatchs.pop_back();
			return ret;
		}
	public:
		iregexp_item_match_base() {}
		virtual ~iregexp_item_match_base() {}
		virtual void match_stack_reset(void)
		{
			stack_clear();
			matchs_clear();
		}
		//size_t match_stack_count(void) const { return fmatch_stack.size(); }
		virtual void match_stack_push(void)
		{
			fmatch_stack.push_back(fmatchs);
			matchs_clear();
		}
		virtual void match_stack_pop()
		{
			fmatchs = fmatch_stack.back();
			fmatch_stack.pop_back();
		}
		//iregexp_item_point matchs(size_t index)
		//{
		//	if (index < fmatch_stack[0].size())
		//		return fmatch_stack[0][index];
		//	else
		//		return iregexp_item_point(0, 0);
		//}
		iregexp_item_point last_matchs()
		{
			return fmatch_stack.back().back();
		}
		bool is_matchs()
		{
			return fmatch_stack.back().size() > 0;
		}
		void match_stack_string(std::wstring& str)
		{
			wchar_t istr[33];
			std::vector<std::vector<iregexp_item_point> >::const_iterator it = fmatch_stack.begin();
			while (it != fmatch_stack.end())
			{
				str += L"[";

				std::vector<iregexp_item_point>::const_iterator i = it->begin();
				while (i != it->end())
				{
					str += L"(";
					_itow_s(i->first, istr, 10);
					str += istr;
					str += L",";
					_itow_s(i->second, istr, 10);
					str += istr;
					str += L")";
					++i;
				}

				str += L"]";
				++it;
			}
		}
	};

	class iregexp_map_base
	{
	public:
		struct map_compare
		{
			bool operator () (const std::pair<target_range, size_t>& l, const std::pair<target_range, size_t>&r)
			{
				return target_range_compare()(l.first.first, l.first.second, r.first.first, r.first.second) == -1;
			}
		};
		std::vector<std::vector<std::pair<target_range, size_t>>> fmap_stack;
		std::vector<std::pair<target_range, size_t>> fmap;
	private:
	protected:
	public:
		iregexp_map_base() {}
		virtual ~iregexp_map_base() {}
		void clear_map(typename iregexp_item_tree::const_iterator src)
		{
			iregexp_item_tree::const_iterator it = src.root();
			(*it)->fmap_stack.clear();
			(*it)->fmap.clear();
		}
		void set_map(typename iregexp_item_tree::const_iterator src, const target_range& name, size_t val)
		{
			iregexp_item_tree::const_iterator it = src.root();
			if (std::binary_search((*it)->fmap.begin(), (*it)->fmap.end(), std::pair<target_range, size_t>(name, val), map_compare()))
				std::lower_bound((*it)->fmap.begin(), (*it)->fmap.end(), std::pair<target_range, size_t>(name, val), map_compare())->second = val;
			else
				(*it)->fmap.insert(std::lower_bound((*it)->fmap.begin(), (*it)->fmap.end(), std::pair<target_range, size_t>(name, val), map_compare()), std::pair<target_range, size_t>(name, val));
		}
		size_t get_map(typename iregexp_item_tree::const_iterator src, const target_range& name) const
		{
			iregexp_item_tree::const_iterator it = src.root();
			if (!std::binary_search((*it)->fmap.begin(), (*it)->fmap.end(), std::pair<target_range, size_t>(name, 0), map_compare()))
				return -1;
			std::vector<std::pair<target_range, size_t>>::const_iterator i = std::lower_bound((*it)->fmap.begin(), (*it)->fmap.end(), std::pair<target_range, size_t>(name, 0), map_compare());
			return i->second;
		}
		void push_map(typename iregexp_item_tree::const_iterator src)
		{
			iregexp_item_tree::const_iterator it = src.root();
			(*it)->fmap_stack.push_back((*it)->fmap);
		}
		void pop_map(typename iregexp_item_tree::const_iterator src)
		{
			iregexp_item_tree::const_iterator it = src.root();
			(*it)->fmap = (*it)->fmap_stack.back();
			(*it)->fmap_stack.pop_back();
		}
		void get_map_string(typename iregexp_item_tree::const_iterator src, std::wstring& str) const
		{
			iregexp_item_tree::const_iterator it = src.root();
			wchar_t istr[33];
			std::vector<std::pair<target_range, size_t>>::const_iterator i = (*it)->fmap.begin();
			while (i != (*it)->fmap.end())
			{
				str += L"[";
				str.append((wchar_t*)&*i->first.first, i->first.second - i->first.first);
				str += L"=";
				_itow_s(i->second, istr, 10);
				str += istr;
				str += L"]";
				++i;
			}
		}
	};

	class iregexp_item_capture_base
	{
	private:
		std::vector<typename iregexp_item_tree::const_iterator> fcapture;
		std::vector<typename iregexp_item_tree::const_iterator> fnamed_capture;
	protected:
		virtual const target_range& name(void) const { throw; }
	public:
		iregexp_item_capture_base() {}
		virtual ~iregexp_item_capture_base() {}
		void get_capture_string(std::wstring& str) const
		{
			wchar_t istr[33];
			if (fcapture.size() > 0)
			{
				str += L"<";
				_itow_s(fcapture.size(), istr, 10);
				str += istr;
				str += L">";
			}
		}
		virtual int is_capture(void) { return 0; }
		void clear_capture(void)
		{
			fcapture.clear();
			fnamed_capture.clear();
		}
		void set_capture(typename iregexp_item_tree::const_iterator src)
		{
			clear_capture();
			std::vector<typename iregexp_item_tree::const_iterator>::iterator vpos = fcapture.end();
			set_capture(src.root(), vpos, false);
		}
		void set_capture(typename iregexp_item_tree::const_iterator src, typename std::vector<typename iregexp_item_tree::const_iterator>::iterator& vpos, bool branch_reset)
		{
			iregexp_item_tree::const_iterator it = src.first_child();
			while (!it.is_end() && it.tree_indent() > src.tree_indent())
			{
				switch ((*it)->is_capture())
				{
				case -1:
					if (!branch_reset)
					{
						iregexp_item_tree::const_iterator i = it.first_child();
						size_t pos = fcapture.size();
						size_t max = 0;
						while (!i.is_end())
						{
							std::vector<typename iregexp_item_tree::const_iterator>::iterator vit = fcapture.begin() + pos;
							set_capture(i, vit, true);
							if (max < vit - fcapture.begin() - pos)
								max = vit - fcapture.begin() - pos;
							i.move_next_sibling();
						}
						vpos = fcapture.begin() + pos;
						for (size_t l = pos; l < max; ++l, ++vpos)
							*vpos = it;
						size_t idt = it.tree_indent();
						while (!(++it).is_end() && it.tree_indent() > idt);
					}
					else
						throw;
					break;
				case 2:
					fnamed_capture.push_back(it);
				case 1:
					if (vpos == fcapture.end())
					{
						fcapture.push_back(branch_reset ? src : it);
						vpos = fcapture.end();
					}
					else
					{
						*vpos = branch_reset ? src : it;
						++vpos;
					}
				default:
					++it;
				}
			}
		}
		typename iregexp_item_tree::const_iterator find_capture(typename iregexp_item_tree::const_iterator src, const target_range fname) const
		{
			iregexp_item_tree::const_iterator it = src.root();
			if (fname.second - fname.first != 0)
			{
				while (!it.is_end())
				{
					if ((*it)->is_capture() == 2)
					{
						if (target_range_equal()(fname.first, fname.second, (*it)->name().first, (*it)->name().second))
							break;
					}
					++it;
				}
			}
			return it;
		}
		typename iregexp_item_tree::const_iterator find_capture(typename iregexp_item_tree::const_iterator src, size_t idx) const
		{
			iregexp_item_tree::const_iterator root = src.root();
			iregexp_item_tree::const_iterator it = root;
			if (idx > 0)
			{
				--idx;
				if (idx < (*root)->fcapture.size())
				{
					it = (*root)->fcapture[idx];
					if ((*it)->is_capture() == -1)
					{
						size_t l = 0;
						for (; idx > 0 && (*root)->fcapture[idx - 1] == it; --idx, ++l);

						iregexp_item_tree::const_iterator i = it.last_child();
						while (!i.is_end() && (**i).is_matchs())
							i.move_prev_sibling();

						size_t idt = i.tree_indent();
						i.move_first_child();
						size_t o = 0;
						while (!i.is_end() && i.tree_indent() > idt)
						{
							if ((*i)->is_capture() > 0)
							{
								if (o == l)
								{
									it = i;
									break;
								}
								++o;
							}
							++i;
						}
					}
				}
				else
					it.move_end();
			}
			return it;
		}
		typename iregexp_item_tree::const_iterator find_capture(typename iregexp_item_tree::const_iterator src, size_t idx, bool prev) const
		{
			iregexp_item_tree::const_iterator root = src.root();
			iregexp_item_tree::const_iterator it = root.end();

			size_t max = (*root)->fcapture.size() - 1;
			size_t pos = max / 2;
			size_t delta = pos;

			while ((*root)->fcapture[pos].absolute_index() != src.absolute_index())
			{
				if ((*root)->fcapture[pos].absolute_index() < src.absolute_index() && pos < max && (*root)->fcapture[pos + 1].absolute_index() > src.absolute_index())
				{
					pos = !prev ? pos + 1 : pos;
					break;
				}
				else if ((*root)->fcapture[pos].absolute_index() > src.absolute_index() && pos > 0 && (*root)->fcapture[pos - 1].absolute_index() < src.absolute_index())
				{
					pos = !prev ? pos : pos - 1;
					break;
				}
				else if ((*root)->fcapture[pos].absolute_index() < src.absolute_index() && pos == max)
				{
					pos = !prev ? -1 : pos;
					break;
				}
				else if ((*root)->fcapture[pos].absolute_index() > src.absolute_index() && pos == 0)
				{
					pos = !prev ? pos : -1;
					break;
				}
				delta /= 2;
				if (delta == 0)
					delta = 1;
				if ((*root)->fcapture[pos].absolute_index() < src.absolute_index())
					pos += delta;
				else if ((*root)->fcapture[pos].absolute_index() > src.absolute_index())
					pos -= delta;
			}
			if (pos != -1)
			{
				--idx;
				if (!prev && pos + idx < (*root)->fcapture.size())
					it = (*root)->fcapture[pos + idx];
				else if (prev && pos >= idx)
					it = (*root)->fcapture[pos - idx];
				else
					it.move_end();
			}
			return it;
		}
	};

	class iregexp_item_repeat_base
	{
	public:
		typedef size_t(iregexp_item_repeat_base::*get_match_size_func)(typename iregexp_item_tree::const_iterator src, bool max);
	private:
		size_t fmin_limit;
		size_t fmax_limit;
		get_match_size_func fget_match_size;
	protected:
		virtual size_t get_min_match_size_(void) { return 0; }
		virtual size_t get_max_match_size_(void) { return 0; }
		virtual bool can_repeat_match(void) { return true; }
		size_t get_serial_match_size(typename iregexp_item_tree::const_iterator src, bool max)
		{
			size_t ret = 0;
			iregexp_item_tree::const_iterator it = src.first_child();
			if (!it.is_end())
			{
				while (!it.is_end())
				{
					ret += (**it).get_match_size(it, max);
					it.move_next_sibling();
				}
			}
			else
				ret = max ? get_max_match_size_() : get_min_match_size_();
			return ret * (max ? max_limit() : min_limit());
		}
		size_t get_parallel_match_size(typename iregexp_item_tree::const_iterator src, bool max)
		{
			size_t ret = 0;
			iregexp_item_tree::const_iterator it = src.first_child();
			if (!it.is_end())
			{
				size_t v = 0;
				while (!it.is_end())
				{
					size_t vv = (**it).get_match_size(it, max);
					if (max)
					{
						if (vv > v)
							v = vv;
					}
					else
					{
						if (vv < v)
							v = vv;
					}
					it.move_next_sibling();
				}
				ret = v;
			}
			else
				ret = max ? get_max_match_size_() : get_min_match_size_();
			return ret * (max ? max_limit() : min_limit());
		}
	public:
		iregexp_item_repeat_base() : fmin_limit(1), fmax_limit(1) {}
		virtual ~iregexp_item_repeat_base() {}
		size_t min_limit(void) const { return fmin_limit; }
		size_t max_limit(void) const { return fmax_limit; }
		void set_min_limit(size_t v) { fmin_limit = v; }
		void set_max_limit(size_t v) { fmax_limit = v; }
		void set_serial_match_size(void) { fget_match_size = &iregexp_item_repeat_base::get_serial_match_size; }
		void set_parallel_match_size(void) { fget_match_size = &iregexp_item_repeat_base::get_parallel_match_size; }
		size_t get_match_size(typename iregexp_item_tree::const_iterator src, bool max) { return (this->*fget_match_size)(src, max); }
		bool get_single_match(void) const { return fmin_limit == 1 && fmax_limit == 1; }
	};

	class iregexp_item
		: public iregexp_item_match_base
		, public iregexp_item_repeat_base
		, public iregexp_item_capture_base
		, public iregexp_map_base
	{
	public:
		enum syntax_type
		{
			stNormal,
			stOr,
			stAll,
			stFirst,
			stJustone,
		};
		enum match_type
		{
			mtNormal,
			mtLimited,
			mtWeak,
			mtForce,
		};
		typedef bool (iregexp_item::*syntax_func)(typename iregexp_item_tree::const_iterator src, const target_iterator vb, target_iterator &vp, const target_iterator ve, bool backtracking);
		typedef bool (iregexp_item::*match_func)(typename iregexp_item_tree::const_iterator src, const target_iterator vb, target_iterator &vp, const target_iterator ve);
		typedef bool (iregexp_item::*backtrack_func)(typename iregexp_item_tree::const_iterator src, const target_iterator vb, target_iterator &vp, const target_iterator ve);
	private:
		syntax_type fsyntax_type;
		match_type fmatch_type;
		match_func fmatch;
		backtrack_func fbacktrack;
		syntax_func fsyntax;
		bool fcan_backtrack;
	protected:
		bool match(typename iregexp_item_tree::const_iterator src, const target_iterator vb, target_iterator &vp, const target_iterator ve) { return (this->*fmatch)(src, vb, vp, ve); }
		bool backtrack(typename iregexp_item_tree::const_iterator src, const target_iterator vb, target_iterator &vp, const target_iterator ve) { return (this->*fbacktrack)(src, vb, vp, ve); }
		bool syntax(typename iregexp_item_tree::const_iterator src, const target_iterator vb, target_iterator &vp, const target_iterator ve, bool backtracking) { return (this->*fsyntax)(src, vb, vp, ve, backtracking); }
		virtual bool can_backtrack(void) { return fcan_backtrack; }
		virtual bool _syntax(typename iregexp_item_tree::const_iterator src, const target_iterator vb, target_iterator &vp, const target_iterator ve, bool backtracking)
		{
			bool ret = false;
			if (!backtracking)
			{
				iregexp_item_tree::const_iterator it = src.first_child();
				while (!it.is_end())
				{
					ret = (**it)(it, vb, vp, ve, false);
					if (ret)
						it.move_next_sibling();
					else
					{
						while (!ret && it.sibling_index() > 0)
						{
							it.move_prev_sibling();
							target_iterator v = vp;
							ret = (**it)(it, vb, vp, ve, true);
						}
						if (!ret)
							break;
						it.move_next_sibling();
					}
				}
			}
			else
			{
				iregexp_item_tree::const_iterator it = src.last_child();
				while (!it.is_end())
				{
					ret = (**it)(it, vb, vp, ve, true);
					if (!ret)
						it.move_prev_sibling();
					else
					{
						iregexp_item_tree::const_iterator i = it;
						it.move_next_sibling();
						while (ret && !it.is_end())
						{
							ret = (**it)(it, vb, vp, ve, false);
							it.move_next_sibling();
						}
						if (it.is_end())
							break;
						it = i;
					}
				}
			}
			return ret;
		}
		bool or_syntax(typename iregexp_item_tree::const_iterator src, const target_iterator vb, target_iterator &vp, const target_iterator ve, bool backtracking)
		{
			bool ret = false;
			if (!backtracking)
			{
				iregexp_item_tree::const_iterator it = src.first_child();
				while (!ret && !it.is_end())
				{
					ret = (**it)(it, vb, vp, ve, false);
					if (!ret)
						it.move_next_sibling();
				}
			}
			else
			{
				iregexp_item_tree::const_iterator it = src.last_child();
				while (!it.is_end())
				{
					if ((**it).is_matchs())
						it.move_prev_sibling();
					else
					{
						target_iterator v = vp;
						ret = (**it)(it, vb, vp, ve, true);
						if (!ret)
						{
							it.move_next_sibling();
							while (!ret && !it.is_end())
							{
								ret = (**it)(it, vb, vp, ve, true);
								if (!ret)
									it.move_next_sibling();
							}
						}
						break;
					}
				}
			}
			return ret;
		}
		bool all_syntax(typename iregexp_item_tree::const_iterator src, const target_iterator vb, target_iterator &vp, const target_iterator ve, bool backtracking)
		{
			bool ret = false;
			iregexp_item_tree::const_iterator it = src.first_child();
			target_iterator vv = vp;
			while (!it.is_end())
			{
				target_iterator v = vp;
				ret = (**it)(it, vb, v, ve, false);
				if (!ret)
					break;
				if (vv < v)
					vv = v;
				it.move_next_sibling();
			}
			if (ret)
				vp = vv;
			return ret;
		}
		bool first_syntax(typename iregexp_item_tree::const_iterator src, const target_iterator vb, target_iterator &vp, const target_iterator ve, bool backtracking)
		{
			iregexp_item_tree::const_iterator it = src.first_child();
			bool ret = false;
			if (!it.is_end())
			{
				target_iterator v = vp;
				if ((**it)(it, vb, v, ve, false))
				{
					iregexp_item_tree::const_iterator i = it.next_sibling();
					while (ret == false && !i.is_end())
					{
						target_iterator vv = vp;
						ret = (**i)(i, vb, vv, ve, false);
						i.move_next_sibling();
					}
					if (!ret)
					{
						vp = v;
						ret = true;
					}
					else
						ret = false;
				}
			}
			return ret;
		}
		bool justone_syntax(typename iregexp_item_tree::const_iterator src, const target_iterator vb, target_iterator &vp, const target_iterator ve, bool backtracking)
		{
			iregexp_item_tree::const_iterator it = src.first_child();
			iregexp_item_tree::const_iterator i;
			target_iterator vv(vb.container());
			size_t a = 0;
			while (!it.is_end())
			{
				target_iterator v = vp;
				if ((**it)(it, vb, v, ve, false))
				{
					if (a == 1)
						break;
					i = it;
					vv = v;
					++a;
				}
				it.move_next_sibling();
			}
			bool ret = false;
			if (a == 1 && it.is_end())
			{
				vp = vv;
				ret = true;
			}
			return ret;
		}
		bool _match(typename iregexp_item_tree::const_iterator src, const target_iterator vb, target_iterator &vp, const target_iterator ve)
		{
			target_iterator i = vp;
			bool ret = syntax(src, vb, i, ve, false);
			if (ret)
			{
				matchs_push(iregexp_item_point(vp - vb, i - vb));
				vp = i;
			}
			match_stack_push();
			return ret;
		}
		bool _backtrack(typename iregexp_item_tree::const_iterator src, const target_iterator vb, target_iterator &vp, const target_iterator ve)
		{
			bool ret = false;
			if (is_matchs())
			{
				match_stack_pop();
				if (can_backtrack() && matchs_count() > 0)
				{
					target_iterator i = vp;
					iregexp_item_point point = matchs_back();
					ret = syntax(src, vb, vp, ve, true) && i != vp;
					if (ret)
						matchs_push(iregexp_item_point(point.first, vp - vb));
					else
						vp = vb + point.first;
				}
				match_stack_push();
			}
			return ret;
		}
		bool repeat_match(typename iregexp_item_tree::const_iterator src, const target_iterator vb, target_iterator &vp, const target_iterator ve)
		{
			bool ret = false;
			bool loop = true;
			while (loop)
			{
				target_iterator i = vp;
				ret = syntax(src, vb, i, ve, false);
				if (ret)
				{
					matchs_push(iregexp_item_point(vp - vb, i - vb));
					vp = i;
					loop = matchs_count() < max_limit();
				}
				else
					loop = false;
				ret = matchs_count() >= min_limit() && matchs_count() <= max_limit();
			}
			match_stack_push();
			return ret;
		}
		bool repeat_backtrack(typename iregexp_item_tree::const_iterator src, const target_iterator vb, target_iterator &vp, const target_iterator ve)
		{
			bool ret = false;
			if (is_matchs())
			{
				match_stack_pop();
				if (matchs_count() > min_limit())
				{
					ret = true;
					target_iterator i = vp;
					iregexp_item_point point = matchs_back();
					if (can_backtrack() && syntax(src, vb, vp, ve, true) && i != vp)
						matchs_push(iregexp_item_point(point.first, vp - vb));
					else
						vp = vb + point.first;
				}
				match_stack_push();
			}
			return ret;
		}
		bool weak_match(typename iregexp_item_tree::const_iterator src, const target_iterator vb, target_iterator &vp, const target_iterator ve)
		{
			bool ret = false;
			bool loop = true;
			while (loop)
			{
				target_iterator i = vp;
				ret = syntax(src, vb, i, ve, false);
				if (ret)
				{
					matchs_push(iregexp_item_point(vp - vb, i - vb));
					vp = i;
					loop = matchs_count() < min_limit();
				}
				else
				{
					loop = false;
				}
				ret = matchs_count() >= min_limit() && matchs_count() <= max_limit();
			}
			match_stack_push();
			return ret;
		}
		bool weak_backtrack(typename iregexp_item_tree::const_iterator src, const target_iterator vb, target_iterator &vp, const target_iterator ve)
		{
			match_stack_pop();
			bool ret = matchs_count() < max_limit();
			if (ret)
			{
				target_iterator i = vp;
				ret = syntax(src, vb, i, ve, false);
				if (ret)
				{
					matchs_push(iregexp_item_point(vp - vb, i - vb));
					vp = i;
				}
			}
			match_stack_push();
			if (!ret && can_backtrack())
				ret = syntax(src, vb, vp, ve, true);
			return ret;
		}
		bool force_match(typename iregexp_item_tree::const_iterator src, const target_iterator vb, target_iterator &vp, const target_iterator ve)
		{
			bool ret = false;
			bool loop = true;
			while (loop)
			{
				target_iterator i = vp;
				ret = syntax(src, vb, i, ve, false);
				if (ret)
				{
					matchs_push(iregexp_item_point(vp - vb, i - vb));
					vp = i;
					loop = matchs_count() < max_limit();
				}
				else
				{
					loop = false;
				}
				ret = matchs_count() >= min_limit() && matchs_count() <= max_limit();
			}
			match_stack_push();
			return ret;
		}
		bool force_backtrack(typename iregexp_item_tree::const_iterator src, const target_iterator vb, target_iterator &vp, const target_iterator ve) { return false; }
	public:
		iregexp_item()
			: fcan_backtrack(true)
		{
			set_normal_match();
			set_normal_syntax();
		}
		virtual ~iregexp_item() {}
		virtual void get_type_string(std::wstring& str) const
		{
			if (is_or_syntax())
				str += L"or_syntax";
			else if (is_all_syntax())
				str += L"all_syntax";
			else if (is_first_syntax())
				str += L"first_syntax";
			else if (is_justone_syntax())
				str += L"justone_syntax";
			else
				str += L"normal_syntax";
		}
		void set_normal_match(void)
		{
			fmatch_type = mtNormal;
			fmatch = &iregexp_item::_match;
			fbacktrack = &iregexp_item::_backtrack;
		}
		void set_limited_match(void)
		{
			fmatch_type = mtLimited;
			fmatch = &iregexp_item::repeat_match;
			fbacktrack = &iregexp_item::repeat_backtrack;
		}
		void set_weak_match(void)
		{
			fmatch_type = mtWeak;
			fmatch = &iregexp_item::weak_match;
			fbacktrack = &iregexp_item::weak_backtrack;
		}
		void set_force_match(void)
		{
			fmatch_type = mtForce;
			fmatch = &iregexp_item::force_match;
			fbacktrack = &iregexp_item::force_backtrack;
		}
		void set_normal_syntax(void)
		{
			fcan_backtrack = true;
			fsyntax_type = stNormal;
			fsyntax = &iregexp_item::_syntax;
			set_serial_match_size();
		}
		void set_or_syntax(void)
		{
			fcan_backtrack = true;
			fsyntax_type = stOr;
			fsyntax = &iregexp_item::or_syntax;
			set_parallel_match_size();
		}
		void set_all_syntax(void)
		{
			fcan_backtrack = false;
			fsyntax_type = stAll;
			fsyntax = &iregexp_item::all_syntax;
			set_parallel_match_size();
		}
		void set_first_syntax(void)
		{
			fcan_backtrack = false;
			fsyntax_type = stFirst;
			fsyntax = &iregexp_item::first_syntax;
			set_parallel_match_size();
		}
		void set_justone_syntax(void)
		{
			fcan_backtrack = false;
			fsyntax_type = stJustone;
			fsyntax = &iregexp_item::justone_syntax;
			set_parallel_match_size();
		}
		bool is_or_syntax(void) const { return fsyntax_type == stOr; }
		bool is_all_syntax(void) const { return fsyntax_type == stAll; }
		bool is_first_syntax(void) const { return fsyntax_type == stFirst; }
		bool is_justone_syntax(void) const { return fsyntax_type == stJustone; }
		bool is_normal_syntax(void) const { return fsyntax_type == stNormal; }
		bool is_default_syntax(void) const { return fsyntax_type == mtNormal; }
		bool is_weak_match(void) const { return fmatch_type == mtWeak; }
		bool is_force_match(void) const { return fmatch_type == mtForce; }
		virtual bool get_single_char_match(void) const { return false; }
		virtual void add_single_char(const iregexp_item& v) {}
		void set_limited(size_t min)
		{
			if (can_repeat_match())
			{
				set_min_limit(min);
				set_max_limit(-1);
				set_limited_match();
			}
		}
		void set_limited(size_t min, size_t max)
		{
			if (can_repeat_match())
			{
				set_min_limit(min);
				set_max_limit(max);
				set_limited_match();
			}
		}
		void reset(void)
		{
			match_stack_reset();
		}
		bool operator () (typename iregexp_item_tree::const_iterator src, const target_iterator vb, target_iterator &vp, const target_iterator ve, bool backtracking)
		{
			return backtracking ? backtrack(src, vb, vp, ve) : match(src, vb, vp, ve);
		}
	};

	class regexp_capture_item
		: public iregexp_item
	{
	private:
	protected:
	public:
		regexp_capture_item() {}
		virtual ~regexp_capture_item() {}
		virtual void get_type_string(std::wstring& str) const
		{
			str += L"capture_p()";
		}
		virtual int is_capture(void) { return 1; }
	};

	class regexp_capture_name_item
		: public regexp_capture_item
	{
	private:
		target_range fname;
	protected:
	public:
		regexp_capture_name_item(const target_range name) : fname(name) {}
		virtual ~regexp_capture_name_item() {}
		virtual const target_range& name(void) const { return fname; }
		virtual void get_type_string(std::wstring& str) const
		{
			str += L"capture_name_p('";
			str.append((wchar_t*)&*fname.first, (wchar_t*)&*fname.second);
			str += L"')";
		}
		virtual int is_capture(void) { return 2; }
	};

	class regexp_branch_reset_item
		: public iregexp_item
	{
	private:
	protected:
		virtual bool _syntax(typename iregexp_item_tree::const_iterator src, const target_iterator vb, target_iterator &vp, const target_iterator ve, bool backtracking)
		{
			return iregexp_item::or_syntax(src, vb, vp, ve, backtracking);
		}
	public:
		regexp_branch_reset_item()
		{
			set_parallel_match_size();
		}
		virtual ~regexp_branch_reset_item() {}
		virtual void get_type_string(std::wstring& str) const
		{
			str += L"branch_reset_p";
		}
		virtual int is_capture(void) { return -1; }
	};
private:
	iregexp_item_tree fbase;
protected:
public:
	iregexp_base()
	{
		add_item(end_item(), new iregexp_item());
	}
	virtual ~iregexp_base() {}
	iregexp_base(const iregexp_base<target_iterator, target_compare>& v)
	{
		*this = v;
	}
	iregexp_base<target_iterator, target_compare>& operator = (const iregexp_base<target_iterator, target_compare>& v)
	{
		if (&v != this)
		{
			fbase = v.fbase;
			(*begin_item())->set_capture(begin_item());
		}
		return *this;
	}
	bool is_null(void) { return fbase.size() == 1; }
	void clear(void)
	{
		fbase.erase_child(begin_item());
		(*begin_item())->clear_capture();
	}
	virtual void get_type_string(std::wstring& str)
	{
		wchar_t istr[33];
		iregexp_item_tree::iterator it = begin_item();
		while (!it.is_end())
		{
			if (it.parent().is_end())
				_itow_s(0, istr, 10);
			else
				_itow_s(it.parent().absolute_index(), istr, 10);
			str += istr;
			str += L"\t";
			_itow_s(it.absolute_index(), istr, 10);
			str += istr;
			str += L"\t";
			_itow_s(it.tree_indent(), istr, 10);
			str += istr;
			str += L"\t";
			_itow_s(it.sibling_index(), istr, 10);
			str += istr;
			str += L",";
			str += L"[";
			_itow_s(it.absolute_index(), istr, 10);
			str += istr;
			str += L"]";
			(*it)->get_type_string(str);
			(*it)->get_capture_string(str);
			if (it == begin_item())
				(*it)->get_map_string(it, str);
			if ((*it)->min_limit() == 0 && (*it)->max_limit() == -1)
			{
				str += L"*";
			}
			else if ((*it)->min_limit() == 1 && (*it)->max_limit() == -1)
			{
				str += L"+";
			}
			else if ((*it)->min_limit() == 0 && (*it)->max_limit() == 1)
			{
				str += L"?";
			}
			else if ((*it)->min_limit() != 1 || (*it)->max_limit() != 1)
			{
				str += L"{";
				_itow_s((*it)->min_limit(), istr, 10);
				str += istr;
				if ((*it)->min_limit() != (*it)->max_limit())
				{
					str += L",";
					if ((*it)->max_limit() != -1)
					{
						_itow_s((*it)->max_limit(), istr, 10);
						str += istr;
					}
				}
				str += L"}";
			}
			if ((*it)->is_weak_match())
				str += L"?";
			else if ((*it)->is_force_match())
			{
				if ((*it)->min_limit() == 1 && (*it)->max_limit() == 1)
					str += L"{1}";
				str += L"+";
			}
			(*it)->match_stack_string(str);
			str += L"\r\n";
			++it;
		}
	}
	void reset(void)
	{
		iregexp_item_tree::iterator it = begin_item();
		while (!it.is_end())
		{
			(*it)->reset();
			++it;
		}
	}
	typename iregexp_item_tree::iterator add_item(typename iregexp_item_tree::iterator &it, iregexp_item *item_ptr = NULL)
	{
		if (item_ptr == NULL)
			item_ptr = new iregexp_item();
		iregexp_item_tree::iterator ret = fbase.add(it, iregexp_item_ptr(item_ptr));
		(*begin_item())->set_capture(begin_item());
		return ret;
	}
	typename iregexp_item_tree::iterator add_item(typename iregexp_item_tree::iterator &it, typename iregexp_item_tree::const_iterator &vp, typename iregexp_item_tree::const_iterator &ve)
	{
		iregexp_item_tree::iterator ret = fbase.add(it, vp, ve);
		(*begin_item())->set_capture(begin_item());
		return ret;
	}
	typename iregexp_item_tree::iterator capture_items(typename iregexp_item_tree::iterator parent)
	{
		return classter_items(parent, new regexp_capture_item());
	}
	typename iregexp_item_tree::iterator classter_items(typename iregexp_item_tree::iterator parent, iregexp_item* item_ptr = NULL)
	{
		if (item_ptr == NULL)
			item_ptr = new iregexp_item();
		iregexp_item_tree::iterator ret;
		if (parent.count_child() == 0)
			ret = fbase.add(parent, iregexp_item_ptr(item_ptr));
		else
			ret = fbase.insert(parent.first_child(), iregexp_item_ptr(item_ptr));
		iregexp_item_tree::iterator it = ret.next_sibling();
		while (!it.is_end())
		{
			fbase.inc_indent(it);
			it = ret.next_sibling();
		}
		(*begin_item())->set_capture(begin_item());
		return ret;
	}
	typename iregexp_item_tree::iterator classter_items(iregexp_item* item_ptr = NULL)
	{
		return classter_items(fbase.begin(), item_ptr);
	}
	typename iregexp_item_tree::iterator begin_item(void)
	{
		return fbase.begin();
	}
	typename iregexp_item_tree::iterator end_item(void)
	{
		return fbase.end();
	}
	typename iregexp_item_tree::iterator last_item(void)
	{
		return begin_item().last_child();
	}
	typename iregexp_item_tree::const_iterator begin_item(void) const
	{
		return fbase.begin();
	}
	typename iregexp_item_tree::const_iterator end_item(void) const
	{
		return fbase.end();
	}
	typename iregexp_item_tree::const_iterator last_item(void) const
	{
		return begin_item().last_child();
	}
	iregexp_base& over_one_match(void)
	{
		iregexp_item_tree::iterator it = begin_item().count_child() > 1 ? classter_items() : begin_item().first_child();
		(*it)->set_limited(1);
		return *this;
	}
	iregexp_base& any_match()
	{
		iregexp_item_tree::iterator it = begin_item().count_child() > 1 ? classter_items() : begin_item().first_child();
		(*it)->set_limited(0);
		return *this;
	}
	iregexp_base& binary_match()
	{
		iregexp_item_tree::iterator it = begin_item().count_child() > 1 ? classter_items() : begin_item().first_child();
		(*it)->set_limited(0, 1);
		return *this;
	}
	iregexp_base& limited_match(size_t begin, size_t end)
	{
		iregexp_item_tree::iterator it = begin_item().count_child() > 1 ? classter_items() : begin_item().first_child();
		if (!it.is_end())
			(*it)->set_limited(begin, end);
		return *this;
	}
	iregexp_base& weak_match(void)
	{
		iregexp_item_tree::iterator it = begin_item().count_child() > 1 ? classter_items() : begin_item().first_child();
		if (!it.is_end())
			(*it)->set_weak_match();
		return *this;
	}
	iregexp_base& force_match(void)
	{
		iregexp_item_tree::iterator it = begin_item().count_child() > 1 ? classter_items() : begin_item().first_child();
		if (!it.is_end())
			(*it)->set_force_match();
		return *this;
	}
	iregexp_base& add_tie(const iregexp_base& v)
	{
		if (v.fbase.size() == 1)
			return *this;
		if (fbase.size() == 1)
			return (*this = v);
		if (!(*begin_item().first_child())->is_normal_syntax())
			iregexp_item_tree::iterator srv = classter_items();
		if ((*v.begin_item().first_child())->is_normal_syntax())
			add_item(begin_item(), v.begin_item().first_child(), v.end_item());
		else
		{
			iregexp_item_tree::iterator crt = add_item(begin_item());
			add_item(crt, v.begin_item().first_child(), v.end_item());
		}
		return *this;
	}
	iregexp_base& or_tie(const iregexp_base& v)
	{
		if (v.fbase.size() == 1)
			return *this;
		if (fbase.size() == 1)
			return (*this = v);
		if (!(*begin_item().first_child())->is_or_syntax())
		{
			iregexp_item_tree::iterator srv = classter_items();
			(*srv)->set_or_syntax();
			classter_items(srv);
		}
		if ((*v.begin_item().first_child())->is_or_syntax())
			add_item(begin_item().first_child(), v.begin_item().first_child().first_child(), v.fbase.end());
		else
		{
			iregexp_item_tree::iterator crt = add_item(begin_item().first_child());
			add_item(crt, v.begin_item().first_child(), v.fbase.end());
		}
		return *this;
	}
	iregexp_base& and_tie(const iregexp_base& v)
	{
		if (v.fbase.size() == 1)
			return *this;
		if (fbase.size() == 1)
			return (*this = v);
		if (!(*begin_item().first_child())->is_all_syntax())
		{
			iregexp_item_tree::iterator srv = classter_items();
			(*srv)->set_all_syntax();
			classter_items(srv);
		}
		if ((*v.begin_item().first_child())->is_all_syntax())
			add_item(begin_item().first_child(), v.begin_item().first_child().first_child(), v.fbase.end());
		else
		{
			iregexp_item_tree::iterator crt = add_item(begin_item().first_child());
			add_item(crt, v.begin_item().first_child(), v.fbase.end());
		}
		return *this;
	}
	iregexp_base& sub_tie(const iregexp_base& v)
	{
		if (v.fbase.size() == 1)
			return *this;
		if (fbase.size() == 1)
			return (*this = v);
		if (!(*begin_item().first_child())->is_first_syntax())
		{
			iregexp_item_tree::iterator srv = classter_items();
			(*srv)->set_first_syntax();
			classter_items(srv);
		}
		if ((*v.begin_item().first_child())->is_first_syntax())
			add_item(begin_item().first_child(), v.begin_item().first_child().first_child(), v.fbase.end());
		else
		{
			iregexp_item_tree::iterator crt = add_item(begin_item().first_child());
			add_item(crt, v.begin_item().first_child(), v.fbase.end());
		}
		return *this;
	}
	iregexp_base& justone_tie(const iregexp_base& v)
	{
		if (v.fbase.size() == 1)
			return *this;
		if (fbase.size() == 1)
			return (*this = v);
		if (!(*begin_item().first_child())->is_justone_syntax())
		{
			iregexp_item_tree::iterator srv = classter_items();
			(*srv)->set_justone_syntax();
			classter_items(srv);
		}
		if ((*v.begin_item().first_child())->is_justone_syntax())
			add_item(begin_item().first_child(), v.begin_item().first_child().first_child(), v.fbase.end());
		else
		{
			iregexp_item_tree::iterator crt = add_item(begin_item().first_child());
			add_item(crt, v.begin_item().first_child(), v.fbase.end());
		}
		return *this;
	}
	void set_branch_reset(void)
	{
		if ((*begin_item().first_child())->is_or_syntax())
			*begin_item().first_child() = std::shared_ptr<iregexp_item >(new regexp_branch_reset_item());
	}
	iregexp_base operator + () const
	{
		iregexp_base ret = *this;
		return ret.over_one_match();
	}
	iregexp_base operator * () const
	{
		iregexp_base ret = *this;
		return ret.any_match();
	}
	iregexp_base operator ! () const
	{
		iregexp_base ret = *this;
		return ret.binary_match();
	}
	iregexp_base operator >> (const iregexp_base& v) const
	{
		iregexp_base ret = *this;
		return ret.add_tie(v);
	}
	//iregexp_base operator && (const iregexp_base& v) const
	//{
	//	iregexp_base ret = *this;
	//	return ret.add_tie(v);
	//}
	iregexp_base operator | (const iregexp_base& v) const
	{
		iregexp_base ret = *this;
		return ret.or_tie(v);
	}
	iregexp_base operator & (const iregexp_base& v) const
	{
		iregexp_base ret = *this;
		return ret.and_tie(v);
	}
	iregexp_base operator - (const iregexp_base& v) const
	{
		iregexp_base ret = *this;
		return ret.sub_tie(v);
	}
	iregexp_base operator ^ (const iregexp_base& v) const
	{
		iregexp_base ret = *this;
		return ret.justone_tie(v);
	}
	iregexp_base& operator >>= (const iregexp_base& v)
	{
		return add_tie(v);
	}
	iregexp_base& operator |= (const iregexp_base& v)
	{
		return or_tie(v);
	}
	iregexp_base& operator &= (const iregexp_base& v)
	{
		return and_tie(v);
	}
	iregexp_base operator -= (const iregexp_base& v)
	{
		return sub_tie(v);
	}
	iregexp_base operator ^= (const iregexp_base& v)
	{
		return justone_tie(v);
	}
	iregexp_item_point matchs(size_t v) const
	{
		iregexp_item_point pt = std::pair<size_t, size_t>(0, 0);
		iregexp_item_tree::const_iterator it = (*fbase.begin())->find_capture(fbase.begin(), v);
		if (!it.is_end() && (*it)->is_matchs())
			pt = (**it).last_matchs();
		return pt;
	}
	iregexp_item_point matchs(target_range v)
	{
		iregexp_item_point pt = std::pair<size_t, size_t>(0, 0);
		iregexp_item_tree::const_iterator it = (*fbase.begin())->find_capture(fbase.begin(), v);
		if (!it.is_end() && (*it)->is_matchs())
			pt = (**it).last_matchs();
		return pt;
	}
	bool match(const target_iterator vb, target_iterator &vp, const target_iterator ve)
	{
		reset();
		return vb != ve && begin_item() != end_item() && (**begin_item())(begin_item(), vb, vp, ve, false);
	}
};

template<typename target_iterator, typename target_compare = default_compare_class>
class branch_reset_regexp
	: public iregexp_base<target_iterator, target_compare>
{
private:
protected:
public:
	branch_reset_regexp(void)
	{
		add_item(begin_item(), new regexp_branch_reset_item());
	}
	virtual ~branch_reset_regexp() {}
	branch_reset_regexp& operator [] (const iregexp_base& v)
	{
		iregexp_item_tree::iterator it = add_item(begin_item().first_child());
		add_item(it, v.begin_item().first_child(), v.end_item());
		return *this;
	}
	iregexp_base& set_items(const iregexp_base& v)
	{
		iregexp_item_tree::const_iterator vit = v.begin_item().first_child();
		iregexp_item_tree::iterator it = begin_item().first_child();
		if ((**vit).is_or_syntax())
			vit.move_first_child();
		else
			it = add_item(it);
		if (!vit.is_end())
		{
			add_item(it, vit, v.end_item());
		}
		return *this;
	}
};

template<typename target_iterator, typename target_compare = default_compare_class>
class character_serial_regexp
	: public iregexp_base<target_iterator, target_compare>
{
	class regexp_serial_item
		: public iregexp_item
	{
	private:
		target_range ftarget;
	protected:
		virtual size_t get_min_match_size_(void) { return ftarget.second - ftarget.first; }
		virtual size_t get_max_match_size_(void) { return ftarget.second - ftarget.first; }
		virtual bool can_backtrack(void) { return false; }
		virtual bool _syntax(typename iregexp_item_tree::const_iterator src, const target_iterator vb, target_iterator &vp, const target_iterator ve, bool backtracking)
		{
			bool ret = ve - vp >= target_range_size()(ftarget) && target_range_equal()(ftarget.first, ftarget.second, vp);
			if (ret)
				vp += target_range_size()(ftarget);
			return ret;
		}
	public:
		regexp_serial_item(const target_range& v)
		{
			ftarget = v;
		}
		virtual ~regexp_serial_item() {}
		virtual void get_type_string(std::wstring& str) const
		{
			str += L"str_p('";
			for (target_iterator i = ftarget.first; i < ftarget.second; ++i)
			{
				if (*((wchar_t*)&*i) < 0x20)
					break;
				str.append((wchar_t*)&*ftarget.first, 1);
			}
			str += L"')";
		}
	};
private:
protected:
public:
	character_serial_regexp(const target_range& v)
	{
		add_item(begin_item(), new regexp_serial_item(v));
	}
	virtual ~character_serial_regexp() {}
};

template<typename target_iterator, typename target_compare = default_compare_class>
class any_char_regexp
	: public iregexp_base<target_iterator, target_compare>
{
	class regexp_any_char_item
		: public iregexp_item
	{
	private:
	protected:
		virtual size_t get_min_match_size_(void) { return 1; }
		virtual size_t get_max_match_size_(void) { return 1; }
		virtual bool can_backtrack(void) { return false; }
		virtual bool _syntax(typename iregexp_item_tree::const_iterator src, const target_iterator vb, target_iterator &vp, const target_iterator ve, bool backtracking)
		{
			if (vp != ve)
			{
				++vp;
				return true;
			}
			else
				return false;
		}
	public:
		virtual ~regexp_any_char_item() {}
		virtual void get_type_string(std::wstring& str) const
		{
			str += L"anychar_p(.)";
		}
	};
private:
protected:
public:
	any_char_regexp()
	{
		add_item(begin_item(), new regexp_any_char_item());
	}
	virtual ~any_char_regexp() {}
};

template<typename target_iterator, typename target_compare = default_compare_class>
class character_parallel_regexp
	: public iregexp_base<target_iterator, target_compare>
{
	class regexp_single_item
		: public iregexp_item
	{
	private:
		std::vector<target_range> ftarget;
		bool fnot;
	protected:
		virtual size_t get_min_match_size_(void) { return ftarget.size(); }
		virtual size_t get_max_match_size_(void) { return ftarget.size(); }
		virtual bool can_backtrack(void) { return false; }
		virtual bool _syntax(typename iregexp_item_tree::const_iterator src, const target_iterator vb, target_iterator &vp, const target_iterator ve, bool backtracking)
		{
			bool ret = ftarget.size() > 0;
			bool not = ftarget.size() == 1 && fnot;
			for (std::vector<target_range>::const_iterator i = ftarget.begin(); ret && i < ftarget.end(); ++i, ++vp)
				ret = vp < ve && not ^ target_range_equal()(i->first, i->second, vp);
			return ret;
		}
	public:
		regexp_single_item() : fnot(false) {}
		regexp_single_item(const target_range& v, bool not = false)
			: fnot(not)
		{
			ftarget.push_back(v);
		}
		virtual ~regexp_single_item() {}
		void set_not(void) { fnot = true; }
		virtual void get_type_string(std::wstring& str) const
		{
			if (ftarget.size() == 1 && fnot)
				str += L"not_";
			str += L"single_p('";
			for (std::vector<target_range>::const_iterator i = ftarget.begin(); i < ftarget.end(); ++i)
			{
				if (*((wchar_t*)&*i->first) < 0x20)
					break;
				str.append((wchar_t*)&*i->first, 1);
			}
			str += L"')";
		}
		void append_character(target_range v)
		{
			ftarget.push_back(v);
		}
		virtual bool get_single_char_match(void) const { return !fnot; }
		virtual void add_single_char(const iregexp_item& v)
		{
			if (dynamic_cast<const regexp_single_item*>(&v)->get_single_char_match())
				ftarget.push_back(*dynamic_cast<const regexp_single_item*>(&v)->ftarget.begin());
		}
	};
	class regexp_parallel_item
		: public iregexp_item
	{
	private:
		target_range ftarget;
		bool fnot;
	protected:
		virtual size_t get_min_match_size_(void) { return 1; }
		virtual size_t get_max_match_size_(void) { return 1; }
		virtual bool can_backtrack(void) { return false; }
		virtual bool _syntax(typename iregexp_item_tree::const_iterator src, const target_iterator vb, target_iterator &vp, const target_iterator ve, bool backtracking)
		{
			bool ret = target_range_size()(ftarget) > 0 && vp < ve && (fnot ^ target_range_find()(ftarget.first, ftarget.second, vp));
			if (ret)
				++vp;
			return ret;
		}
	public:
		regexp_parallel_item(const target_range& v, bool not = false)
			: fnot(not), ftarget(v)
		{
		}
		virtual ~regexp_parallel_item() {}
		void set_not(void) { fnot = true; }
		virtual void get_type_string(std::wstring& str) const
		{
			if (fnot)
				str += L"not_";
			str += L"ch_p('";
			for (target_iterator i = ftarget.first; i < ftarget.second; ++i)
			{
				if (*((wchar_t*)&*i) < 0x20)
					break;
				str.append((wchar_t*)&*i, 1);
			}
			str += L"')";
		}
	};
private:
protected:
public:
	character_parallel_regexp()
	{
		add_item(begin_item(), new regexp_single_item());
	}
	character_parallel_regexp(const target_range& v, bool not = false)
	{
		add_item(begin_item(), target_range_size()(v) == 1 ?
			(iregexp_item*)new regexp_single_item(v, not) :
			(iregexp_item*)new regexp_parallel_item(v, not));
	}
	virtual ~character_parallel_regexp() {}
	iregexp_base& operator ~ (void)
	{
		if (dynamic_cast<regexp_single_item*>(&**begin_item().first_child()) != NULL)
			((regexp_single_item*)&**begin_item().first_child())->set_not();
		else if (dynamic_cast<regexp_parallel_item*>(&**begin_item().first_child()) != NULL)
			((regexp_parallel_item*)&**begin_item().first_child())->set_not();
		return *this;
	}
};

template<typename target_iterator, typename target_compare = default_compare_class>
class repeat_regexp
	: public iregexp_base<target_iterator, target_compare>
{
private:
protected:
public:
	repeat_regexp(size_t n)
	{
		iregexp_item_tree::iterator it = classter_items();
		(*it)->set_limited(n, n);
	}
	repeat_regexp(size_t min, size_t max)
	{
		iregexp_item_tree::iterator it = classter_items();
		(*it)->set_limited(min, max);
	}
	virtual ~repeat_regexp() {}
	iregexp_base& operator [] (const iregexp_base& v)
	{
		add_item(begin_item().first_child(), v.begin_item().first_child(), v.end_item());
		return *this;
	}
};

template<typename target_iterator, typename target_compare = default_compare_class>
class no_backtrack_regexp
	: public iregexp_base<target_iterator, target_compare>
{
private:
protected:
public:
	no_backtrack_regexp(const iregexp_base& v)
	{
		iregexp_item_tree::iterator it = classter_items();
		(**it).set_force_match();
		add_item(it, v.begin_item().first_child(), v.end_item());
	}
	virtual ~no_backtrack_regexp() {}
};

template<typename target_iterator, typename target_compare = default_compare_class>
class null_regexp
	: public iregexp_base<target_iterator, target_compare>
{
	class regexp_null_item
		: public iregexp_item
	{
	private:
	protected:
		virtual bool can_repeat_match(void) { return false; }
		virtual bool can_backtrack(void) { return false; }
		virtual bool _syntax(typename iregexp_item_tree::const_iterator src, const target_iterator vb, target_iterator &vp, const target_iterator ve, bool backtracking)
		{
			return true;
		}
	public:
		regexp_null_item() {}
		virtual ~regexp_null_item() {}
		virtual void get_type_string(std::wstring& str) const
		{
			str += L"null_p";
		}
	};
private:
protected:
public:
	null_regexp(void)
	{
		add_item(begin_item(), new regexp_null_item());
	}
	null_regexp(const iregexp_base& v)
	{
		iregexp_item_tree::iterator it = add_item(begin_item(), new regexp_null_item());
		it = add_item(it);
		add_item(it, v.begin_item().first_child(), v.end_item());
	}
	virtual ~null_regexp() {}
	iregexp_base& operator [] (const iregexp_base& v)
	{
		iregexp_item_tree::iterator it;
		if ((*v.begin_item().first_child())->is_capture() == 0)
			it = add_item(begin_item().first_child(), new regexp_capture_item());
		else
			it = begin_item().first_child();
		add_item(it, v.begin_item().first_child(), v.end_item());
		return *this;
	}
};

template<typename target_iterator, typename target_compare = default_compare_class>
class begin_regexp
	: public iregexp_base<target_iterator, target_compare>
{
	class regexp_begin_item
		: public iregexp_item
	{
	private:
	protected:
		virtual bool can_repeat_match(void) { return false; }
		virtual bool can_backtrack(void) { return false; }
		virtual bool _syntax(typename iregexp_item_tree::const_iterator src, const target_iterator vb, target_iterator &vp, const target_iterator ve, bool backtracking)
		{
			return vp == vb;
		}
	public:
		regexp_begin_item() {}
		virtual ~regexp_begin_item() {}
		virtual void get_type_string(std::wstring& str) const
		{
			str += L"begin_p";
		}
	};
private:
protected:
public:
	begin_regexp(void)
	{
		add_item(begin_item(), new regexp_begin_item());
	}
	virtual ~begin_regexp() {}
};

template<typename target_iterator, typename target_compare = default_compare_class>
class end_regexp
	: public iregexp_base<target_iterator, target_compare>
{
	class regexp_end_item
		: public iregexp_item
	{
	private:
	protected:
		virtual bool can_repeat_match(void) { return false; }
		virtual bool can_backtrack(void) { return false; }
		virtual bool _syntax(typename iregexp_item_tree::const_iterator src, const target_iterator vb, target_iterator &vp, const target_iterator ve, bool backtracking)
		{
			return vp == ve;
		}
	public:
		regexp_end_item() {}
		virtual ~regexp_end_item() {}
		virtual void get_type_string(std::wstring& str) const
		{
			str += L"end_p";
		}
	};
private:
protected:
public:
	end_regexp(void)
	{
		add_item(begin_item(), new regexp_end_item());
	}
	virtual ~end_regexp() {}
};

template<typename target_iterator, typename target_compare = default_compare_class>
class befor_assertion_regexp
	: public iregexp_base<target_iterator, target_compare>
{
public:
	class regexp_befor_assertion_item
		: public iregexp_item
	{
	private:
		bool ftype;
	protected:
		virtual bool can_repeat_match(void) { return false; }
		virtual bool can_backtrack(void) { return false; }
		virtual bool _syntax(typename iregexp_item_tree::const_iterator src, const target_iterator vb, target_iterator &vp, const target_iterator ve, bool backtracking)
		{
			target_iterator v = vp;
			bool ret = ftype ^ iregexp_item::_syntax(src, vb, v, ve, backtracking);
			return ret;
		}
	public:
		regexp_befor_assertion_item(bool type) : ftype(type) {}
		virtual ~regexp_befor_assertion_item() {}
		virtual void get_type_string(std::wstring& str) const
		{
			str += L"befor_assertion_p(";
			str += ftype ? L"!=" : L"==";
			str += L")";
		}
	};
private:
protected:
public:
	befor_assertion_regexp(const iregexp_base& v, bool not = false)
	{
		iregexp_item_tree::iterator it = add_item(begin_item(), new regexp_befor_assertion_item(not));
		add_item(it, v.begin_item().first_child(), v.end_item());
	}
	virtual ~befor_assertion_regexp() {}
};

template<typename target_iterator, typename target_compare = default_compare_class>
class after_assertion_regexp
	: public iregexp_base<target_iterator, target_compare>
{
public:
	class regexp_after_assertion_item
		: public iregexp_item
	{
	private:
		bool ftype;
	protected:
		virtual bool can_repeat_match(void) { return false; }
		virtual bool can_backtrack(void) { return false; }
		virtual bool _syntax(typename iregexp_item_tree::const_iterator src, const target_iterator vb, target_iterator &vp, const target_iterator ve, bool backtracking)
		{
			size_t min = (**src).get_match_size(src, false);
			size_t max = (**src).get_match_size(src, true);
			size_t off = vp - vb;
			if (max > off)
				max = off;
			bool ret = false;
			if (min <= off)
			{
				for (size_t p = min; p <= max && !ret; ++p)
				{
					target_iterator v = vp - p;
					target_iterator vv = v;
					ret = ftype ^ (iregexp_item::_syntax(src, vv, v, vp, backtracking) && v == vp);
				}
			}
			return ret;
		}
	public:
		regexp_after_assertion_item(bool type) : ftype(type) {}
		virtual ~regexp_after_assertion_item() {}
		virtual void get_type_string(std::wstring& str) const
		{
			str += L"after_assertion_p(";
			str += ftype ? L"!=" : L"==";
			str += L")";
		}
	};
private:
protected:
public:
	after_assertion_regexp(const iregexp_base& v, bool not = false)
	{
		iregexp_item_tree::iterator it = add_item(begin_item(), new regexp_after_assertion_item(not));
		add_item(it, v.begin_item().first_child(), v.end_item());
	}
	virtual ~after_assertion_regexp() {}
};

template<typename target_iterator, typename target_compare = default_compare_class>
class boundary_assertion_regexp
	: public iregexp_base<target_iterator, target_compare>
{
public:
	class regexp_boundary_item
		: public iregexp_item
	{
	private:
		bool ftype;
	protected:
		virtual bool can_repeat_match(void) { return false; }
		virtual bool can_backtrack(void) { return false; }
		virtual bool _syntax(typename iregexp_item_tree::const_iterator src, const target_iterator vb, target_iterator &vp, const target_iterator ve, bool backtracking)
		{
			bool ret = false;
			if (vp != vb && vp != ve)
			{
				target_iterator v = vp;
				bool befor = iregexp_item::_syntax(src, vb, v, ve, backtracking);
				size_t min = (**src).get_match_size(src, false);
				size_t max = (**src).get_match_size(src, true);
				size_t off = vp - vb;
				if (max > off)
					max = off;
				bool after = befor;
				if (min <= off)
				{
					for (size_t p = min; p <= max && after == befor; ++p)
					{
						target_iterator v = vp - p;
						target_iterator vv = v;
						after = iregexp_item::_syntax(src, vv, v, vp, backtracking) && v == vp;
					}
				}
				ret = befor ^ after ^ ftype;
			}
			return ret;
		}
	public:
		regexp_boundary_item(bool type) : ftype(type) {}
		virtual ~regexp_boundary_item() {}
		virtual void get_type_string(std::wstring& str) const
		{
			str += L"boundary_p(";
			str += ftype ? L"!=" : L"==";
			str += L")";
		}
	};
private:
protected:
public:
	boundary_assertion_regexp(const iregexp_base& v, bool not = false)
	{
		iregexp_item_tree::iterator it = add_item(begin_item(), new regexp_boundary_item(not));
		add_item(it, v.begin_item().first_child(), v.end_item());
	}
	virtual ~boundary_assertion_regexp() {}
};

template<typename target_iterator, typename target_compare = default_compare_class>
class while_regexp
	: public iregexp_base<target_iterator, target_compare>
{
	class regexp_while_item
		: public iregexp_item
	{
	private:
	protected:
		virtual bool _syntax(typename iregexp_item_tree::const_iterator src, const target_iterator vb, target_iterator &vp, const target_iterator ve, bool backtracking)
		{
			iregexp_item_tree::const_iterator it = src.first_child();
			if (it.is_end())
				return false;
			iregexp_item_tree::const_iterator i = it.next_sibling();
			if (i.is_end())
				return false;
			size_t cnt = 0;
			while ((**it)(it, vb, vp, ve, backtracking) && (**i)(i, vb, vp, ve, backtracking))
				++cnt;
			bool ret = cnt != 0;
			return ret;
		}
	public:
		regexp_while_item() {}
		virtual ~regexp_while_item() {}
		virtual void get_type_string(std::wstring& str) const
		{
			str += L"while_p";
		}
	};
private:
protected:
public:
	while_regexp(const iregexp_base& v)
	{
		iregexp_item_tree::iterator it = classter_items(new regexp_while_item());
		it = add_item(it);
		add_item(it, v.begin_item().first_child(), v.end_item());
	}
	virtual ~while_regexp() {}
	iregexp_base& operator [] (const iregexp_base& v)
	{
		iregexp_item_tree::iterator it = add_item(begin_item().first_child());
		add_item(it, v.begin_item().first_child(), v.end_item());
		return *this;
	}
};

template<typename target_iterator, typename target_compare = default_compare_class>
class do_while_regexp
	: public iregexp_base<target_iterator, target_compare>
{
	class regexp_do_while_item
		: public iregexp_item
	{
	private:
	protected:
		virtual bool _syntax(typename iregexp_item_tree::const_iterator src, const target_iterator vb, target_iterator &vp, const target_iterator ve, bool backtracking)
		{
			iregexp_item_tree::const_iterator it = src.first_child();
			if (it.is_end())
				return false;
			iregexp_item_tree::const_iterator i = it.next_sibling();
			if (i.is_end())
				return false;
			size_t cnt = 0;
			while ((**i)(i, vb, vp, ve, backtracking) && (**it)(it, vb, vp, ve, backtracking))
				++cnt;
			bool ret = cnt != 0;
			return ret;
		}
	public:
		regexp_do_while_item() {}
		virtual ~regexp_do_while_item() {}
		virtual void get_type_string(std::wstring& str) const
		{
			str += L"do_while_p";
		}
	};
private:
protected:
public:
	do_while_regexp(const iregexp_base& v)
	{
		iregexp_item_tree::iterator it = classter_items(new regexp_do_while_item());
		it = add_item(it);
		add_item(it, v.begin_item().first_child(), v.end_item());
	}
	virtual ~do_while_regexp() {}
	iregexp_base& while_p(const iregexp_base& v)
	{
		iregexp_item_tree::iterator it = add_item(begin_item().first_child());
		add_item(it, v.begin_item().first_child(), v.end_item());
		return *this;
	}
};

template<typename target_iterator, typename target_compare = default_compare_class>
class capture_regexp
	: public iregexp_base<target_iterator, target_compare>
{
private:
protected:
public:
	capture_regexp() {}
	capture_regexp(const iregexp_base& v)
	{
		iregexp_item_tree::iterator it = add_item(begin_item(), new regexp_capture_item());
		add_item(it, v.begin_item().first_child(), v.end_item());
	}
	capture_regexp(const target_range name)
	{
		add_item(begin_item(), new regexp_capture_name_item(name));
	}
	capture_regexp(size_t n)
	{
		add_item(begin_item(), new regexp_capture_call_item(n));
	}
	capture_regexp(size_t n, bool prev)
	{
		add_item(begin_item(), new regexp_capture_delta_call_item(n, prev));
	}
	iregexp_base& operator [] (const iregexp_base& v)
	{
		iregexp_item_tree::iterator it = begin_item().first_child();
		add_item(it, v.begin_item().first_child(), v.end_item());
		return *this;
	}
	virtual ~capture_regexp() {}
};

template<typename target_iterator, typename target_compare = default_compare_class>
class call_capture_regexp
	: public capture_regexp<target_iterator, target_compare>
{
private:
	class call_stack
	{
	private:
		size_t fpos;
		std::vector<size_t> fpos_stack;
		std::vector<std::vector<size_t> > fstack;
	protected:
	public:
		void reset(void)
		{
			fpos_stack.clear();
			fstack.clear();
		}
		void pos_push(size_t pos)
		{
			fpos_stack.push_back(pos);
		}
		void pos_pop(size_t pos)
		{
			fpos_stack.pop_back(pos);
		}
		size_t last_pos(void) const
		{
			return fpos_stack.back();
		}
		size_t pos_stack_size(void) const
		{
			return fpos_stack.size();
		}
		void stack_push(void)
		{
			fstack.push_back(fpos_stack);
		}
		void stack_pop()
		{
			fpos_stack = fstack.back();
			fstack.pop_back();
		}
	};
	class regexp_capture_call_item
		: public iregexp_item
		, public call_stack
	{
	private:
		size_t fn;
	protected:
		virtual bool _syntax(typename iregexp_item_tree::const_iterator src, const target_iterator vb, target_iterator &vp, const target_iterator ve, bool backtracking)
		{
			iregexp_item_tree::const_iterator it = find_capture(src, fn);
			bool ret = false;
			size_t pos = vp - vb;
			if (backtracking || pos_stack_size() == 0 || last_pos() != pos)
			{
				if (!backtracking)
					pos_push(pos);
				if (!it.is_end())
					ret = (**it)(it, vb, vp, ve, backtracking);
			}
			return ret;
		}
	public:
		regexp_capture_call_item(size_t n) : fn(n) {}
		virtual ~regexp_capture_call_item() {}
		virtual void get_type_string(std::wstring& str) const
		{
			str += L"call_p(\\";
			wchar_t istr[33];
			_itow_s(fn, istr, 10);
			str += istr;
			str += L")";
		}
		virtual void match_stack_reset(void)
		{
			iregexp_item_match_base::match_stack_reset();
			call_stack::reset();
		}
		virtual void match_stack_push(void)
		{
			iregexp_item_match_base::match_stack_push();
			call_stack::stack_push();
		}
		virtual void match_stack_pop()
		{
			iregexp_item_match_base::match_stack_pop();
			call_stack::stack_pop();
		}
	};
	class regexp_capture_delta_call_item
		: public iregexp_item
		, public call_stack
	{
	private:
		size_t fn;
		bool fdelta;
	protected:
		virtual bool _syntax(typename iregexp_item_tree::const_iterator src, const target_iterator vb, target_iterator &vp, const target_iterator ve, bool backtracking)
		{
			iregexp_item_tree::const_iterator it = find_capture(src, fn, fdelta);
			bool ret = false;
			size_t pos = vp - vb;
			if (backtracking || pos_stack_size() == 0 || last_pos() != pos)
			{
				if (!backtracking)
					pos_push(pos);
				if (!it.is_end())
					ret = (**it)(it, vb, vp, ve, backtracking);
			}
			return ret;
		}
	public:
		regexp_capture_delta_call_item(size_t n, bool prev) : fn(n), fdelta(prev) {}
		virtual ~regexp_capture_delta_call_item() {}
		virtual void get_type_string(std::wstring& str) const
		{
			str += L"delta_call_p(\\";
			str += fdelta ? L"+" : L"-";
			wchar_t istr[33];
			_itow_s(fn, istr, 10);
			str += istr;
			str += L")";
		}
		virtual void match_stack_reset(void)
		{
			iregexp_item_match_base::match_stack_reset();
			call_stack::reset();
		}
		virtual void match_stack_push(void)
		{
			iregexp_item_match_base::match_stack_push();
			call_stack::stack_push();
		}
		virtual void match_stack_pop()
		{
			iregexp_item_match_base::match_stack_pop();
			call_stack::stack_pop();
		}
	};
	class regexp_capture_call_name_item
		: public iregexp_item
		, public call_stack
	{
	private:
		target_range fname;
	protected:
		virtual bool _syntax(typename iregexp_item_tree::const_iterator src, const target_iterator vb, target_iterator &vp, const target_iterator ve, bool backtracking)
		{
			iregexp_item_tree::const_iterator it = find_capture(src, fname);
			bool ret = false;
			size_t pos = vp - vb;
			if (backtracking || pos_stack_size() == 0 || last_pos() != pos)
			{
				if (!backtracking)
					pos_push(pos);
				if (!it.is_end())
					ret = (**it)(it, vb, vp, ve, backtracking);
			}
			return ret;
		}
	public:
		regexp_capture_call_name_item(const target_range name) : fname(name) {}
		virtual ~regexp_capture_call_name_item() {}
		virtual void get_type_string(std::wstring& str) const
		{
			str += L"name_call_p('";
			str.append((wchar_t*)&*fname.first, fname.second - fname.first);
			str += L"')";
		}
		virtual void match_stack_reset(void)
		{
			iregexp_item_match_base::match_stack_reset();
			call_stack::reset();
		}
		virtual void match_stack_push(void)
		{
			iregexp_item_match_base::match_stack_push();
			call_stack::stack_push();
		}
		virtual void match_stack_pop()
		{
			iregexp_item_match_base::match_stack_pop();
			call_stack::stack_pop();
		}
	};
protected:
public:
	call_capture_regexp(const target_range name) { add_item(begin_item(), new regexp_capture_call_name_item(name)); }
	call_capture_regexp(size_t n) { add_item(begin_item(), new regexp_capture_call_item(n)); }
	call_capture_regexp(size_t n, bool prev) { add_item(begin_item(), new regexp_capture_delta_call_item(n, prev)); }
};

template<typename target_iterator, typename target_compare = default_compare_class>
class use_match_capture_regexp
	: public capture_regexp<target_iterator, target_compare>
{
private:
	class regexp_capture_use_item
		: public iregexp_item
	{
	private:
		size_t fn;
	protected:
		virtual bool can_backtrack(void) { return false; }
		virtual bool _syntax(typename iregexp_item_tree::const_iterator src, const target_iterator vb, target_iterator &vp, const target_iterator ve, bool backtracking)
		{
			iregexp_item_tree::const_iterator it = find_capture(src, fn);
			bool ret = false;
			if (!it.is_end() && (**it).is_matchs())
			{
				iregexp_item_point ip = (**it).last_matchs();
				ret = ve - vp >= ip.second - ip.first && target_range_equal()(vb + ip.first, vb + ip.second, vp);
				if (ret)
					vp += ip.second - ip.first;
			}
			return ret;
		}
	public:
		regexp_capture_use_item(size_t n) : fn(n) {}
		virtual ~regexp_capture_use_item() {}
		virtual void get_type_string(std::wstring& str) const
		{
			str += L"use_p(\\";
			wchar_t istr[33];
			_itow_s(fn, istr, 10);
			str += istr;
			str += L")";
		}
	};
	class regexp_capture_delta_use_item
		: public iregexp_item
	{
	private:
		size_t fn;
		size_t fdelta;
	protected:
		virtual bool can_backtrack(void) { return false; }
		virtual bool _syntax(typename iregexp_item_tree::const_iterator src, const target_iterator vb, target_iterator &vp, const target_iterator ve, bool backtracking)
		{
			iregexp_item_tree::const_iterator it = find_capture(src, fn, true);
			bool ret = false;
			if (!it.is_end() && (**it).is_matchs())
			{
				iregexp_item_point ip = (**it).last_matchs();
				ret = ve - vp >= ip.second - ip.first && target_range_equal()(vb + ip.first, vb + ip.second, vp);
				if (ret)
					vp += ip.second - ip.first;
			}
			return ret;
		}
	public:
		regexp_capture_delta_use_item(size_t n, bool prev) : fn(n), fdelta(prev ? -1 : 1) {}
		virtual ~regexp_capture_delta_use_item() {}
		virtual void get_type_string(std::wstring& str) const
		{
			str += L"delta_use_p(\\";
			wchar_t istr[33];
			_itow_s(fn, istr, 10);
			str += istr;
			str += L")";
		}
	};
	class regexp_capture_use_name_item
		: public iregexp_item
	{
	private:
		target_range fname;
	protected:
		virtual bool can_backtrack(void) { return false; }
		virtual bool _syntax(typename iregexp_item_tree::const_iterator src, const target_iterator vb, target_iterator &vp, const target_iterator ve, bool backtracking)
		{
			iregexp_item_tree::const_iterator it = find_capture(src, fname);
			bool ret = false;
			if (!it.is_end() && (**it).is_matchs())
			{
				iregexp_item_point ip = (**it).last_matchs();
				ret = ve - vp >= ip.second - ip.first && target_range_equal()(vb + ip.first, vb + ip.second, vp);
				if (ret)
					vp += ip.second - ip.first;
			}
			return ret;
		}
	public:
		regexp_capture_use_name_item(const target_range name) : fname(name) {}
		virtual ~regexp_capture_use_name_item() {}
		virtual void get_type_string(std::wstring& str) const
		{
			str += L"name_use_p('";
			str.append((wchar_t*)&*fname.first, fname.second - fname.first);
			str += L"')";
		}
	};
protected:
public:
	use_match_capture_regexp(const target_range name) { add_item(begin_item(), new regexp_capture_use_name_item(name)); }
	use_match_capture_regexp(size_t n) { add_item(begin_item(), new regexp_capture_use_item(n)); }
	use_match_capture_regexp(size_t n, bool prev) { add_item(begin_item(), new regexp_capture_delta_use_item(n, prev)); }
};

template<typename target_iterator, typename target_compare = default_compare_class>
class is_match_capture_regexp
	: public capture_regexp<target_iterator, target_compare>
{
private:
	class regexp_capture_match_item
		: public iregexp_item
	{
	private:
		size_t fn;
	protected:
		virtual bool can_backtrack(void) { return false; }
		virtual bool _syntax(typename iregexp_item_tree::const_iterator src, const target_iterator vb, target_iterator &vp, const target_iterator ve, bool backtracking)
		{
			iregexp_item_tree::const_iterator it = find_capture(src, fn);
			bool ret = false;
			if (!it.is_end() && (**it).is_matchs())
			{
				iregexp_item_point ip = (**it).last_matchs();
				ret = ve - vp > 0;
			}
			return ret;
		}
	public:
		regexp_capture_match_item(size_t n) : fn(n) {}
		virtual ~regexp_capture_match_item() {}
		virtual void get_type_string(std::wstring& str) const
		{
			str += L"match_p(\\";
			wchar_t istr[33];
			_itow_s(fn, istr, 10);
			str += istr;
			str += L")";
		}
	};
	class regexp_capture_delta_match_item
		: public iregexp_item
	{
	private:
		size_t fn;
		size_t fdelta;
	protected:
		virtual bool can_backtrack(void) { return false; }
		virtual bool _syntax(typename iregexp_item_tree::const_iterator src, const target_iterator vb, target_iterator &vp, const target_iterator ve, bool backtracking)
		{
			iregexp_item_tree::iterator it = find_capture(src, fn, true);
			bool ret = false;
			if (!it.is_end() && (**it).is_matchs())
			{
				iregexp_item_point ip = (**it).last_matchs();
				ret = ve - vp > 0;
			}
			return ret;
		}
	public:
		regexp_capture_delta_match_item(size_t n, bool prev) : fn(n), fdelta(prev ? -1 : 1) {}
		virtual ~regexp_capture_delta_match_item() {}
		virtual void get_type_string(std::wstring& str) const
		{
			str += L"delta_match_p(\\";
			wchar_t istr[33];
			_itow_s(fn, istr, 10);
			str += istr;
			str += L")";
		}
	};
	class regexp_capture_match_name_item
		: public iregexp_item
	{
	private:
		target_range fname;
	protected:
		virtual bool can_backtrack(void) { return false; }
		virtual bool _syntax(typename iregexp_item_tree::const_iterator src, const target_iterator vb, target_iterator &vp, const target_iterator ve, bool backtracking)
		{
			iregexp_item_tree::const_iterator it = find_capture(src, fname);
			bool ret = false;
			if (!it.is_end() && (**it).is_matchs())
			{
				iregexp_item_point ip = (**it).last_matchs();
				ret = ve - vp > 0;
			}
			return ret;
		}
	public:
		regexp_capture_match_name_item(const target_range name) : fname(name) {}
		virtual ~regexp_capture_match_name_item() {}
		virtual void get_type_string(std::wstring& str) const
		{
			str += L"name_match_p('";
			str.append((wchar_t*)&*fname.first, fname.second - fname.first);
			str += L"')";
		}
	};
protected:
public:
	is_match_capture_regexp(const target_range name) { add_item(begin_item(), new regexp_capture_match_name_item(name)); }
	is_match_capture_regexp(size_t n) { add_item(begin_item(), new regexp_capture_match_item(n)); }
	is_match_capture_regexp(size_t n, bool prev) { add_item(begin_item(), new regexp_capture_delta_match_item(n, prev)); }
};

template<typename target_iterator, typename target_compare = default_compare_class>
class recursion_capture_regexp
	: public capture_regexp<target_iterator, target_compare>
{
private:
	class regexp_recursion0_item
		: public iregexp_item
	{
	private:
	protected:
		virtual bool can_backtrack(void) { return false; }
		virtual bool _syntax(typename iregexp_item_tree::const_iterator src, const target_iterator vb, target_iterator &vp, const target_iterator ve, bool backtracking)
		{
			bool ret = false;
			return ret;
		}
	public:
		regexp_recursion0_item() {}
		virtual ~regexp_recursion0_item() {}
		virtual void get_type_string(std::wstring& str) const
		{
			str += L"recursion_p(\\0)";
		}
	};
	class regexp_recursion_item
		: public iregexp_item
	{
	private:
		size_t fn;
	protected:
		virtual bool can_backtrack(void) { return false; }
		virtual bool _syntax(typename iregexp_item_tree::const_iterator src, const target_iterator vb, target_iterator &vp, const target_iterator ve, bool backtracking)
		{
			bool ret = false;
			return ret;
		}
	public:
		regexp_recursion_item(size_t n) : fn(n) {}
		virtual ~regexp_recursion_item() {}
		virtual void get_type_string(std::wstring& str) const
		{
			str += L"recursion_p(\\";
			wchar_t istr[33];
			_itow_s(fn, istr, 10);
			str += istr;
			str += L")";
		}
	};
	class regexp_recursion_name_item
		: public iregexp_item
	{
	private:
		target_range fname;
	protected:
		virtual bool can_backtrack(void) { return false; }
		virtual bool _syntax(typename iregexp_item_tree::const_iterator src, const target_iterator vb, target_iterator &vp, const target_iterator ve, bool backtracking)
		{
			bool ret = false;
			return ret;
		}
	public:
		regexp_recursion_name_item(const target_range name) : fname(name) {}
		virtual ~regexp_recursion_name_item() {}
		virtual void get_type_string(std::wstring& str) const
		{
			str += L"recursion_name_p('";
			str.append((wchar_t*)&*fname.first, fname.second - fname.first);
			str += L"')";
		}
	};
protected:
public:
	recursion_capture_regexp(const target_range name) { add_item(begin_item(), new regexp_recursion_name_item(name)); }
	recursion_capture_regexp(size_t n) { add_item(begin_item(), new regexp_recursion_item(n)); }
	recursion_capture_regexp() { add_item(begin_item(), new regexp_recursion0_item()); }
};

template<typename target_iterator, typename target_compare = default_compare_class>
class if_regexp
	: public iregexp_base<target_iterator, target_compare>
{
	class regexp_if_item
		: public iregexp_item
	{
	private:
		bool fif_match;
		bool fnot;
		std::vector<bool> fstack;
	protected:
		virtual bool _syntax(typename iregexp_item_tree::const_iterator src, const target_iterator vb, target_iterator &vp, const target_iterator ve, bool backtracking)
		{
			bool ret = false;
			if (!backtracking)
			{
				iregexp_item_tree::const_iterator it = src.first_child();
				if (it.is_end())
					return ret;
				iregexp_item_tree::const_iterator i = it.next_sibling();
				if (fnot ^ (**it)(it, vb, vp, ve, false))
				{
					if (!i.is_end())
						ret = (**i)(i, vb, vp, ve, false);
					fif_match = true;
				}
				else
				{
					i.move_next_sibling();
					if (!i.is_end())
						ret = (**i)(i, vb, vp, ve, false);
					fif_match = false;
				}
			}
			else
			{
				iregexp_item_tree::const_iterator it = src.first_child();
				iregexp_item_tree::const_iterator i = it.next_sibling();
				target_iterator v = vp;
				if (fif_match)
				{
					if (!i.is_end())
						ret = (**i)(i, vb, vp, ve, true) && v != vp;
				}
				else
				{
					i.move_next_sibling();
					if (!i.is_end())
						ret = (**i)(i, vb, vp, ve, true) && v != vp;
				}
			}
			return ret;
		}
	public:
		regexp_if_item(bool not = false)
			: fnot(not)
		{
			set_parallel_match_size();
		}
		virtual ~regexp_if_item() {}
		virtual void get_type_string(std::wstring& str) const
		{
			str += L"if_p";
		}
		virtual void match_stack_reset(void)
		{
			iregexp_item_match_base::match_stack_reset();
			fstack.clear();
		}
		virtual void match_stack_push(void)
		{
			iregexp_item_match_base::match_stack_push();
			fstack.push_back(fif_match);
		}
		virtual void match_stack_pop()
		{
			iregexp_item_match_base::match_stack_pop();
			fif_match = fstack.back();
			fstack.pop_back();
		}
	};
private:
protected:
public:
	if_regexp(const iregexp_base& v, bool not = false)
	{
		iregexp_item_tree::iterator it = classter_items(new regexp_if_item(not));
		it = classter_items(it);
		add_item(it, v.begin_item().first_child(), v.end_item());
	}
	virtual ~if_regexp() {}
	if_regexp& operator [] (const iregexp_base& v)
	{
		iregexp_item_tree::iterator it = add_item(begin_item().first_child());
		add_item(it, v.begin_item().first_child(), v.end_item());
		return *this;
	}
	iregexp_base& else_p(const iregexp_base& v)
	{
		iregexp_item_tree::iterator it = add_item(begin_item().first_child());
		add_item(it, v.begin_item().first_child(), v.end_item());
		return *this;
	}
	if_regexp& set_items(const iregexp_base& v)
	{
		iregexp_item_tree::const_iterator vit = v.begin_item().first_child();
		iregexp_item_tree::iterator it = begin_item().first_child();
		if ((**vit).is_or_syntax())
			vit.move_first_child();
		else
			it = add_item(it);
		if (!vit.is_end())
		{
			add_item(it, vit, v.end_item());
		}
		return *this;
	}
};

template<typename target_iterator, typename target_compare = default_compare_class>
class iregexp_callback
{
public:
	virtual void OnIfAction(long param, bool success, const target_iterator vb, target_iterator &vp, const target_iterator ve) {}
	virtual void OnAction(long param, const target_iterator vb, target_iterator &vp, const target_iterator ve) {}
	virtual short OnSwitch(long param) { return -1; }
	virtual void OnFunc(long param, const target_iterator vb, target_iterator &vp, const target_iterator ve) {}
	virtual bool OnQuery(long param, const target_iterator vb, target_iterator &vp, const target_iterator ve) { return false; }
	virtual void OnSelect(long param, short index, const target_iterator vb, target_iterator &vp, const target_iterator ve) {}
	virtual void OnForInit(long param, const target_iterator vb, target_iterator &vp, const target_iterator ve) {}
	virtual void OnForReinit(long param, const target_iterator vb, target_iterator &vp, const target_iterator ve) {}
};

template<typename target_iterator, typename target_compare = default_compare_class>
class for_regexp
	: public iregexp_base<target_iterator, target_compare>
{
	class regexp_for_item
		: public iregexp_item
	{
	private:
		long fparam;
		iregexp_callback<target_iterator>* fcallback;
	protected:
		virtual bool _syntax(typename iregexp_item_tree::const_iterator src, const target_iterator vb, target_iterator &vp, const target_iterator ve, bool backtracking)
		{
			iregexp_item_tree::const_iterator it = src.first_child();
			iregexp_item_tree::const_iterator i = it.next_sibling();
			size_t cnt = 0;
			fcallback->OnForInit(fparam, vb, vp, ve);
			while ((**it)(it, vb, vp, ve, backtracking) && (**i)(i, vb, vp, ve, backtracking))
			{
				++cnt;
				fcallback->OnForReinit(fparam, vb, vp, ve);
			}
			bool ret = cnt != 0;
			return ret;
		}
	public:
		regexp_for_item(long param, iregexp_callback<target_iterator>* call_back)
			: fparam(param), fcallback(call_back) {}
		virtual ~regexp_for_item() {}
		virtual void get_type_string(std::wstring& str) const
		{
			str += L"for_p";
			wchar_t istr[33];
			str += L"(";
			_itow_s(fparam, istr, 10);
			str += istr;
			str += L")";
		}
	};
private:
protected:
public:
	for_regexp(const iregexp_base& v, long param, iregexp_callback<target_iterator>* call_back)
	{
		iregexp_item_tree::iterator it = add_item(begin_item(), new regexp_for_item(param, call_back));
		it = add_item(it);
		add_item(it, v.begin_item().first_child(), v.end_item());
	}
	iregexp_base& operator [] (const iregexp_base& v)
	{
		iregexp_item_tree::iterator it = add_item(begin_item().first_child());
		add_item(it, v.begin_item().first_child(), v.end_item());
		return *this;
	}
	virtual ~for_regexp() {}
};

template<typename target_iterator, typename target_compare = default_compare_class>
class if_action_regexp
	: public iregexp_base<target_iterator, target_compare>
{
	class regexp_if_action_item
		: public iregexp_item
	{
	private:
		long fparam;
		iregexp_callback<target_iterator> *fcallback;
	protected:
		virtual bool can_backtrack(void) { return false; }
		virtual bool _syntax(typename iregexp_item_tree::const_iterator src, const target_iterator vb, target_iterator &vp, const target_iterator ve, bool backtracking)
		{
			target_iterator v = vp;
			bool ret = iregexp_item::_syntax(src, vb, v, ve, backtracking);
			fcallback->OnIfAction(fparam, ret, vb, vp, v);
			vp = v;
			return ret;
		}
	public:
		regexp_if_action_item(long param, iregexp_callback<target_iterator>* callback)
			: fparam(param), fcallback(callback) {}
		virtual ~regexp_if_action_item() {}
		virtual void get_type_string(std::wstring& str) const
		{
			str += L"if_action_p";
			wchar_t istr[33];
			str += L"(";
			_itow_s(fparam, istr, 10);
			str += istr;
			str += L")";
		}
	};
private:
protected:
public:
	if_action_regexp(long param, iregexp_callback<target_iterator>* callback)
	{
		add_item(begin_item(), new regexp_if_action_item(param, callback));
	}
	if_action_regexp(const iregexp_base& v, long param, iregexp_callback<target_iterator>* callback)
	{
		iregexp_item_tree::iterator it = add_item(begin_item(), new regexp_if_action_item(param, callback));
		add_item(it, v.begin_item().first_child(), v.end_item());
	}
	virtual ~if_action_regexp() {}
	iregexp_base& operator [] (const iregexp_base& v)
	{
		iregexp_item_tree::iterator it = begin_item().first_child();
		add_item(it, v.begin_item().first_child(), v.end_item());
		return *this;
	}
};

template<typename target_iterator, typename target_compare = default_compare_class>
class action_regexp
	: public iregexp_base<target_iterator, target_compare>
{
	class regexp_action_item
		: public iregexp_item
	{
	private:
		bool fnot;
		long fparam;
		iregexp_callback<target_iterator> *fcallback;
	protected:
		virtual bool can_backtrack(void) { return false; }
		virtual bool _syntax(typename iregexp_item_tree::const_iterator src, const target_iterator vb, target_iterator &vp, const target_iterator ve, bool backtracking)
		{
			target_iterator v = vp;
			bool ret = iregexp_item::_syntax(src, vb, v, ve, backtracking);
			if (fnot ^ ret)
				fcallback->OnAction(fparam, vb, vp, v);
			vp = v;
			return ret;
		}
	public:
		regexp_action_item(long param, iregexp_callback<target_iterator>* callback, bool not)
			: fparam(param), fnot(not), fcallback(callback) {}
		virtual ~regexp_action_item() {}
		virtual void get_type_string(std::wstring& str) const
		{
			str += L"action_p(";
			str += fnot ? L"!" : L"=";
			wchar_t istr[33];
			_itow_s(fparam, istr, 10);
			str += istr;
			str += L")";
		}
	};
private:
protected:
public:
	action_regexp(long param, iregexp_callback<target_iterator>* callback, bool not = false)
	{
		add_item(begin_item(), new regexp_action_item(param, callback, not));
	}
	action_regexp(const iregexp_base& v, long param, iregexp_callback<target_iterator>* callback, bool not = false)
	{
		iregexp_item_tree::iterator it = add_item(begin_item(), new regexp_action_item(param, callback, not));
		add_item(it, v.begin_item().first_child(), v.end_item());
	}
	virtual ~action_regexp() {}
	iregexp_base& operator [] (const iregexp_base& v)
	{
		iregexp_item_tree::iterator it = begin_item().first_child();
		add_item(it, v.begin_item().first_child(), v.end_item());
		return *this;
	}
};

template<typename target_iterator, typename target_compare = default_compare_class>
class switch_regexp
	: public iregexp_base<target_iterator, target_compare>
{
	class regexp_switch_item
		: public iregexp_item
	{
	private:
		long fparam;
		iregexp_callback<target_iterator> *fcallback;
		short fswitch_value;
		std::vector<short> fstack;
	protected:
		virtual bool _syntax(typename iregexp_item_tree::const_iterator src, const target_iterator vb, target_iterator &vp, const target_iterator ve, bool backtracking)
		{
			bool ret = false;
			if (!backtracking)
			{
				iregexp_item_tree::const_iterator it = src.first_child();
				fswitch_value = fcallback->OnSwitch(fparam);
				if (fswitch_value == -1)
					ret = (**it)(it, vb, vp, ve, false);
				else
				{
					it.move_next_sibling();
					for (int n = 0; !it.is_end(); ++n)
					{
						if (n == fswitch_value)
						{
							ret = (**it)(it, vb, vp, ve, false);
							break;
						}
						it.move_next_sibling();
					}
				}
			}
			else
			{
				iregexp_item_tree::const_iterator it = src.first_child();
				if (fswitch_value == -1)
					ret = (**it)(it, vb, vp, ve, true);
				else
				{
					it.move_next_sibling();
					for (int n = 0; !it.is_end(); ++n)
					{
						if (n == fswitch_value)
						{
							ret = (**it)(it, vb, vp, ve, true);
							break;
						}
						it.move_next_sibling();
					}
				}
			}
			return ret;
		}
	public:
		regexp_switch_item(long param, iregexp_callback<target_iterator>* callback)
			: fparam(param), fcallback(callback)
		{
			set_parallel_match_size();
		}
		virtual ~regexp_switch_item() {}
		virtual void get_type_string(std::wstring& str) const
		{
			str += L"switch_p";
			wchar_t istr[33];
			str += L"(";
			_itow_s(fparam, istr, 10);
			str += istr;
			str += L")";
		}
		virtual void match_stack_reset(void)
		{
			iregexp_item_match_base::match_stack_reset();
			fstack.clear();
		}
		virtual void match_stack_push(void)
		{
			iregexp_item_match_base::match_stack_push();
			fstack.push_back(fswitch_value);
		}
		virtual void match_stack_pop()
		{
			iregexp_item_match_base::match_stack_pop();
			fswitch_value = fstack.back();
			fstack.pop_back();
		}
	};
private:
protected:
public:
	switch_regexp(long param, iregexp_callback<target_iterator>* callback)
	{
		iregexp_item_tree::iterator it = add_item(begin_item(), new regexp_switch_item(param, callback));
		it = add_item(it);
	}
	switch_regexp& operator [] (const iregexp_base& v)
	{
		iregexp_item_tree::iterator it = add_item(begin_item().first_child());
		add_item(it, v.begin_item().first_child(), v.end_item());
		return *this;
	}
	iregexp_base& default_p(const iregexp_base& v)
	{
		iregexp_item_tree::iterator it = begin_item().first_child().first_child();
		add_item(it, v.begin_item().first_child(), v.end_item());
		return *this;
	}
	switch_regexp& set_items(const iregexp_base& v)
	{
		iregexp_item_tree::const_iterator vit = v.begin_item().first_child();
		iregexp_item_tree::iterator it = begin_item().first_child();
		if ((**vit).is_or_syntax())
			vit.move_first_child();
		else
			it = add_item(it);
		if (!vit.is_end())
		{
			add_item(it, vit, v.end_item());
		}
		return *this;
	}
	virtual ~switch_regexp() {}
};

template<typename target_iterator, typename target_compare = default_compare_class>
class list_switch_regexp
	: public iregexp_base<target_iterator, target_compare>
{
	class regexp_list_switch_item
		: public iregexp_item
	{
	private:
		enum capture_type
		{
			ctIndex,
			ctDelta,
			ctName,
		};
		capture_type fcapture_type;
		size_t findex;
		bool fdlt;
		target_range fname;
		short fswitch_value;
		std::vector<short> fstack;
		std::vector<std::pair<target_range, size_t>> flist;
		void set_list(const target_range& v)
		{
			target_iterator s = v.first;
			target_iterator c = v.second - 1;
			size_t l = 0;
			for (target_iterator p = s + 1; p != v.second; ++p)
			{
				if (target_equal()(p, c))
				{
					target_range item(s, p);
					if (std::binary_search(flist.begin(), flist.end(), std::pair<target_range, size_t>(item, val), map_compare()))
						std::lower_bound(flist.begin(), flist.end(), std::pair<target_range, size_t>(item, val), map_compare())->second = val;
					else
						flist.insert(std::lower_bound(flist.begin(), flist.end(), std::pair<target_range, size_t>(item, val), map_compare()), std::pair<target_range, size_t>(item, val));
					++p;
					s = p;
				}
			}
		}
	protected:
		virtual bool _syntax(typename iregexp_item_tree::const_iterator src, const target_iterator vb, target_iterator &vp, const target_iterator ve, bool backtracking)
		{
			if (!backtracking)
			{
				iregexp_item_tree::const_iterator it;
				switch (fcapture_type)
				{
				case ctIndex:
					it = find_capture(src, findex);
					break;
				case ctName:
					it = find_capture(src, fname);
					break;
				case ctDelta:
					it = find_capture(src, findex, fdlt);
					break;
				default:
					it = src.end();
				}
				if (!it.is_end() && (**it).is_matchs())
				{
					iregexp_item_point pos = (**it).last_matchs();
					target_range item(vb + pos.first, vb + pos.second);
					if (std::binary_search(flist.begin(), flist.end(), std::pair<target_range, size_t>(item, 0), map_compare()))
					{
						std::vector<std::pair<target_range, size_t>>::const_iterator s = std::lower_bound(flist.begin(), flist.end(), std::pair<target_range, size_t>(item, 0), map_compare());
						fswitch_value = s->second;
					}
					else
						fswitch_value = -1;
				}
			}
			bool ret = false;
			iregexp_item_tree::const_iterator it = src.first_child();
			if (fswitch_value == -1)
				ret = (**it)(it, vb, vp, ve, backtracking);
			else
			{
				iregexp_item_tree::const_iterator i = it.next_sibling();
				for (int l = 0; l < fswitch_value && !i.is_end(); ++l, i.move_next_sibling());
				if (!i.is_end())
					ret = (**i)(i, vb, vp, ve, backtracking);
				else
					ret = (**it)(it, vb, vp, ve, backtracking);
			}
			return ret;
		}
	public:
		regexp_list_switch_item(size_t capture_index, const target_range& list)
		{
			set_parallel_match_size();
			fcapture_type = ctIndex;
			findex = capture_index;
			set_list(list);
		}
		regexp_list_switch_item(size_t capture_index, bool prev, const target_range& list)
		{
			set_parallel_match_size();
			fcapture_type = ctDeltaIndex;
			findex = capture_index;
			fdlt = prev;
			set_list(list);
		}
		regexp_list_switch_item(const target_range& capture_name, const target_range& list)
		{
			set_parallel_match_size();
			fcapture_type = ctName;
			fname = capture_name;
			set_list(list);
		}
		virtual ~regexp_list_switch_item() {}
		virtual void get_type_string(std::wstring& str) const
		{
			str += L"list_switch_p";
			wchar_t istr[33];
			str += L"(";
			switch (fcapture_type)
			{
			case ctDelta:
				str += fdlt ? L"-" : L"+";
			case ctIndex:
				_itow_s(findex, istr, 10);
				str += istr;
				break;
			case ctName:
				str += L"'";
				str.append((wchar_t*)&*fname.begin(), fname.size());
				str += L"'";
				break;
			}
		}
		virtual void match_stack_reset(void)
		{
			iregexp_item_match_base::match_stack_reset();
			fstack.clear();
		}
		virtual void match_stack_push(void)
		{
			iregexp_item_match_base::match_stack_push();
			fstack.push_back(fswitch_value);
		}
		virtual void match_stack_pop()
		{
			iregexp_item_match_base::match_stack_pop();
			fswitch_value = fstack.back();
			fstack.pop_back();
		}
	};
private:
protected:
public:
	list_switch_regexp(size_t capture_index, const target_range& list)
	{
		iregexp_item_tree::iterator it = add_item(begin_item(), new regexp_list_switch_item(capture_index, list));
		it = add_item(it);
	}
	list_switch_regexp(size_t capture_index, bool prev, const target_range& list)
	{
		iregexp_item_tree::iterator it = add_item(begin_item(), new regexp_list_switch_item(capture_index, prev, list));
		it = add_item(it);
	}
	list_switch_regexp(const target_range& capture_name, const target_range& list)
	{
		iregexp_item_tree::iterator it = add_item(begin_item(), new regexp_list_switch_item(capture_name, list));
		it = add_item(it);
	}
	list_switch_regexp& operator [] (const iregexp_base& v)
	{
		iregexp_item_tree::iterator it = add_item(begin_item().first_child());
		add_item(it, v.begin_item().first_child(), v.end_item());
		return *this;
	}
	iregexp_base& default_p(const iregexp_base& v)
	{
		iregexp_item_tree::iterator it = begin_item().first_child().first_child();
		add_item(it, v.begin_item().first_child(), v.end_item());
		return *this;
	}
	list_switch_regexp& set_items(const iregexp_base& v)
	{
		iregexp_item_tree::const_iterator vit = v.begin_item().first_child();
		iregexp_item_tree::iterator it = begin_item().first_child();
		if ((**vit).is_or_syntax())
			vit.move_first_child();
		else
			it = add_item(it);
		if (!vit.is_end())
		{
			add_item(it, vit, v.end_item());
		}
		return *this;
	}
	virtual ~list_switch_regexp() {}
};

template<typename target_iterator, typename target_compare = default_compare_class>
class map_variable_regexp
	: public iregexp_base<target_iterator, target_compare>
{
public:
	enum compare_type
	{
		ctEqual,	// ==
		ctNot,		// !=
		ctLarge,	// >
		ctSmall,	// <
		ctMore,		// >=
		ctLess,		// <=
	};
private:
	class regexp_set_map_variable_item
		: public iregexp_item
	{
	private:
		size_t fvalue;
		target_range fvar_name;
	protected:
		virtual bool _syntax(typename iregexp_item_tree::const_iterator src, const target_iterator vb, target_iterator &vp, const target_iterator ve, bool backtracking)
		{
			set_map(src, fvar_name, fvalue);
			return true;
		}
	public:
		regexp_set_map_variable_item(size_t value, const target_range& var_name)
		{
			fvalue = value;
			fvar_name = var_name;
		}
		virtual ~regexp_set_map_variable_item() {}
		virtual void get_type_string(std::wstring& str) const
		{
			str += L"set_map_variable_p";
			wchar_t istr[33];
			str += L"(";
			_itow_s(fvalue, istr, 10);
			str += istr;
			str += L"=>";
			str.append((wchar_t*)&*fvar_name.first, fvar_name.second - fvar_name.first);
			str += L")";
		}
	};
	class regexp_compare_map_variable_item
		: public iregexp_item
	{
	private:
		compare_type ftype;
		size_t fvalue;
		target_range fvar_name;
	protected:
		virtual bool _syntax(typename iregexp_item_tree::const_iterator src, const target_iterator vb, target_iterator &vp, const target_iterator ve, bool backtracking)
		{
			bool ret;
			switch (ftype)
			{
			case ctEqual:	/* == */	ret = fvalue == get_map(src, fvar_name); break;
			case ctNot:		/* != */	ret = fvalue != get_map(src, fvar_name); break;
			case ctLarge:	/* > */		ret = fvalue > get_map(src, fvar_name); break;
			case ctSmall:	/* < */		ret = fvalue < get_map(src, fvar_name); break;
			case ctMore:	/* >= */	ret = fvalue >= get_map(src, fvar_name); break;
			case ctLess:	/* <= */	ret = fvalue <= get_map(src, fvar_name); break;
			}
			return ret;
		}
	public:
		regexp_compare_map_variable_item(compare_type type, size_t value, const target_range& var_name)
		{
			ftype = type;
			fvalue = value;
			fvar_name = var_name;
		}
		virtual ~regexp_compare_map_variable_item() {}
		virtual void get_type_string(std::wstring& str) const
		{
			str += L"set_map_variable_p";
			wchar_t istr[33];
			str += L"(";
			_itow_s(fvalue, istr, 10);
			str += istr;
			str += L"=>";
			str.append((wchar_t*)&*fvar_name.first, fvar_name.second - fvar_name.first);
			str += L")";
		}
	};
	class regexp_stack_map_variable_item
		: public iregexp_item
	{
	private:
		bool fstack;
	protected:
		virtual bool _syntax(typename iregexp_item_tree::const_iterator src, const target_iterator vb, target_iterator &vp, const target_iterator ve, bool backtracking)
		{
			if (!backtracking)
			{
				if (fstack)
					push_map(src);
				else
					pop_map(src);
			}
			else
			{
				if (fstack)
					pop_map(src);
				else
					push_map(src);
			}
			return true;
		}
	public:
		regexp_stack_map_variable_item(bool stack) : fstack(stack) {}
		virtual ~regexp_stack_map_variable_item() {}
		virtual void get_type_string(std::wstring& str) const
		{
			str += L"set_stack_map_variable_p";
			str += L"(";
			if (fstack)
				str += L"push";
			else
				str += L"pop";
			str += L")";
		}
	};
	class regexp_map_variable_item
		: public iregexp_item
	{
	private:
		enum capture_type
		{
			ctIndex,
			ctName,
			ctDelta,
		};
		capture_type fcapture_type;
		bool fdlt;
		size_t findex;
		target_range fname;
		target_range fvar_name;
		std::vector<std::pair<target_range, size_t>> flist;
		void set_list(const target_range& v)
		{
			target_iterator s = v.first;
			target_iterator c = v.second - 1;
			size_t l = 0;
			for (target_iterator p = s + 1; p != v.second; ++p)
			{
				if (target_equal()(p, c))
				{
					target_range item(s, p);
					if (std::binary_search(flist.begin(), flist.end(), std::pair<target_range, size_t>(item, l), map_compare()))
						std::lower_bound(flist.begin(), flist.end(), std::pair<target_range, size_t>(item, l), map_compare())->second = l;
					else
						flist.insert(std::lower_bound(flist.begin(), flist.end(), std::pair<target_range, size_t>(item, l), map_compare()), std::pair<target_range, size_t>(item, l));
					++p;
					++l;
					s = p;
				}
			}
		}
	protected:
		virtual bool _syntax(typename iregexp_item_tree::const_iterator src, const target_iterator vb, target_iterator &vp, const target_iterator ve, bool backtracking)
		{
			iregexp_item_tree::const_iterator it;
			switch (fcapture_type)
			{
			case ctIndex:
				it = find_capture(src, findex);
				break;
			case ctName:
				it = find_capture(src, fname);
				break;
			case ctDelta:
				it = find_capture(src, findex, fdlt);
				break;
			}
			if ((**it).is_matchs())
			{
				iregexp_item_point pos = (**it).last_matchs();
				target_range item(vb + pos.first, vb + pos.second);
				iregexp_item_tree::const_iterator it = src.first_child();
				if (std::binary_search(flist.begin(), flist.end(), std::pair<target_range, size_t>(item, 0), map_compare()))
				{
					std::vector<std::pair<target_range, size_t>>::const_iterator s = std::lower_bound(flist.begin(), flist.end(), std::pair<target_range, size_t>(item, 0), map_compare());
					set_map(src, fvar_name, s->second);
					return true;
				}
				else
					return false;
			}
			else
				return false;
		}
	public:
		regexp_map_variable_item(size_t index, const target_range& v, const target_range& var_name)
		{
			fcapture_type = ctIndex;
			findex = index;
			fvar_name = var_name;
			set_parallel_match_size();
			set_list(v);
		}
		regexp_map_variable_item(size_t index, bool prev, const target_range& v, const target_range& var_name)
		{
			fcapture_type = ctDelta;
			findex = index;
			fdlt = prev;
			fvar_name = var_name;
			set_parallel_match_size();
			set_list(v);
		}
		regexp_map_variable_item(const target_range& name, const target_range& v, const target_range& var_name)
		{
			fcapture_type = ctName;
			fname = name;
			fvar_name = var_name;
			set_parallel_match_size();
			set_list(v);
		}
		virtual ~regexp_map_variable_item() {}
		virtual void get_type_string(std::wstring& str) const
		{
			str += L"map_variable_p";
			wchar_t istr[33];
			str += L"(";
			switch (fcapture_type)
			{
			case ctDelta:
				str += fdlt ? L"-" : L"+";
			case ctIndex:
				_itow_s(findex, istr, 10);
				str += istr;
				break;
			case ctName:
				str += L"'";
				str.append((wchar_t*)&*fname.first, fname.second - fname.first);
				str += L"'";
				break;
			}
			std::vector<std::pair<target_range, size_t>>::const_iterator i = flist.begin();
			while (i != flist.end())
			{
				str += L"[";
				_itow_s(i->second, istr, 10);
				str += istr;
				str += L",";
				str.append((wchar_t*)&*i->first.first, i->first.second - i->first.first);
				str += L"]";
				++i;
			}
			str += L"=>";
			str.append((wchar_t*)&*fvar_name.first, fvar_name.second - fvar_name.first);
			str += L")";
		}
	};
private:
protected:
public:
	map_variable_regexp(size_t value, const target_range& var_name)
	{
		add_item(begin_item(), new regexp_set_map_variable_item(value, var_name));
	}
	map_variable_regexp(compare_type type, size_t value, const target_range& var_name)
	{
		add_item(begin_item(), new regexp_compare_map_variable_item(type, value, var_name));
	}
	map_variable_regexp(size_t capture_index, const target_range& list, const target_range& var_name)
	{
		add_item(begin_item(), new regexp_map_variable_item(capture_index, list, var_name));
	}
	map_variable_regexp(size_t capture_index, bool prev, const target_range& list, const target_range& var_name)
	{
		add_item(begin_item(), new regexp_map_variable_item(capture_index, prev, list, var_name));
	}
	map_variable_regexp(const target_range& capture_name, const target_range& list, const target_range& var_name)
	{
		add_item(begin_item(), new regexp_map_variable_item(capture_name, list, var_name));
	}
	map_variable_regexp(bool stack_push)
	{
		add_item(begin_item(), new regexp_stack_map_variable_item(stack_push));
	}
	virtual ~map_variable_regexp() {}
};

template<typename target_iterator, typename target_compare = default_compare_class>
class map_switch_regexp
	: public iregexp_base<target_iterator, target_compare>
{
	class regexp_map_switch_item
		: public iregexp_item
	{
	private:
		target_range fname;
		short fswitch_value;
		std::vector<short> fstack;
	protected:
		virtual bool _syntax(typename iregexp_item_tree::const_iterator src, const target_iterator vb, target_iterator &vp, const target_iterator ve, bool backtracking)
		{
			iregexp_item_tree::const_iterator it = src.first_child();
			if (!backtracking)
				fswitch_value = get_map(src, fname);
			bool ret = false;
			if (fswitch_value == -1)
				ret = (**it)(it, vb, vp, ve, backtracking);
			else
			{
				iregexp_item_tree::const_iterator i = it.next_sibling();
				for (int l = 0; l < fswitch_value && !i.is_end(); ++l, i.move_next_sibling());
				if (!i.is_end())
					ret = (**i)(i, vb, vp, ve, backtracking);
				else
					ret = (**it)(it, vb, vp, ve, backtracking);
			}
			return ret;
		}
	public:
		regexp_map_switch_item(const target_range& var_name)
		{
			fname = var_name;
			set_parallel_match_size();
		}
		virtual ~regexp_map_switch_item() {}
		virtual void get_type_string(std::wstring& str) const
		{
			str += L"map_switch_p";
			str += L"('";
			str.append((wchar_t*)&*fname.first, fname.second - fname.first);
			str += L"')";
		}
		virtual void match_stack_reset(void)
		{
			iregexp_item_match_base::match_stack_reset();
			fstack.clear();
		}
		virtual void match_stack_push(void)
		{
			iregexp_item_match_base::match_stack_push();
			fstack.push_back(fswitch_value);
		}
		virtual void match_stack_pop()
		{
			iregexp_item_match_base::match_stack_pop();
			fswitch_value = fstack.back();
			fstack.pop_back();
		}
	};
private:
protected:
public:
	map_switch_regexp(const target_range& var_name)
	{
		iregexp_item_tree::iterator it = add_item(begin_item(), new regexp_map_switch_item(var_name));
		it = add_item(it);
	}
	map_switch_regexp& operator [] (const iregexp_base& v)
	{
		iregexp_item_tree::iterator it = add_item(begin_item().first_child());
		add_item(it, v.begin_item().first_child(), v.end_item());
		return *this;
	}
	iregexp_base& default_p(const iregexp_base& v)
	{
		iregexp_item_tree::iterator it = begin_item().first_child().first_child();
		add_item(it, v.begin_item().first_child(), v.end_item());
		return *this;
	}
	iregexp_base& set_items(const iregexp_base& v)
	{
		iregexp_item_tree::const_iterator vit = v.begin_item().first_child();
		iregexp_item_tree::iterator it = begin_item().first_child();
		if ((**vit).is_or_syntax())
			vit.move_first_child();
		else
			it = add_item(it);
		if (!vit.is_end())
		{
			add_item(it, vit, v.end_item());
		}
		return *this;
	}
	virtual ~map_switch_regexp() {}
};

template<typename target_iterator, typename target_compare = default_compare_class>
class list_select_regexp
	: public iregexp_base<target_iterator, target_compare>
{
	class regexp_list_select_item
		: public iregexp_item
	{
	private:
		long fparam;
		iregexp_callback<target_iterator> *fcallback;
		enum capture_type
		{
			ctIndex,
			ctName,
			ctDelta,
		};
		capture_type fcapture_type;
		bool fdlt;
		size_t findex;
		target_range fname;
		std::vector<std::pair<target_range, size_t>> flist;
		void set_list(const target_range& v)
		{
			target_iterator s = v.first;
			target_iterator c = v.second - 1;
			size_t l = 0;
			for (target_iterator p = s + 1; p != v.second; ++p)
			{
				if (target_equal()(p, c))
				{
					target_range item(s, p);
					if (std::binary_search(flist.begin(), flist.end(), std::pair<target_range, size_t>(item, l), map_compare()))
						std::lower_bound(flist.begin(), flist.end(), std::pair<target_range, size_t>(item, l), map_compare())->second = l;
					else
						flist.insert(std::lower_bound(flist.begin(), flist.end(), std::pair<target_range, size_t>(item, l), map_compare()), std::pair<target_range, size_t>(item, l));
					++p;
					++l;
					s = p;
				}
			}
		}
	protected:
		virtual bool _syntax(typename iregexp_item_tree::const_iterator src, const target_iterator vb, target_iterator &vp, const target_iterator ve, bool backtracking)
		{
			iregexp_item_tree::const_iterator it;
			switch (fcapture_type)
			{
			case ctIndex:
				it = find_capture(src, findex);
				break;
			case ctName:
				it = find_capture(src, fname);
				break;
			case ctDelta:
				it = find_capture(src, findex, fdlt);
				break;
			default:
				it = src.end();
			}
			if (!it.is_end() && (**it).is_matchs())
			{
				iregexp_item_point pos = (**it).last_matchs();
				target_range item(vb + pos.first, vb + pos.second);
				if (std::binary_search(flist.begin(), flist.end(), std::pair<target_range, size_t>(item, 0), map_compare()))
				{
					std::vector<std::pair<target_range, size_t>>::const_iterator s = std::lower_bound(flist.begin(), flist.end(), std::pair<target_range, size_t>(item, 0), map_compare());
					fcallback->OnSelect(fparam, s->second, vb, vp, ve);
					return true;
				}
				else
					return false;
			}
			else
				return false;
		}
	public:
		regexp_list_select_item(size_t capture_index, const target_range& list, long param, iregexp_callback<target_iterator>* callback)
			: fparam(param), fcallback(callback)
		{
			set_list(list);
			fcapture_type = ctIndex;
			findex = capture_index;
		}
		regexp_list_select_item(size_t capture_index, bool prev, const target_range& list, long param, iregexp_callback<target_iterator>* callback)
			: fparam(param), fcallback(callback)
		{
			set_list(list);
			fcapture_type = ctDelta;
			findex = capture_index;
			fdlt = prev;
		}
		regexp_list_select_item(const target_range& name, const target_range& list, long param, iregexp_callback<target_iterator>* callback)
			: fparam(param), fcallback(callback)
		{
			set_list(list);
			fcapture_type = ctName;
			fname = name;
		}
		virtual ~regexp_list_select_item() {}
		virtual void get_type_string(std::wstring& str) const
		{
			str += L"list_select_p";
			wchar_t istr[33];
			str += L"(";
			switch (fcapture_type)
			{
			case ctDelta:
				str += fdlt ? L"-" : L"+";
			case ctIndex:
				_itow_s(findex, istr, 10);
				str += istr;
				break;
			case ctName:
				str += L"'";
				str.append((wchar_t*)&*fname.first, fname.second - fname.first);
				str += L"'";
				break;
			}
			std::vector<std::pair<target_range, size_t>>::const_iterator i = flist.begin();
			while (i != flist.end())
			{
				str += L"[";
				_itow_s(i->second, istr, 10);
				str += istr;
				str += L",";
				str.append((wchar_t*)&*i->first.first, i->first.second - i->first.first);
				str += L"]";
				++i;
			}
			str += L"=>";
			_itow_s(fparam, istr, 10);
			str += istr;
			str += L")";
		}
	};
private:
protected:
public:
	list_select_regexp(size_t capture_index, const target_range& list, long param, iregexp_callback<target_iterator>* callback)
	{
		add_item(begin_item(), new regexp_list_select_item(capture_index, list, param, callback));
	}
	list_select_regexp(size_t capture_index, bool prev, const target_range& list, long param, iregexp_callback<target_iterator>* callback)
	{
		add_item(begin_item(), new regexp_list_select_item(capture_index, prev, list, param, callback));
	}
	list_select_regexp(const target_range& name, const target_range& list, long param, iregexp_callback<target_iterator>* callback)
	{
		add_item(begin_item(), new regexp_list_select_item(name, list, param, callback));
	}
	virtual ~list_select_regexp() {}
};

template<typename target_iterator, typename target_compare = default_compare_class>
class map_select_regexp
	: public iregexp_base<target_iterator, target_compare>
{
	class regexp_map_select_item
		: public iregexp_item
	{
	private:
		target_range fvar_name;
		short fselect_value;
		std::vector<short> fstack;
	protected:
		virtual bool _syntax(typename iregexp_item_tree::const_iterator src, const target_iterator vb, target_iterator &vp, const target_iterator ve, bool backtracking)
		{
			bool ret = false;
			int value;
			if (!backtracking)
			{
				iregexp_item_tree::const_iterator it = src.first_child();
				for (value = 0; !it.is_end(); ++value)
				{
					target_iterator v = vp;
					ret = (**it)(it, vb, v, ve, false);
					if (ret)
					{
						vp = v;
						break;
					}
					it.move_next_sibling();
				}
				if (it.is_end())
					value = -1;
			}
			else
			{
				iregexp_item_tree::const_iterator it = src.first_child();
				value = fselect_value;
				if (value != -1)
				{
					for (int n = 0; !it.is_end(); ++n)
					{
						if (n == value)
						{
							ret = (**it)(it, vb, vp, ve, true);
							break;
						}
						it.move_next_sibling();
					}
				}
			}
			fselect_value = value;
			set_map(src, fvar_name, value);
			return ret;
		}
	public:
		regexp_map_select_item(const target_range& name)
			: fvar_name(name)
		{
			set_parallel_match_size();
		}
		virtual ~regexp_map_select_item() {}
		virtual void get_type_string(std::wstring& str) const
		{
			str += L"map_select_p(";
			str.append((wchar_t*)&*fvar_name.first, fvar_name.second - fvar_name.first);
			str += L")";
		}
		virtual void match_stack_reset(void)
		{
			iregexp_item_match_base::match_stack_reset();
			fstack.clear();
		}
		virtual void match_stack_push(void)
		{
			iregexp_item_match_base::match_stack_push();
			fstack.push_back(fselect_value);
		}
		virtual void match_stack_pop()
		{
			iregexp_item_match_base::match_stack_pop();
			fselect_value = fstack.back();
			fstack.pop_back();
		}
	};
private:
protected:
public:
	map_select_regexp(const target_range& name)
	{
		add_item(begin_item(), new regexp_map_select_item(name));
	}
	map_select_regexp& operator [] (const iregexp_base& v)
	{
		iregexp_item_tree::iterator it = add_item(begin_item().first_child());
		add_item(it, v.begin_item().first_child(), v.end_item());
		return *this;
	}
	iregexp_base& set_items(const iregexp_base& v)
	{
		iregexp_item_tree::const_iterator vit = v.begin_item().first_child();
		iregexp_item_tree::iterator it = begin_item().first_child();
		if ((**vit).is_or_syntax())
			vit.move_first_child();
		else
			it = add_item(it);
		if (!vit.is_end())
		{
			add_item(it, vit, v.end_item());
		}
		return *this;
	}
	virtual ~map_select_regexp() {}
};

template<typename target_iterator, typename target_compare = default_compare_class>
class select_regexp
	: public iregexp_base<target_iterator, target_compare>
{
	class regexp_select_item
		: public iregexp_item
	{
	private:
		bool fcall_default;
		long fparam;
		iregexp_callback<target_iterator> *fcallback;
		short fselect_value;
		std::vector<short> fstack;
	protected:
		virtual bool _syntax(typename iregexp_item_tree::const_iterator src, const target_iterator vb, target_iterator &vp, const target_iterator ve, bool backtracking)
		{
			bool ret = false;
			int value;
			if (!backtracking)
			{
				iregexp_item_tree::const_iterator it = src.first_child();
				for (value = 0; !it.is_end(); ++value)
				{
					target_iterator v = vp;
					ret = (**it)(it, vb, v, ve, false);
					if (ret)
					{
						fcallback->OnSelect(fparam, value, vb, vp, v);
						vp = v;
						break;
					}
					it.move_next_sibling();
				}
				if (it.is_end())
				{
					value = -1;
					if (fcall_default)
						fcallback->OnSelect(fparam, fselect_value, vb, vp, ve);
				}
			}
			else
			{
				iregexp_item_tree::const_iterator it = src.first_child();
				value = fselect_value;
				if (value != -1)
				{
					for (int n = 0; !it.is_end(); ++n)
					{
						if (n == value)
						{
							ret = (**it)(it, vb, vp, ve, true);
							break;
						}
						it.move_next_sibling();
					}
				}
			}
			fselect_value = value;
			return ret;
		}
	public:
		regexp_select_item(long param, iregexp_callback<target_iterator>* callback, bool call_default)
			: fcall_default(call_default), fparam(param), fcallback(callback)
		{
			set_parallel_match_size();
		}
		virtual ~regexp_select_item() {}
		virtual void get_type_string(std::wstring& str) const
		{
			str += L"select_p";
			wchar_t istr[33];
			str += L"(";
			_itow_s(fparam, istr, 10);
			str += istr;
			str += L")";
		}
		virtual void match_stack_reset(void)
		{
			iregexp_item_match_base::match_stack_reset();
			fstack.clear();
		}
		virtual void match_stack_push(void)
		{
			iregexp_item_match_base::match_stack_push();
			fstack.push_back(fselect_value);
		}
		virtual void match_stack_pop()
		{
			iregexp_item_match_base::match_stack_pop();
			fselect_value = fstack.back();
			fstack.pop_back();
		}
	};
private:
protected:
public:
	select_regexp(long param, iregexp_callback<target_iterator>* callback, bool call_default = false)
	{
		add_item(begin_item(), new regexp_select_item(param, callback, call_default));
	}
	select_regexp& operator [] (const iregexp_base& v)
	{
		iregexp_item_tree::iterator it = add_item(begin_item().first_child());
		add_item(it, v.begin_item().first_child(), v.end_item());
		return *this;
	}
	iregexp_base& set_items(const iregexp_base& v)
	{
		iregexp_item_tree::const_iterator vit = v.begin_item().first_child();
		iregexp_item_tree::iterator it = begin_item().first_child();
		if ((**vit).is_or_syntax())
			vit.move_first_child();
		else
			it = add_item(it);
		if (!vit.is_end())
		{
			add_item(it, vit, v.end_item());
		}
		return *this;
	}
	virtual ~select_regexp() {}
};

template<typename target_iterator, typename target_compare = default_compare_class>
class func_regexp
	: public iregexp_base<target_iterator, target_compare>
{
	class regexp_func_item
		: public iregexp_item
	{
	private:
		long fparam;
		iregexp_callback<target_iterator> *fcallback;
	protected:
		virtual bool can_backtrack(void) { return false; }
		virtual bool _syntax(typename iregexp_item_tree::const_iterator src, const target_iterator vb, target_iterator &vp, const target_iterator ve, bool backtracking)
		{
			fcallback->OnFunc(fparam, vb, vp, ve);
			return true;
		}
	public:
		regexp_func_item(long param, iregexp_callback<target_iterator>* callback)
			: fparam(param), fcallback(callback) {}
		virtual ~regexp_func_item() {}
		virtual void get_type_string(std::wstring& str) const
		{
			str += L"func_p";
			wchar_t istr[33];
			str += L"(";
			_itow_s(fparam, istr, 10);
			str += istr;
			str += L")";
		}
	};
private:
protected:
public:
	func_regexp(long param, iregexp_callback<target_iterator>* callback)
	{
		add_item(begin_item(), new regexp_func_item(param, callback));
	}
	virtual ~func_regexp() {}
};

template<typename target_iterator, typename target_compare = default_compare_class>
class query_regexp
	: public iregexp_base<target_iterator, target_compare>
{
	class regexp_query_item
		: public iregexp_item
	{
	private:
		long fparam;
		iregexp_callback<target_iterator> *fcallback;
	protected:
		virtual bool can_backtrack(void) { return false; }
		virtual bool _syntax(typename iregexp_item_tree::const_iterator src, const target_iterator vb, target_iterator &vp, const target_iterator ve, bool backtracking)
		{
			bool ret;
			ret = fcallback->OnQuery(fparam, vb, vp, ve);
			return ret;
		}
	public:
		regexp_query_item(long param, iregexp_callback<target_iterator>* callback)
			: fparam(param), fcallback(callback) {}
		virtual ~regexp_query_item() {}
		virtual void get_type_string(std::wstring& str) const
		{
			str += L"query_p";
			wchar_t istr[33];
			str += L"(";
			_itow_s(fparam, istr, 10);
			str += istr;
			str += L")";
		}
	};
private:
protected:
public:
	query_regexp(long param, iregexp_callback<target_iterator>* callback)
	{
		add_item(begin_item(), new regexp_query_item(param, callback));
	}
	virtual ~query_regexp() {}
};

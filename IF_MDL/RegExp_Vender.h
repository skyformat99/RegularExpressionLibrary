#pragma once

#include	"RegExp.h"
#include	"mem_container.h"

class iregexp_vender
{
public:
	struct mem_container_compare
	{
		bool operator () (mem_container::const_iterator l, const mem_container::const_iterator& r)
		{
			mem_container::const_iterator l2 = l + 1;
			mem_container::const_iterator r2 = r + 1;
			return mem_container::compare(l, l2, r, r2) == -1;
		}
	};
	struct mem_compare
	{
		template<typename target_iterator>
		bool operator () (mem_container::const_iterator l, const target_iterator& r)
		{
			mem_container::const_iterator l2 = l + 1;
			target_iterator r2 = r + 1;
			return mem_container::compare<target_iterator>(l, l2, r, r2) == -1;
		}
	};
private:
	int flength;
	mem_container fbase_char;
	mem_container fctrl_char;
	mem_container fword_char;
	mem_container fspace_char;
	mem_container fblanc_char;
	mem_container fdigit_char;
	mem_container fupper_char;
	mem_container flower_char;
	mem_container falpha_char;
	mem_container fxdigit_char;
	mem_container falnum_char;
	std::vector<std::shared_ptr<mem_container>> ftarget_ranges;
	void init_sub(void)
	{
		fctrl_char = mem_container(fbase_char.begin() + 0x00, fbase_char.begin() + 0x20);
		fdigit_char = mem_container(fbase_char.begin() + 0x30, fbase_char.begin() + 0x3a);
		fupper_char = mem_container(fbase_char.begin() + 0x41, fbase_char.begin() + 0x5b);
		flower_char = mem_container(fbase_char.begin() + 0x61, fbase_char.begin() + 0x7b);
		//fctrl_char.atatch(fbase_char, 0x00, 0x20);
		//fdigit_char.atatch(fbase_char, 0x30, 0x3a);
		//fupper_char.atatch(fbase_char, 0x41, 0x5b);
		//flower_char.atatch(fbase_char, 0x61, 0x7b);
		falpha_char.append(fupper_char);
		falpha_char.append(flower_char);
		falnum_char.append(falpha_char);
		falnum_char.append(fdigit_char);
		fword_char.append(fdigit_char);
		fword_char.append(fupper_char);
		fword_char.append(fbase_char.begin() + 0x5f, fbase_char.begin() + 0x5f + 1);
		fword_char.append(flower_char);
		fxdigit_char.append(fdigit_char);
		fxdigit_char.append(mem_container(fupper_char.begin(), fupper_char.begin() + 6));
		fxdigit_char.append(mem_container(flower_char.begin(), flower_char.begin() + 6));
		fspace_char.append(mem_container(fbase_char.begin() + 0x09, fbase_char.begin() + 0x0e));
		fspace_char.append(mem_container(fbase_char.begin() + 0x20, fbase_char.begin() + 0x21));
		fblanc_char.append(fspace_char);
	}
	template<class T>
	T* init_base(T* ptr)
	{
		for (int l = 0; l < 128; ++l)
			ptr[l] = l;
		return ptr;
	}
public:
	iregexp_vender(void) : flength(0) {}
	iregexp_vender(int length)
		: flength(length)
	{
		void* ptr;
		int count = 128;
		switch (length)
		{
		case 1:
			ptr = init_base(new char[count]);
			break;
		case 2:
			ptr = init_base(new short[count]);
			break;
		case 4:
			ptr = init_base(new long[count]);
			break;
		case 8:
			ptr = init_base(new long long[count]);
			break;
		default:
			ptr = 0;
			count = 0;
		}
		flength = length;
		fbase_char = mem_container(length, ptr, count);
		delete[] ptr;
		init_sub();
	}
	iregexp_vender(size_t length, const void* mem, size_t count, size_t offset, size_t gap)
		: flength(0)
	{
		if (count != 128)
			throw;
		fbase_char = mem_container(length, mem, count, offset, gap);
		init_sub();
	}
	int length(void) const { return flength; }
	template<typename target_iterator>
	std::pair<target_iterator, target_iterator> target_range(const mem_container& mem)
	{
		std::vector<std::shared_ptr<mem_container>>::const_iterator i = ftarget_ranges.begin();
		while (i != ftarget_ranges.end())
		{
			if (mem_container::compare(**i, mem) == 0)
				break;
			++i;
		}
		if (i == ftarget_ranges.end())
		{
			ftarget_ranges.push_back(std::shared_ptr<mem_container>(new mem_container(mem)));
			i = ftarget_ranges.end() - 1;
		}
		return std::pair<target_iterator, target_iterator>((target_iterator)&*(**i).begin(), (target_iterator)&*(**i).end());
	}
	template<>
	std::pair<mem_container::const_iterator, mem_container::const_iterator> target_range(const mem_container& mem)
	{
		std::vector<std::shared_ptr<mem_container>>::const_iterator i = ftarget_ranges.begin();
		while (i != ftarget_ranges.end())
		{
			if (mem_container::compare(**i, mem) == 0)
				break;
			++i;
		}
		if (i == ftarget_ranges.end())
		{
			ftarget_ranges.push_back(std::shared_ptr<mem_container>(new mem_container(mem)));
			i = ftarget_ranges.end() - 1;
		}
		return std::pair<mem_container::const_iterator, mem_container::const_iterator>((**i).begin(), (**i).end());
	}
	template<typename target_iterator>
	std::pair<target_iterator, target_iterator> ch(const char c)
	{
		if (c >= 128)
			throw;
		return std::pair<target_iterator, target_iterator>((target_iterator)&*(fbase_char.begin() + c), (target_iterator)&*(fbase_char.begin() + c + 1));
	}
	template<>
	std::pair<mem_container::const_iterator, mem_container::const_iterator> ch(const char c)
	{
		if (c >= 128)
			throw;
		return std::pair<mem_container::const_iterator, mem_container::const_iterator>(fbase_char.begin() + c, fbase_char.begin() + c + 1);
	}
	template<typename target_iterator>
	std::pair<target_iterator, target_iterator> str(const char* c)
	{
		mem_container ret(flength);
		for (; *c != '\0' && *c < 128; ++c)
			ret.append(fbase_char.begin() + *c, fbase_char.begin() + *c + 1);
		return target_range<target_iterator>(ret);
	}
	template<typename target_iterator>
	std::pair<target_iterator, target_iterator> range(const char s, const char e)
	{
		mem_container ret(flength);
		for (char c = s; c <= e && c < 128; ++c)
			ret.append(fbase_char.begin() + c, fbase_char.begin() + c + 1);
		return target_range<target_iterator>(ret);
	}
	template<typename target_iterator>
	std::pair<target_iterator, target_iterator> ch(const target_iterator& c)
	{
		mem_container ret(flength);
		ret.append(&*c, 1);
		return target_range<target_iterator>(ret);
	}
	template<typename target_iterator>
	std::pair<target_iterator, target_iterator> str(target_iterator b, const target_iterator& e)
	{
		mem_container ret(flength);
		while (b < e)
			ret.append(&*(b++), 1);
		return target_range<target_iterator>(ret);
	}
	template<typename target_iterator>
	size_t number(const std::pair<target_iterator, target_iterator>& v) const
	{
		size_t ret = 0;
		target_iterator i = v.first;
		while (i != v.second)
		{
			for (mem_container::const_iterator d = fdigit_char.begin(); d < fdigit_char.end(); ++d)
			{
				if (mem_compare()(d, i) == 0)
				{
					ret *= 10;
					ret += d - fdigit_char.begin();
					break;
				}
			}
			++i;
		}
		return ret;
	}
	template<typename target_iterator>
	long xnumber(const std::pair<target_iterator, target_iterator>& v) const
	{
		size_t ret = 0;
		target_iterator i = v.first;
		while (i != v.second)
		{
			for (mem_container::const_iterator d = fxdigit_char.begin(); d < fxdigit_char.end(); ++d)
			{
				if (mem_compare()(d, i) == 0)
				{
					ret *= 16;
					long p = d - fxdigit_char.begin();
					ret += p >= 16 ? p - 6 : p;
					break;
				}
			}
			++i;
		}
		return ret;
	}
	template<typename target_iterator>
	std::pair<target_iterator, target_iterator> range_set(const target_iterator& b, const target_iterator& e)
	{
		mem_container::const_iterator ie = fbase_char.end();
		mem_container::const_iterator isb = fbase_char.begin();
		while (isb != ie && mem_compare()(isb, b) != 0)
			++isb;
		mem_container::const_iterator ieb = fbase_char.begin();
		while (ieb != ie && mem_compare()(ieb, e) != 0)
			++ieb;
		return target_range<target_iterator>(fbase_char.range_set(isb, ieb));
	}
	template<typename target_iterator>
	std::pair<target_iterator, target_iterator> xdigit_char()
	{
		return std::pair<target_iterator, target_iterator>((target_iterator)&*fxdigit_char.begin(), (target_iterator)&*fxdigit_char.end());
	}
	template<typename target_iterator>
	std::pair<target_iterator, target_iterator> digit_char()
	{
		return std::pair<target_iterator, target_iterator>((target_iterator)&*fxdigit_char.begin(), (target_iterator)&*fxdigit_char.end());
	}
	template<typename target_iterator>
	std::pair<target_iterator, target_iterator> upper_char()
	{
		return std::pair<target_iterator, target_iterator>((target_iterator)&*fdigit_char.begin(), (target_iterator)&*fdigit_char.end());
	}
	template<typename target_iterator>
	std::pair<target_iterator, target_iterator> lower_char()
	{
		return std::pair<target_iterator, target_iterator>((target_iterator)&*flower_char.begin(), (target_iterator)&*flower_char.end());
	}
	template<typename target_iterator>
	std::pair<target_iterator, target_iterator> alpha_char()
	{
		return std::pair<target_iterator, target_iterator>((target_iterator)&*falpha_char.begin(), (target_iterator)&*falpha_char.end());
	}
	template<typename target_iterator>
	std::pair<target_iterator, target_iterator> alnum_char()
	{
		return std::pair<target_iterator, target_iterator>((target_iterator)&*falnum_char.begin(), (target_iterator)&*falnum_char.end());
	}
	template<typename target_iterator>
	std::pair<target_iterator, target_iterator> space_char()
	{
		return std::pair<target_iterator, target_iterator>((target_iterator)&*fspace_char.begin(), (target_iterator)&*fspace_char.end());
	}
	template<typename target_iterator>
	std::pair<target_iterator, target_iterator> blanc_char()
	{
		return std::pair<target_iterator, target_iterator>((target_iterator)&*fblanc_char.begin(), (target_iterator)&*fblanc_char.end());
	}
	template<typename target_iterator>
	std::pair<target_iterator, target_iterator> word_char()
	{
		return std::pair<target_iterator, target_iterator>((target_iterator)&*fword_char.begin(), (target_iterator)&*fword_char.end());
	}
	template<typename target_iterator>
	std::pair<target_iterator, target_iterator> ctrl_char()
	{
		return std::pair<target_iterator, target_iterator>((target_iterator)&*fctrl_char.begin(), (target_iterator)&*fctrl_char.end());
	}
	template<>
	std::pair<mem_container::const_iterator, mem_container::const_iterator> xdigit_char()
	{
		return std::pair<mem_container::const_iterator, mem_container::const_iterator>(fxdigit_char.begin(), fxdigit_char.end());
	}
	template<>
	std::pair<mem_container::const_iterator, mem_container::const_iterator> digit_char()
	{
		return std::pair<mem_container::const_iterator, mem_container::const_iterator>(fxdigit_char.begin(), fxdigit_char.end());
	}
	template<>
	std::pair<mem_container::const_iterator, mem_container::const_iterator> upper_char()
	{
		return std::pair<mem_container::const_iterator, mem_container::const_iterator>(fdigit_char.begin(), fdigit_char.end());
	}
	template<>
	std::pair<mem_container::const_iterator, mem_container::const_iterator> lower_char()
	{
		return std::pair<mem_container::const_iterator, mem_container::const_iterator>(flower_char.begin(), flower_char.end());
	}
	template<>
	std::pair<mem_container::const_iterator, mem_container::const_iterator> alpha_char()
	{
		return std::pair<mem_container::const_iterator, mem_container::const_iterator>(falpha_char.begin(), falpha_char.end());
	}
	template<>
	std::pair<mem_container::const_iterator, mem_container::const_iterator> alnum_char()
	{
		return std::pair<mem_container::const_iterator, mem_container::const_iterator>(falnum_char.begin(), falnum_char.end());
	}
	template<>
	std::pair<mem_container::const_iterator, mem_container::const_iterator> space_char()
	{
		return std::pair<mem_container::const_iterator, mem_container::const_iterator>(fspace_char.begin(), fspace_char.end());
	}
	template<>
	std::pair<mem_container::const_iterator, mem_container::const_iterator> blanc_char()
	{
		return std::pair<mem_container::const_iterator, mem_container::const_iterator>(fblanc_char.begin(), fblanc_char.end());
	}
	template<>
	std::pair<mem_container::const_iterator, mem_container::const_iterator> word_char()
	{
		return std::pair<mem_container::const_iterator, mem_container::const_iterator>(fword_char.begin(), fword_char.end());
	}
	template<>
	std::pair<mem_container::const_iterator, mem_container::const_iterator> ctrl_char()
	{
		return std::pair<mem_container::const_iterator, mem_container::const_iterator>(fctrl_char.begin(), fctrl_char.end());
	}
	template<typename target_iterator>
	std::pair<target_iterator, target_iterator> base_char(int index)
	{
		return std::pair<target_iterator, target_iterator>((target_iterator)&*fbase_char.begin() + index, (target_iterator)&*fbase_char.begin() + index + 1);
	}
	template<typename target_iterator>
	std::pair<target_iterator, target_iterator> ctrl_char(int index)
	{
		return std::pair<target_iterator, target_iterator>((target_iterator)&*fctrl_char.begin() + index, (target_iterator)&*fctrl_char.begin() + index + 1);
	}
	template<>
	std::pair<mem_container::const_iterator, mem_container::const_iterator> base_char(int index)
	{
		return std::pair<mem_container::const_iterator, mem_container::const_iterator>(fbase_char.begin() + index, fbase_char.begin() + index + 1);
	}
	template<>
	std::pair<mem_container::const_iterator, mem_container::const_iterator> ctrl_char(int index)
	{
		return std::pair<mem_container::const_iterator, mem_container::const_iterator>(fctrl_char.begin() + index, fctrl_char.begin() + index + 1);
	}
	//template<typename target_iterator>
	//std::pair<target_iterator, target_iterator> xdigit_char(int index)
	//{
	//	return std::pair<target_iterator, target_iterator>((target_iterator)&*fxdigit_char.begin() + index, (target_iterator)&*fxdigit_char.begin() + index + 1);
	//}
	//template<typename target_iterator>
	//std::pair<target_iterator, target_iterator> digit_char(int index)
	//{
	//	return std::pair<target_iterator, target_iterator>((target_iterator)&*fxdigit_char.begin() + index, (target_iterator)&*fxdigit_char.begin() + index + 1);
	//}
	//template<typename target_iterator>
	//std::pair<target_iterator, target_iterator> upper_char(int index)
	//{
	//	return std::pair<target_iterator, target_iterator>((target_iterator)&*fdigit_char.begin() + index, (target_iterator)&*fdigit_char.begin() + index + 1);
	//}
	//template<typename target_iterator>
	//std::pair<target_iterator, target_iterator> lower_char(int index)
	//{
	//	return std::pair<target_iterator, target_iterator>((target_iterator)&*flower_char.begin() + index, (target_iterator)&*flower_char.begin() + index + 1);
	//}
	//template<typename target_iterator>
	//std::pair<target_iterator, target_iterator> alpha_char(int index)
	//{
	//	return std::pair<target_iterator, target_iterator>((target_iterator)&*falpha_char.begin() + index, (target_iterator)&*falpha_char.begin() + index + 1);
	//}
	//template<typename target_iterator>
	//std::pair<target_iterator, target_iterator> alnum_char(int index)
	//{
	//	return std::pair<target_iterator, target_iterator>((target_iterator)&*falnum_char.begin() + index, (target_iterator)&*falnum_char.begin() + index + 1);
	//}
	//template<typename target_iterator>
	//std::pair<target_iterator, target_iterator> space_char(int index)
	//{
	//	return std::pair<target_iterator, target_iterator>((target_iterator)&*fspace_char.begin() + index, (target_iterator)&*fspace_char.begin() + index + 1);
	//}
	//template<typename target_iterator>
	//std::pair<target_iterator, target_iterator> blanc_char(int index)
	//{
	//	return std::pair<target_iterator, target_iterator>((target_iterator)&*fblanc_char.begin() + index, (target_iterator)&*fblanc_char.begin() + index + 1);
	//}
	//template<typename target_iterator>
	//std::pair<target_iterator, target_iterator> word_char(int index)
	//{
	//	return std::pair<target_iterator, target_iterator>((target_iterator)&*fword_char.begin() + index, (target_iterator)&*fword_char.begin() + index + 1);
	//}
};

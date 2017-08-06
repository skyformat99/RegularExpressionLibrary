#pragma once

#include	<iterator>

class mem_content
{
private:
	mem_content() {}
	mem_content(const mem_content& v) { *this = v;  }
	mem_content& operator = (const mem_content& v) { return const_cast<mem_content&>(*this); }
	bool operator == (const mem_content& v) const { return true; }
	bool operator != (const mem_content& v) const { return false; }
public:
};

class mem_container
{
public:
	class iterator;
	class const_iterator
		: public std::iterator<std::random_access_iterator_tag, mem_content, int>
	{
		friend class mem_container;
	private:
		const mem_container* fmem_container;
		size_t findex;
	protected:
		mem_container& container(void) { return const_cast<mem_container&>(*fmem_container); };
		size_t index(void) const { return findex; }
		void index(size_t v) { findex = v; }
		void add(size_t v)
		{
			findex += v;
			if (findex > container().size())
				findex = container().size();
		}
		void sub(size_t v)
		{
			if (v > findex)
				v = findex;
			findex -= v;
		}
	public:
		const_iterator() : fmem_container(nullptr) { index(0); }
		const_iterator(const mem_container& mem_container) : fmem_container(&mem_container) { index(0); }
		const_iterator(const mem_container& mem_container, size_t v) : fmem_container(&mem_container) { index(v); }
		const_iterator(const const_iterator& it) : fmem_container(it.fmem_container) { index(it.findex); }
		mem_container& container(void) const { return const_cast<mem_container&>(*fmem_container); };
		mem_content* content(void) const { return fmem_container->content(index()); }
		mem_content* content(void) { return fmem_container->content(index()); }
		mem_content& operator * () const { return *content(); }
		mem_content& operator * () { return *content(); }
		//mem_content& operator [] (size_t v) const { return fmem_container->content(v); }
		//mem_content& operator [] (size_t v) { return fmem_container->content(v); }
		mem_content* operator -> () const { return content(); }
		mem_content* operator -> () { return content(); }
		const_iterator& operator ++ () { add(1); return *this; }
		const_iterator& operator -- () { sub(1); return *this; }
		const_iterator operator++ (int) { const_iterator ret = *this; add(1); return ret; }
		const_iterator operator -- (int) { const_iterator ret = *this; sub(1); return ret; }
		const_iterator& operator += (int v) { add(v); return *this; }
		const_iterator& operator -= (int v) { sub(v); return *this; }
		const_iterator operator + (int v) const { const_iterator ret = *this; ret.add(v); return ret; }
		const_iterator operator - (int v) const { const_iterator ret = *this; ret.sub(v); return ret; }
		const_iterator operator + (const const_iterator& v) { throw; }
		const_iterator& operator = (const const_iterator& v) { fmem_container = v.fmem_container; index(v.index()); return *this; }
		size_t operator - (const const_iterator& v) const { return &container() != &v.container() ? throw : index() - v.index(); }
		bool operator < (const const_iterator& v) const { return &container() != &v.container() ? throw : index() < v.index(); }
		bool operator > (const const_iterator& v) const { return &container() != &v.container() ? throw : index() > v.index(); }
		bool operator <= (const const_iterator& v) const { return &container() != &v.container() ? throw : index() <= v.index(); }
		bool operator >= (const const_iterator& v) const { return &container() != &v.container() ? throw : index() >= v.index(); }
		bool operator == (const const_iterator& v) const { return &container() != &v.container() ? throw : index() == v.index(); }
		bool operator != (const const_iterator& v) const { return &container() != &v.container() ? throw : index() != v.index(); }
	};
	class iterator
		: public const_iterator
	{
	public:
		iterator(const mem_container& mem_container) : const_iterator(mem_container) {}
		iterator(const mem_container& mem_container, size_t index) : const_iterator(mem_container, index) {}
		iterator(const iterator& it) : const_iterator(it.container(), it.index()) {}
		iterator(const const_iterator& it) : const_iterator(it.container(), it.index()) {}
		iterator& operator ++ () { add(1); return *this; }
		iterator& operator -- () { sub(1); return *this; }
		iterator operator++ (int) { iterator ret = *this; add(1); return ret; }
		iterator operator -- (int) { iterator ret = *this; sub(1); return ret; }
		iterator& operator += (int v) { add(v); return *this; }
		iterator& operator -= (int v) { sub(v); return *this; }
		iterator operator + (int v) const { iterator ret = *this; ret.add(v); return ret; }
		iterator operator - (int v) const { iterator ret = *this; ret.sub(v); return ret; }
		iterator operator + (const const_iterator& v) { throw; }
		iterator& operator = (const iterator& v) { &container() != &v.container() ? throw : index(v.index()); return *this; }
		iterator& operator = (const const_iterator& v) { &container() != &v.container() ? throw : index(v.index()); return *this; }
		size_t operator - (const const_iterator& v) { return &container() != &v.container() ? throw : index() - v.index(); }
		size_t operator - (const iterator& v) { return &container() != &v.container() ? throw : index() - v.index(); }
		bool operator < (const const_iterator& v) const { return &container() != &v.container() ? throw : index() < v.index(); }
		bool operator > (const const_iterator& v) const { return &container() != &v.container() ? throw : index() > v.index(); }
		bool operator <= (const const_iterator& v) const { return &container() != &v.container() ? throw : index() <= v.index(); }
		bool operator >= (const const_iterator& v) const { return &container() != &v.container() ? throw : index() >= v.index(); }
		bool operator == (const const_iterator& v) const { return &container() != &v.container() ? throw : index() == v.index(); }
		bool operator != (const const_iterator& v) const { return &container() != &v.container() ? throw : index() != v.index(); }
		iterator operator + (const iterator& v) { throw; }
		bool operator < (const iterator& v) const { return &container() != &v.container() ? throw : index() < v.index(); }
		bool operator > (const iterator& v) const { return &container() != &v.container() ? throw : index() > v.index(); }
		bool operator <= (const iterator& v) const { return &container() != &v.container() ? throw : index() <= v.index(); }
		bool operator >= (const iterator& v) const { return &container() != &v.container() ? throw : index() >= v.index(); }
		bool operator == (const iterator& v) const { return &container() != &v.container() ? throw : index() == v.index(); }
		bool operator != (const iterator& v) const { return &container() != &v.container() ? throw : index() != v.index(); }
	};
private:
	const size_t freserve_size = 20;
	size_t freserve;
	char* fmem;
	size_t fsize;
	size_t foffset;
	size_t flength;
	size_t fgap;
protected:
	mem_content* content(size_t v)
	{
		return reinterpret_cast<mem_content*>(fmem + content_length() * v + foffset);
	}
	mem_content* content(size_t v) const
	{
		return reinterpret_cast<mem_content*>(fmem + content_length() * v + foffset);
	}
	void init(const void* mem, size_t size, size_t offset, size_t length, size_t gap)
	{
		if (freserve > 0)
		{
			delete[] fmem;
			freserve = 0;
		}
		fmem = NULL;
		if (freserve < size)
		{
			while (freserve < size)
				freserve += freserve_size;
			if (freserve * length > 0)
				fmem = new char[freserve * length];
		}
		for (fsize = 0; length > 0 && fsize < size; ++fsize)
			std::memmove(fmem + length * fsize, (char*)mem + (offset + length + gap) * fsize + offset, length);
		foffset = 0;
		flength = length;
		fgap = 0;
	}
public:
	mem_container()
		: freserve(0), fmem(NULL), fsize(0), foffset(0), flength(0), fgap(0)
	{
	}
	mem_container(size_t length)
		: freserve(0), fmem(NULL), fsize(0), foffset(0), flength(length), fgap(0)
	{
	}
	mem_container(size_t length, void* mem, size_t count)
		: freserve(0), fmem(NULL)
	{
		init(mem, count, 0, length, 0);
	}
	mem_container(size_t length, const void* mem, size_t count, size_t offset, size_t gap)
		: freserve(0), fmem(NULL)
	{
		init(mem, count, offset, length, gap);
	}
	mem_container(const wchar_t* v, size_t count)
		: freserve(0), fmem(NULL)
	{
		init(v, count, 0, sizeof(wchar_t), 0);
	}
	mem_container(const char* v, size_t count)
		: freserve(0), fmem(NULL)
	{
		init(v, count, 0, sizeof(char), 0);
	}
	mem_container(const const_iterator& begin, const const_iterator& end)
		: freserve(0), fmem(NULL)
	{
		init(begin.content(), end - begin, begin.container().offset(), begin.container().length(), begin.container().gap());
	}
	mem_container(const mem_container& v)
		: freserve(0), fmem(NULL)
	{
		*this = v;
	}
	mem_container& operator = (const mem_container& v)
	{
		if (this != &v)
			init(v.fmem, v.fsize, v.foffset, v.flength, v.fgap);
		return *this;
	}
	mem_container(mem_container&& v)
		: freserve(0), fmem(NULL)
	{
		*this = std::move(v);
	}
	mem_container& operator = (mem_container&& v)
	{
		if (this != &v)
		{
			delete[] fmem;
			freserve = v.freserve;
			fmem = v.fmem;
			fsize = v.fsize;
			foffset = v.foffset;
			flength = v.flength;
			fgap = v.fgap;
			v.fmem = nullptr;
			v.freserve = 0;
		}
		return *this;
	}
	virtual ~mem_container()
	{
		if (freserve > 0)
			delete[] fmem;
	}
	void clear(void)
	{
		init(NULL, 0, 0, 0, 0);
	}
	void atatch(size_t length, const void* mem, size_t count, size_t offset, size_t gap)
	{
		if (freserve > 0)
			delete[] fmem;
		freserve = 0;
		fmem = (char*)mem;
		fsize = count;
		foffset = offset;
		flength = length;
		fgap = gap;
	}
	void atatch(mem_container mem, size_t begin, size_t end)
	{
		if (freserve > 0)
			delete[] fmem;
		freserve = 0;
		fmem = (char*)mem.fmem + begin;
		fsize = end - begin;
		foffset = mem.foffset;
		flength = mem.flength;
		fgap = mem.fgap;
	}
	void* detatch(void)
	{
		if (freserve > 0)
			throw;
		void* ret = fmem;
		freserve = 0;
		fmem = NULL;
		fsize = 0;
		foffset = 0;
		flength = 0;
		fgap = 0;
		return ret;
	}
	void append(size_t length, const void* mem, size_t count, size_t offset, size_t gap)
	{
		if (flength == 0)
			flength = length;
		if (length != flength)
			throw;
		if (fmem != NULL && freserve == 0)
			throw;
		if (fmem == NULL || freserve < fsize + count)
		{
			while (freserve < fsize + count)
				freserve += freserve_size;
			if (freserve * flength > 0)
			{
				char* nmem = new char[freserve * flength];
				std::memmove(nmem, fmem, flength * fsize);
				delete[] fmem;
				fmem = nmem;
			}
		}
		for (size_t l = 0; l < count; ++l)
			std::memmove(fmem + (fsize + l) * flength, (char*)mem + l * (offset + length + gap) + offset, length);
		fsize += count;
	}
	void append(const void* mem, size_t count)
	{
		append(flength, mem, count, foffset, fgap);
	}
	void append(const const_iterator& begin, const const_iterator& end)
	{
		append(begin.container().length(), &*begin, end - begin, begin.container().offset(), begin.container().gap());
	}
	void append(const mem_container& v)
	{
		append(v.begin(), v.end());
	}
	void insert(size_t index, size_t length, const void* mem, size_t count, size_t offset, size_t gap)
	{
		if (flength == 0)
			flength = length;
		if (length != flength)
			throw;
		if (fmem != NULL && freserve == 0)
			throw;
		if (index > fsize)
			throw;
		if (fmem == NULL || freserve < fsize + count)
		{
			while (freserve < fsize + count)
				freserve += freserve_size;
			if (freserve * flength > 0)
			{
				char* nmem = new char[freserve * flength];
				std::memmove(nmem, fmem, flength * index);
				std::memmove(nmem + flength * (index + count), fmem + flength * index, flength * (fsize - index));
				delete[] fmem;
				fmem = nmem;
			}
		}
		for (size_t l = 0; l < count; ++l)
			std::memmove(fmem + (index + l) * flength, (char*)mem + l * (offset + length + gap) + offset, flength);
		fsize += count;
	}
	void insert(size_t index, const void* mem, size_t count)
	{
		insert(index, flength, mem, count, foffset, fgap);
	}
	void insert(const const_iterator& pos, const_iterator begin, const const_iterator& end)
	{
		insert(end - begin, begin.container().length(), &*begin, end - begin, begin.container().offset(), begin.container().gap());
	}
	void insert(const const_iterator& pos, const mem_container& v)
	{
		insert(pos, v.begin(), v.end());
	}
	size_t content_length(void) const { return foffset + flength + fgap; }
	size_t length(void) const { return flength; }
	size_t size(void) const { return fsize; }
	size_t offset(void) const { return foffset; }
	size_t gap(void) const { return fgap; }
	const_iterator begin(void) const { return const_iterator(*this); }
	iterator begin(void) { return iterator(*this); }
	const_iterator end(void) const { return const_iterator(*this, fsize); }
	iterator end(void) { return iterator(*this, fsize); }
	mem_container range_set(const const_iterator& b, const const_iterator& e) const
	{
		const_iterator isb = begin();
		while (isb != end() && compare(isb, isb + 1, b, b + 1) != 0)
			++isb;
		const_iterator ieb = begin();
		while (ieb != end() && compare(ieb, ieb + 1, e, e + 1) != 0)
			++ieb;
		mem_container ret;
		if (isb != end() && ieb != end())
		{
			if (isb > ieb)
				std::swap(isb, ieb);
			while (isb != ieb)
				ret.append(isb++, isb);
			ret.append(isb++, isb);
		}
		return std::move(ret);
	}
	mem_container sub_set(const const_iterator& b, const const_iterator& e) const
	{
		return std::move(mem_container(b, e));
	}
	static int compare(const const_iterator& l, const const_iterator& r)
	{
		if (l.container().length() != r.container().length())
			throw;
		return memcmp(&*l, &*r, l.container().length());
	}
	static int compare(const mem_container& l, const mem_container& r)
	{
		return compare(l.begin(), l.end(), r.begin(), r.end());
	}
	static int compare(const_iterator lb, const const_iterator& le, const_iterator rb, const const_iterator& re)
	{
		if (lb.container().length() != rb.container().length())
			throw;
		if (le - lb > re - rb)
			return 1;
		else if (le - lb > re - rb)
			return -1;
		else
		{
			size_t length = lb.container().length();
			int ret = 0;
			while (ret == 0 && lb != le)
				ret = memcmp(&*(lb++), &*(rb++), length);
			return ret;
		}
	}
	template<typename target_iterator>
	static int compare(const_iterator lb, const const_iterator& le, target_iterator rb, const target_iterator& re)
	{
		if (le - lb > re - rb)
			return 1;
		else if (le - lb > re - rb)
			return -1;
		else
		{
			size_t length = lb.container().length();
			int ret = 0;
			while (ret == 0 && lb != le)
				ret = memcmp(&*(lb++), &*(rb++), length);
			return ret;
		}
	}
};

struct mem_container_compare
{
	int operator () (const mem_container::const_iterator& l, const mem_container::const_iterator& r)
	{
		return mem_container::compare(l, r);
	}
};
#pragma once

#include	<vector>
#include	<utility>

template<typename T>
class tree
{
private:
public:
	class iterator;
	class const_iterator;
protected:
	class node_traits
	{
		friend class tree<T>;
	private:
		tree<T> *m_owner;
		size_t m_indent;
		size_t m_sibling_index;
	public:
		node_traits(tree<T>* const Owner, size_t TreeIndent = 0, size_t SiblingIndex = 0)
			: m_owner(Owner)
			, m_indent(TreeIndent)
			, m_sibling_index(SiblingIndex)
		{
		}
		node_traits(void)
			: m_owner(NULL)
			, m_indent(0)
			, m_sibling_index(0)
		{
		}
		void inc_indent(void) { ++ m_indent; }
		void dec_indent(void) { -- m_indent; }
	};
	typedef std::pair<node_traits, T> node;
	typedef std::vector<node> tree_trunk;
	tree_trunk m_trunk;
	typedef typename tree_trunk::iterator tree_iterator;
	typedef typename tree_trunk::const_iterator const_tree_iterator;
	iterator add_(iterator& parent, const T& value)
	{
		bool nullparent = parent.is_end();
		iterator r = parent;
		if (! nullparent)
		{
			do {
				++ r;
			} while (! r.is_end() && r.tree_indent() > parent.tree_indent());
		}
		iterator i = nullparent ? end(): parent.last_child();
		size_t idx = i.is_end() ? 0: i.sibling_index() + 1;
		size_t idt = nullparent ? 0: parent.tree_indent() + 1;
		node v(node_traits(this, idt, idx), value);
		m_trunk.insert(m_trunk.begin() + r.m_index, v);
		return r;
	}
	iterator insert_(iterator& it, const T& value)
	{
		size_t idx = it.is_end() ? 0: it.sibling_index();
		size_t idt = it.is_end() ? 0: it.tree_indent();
		node v(node_traits(this, idt, idx), value);
		m_trunk.insert(m_trunk.begin() + it.m_index, v);
		tree_trunk::iterator i = m_trunk.begin() + it.m_index + 1;
		while (i != m_trunk.end())
		{
			if (i->first.m_indent < idt)
				break;
			else if (i->first.m_indent == idt)
				++ i->first.m_sibling_index;
			++ i;
		}
		return it;
	}
	const_iterator add_(const_iterator& parent, const T& value)
	{
		bool nullparent = parent.is_end();
		const_iterator r = parent;
		if (! nullparent)
		{
			do {
				++ r;
			} while (! r.is_end() && r.tree_indent() > parent.tree_indent());
		}
		const_iterator i = nullparent ? parent: parent.last_child();
		size_t idx = i.is_end() ? 0: i.sibling_index() + 1;
		size_t idt = nullparent ? 0: parent.tree_indent() + 1;
		node v(node_traits(this, idt, idx), value);
		m_trunk.insert(m_trunk.begin() + r.m_index, v);
		return r;
	}
	const_iterator insert_(const_iterator& it, const T& value)
	{
		size_t idx = it.is_end() ? 0: it.sibling_index();
		size_t idt = it.is_end() ? 0: it.tree_indent();
		node v(node_traits(this, idt, idx), value);
		m_trunk.insert(m_trunk.begin() + it.m_index, v);
		tree_trunk::iterator i = m_trunk.begin() + it.m_index + 1;
		while (i != m_trunk.end())
		{
			if (i->first.m_indent < idt)
				break;
			else if (i->first.m_indent == idt)
				++ i->first.m_sibling_index;
			++ i;
		}
		return it;
	}
public:


	//class const_sibling_iterator
	//	: public std::iterator<std::random_access_iterator_tag, node>
	//{
	//private:
	//	friend class tree<T>;
	//	friend iterator;
	//	typedef const T* const_pointer;
	//	typedef const T& const_reference;
	//	const tree<T>* m_owner;
	//	size_t m_index;
	//protected:
	//	tree<T>* owner(void) { return const_cast<tree<T>*>(m_owner); }
	//	tree<T>* owner(void) const { return const_cast<tree<T>*>(m_owner); }
	//	void subtract(size_t v)
	//	{
	//		if (v > m_index)
	//			m_index = owner()->m_trunk.size();
	//		else
	//			m_index -= v;
	//	}
	//	void append(size_t v)
	//	{
	//		if ((unsigned) v > owner()->m_trunk.size() - m_index)
	//			m_index = owner()->m_trunk.size();
	//		else
	//			m_index += v;
	//	}
	//	//void tree_indent(size_t v) { (owner()->m_trunk.begin() + m_index)->first.m_indent = v; }
	//	//void sibling_index(size_t v) { (owner()->m_trunk.begin() + m_index)->first.m_sibling_index = v; }
	//public:
	//	const_sibling_iterator(void)
	//		: m_owner(NULL)
	//		, m_index(0)
	//	{
	//	}
	//	const_sibling_iterator(const tree<T>* owner, size_t v)
	//		: m_owner(owner)
	//		, m_index(v)
	//	{
	//		if (m_owner == NULL)
	//			m_index = 0;
	//		else if (m_index >= m_owner->m_trunk.size())
	//			m_index = m_owner->m_trunk.size();
	//	}
	//	const_reference operator * () const { return (m_owner->m_trunk.begin() + m_index)->second; }
	//	const_reference operator [] (size_t v) const { return (m_owner->m_trunk.begin() + m_index + v)->second; }
	//	const_pointer operator -> () const { return &(m_owner->m_trunk.begin() + m_index)->second; }
	//	const_iterator& operator ++ () { append(1); return *this; }
	//	const_iterator& operator -- () { subtract(1); return *this; }
	//	const_iterator operator ++ (int) { const_iterator r = *this; append(1); return r; }
	//	const_iterator operator -- (int) { const_iterator r = *this; subtract(1); return r; }
	//	const_iterator& operator += (difference_type v) { append(v); return *this; }
	//	const_iterator& operator -= (difference_type v) { subtract(v); return *this; }
	//	const_iterator& operator << (difference_type v) { m_index = v; return *this; }
	//	const_iterator operator + (difference_type v) const { const_iterator r = *this; append(v); return r; }
	//	const_iterator operator - (difference_type v) const { const_iterator r = *this; subtract(v); return r; }
	//	//bool operator == (const iterator& v) const { return m_owner == v.m_owner && m_index == v.m_index; }
	//	//bool operator != (const iterator& v) const { return m_owner != v.m_owner || m_index != v.m_index; }
	//	//bool operator <= (const iterator& v) const { return m_owner == v.m_owner && m_index <= v.m_index; }
	//	//bool operator >= (const iterator& v) const { return m_owner == v.m_owner && m_index >= v.m_index; }
	//	//bool operator < (const iterator& v) const { return m_owner == v.m_owner && m_index < v.m_index; }
	//	//bool operator > (const iterator& v) const { return m_owner == v.m_owner && m_index > v.m_index; }
	//	//bool operator == (const const_iterator& v) const { return m_owner == v.m_owner && m_index == v.m_index; }
	//	//bool operator != (const const_iterator& v) const { return m_owner != v.m_owner || m_index != v.m_index; }
	//	//bool operator <= (const const_iterator& v) const { return m_owner == v.m_owner && m_index <= v.m_index; }
	//	//bool operator >= (const const_iterator& v) const { return m_owner == v.m_owner && m_index >= v.m_index; }
	//	//bool operator < (const const_iterator& v) const { return m_owner == v.m_owner && m_index < v.m_index; }
	//	//bool operator > (const const_iterator& v) const { return m_owner == v.m_owner && m_index > v.m_index; }
	//	bool is_null(void) const { return owner() == NULL; }
	//	bool is_empty(void) const { return is_null() || owner()->m_trunk.empty(); }
	//	bool is_begin(void) const { return is_empty() == false && m_index == 0; }
	//	bool is_end(void) const { return is_empty() == true || m_index == owner()->m_trunk.size(); }
	//	size_t absolute_index(void) const { return m_index; }
	//	size_t sibling_index(void) const { return (m_owner->m_trunk.begin() + m_index)->first.m_sibling_index; }
	//	size_t tree_indent(void) const { return (m_owner->m_trunk.begin() + m_index)->first.m_indent; }
	//	size_t count_child(void) const
	//	{
	//		return is_empty() ? 0: first_child().count_sibling();
	//	}
	//	size_t count_sibling(void) const
	//	{
	//		if (is_end())
	//			return 0;
	//		const_iterator i = *this;
	//		size_t idx = i.sibling_index();
	//		do
	//			++ idx;
	//		while (i.move_next_sibling() != owner()->end());
	//		return idx;
	//	}
	//	const_iterator& move_begin(void) { *this = begin(); return *this; }
	//	const_iterator begin(void) const
	//	{
	//		return owner()->begin();
	//	}
	//	const_iterator& move_end(void) { *this = end(); return *this; }
	//	const_iterator end(void) const
	//	{
	//		return owner()->end();
	//	}
	//	const_iterator& move_root(void) { *this = root(); return *this; }
	//	const_iterator root(void) const
	//	{
	//		if (is_end())
	//			return owner()->end();
	//		const_iterator r = *this;
	//		while (r.tree_indent())
	//			r.move_parent();
	//		return r;
	//	}
	//	const_iterator& move_parent(void) { *this = parent(); return *this; }
	//	const_iterator parent(void) const
	//	{
	//		if (is_end())
	//			return owner()->end();
	//		const_iterator r = *this - 1;
	//		while (! r.is_end())
	//		{
	//			if (r.tree_indent() == tree_indent() - 1)
	//				break;
	//			else if (r.tree_indent() < tree_indent())
	//				r = owner()->end();
	//			else
	//				-- r;
	//		}
	//		return r;
	//	}
	//	const_iterator& move_first_child(void) { *this = first_child(); return *this; }
	//	const_iterator first_child(void) const
	//	{
	//		if (is_end())
	//			return owner()->end();
	//		const_iterator r = *this + 1;
	//		if (! r.is_end() && r.tree_indent() == tree_indent() + 1)
	//			return r;
	//		else
	//			return owner()->end();
	//	}
	//	const_iterator& move_last_child(void) { *this = last_child(); return *this; }
	//	const_iterator last_child(void) const
	//	{
	//		if (is_end())
	//			return owner()->end();
	//		const_iterator old = *this;
	//		const_iterator r = old + 1;
	//		while (! r.is_end())
	//		{
	//			if (r.tree_indent() <= tree_indent())
	//			{
	//				r = old == *this ? owner()->end(): old;
	//				break;
	//			}
	//			else if (r.tree_indent() == tree_indent() + 1)
	//				old = r;
	//			++ r;
	//		}
	//		if (r == owner()->end())
	//			r = old == *this ? owner()->end(): old;
	//		return r;
	//	}
	//	const_iterator& move_first_sibling(void) { *this = first_sibling(); return *this; }
	//	const_iterator first_sibling(void) const
	//	{
	//		if (is_end())
	//			return owner()->end();
	//		if (indent())
	//			return parent().first_child();
	//		else
	//			return begin();
	//	}
	//	const_iterator& move_last_sibling(void) { *this = last_sibling(); return *this; }
	//	const_iterator last_sibling(void) const
	//	{
	//		if (is_end())
	//			return owner()->end();
	//		const_iterator r = *this;
	//		if (tree_indent())
	//			r.move_parent().move_last_child();
	//		else
	//		{
	//			r = owner()->end();
	//			while (r.tree_indent())
	//				-- r;
	//		}
	//		return r;
	//	}
	//	const_iterator& move_prev_sibling(void) { *this = prev_sibling(); return *this; }
	//	const_iterator prev_sibling(void) const
	//	{
	//		if (is_end())
	//			return owner()->end();
	//		const_iterator r = *this - 1;
	//		while (! r.is_end())
	//		{
	//			if (r.tree_indent() == tree_indent())
	//				break;
	//			else if (r.tree_indent() < tree_indent())
	//				r = owner()->end();
	//			else
	//				-- r;
	//		}
	//		return r;
	//	}
	//	const_iterator& move_next_sibling(void) { *this = next_sibling(); return *this; }
	//	const_iterator next_sibling(void) const
	//	{
	//		if (is_end())
	//			return owner()->end();
	//		const_iterator r = *this + 1;
	//		while (! r.is_end())
	//		{
	//			if (r.tree_indent() < tree_indent())
	//				r = owner()->end();
	//			else if (r.tree_indent() == tree_indent())
	//				break;
	//			else
	//				++ r;
	//		}
	//		return r;
	//	}
	//};


	class const_iterator
		: public std::iterator<std::random_access_iterator_tag, node>
	{
	private:
		friend class tree<T>;
		friend iterator;

		typedef const T* const_pointer;
		typedef const T& const_reference;

		const tree<T>* m_owner;
		size_t m_index;
	protected:
		tree<T>* owner(void) { return const_cast<tree<T>*>(m_owner); }
		tree<T>* owner(void) const { return const_cast<tree<T>*>(m_owner); }
		void subtract(size_t v)
		{
			if (v > m_index)
				m_index = owner()->m_trunk.size();
			else
				m_index -= v;
		}
		void append(size_t v)
		{
			if (m_index + v >= owner()->m_trunk.size())
				m_index = owner()->m_trunk.size();
			else
				m_index += v;
		}
		void tree_indent(size_t v) { (owner()->m_trunk.begin() + m_index)->first.m_indent = v; }
		void sibling_index(size_t v) { (owner()->m_trunk.begin() + m_index)->first.m_sibling_index = v; }
	public:
		const_iterator(void)
			: m_owner(NULL)
			, m_index(0)
		{
		}
		const_iterator(const tree<T>* owner, size_t v)
			: m_owner(owner)
			, m_index(v)
		{
			if (m_owner == NULL)
				m_index = 0;
			else if (m_index >= m_owner->m_trunk.size())
				m_index = m_owner->m_trunk.size();
		}
		const_reference operator * () const { return (m_owner->m_trunk.begin() + m_index)->second; }
		const_reference operator [] (size_t v) const { return (m_owner->m_trunk.begin() + m_index + v)->second; }
		const_pointer operator -> () const { return &(m_owner->m_trunk.begin() + m_index)->second; }
		const_iterator& operator ++ () { append(1); return *this; }
		const_iterator& operator -- () { subtract(1); return *this; }
		const_iterator operator ++ (int) { const_iterator r = *this; append(1); return r; }
		const_iterator operator -- (int) { const_iterator r = *this; subtract(1); return r; }
		const_iterator& operator += (difference_type v) { append(v); return *this; }
		const_iterator& operator -= (difference_type v) { subtract(v); return *this; }
		const_iterator& operator << (difference_type v) { m_index = v; return *this; }
		const_iterator operator + (difference_type v) const { const_iterator r = *this; r.append(v); return r; }
		const_iterator operator - (difference_type v) const { const_iterator r = *this; r.subtract(v); return r; }
		//difference_type operator + (const const_iterator& v) const { return append(v.m_index); }
		//difference_type operator - (const const_iterator& v) const { return subtract(v.m_index); }
		bool operator == (const iterator& v) const { return m_owner == v.m_owner && m_index == v.m_index; }
		bool operator != (const iterator& v) const { return m_owner != v.m_owner || m_index != v.m_index; }
		bool operator <= (const iterator& v) const { return m_owner == v.m_owner && m_index <= v.m_index; }
		bool operator >= (const iterator& v) const { return m_owner == v.m_owner && m_index >= v.m_index; }
		bool operator < (const iterator& v) const { return m_owner == v.m_owner && m_index < v.m_index; }
		bool operator > (const iterator& v) const { return m_owner == v.m_owner && m_index > v.m_index; }
		bool operator == (const const_iterator& v) const { return m_owner == v.m_owner && m_index == v.m_index; }
		bool operator != (const const_iterator& v) const { return m_owner != v.m_owner || m_index != v.m_index; }
		bool operator <= (const const_iterator& v) const { return m_owner == v.m_owner && m_index <= v.m_index; }
		bool operator >= (const const_iterator& v) const { return m_owner == v.m_owner && m_index >= v.m_index; }
		bool operator < (const const_iterator& v) const { return m_owner == v.m_owner && m_index < v.m_index; }
		bool operator > (const const_iterator& v) const { return m_owner == v.m_owner && m_index > v.m_index; }
		bool is_null(void) const { return owner() == NULL; }
		bool is_empty(void) const { return is_null() || owner()->m_trunk.empty(); }
		bool is_begin(void) const { return is_empty() == false && m_index == 0; }
		bool is_end(void) const { return is_empty() == true || m_index == owner()->m_trunk.size(); }
		bool is_roots(const const_iterator& v) const
		{
			bool ret = *this == v;
			const_iterator it = parent();
			while (!ret && !it.is_end())
			{
				ret = it == v;
				it.move_parent();
			}
			return ret;
		}
		bool is_roots(const iterator& v) const
		{
			bool ret = *this == v;
			const_iterator it = parent();
			while (!ret && !it.is_end())
			{
				ret = it == v;
				it.move_parent();
			}
			return ret;
		}
		size_t absolute_index(void) const { return m_index; }
		size_t sibling_index(void) const { return (m_owner->m_trunk.begin() + m_index)->first.m_sibling_index; }
		size_t tree_indent(void) const { return (m_owner->m_trunk.begin() + m_index)->first.m_indent; }
		size_t count_child(void) const
		{
			return is_empty() ? 0: first_child().count_sibling();
		}
		size_t count_sibling(void) const
		{
			if (is_end())
				return 0;
			const_iterator i = *this;
			size_t idx = i.sibling_index();
			do
				++ idx;
			while (i.move_next_sibling() != owner()->end());
			return idx;
		}
		const_iterator& move_begin(void) { *this = begin(); return *this; }
		const_iterator begin(void) const
		{
			return owner()->begin();
		}
		const_iterator& move_end(void) { *this = end(); return *this; }
		const_iterator end(void) const
		{
			return owner()->end();
		}
		const_iterator& move_root(void) { *this = root(); return *this; }
		const_iterator root(void) const
		{
			if (is_end())
				return owner()->end();
			const_iterator r = *this;
			while (r.tree_indent())
				r.move_parent();
			return r;
		}
		const_iterator& move_parent(void) { *this = parent(); return *this; }
		const_iterator parent(void) const
		{
			if (is_end())
				return owner()->end();
			const_iterator r = *this - 1;
			while (! r.is_end())
			{
				if (r.tree_indent() == tree_indent() - 1)
					break;
				else if (r.tree_indent() < tree_indent())
					r = owner()->end();
				else
					-- r;
			}
			return r;
		}
		const_iterator& move_first_child(void) { *this = first_child(); return *this; }
		const_iterator first_child(void) const
		{
			if (is_end())
				return owner()->end();
			const_iterator r = *this + 1;
			if (! r.is_end() && r.tree_indent() == tree_indent() + 1)
				return r;
			else
				return owner()->end();
		}
		const_iterator& move_last_child(void) { *this = last_child(); return *this; }
		const_iterator last_child(void) const
		{
			if (is_end())
				return owner()->end();
			const_iterator old = *this;
			const_iterator r = old + 1;
			while (! r.is_end())
			{
				if (r.tree_indent() <= tree_indent())
				{
					r = old == *this ? owner()->end(): old;
					break;
				}
				else if (r.tree_indent() == tree_indent() + 1)
					old = r;
				++ r;
			}
			if (r == owner()->end())
				r = old == *this ? owner()->end(): old;
			return r;
		}
		const_iterator& move_first_sibling(void) { *this = first_sibling(); return *this; }
		const_iterator first_sibling(void) const
		{
			if (is_end())
				return owner()->end();
			if (indent())
				return parent().first_child();
			else
				return begin();
		}
		const_iterator& move_last_sibling(void) { *this = last_sibling(); return *this; }
		const_iterator last_sibling(void) const
		{
			if (is_end())
				return owner()->end();
			const_iterator r = *this;
			if (tree_indent())
				r.move_parent().move_last_child();
			else
			{
				r = owner()->end();
				while (r.tree_indent())
					-- r;
			}
			return r;
		}
		const_iterator& move_prev_sibling(void) { *this = prev_sibling(); return *this; }
		const_iterator prev_sibling(void) const
		{
			if (is_end())
				return owner()->end();
			const_iterator r = *this - 1;
			while (! r.is_end())
			{
				if (r.tree_indent() == tree_indent())
					break;
				else if (r.tree_indent() < tree_indent())
					r = owner()->end();
				else
					-- r;
			}
			return r;
		}
		const_iterator& move_next_sibling(void) { *this = next_sibling(); return *this; }
		const_iterator next_sibling(void) const
		{
			if (is_end())
				return owner()->end();
			const_iterator r = *this + 1;
			while (! r.is_end())
			{
				if (r.tree_indent() < tree_indent())
					r = owner()->end();
				else if (r.tree_indent() == tree_indent())
					break;
				else
					++ r;
			}
			return r;
		}
	};

	class iterator
		: public const_iterator
	{
	private:
		typedef T* pointer;
		typedef T& reference;

		friend class tree<T>;
		friend const_iterator;
	public:
		iterator(void)
		{
		}
		iterator(tree<T>* owner, size_t v)
			: const_iterator(owner, v)
		{
		}
		reference operator * () { return (owner()->m_trunk.begin() + m_index)->second; }
		reference operator [] (size_t v) { return (owner()->m_trunk.begin() + m_index + v)->second; }
		pointer operator -> () { return &(owner()->m_trunk.begin() + m_index)->second; }
		iterator& operator ++ () { append(1); return *this; }
		iterator& operator -- () { subtract(1); return *this; }
		iterator operator ++ (int) { iterator r = *this; append(1); return r; }
		iterator operator -- (int) { iterator r = *this; subtract(1); return r; }
		iterator& operator += (difference_type v) { append(v); return *this; }
		iterator& operator -= (difference_type v) { subtract(v); return *this; }
		iterator& operator << (difference_type v) { m_index = v; return *this; }
		iterator& operator << (const tree<T>& v) { owner()->add_tree(*this, v); return *this; }
		iterator& operator >> (const tree<T>& v) { owner()->insert_tree(owner()->NextSibling(*this), v); return *this; }
		iterator operator + (difference_type v) const { iterator r = *this; r.append(v); return r; }
		iterator operator - (difference_type v) const { iterator r = *this; r.subtract(v); return r; }
		//difference_type operator + (const iterator& v) const { return append(v.m_index); }
		//difference_type operator - (const iterator& v) const { return subtract(v.m_index); }
		bool operator == (const iterator& v) const { return owner() == v.owner() && m_index == v.m_index; }
		bool operator != (const iterator& v) const { return owner() != v.owner() || m_index != v.m_index; }
		bool operator <= (const iterator& v) const { return owner() == v.owner() && m_index <= v.m_index; }
		bool operator >= (const iterator& v) const { return owner() == v.owner() && m_index >= v.m_index; }
		bool operator < (const iterator& v) const { return owner() == v.owner() && m_index < v.m_index; }
		bool operator > (const iterator& v) const { return owner() == v.owner() && m_index > v.m_index; }
		bool operator == (const const_iterator& v) const { return owner() == v.owner() && m_index == v.m_index; }
		bool operator != (const const_iterator& v) const { return owner() != v.owner() || m_index != v.m_index; }
		bool operator <= (const const_iterator& v) const { return owner() == v.owner() && m_index <= v.m_index; }
		bool operator >= (const const_iterator& v) const { return owner() == v.owner() && m_index >= v.m_index; }
		bool operator < (const const_iterator& v) const { return owner() == v.owner() && m_index < v.m_index; }
		bool operator > (const const_iterator& v) const { return owner() == v.owner() && m_index > v.m_index; }
		iterator& move_begin(void) { *this = begin(); return *this; }
		iterator begin(void) const
		{
			return owner()->begin();
		}
		iterator& move_end(void) { *this = end(); return *this; }
		iterator end(void) const
		{
			return owner()->end();
		}
		iterator& move_root(void) { *this = root(); return *this; }
		iterator root(void) const
		{
			if (is_end())
				return owner()->end();
			iterator r = *this;
			while (r.tree_indent())
				r.move_parent();
			return r;
		}
		iterator& move_parent(void) { *this = parent(); return *this; }
		iterator parent(void) const
		{
			if (is_end())
				return owner()->end();
			iterator r = *this - 1;
			while (! r.is_end())
			{
				if (r.tree_indent() == tree_indent() - 1)
					break;
				else if (r.tree_indent() < tree_indent())
					r = owner()->end();
				else
					-- r;
			}
			return r;
		}
		iterator& move_first_child(void) { *this = first_child(); return *this; }
		iterator first_child(void) const
		{
			if (is_end())
				return owner()->end();
			iterator r = *this + 1;
			if (! r.is_end() && r.tree_indent() == tree_indent() + 1)
				return r;
			else
				return owner()->end();
		}
		iterator& move_last_child(void) { *this = last_child(); return *this; }
		iterator last_child(void) const
		{
			if (is_end())
				return owner()->end();
			iterator old = *this;
			iterator r = old + 1;
			while (! r.is_end())
			{
				if (r.tree_indent() <= tree_indent())
				{
					r = old == *this ? owner()->end(): old;
					break;
				}
				else if (r.tree_indent() == tree_indent() + 1)
					old = r;
				++ r;
			}
			if (r == owner()->end())
				r = old == *this ? owner()->end(): old;
			return r;
		}
		iterator& move_first_sibling(void) { *this = first_sibling(); return *this; }
		iterator first_sibling(void) const
		{
			if (is_end())
				return owner()->end();
			if (tree_indent())
				return parent().first_child();
			else
				return owner()->begin();
		}
		iterator& move_last_sibling(void) { *this = last_sibling(); return *this; }
		iterator last_sibling(void) const
		{
			if (is_end())
				return owner()->end();
			iterator r = *this;
			if (tree_indent())
				r.move_parent().move_last_child();
			else
			{
				r = owner()->end();
				while ((-- r).tree_indent());
			}
			return r;
		}
		iterator& move_prev_sibling(void) { *this = prev_sibling(); return *this; }
		iterator prev_sibling(void) const
		{
			if (is_end())
				return owner()->end();
			iterator r = *this - 1;
			while (! r.is_end())
			{
				if (r.tree_indent() == tree_indent())
					break;
				else if (r.tree_indent() < tree_indent())
					r = owner()->end();
				else
					-- r;
			}
			return r;
		}
		iterator& move_next_sibling(void) { *this = next_sibling(); return *this; }
		iterator next_sibling(void) const
		{
			if (is_end())
				return owner()->end();
			iterator r = *this + 1;
			while (! r.is_end())
			{
				if (r.tree_indent() < tree_indent())
					r = owner()->end();
				else if (r.tree_indent() == tree_indent())
					break;
				else
					++ r;
			}
			return r;
		}
	};
public:
	tree() {}
	virtual ~tree() {}
	tree(const tree<T>& v)
	{
		*this = v;
	}
	tree<T>& operator = (const tree<T>& v)
	{
		if (&v != this)
		{
			m_trunk = v.m_trunk;
			std::vector<node>::iterator it = m_trunk.begin();
			while (it != m_trunk.end())
			{
				it->first.m_owner = this;
				++it;
			}
		}
		return *this;
	}
	tree(tree<T>&& v)
	{
		*this = std::move(v);
	}
	tree<T>& operator = (tree<T>&& v)
	{
		if (&v != this)
		{
			m_trunk = std::move(v.m_trunk);
			std::vector<node>::iterator it = m_trunk.begin();
			while (it != m_trunk.end())
			{
				it->first.m_owner = this;
				++it;
			}
		}
		return *this;
	}
	T& operator [] (size_t Index) { return m_trunk[Index].second; }
	const T& operator [] (size_t Index) const { return m_trunk[Index].second; }
	T& at(size_t Index) { return m_trunk.at(Index).second; }
	const T& at(size_t Index) const { return m_trunk.at(Index).second; }
	T& back(void) { return m_trunk.back().second; }
	void reserve(size_t Count) { m_trunk.reserve(Count); }
	bool empty(void) const { return m_trunk.empty(); }
	size_t size(void) const { return m_trunk.size(); }
	size_t max_size(void) const { return m_trunk.max_size(); }
	virtual const_iterator add(const_iterator& parent, const tree& tree)
	{
		return add(parent, tree.begin(), tree.end());
	}
	virtual const_iterator add(const_iterator& parent, const_iterator& begin, const_iterator& end)
	{
		if (begin == end)
			return end;
		const_iterator it = begin;
		const_iterator r = add_(parent, *it);
		size_t index = r.sibling_index();
		size_t indent = r.tree_indent();
		size_t insert = r.m_index;
		while (++ it != end && it.tree_indent() >= begin.tree_indent())
		{
			++ insert;
			size_t idx = it.tree_indent() == begin.tree_indent() ? ++ index: it.sibling_index();
			size_t idt = it.tree_indent() - begin.tree_indent() + indent;
			m_trunk.insert(m_trunk.begin() + insert, node(node_traits(this, idt, idx), *it));
		}
		return r;
	}
	virtual iterator add(iterator& parent, const tree& tree)
	{
		return add(parent, tree.begin(), tree.end());
	}
	virtual iterator add(iterator& parent, const_iterator& begin, const_iterator& end)
	{
		if (begin == end)
			return parent;
		const_iterator it = begin;
		iterator r = add_(parent, *it);
		size_t index = r.sibling_index();
		size_t indent = r.tree_indent();
		size_t insert = r.m_index;
		while (++ it != end && it.tree_indent() >= begin.tree_indent())
		{
			++ insert;
			size_t idx = it.tree_indent() == begin.tree_indent() ? ++ index: it.sibling_index();
			size_t idt = it.tree_indent() - begin.tree_indent() + indent;
			m_trunk.insert(m_trunk.begin() + insert, node(node_traits(this, idt, idx), *it));
		}
		return r;
	}
	virtual const_iterator insert(const_iterator& it, const tree& tree)
	{
		return insert(it, tree.begin(), tree.end());
	}
	virtual const_iterator insert(const_iterator& in, const_iterator& begin, const_iterator& end)
	{
		if (begin == end)
			return in;
		const_iterator it = begin;
		const_iterator r = insert_(in, *it);
		size_t index = r.sibling_index();
		size_t indent = r.tree_indent();
		size_t insert = r.m_index;
		while (++ it != end)
		{
			++ insert;
			size_t idx = it.tree_indent() == begin.tree_indent() ? ++ index: it.sibling_index();
			size_t idt = it.tree_indent() - begin.tree_indent() + indent;
			tree_iterator i = m_trunk.insert(m_trunk.begin() + insert, node(node_traits(this, idt, idx), *it));
			while (++ i != m_trunk.end())
			{
				if (i->first.m_indent < in.tree_indent())
					break;
				else if (i->first.m_indent == in.tree_indent())
					++ i->first.m_sibling_index;
			}
		}
		return r;
	}
	virtual iterator insert(iterator& it, const tree& tree)
	{
		return insert(it, tree.begin(), tree.end());
	}
	virtual iterator insert(iterator& in, const_iterator& begin, const_iterator& end)
	{
		if (begin == end)
			return in;
		const_iterator it = begin;
		iterator r = insert_(in, *it);
		size_t index = r.sibling_index();
		size_t indent = r.tree_indent();
		size_t insert = r.m_index;
		while (++ it != end)
		{
			++ insert;
			size_t idx = it.tree_indent() == begin.tree_indent() ? ++ index: it.sibling_index();
			size_t idt = it.tree_indent() - begin.tree_indent() + indent;
			tree_iterator i = m_trunk.insert(m_trunk.begin() + insert, node(node_traits(this, idt, idx), *it));
			while (++ i != m_trunk.end())
			{
				if (i->first.m_indent < in.tree_indent())
					break;
				else if (i->first.m_indent == in.tree_indent())
					++ i->first.m_sibling_index;
			}
		}
		return r;
	}
	void join(const tree& tree)
	{
		size_t idt = size() ? begin().last_sibling().sibling_index(): 0;
		for (tree_trunk::const_iterator i = tree.m_trunk.begin(); i != tree.m_trunk.end(); ++ i)
		{
			m_trunk.push_back(*i);
			if (m_trunk.back().first.m_indent == 0)
				m_trunk.back().first.m_sibling_index = ++ idt;
		}
	}
	tree split(iterator it)
	{
		tree ret;
		size_t idx = it.is_end() ? 0: it.sibling_index();
		size_t idt = it.is_end() ? 0: it.tree_indent();
		tree_trunk::iterator i = m_trunk.begin() + it.absolute_index();
		bool f = true;
		while (i != m_trunk.end())
		{
			ret.m_trunk.push_back(*i);
			i = m_trunk.erase(i);
			if (f)
			{
				tree_trunk::iterator r = ret.m_trunk.end() - 1;
				if (r->first.m_indent < idt)
					f = false;
				else if (r->first.m_indent == idt)
					r->first.m_sibling_index -= idx;
			}
		}
		return ret;
	}
	virtual iterator add(iterator& parent, const T& Value)
	{
		return add_(parent, Value);
	}
	virtual iterator add(iterator& parent, size_t count)
	{
		if (count == 0)
			return end();
		iterator r = add_(parent, T());
		size_t idx = r.sibling_index();
		size_t idt = r.tree_indent();
		node v(node_traits(this, idt, idx), T());
		m_trunk.insert(m_trunk.begin() + r.m_index + 1, count - 1, v);
		for (size_t l = r.m_index + 1; l < r.m_index + count; ++ l)
			m_trunk[l].first.m_sibling_index = ++ idx;
		return r;
	}
	virtual const_iterator add(const_iterator& parent, const T& value)
	{
		return add_(parent, value);
	}
	virtual const_iterator add(const_iterator& parent, size_t count)
	{
		if (count == 0)
			return end();
		const_iterator r = add_(parent, T());
		size_t idx = r.sibling_index();
		size_t idt = r.tree_indent();
		node v(node_traits(this, idt, idx), T());
		m_trunk.insert(m_trunk.begin() + r.m_index + 1, count - 1, v);
		for (size_t l = r.m_index + 1; l < r.m_index + count; ++ l)
			m_trunk[l].first.m_sibling_index = ++ idx;
		return r;
	}
	virtual iterator insert(iterator& it, const T& value)
	{
		return insert_(it, value);
	}
	virtual iterator insert(iterator& it, size_t count)
	{
		if (count == 0)
			return end();
		iterator r = insert_(it, T());
		size_t idx = r.sibling_index();
		size_t idt = r.tree_indent();
		node v(node_traits(this, idt, idx), T());
		m_trunk.insert(m_trunk.begin() + r.m_index + 1, count - 1, v);
		for (size_t l = r.m_index + 1; l < r.m_index + count; ++ l)
			m_trunk[l].first.m_sibling_index = ++ idx;
		return r;
	}
	virtual const_iterator insert(const_iterator& it, const T& value)
	{
		return insert_(it, value);
	}
	virtual const_iterator insert(const_iterator& it, size_t count)
	{
		if (count == 0)
			return end();
		const_iterator r = insert_(it, T());
		size_t idx = r.sibling_index();
		size_t idt = r.tree_indent();
		node v(node_traits(this, idt, idx), T());
		m_trunk.insert(m_trunk.begin() + r.m_index + 1, count - 1, v);
		for (size_t l = r.m_index + 1; l < r.m_index + count; ++ l)
			m_trunk[l].first.m_sibling_index = ++ idx;
		return r;
	}
	virtual void clear(void)
	{
		m_trunk.clear();
	}
	virtual iterator erase(iterator& i)
	{
		if (empty())
			return end();
		size_t idt = i.tree_indent();
		iterator r = i + 1;
		while (! r.is_end() && r.tree_indent() > i.tree_indent())
			++ r;
		while (! empty() && !(-- r).is_end() && r >= i)
			m_trunk.erase(m_trunk.begin() + r.absolute_index());
		r = i;
		while (! empty() && ! r.is_end())
		{
			if (r.tree_indent() < idt)
				break;
			else if (r.tree_indent() == idt)
				r.sibling_index(r.sibling_index() - 1);
			++ r;
		}
		return i;
	}
	virtual void erase_child(iterator& i)
	{
		if (empty())
			return;
		iterator r = i.first_child();
		while (!r.is_end())
		{
			erase(r);
			r = i.first_child();
		}
	}
	void inc_indent(iterator it)
	{
		if (it.sibling_index() == 0)
			return;
		size_t idt = it.tree_indent();
		size_t idx = it.prev_sibling().count_child() == 0 ? 0: it.prev_sibling().last_child().sibling_index() + 1;
		it.tree_indent(idt + 1);
		it.sibling_index(idx);
		iterator i = it;
		while (++ i != end())
		{
			if (i.tree_indent() <= idt)
				break;
			i.tree_indent(i.tree_indent() + 1);
		}
		i = it.next_sibling();
		while (++ i != end())
		{
			i.sibling_index(++ idx);
			i.move_next_sibling();
		}
	}
	void dec_indent(iterator it)
	{
		size_t idt = it.tree_indent();
		if (idt == 0)
			return;
		size_t idx = it.parent().sibling_index();
		it.tree_indent(idt - 1);
		it.sibling_index(++ idx);
		iterator i = it;
		while (++ i != end())
		{
			if (i.tree_indent() <= idt)
				break;
			i.tree_indent(i.tree_indent() - 1);
		}
		i = it.first_child();
		idx = 0;
		while (++ i != end())
		{
			i.sibling_index(idx ++);
			i.move_next_sibling();
		}
	}
	iterator begin(void) { return iterator(this, 0); }
	iterator end(void) { return iterator(this, m_trunk.size()); }
	const_iterator begin(void) const { return const_iterator(this, 0); }
	const_iterator end(void) const { return const_iterator(this, m_trunk.size()); }
};

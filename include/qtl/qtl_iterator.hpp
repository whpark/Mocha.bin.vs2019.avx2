///////////////////////////////////////////////////////////////////////////////
//
// qtl_iterator.hpp
//
//			QTL : (Q)QRS (T)Template (L)Library
//
//
// helper classes for *PtrContainer
//
//
// 2017.07.20.
//
// PWH
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "default.defines.h"

#pragma pack(push, 8)
namespace qtl {	// Q(QRS) T(Template) L(library)

	//--------------------------------------------------------------------------------------------------------------------------------------------------------
	// Iterators ( iterator, const_iterator, reverse_iterator, const_reverse_iterator )
	template < class base_iterator, class owner_t, class converter >
	class t_iterator : public base_iterator {
	public:
		typedef t_iterator(owner_t::*iter_func)();
	private:
		owner_t& owner;
		mutable iter_func endp;
	public:
		t_iterator end() { return (owner.*endp)(); }

	public:
		typedef t_iterator					this_t;
		typedef typename owner_t::type_t	T;
		typedef typename owner_t::index_t	index_t;

		using value_type = typename owner_t::type_t;

	public:
		//---------------------------------------------------------------------
		// Constructors,
		using base_iterator::base_iterator;
		t_iterator() = delete;
		t_iterator(owner_t& _owner, iter_func _endp) : owner(_owner), end(_endp), base_iterator() {}
		t_iterator(const t_iterator& ) = default;
		t_iterator(t_iterator&& ) = default;
		t_iterator(owner_t& _owner, iter_func _endp, typename base_iterator iter) : owner(_owner), endp(_endp), base_iterator(iter) {}

		// Check End ( == NULL )
		bool IsEnd()	const { return *this == (owner.*endp)(); }
		bool IsNotEnd()	const { return *this != (owner.*endp)(); }

		//---------------------------------------------------------------------
		// operators
		this_t& operator =  (base_iterator iter)				{ *(base_iterator*)this = iter; return *this; }
		this_t& operator =  (this_t iter)						{ *(base_iterator*)this = iter; return *this; }
		this_t& operator ++ ()									{ base_iterator::operator ++ (); return *this; }
		this_t& operator -- ()									{ base_iterator::operator -- (); return *this; }
		this_t  operator ++ (int)								{ this_t iter_backup = *this; base_iterator::operator ++ (); return iter_backup; }
		this_t  operator -- (int)								{ this_t iter_backup = *this; base_iterator::operator -- (); return iter_backup; }
		this_t& operator += (index_t offset)					{ base_iterator::operator += (offset); return *this; }
		this_t& operator -= (index_t offset)					{ base_iterator::operator -= (offset); return *this; }
		this_t  operator +  (index_t offset) const				{ this_t iter(*this); iter = base_iterator::operator + (offset); return iter; }
		this_t  operator -  (index_t offset) const				{ this_t iter(*this); iter = base_iterator::operator - (offset); return iter; }
		index_t     operator -  (const this_t& B) const			{ return base_iterator::operator - (B); }

		//auto operator <=> (const this_t& B) const				{ return base_iterator::operator <=> (B); }

			  T& operator * () const									{ return *converter::GetPtr(__super::operator*()); }
		//const T& operator * () const							{ return *converter::GetPtr(__super::operator*()); }
			  T* operator -> () const									{ return converter::GetPtr(__super::operator*()); }
		//const T* operator -> () const							{ return converter::GetPtr(__super::operator*()); }

		__if_exists(converter::GetPtrWrap) {
			typedef typename owner_t::smart_ptr_t smart_ptr_t;

			//operator	   smart_ptr_t& ()							{ return converter::GetPtrWrap(__super::operator*()); }
			//operator const smart_ptr_t& () const					{ return converter::GetPtrWrap(__super::operator*()); }
				  smart_ptr_t& GetPtrWrap()							{ return converter::GetPtrWrap(__super::operator*()); }
			const smart_ptr_t& GetPtrWrap() const					{ return converter::GetPtrWrap(__super::operator*()); }
		}

		operator	   T* ()									{ return *this != (owner.*endp)() ? converter::GetPtr(__super::operator*()) : nullptr; }
		operator const T* () const								{ return *this != (owner.*endp)() ? converter::GetPtr(__super::operator*()) : nullptr; }

			  T*	  GetPtr()									{ return *this != (owner.*endp)() ? converter::GetPtr(__super::operator*()) : nullptr; }
		const T*	  GetPtr() const							{ return *this != (owner.*endp)() ? converter::GetPtr(__super::operator*()) : nullptr; }
		template < class T_CAST >
			  T_CAST* GetPtr()									{ return dynamic_cast<T_CAST*>(GetPtr()); }
		template < class T_CAST >
		const T_CAST* GetPtr() const							{ return dynamic_cast<const T_CAST*>(GetPtr()); }

		T& GetRef() {
			T* p = GetPtr();
			if (!p)
				throw std::exception(std::invalid_argument(__FMSG("nullptr (GetRef)")));
			return *p;
		}
		const T& GetRef() const {
			const T* p = GetPtr();
			if (!p)
				throw std::exception(std::invalid_argument(__FMSG("nullptr (GetRef const)")));
			return *p;
		}
		template < class T_CAST >
		T_CAST& GetRef() {
			T_CAST* p = GetPtr<T_CAST>();
			if (!p)
				throw std::exception(std::invalid_argument(__FMSG("nullptr ((Casting) GetRef)")));
			return *p;
		}
		template < class T_CAST >
		const T_CAST& GetRef() const {
			const T_CAST* p = GetPtr<T_CAST>();
			if (!p)
				throw std::exception(std::invalid_argument(__FMSG("nullptr ((Casting) GetRef const)")));
			return *p;
		}

	};
	template < class base_iterator, class owner_t, class converter >
	void iter_swap(t_iterator<base_iterator, owner_t, converter> _Left, t_iterator<base_iterator, owner_t, converter> _Right) {
		// swap *_Left and *_Right
		auto c = std::move(_Left.GetPtrWrap());
		_Left.GetPtrWrap() = std::move(_Right.GetPtrWrap());
		_Right.GetPtrWrap() = std::move(c);
	}

	//--------------------------------------------------------------------------------------------------------------------------------------------------------
	// const iterator
	template < class base_iterator, class owner_t, class converter >
	class t_citerator : public base_iterator {
		typedef t_citerator(owner_t::*iter_func)() const;
		const owner_t& owner;
		iter_func endp;
	public:
		t_citerator end() const { return (owner.*endp)(); }
	public:
		typedef t_citerator					this_t;
		typedef typename owner_t::type_t	T;
		typedef typename owner_t::index_t	index_t;

	public:
		using base_iterator::base_iterator;
		t_citerator() = delete;
		t_citerator(const owner_t& _owner, iter_func _endp) : owner(_owner), end(_endp), base_iterator() {}
		t_citerator(const t_citerator& ) = default;
		t_citerator(t_citerator&& ) = default;
		t_citerator(const owner_t& _owner, iter_func _endp, typename base_iterator iter) : owner(_owner), endp(_endp), base_iterator(iter) {}

		// Check End ( == NULL )
		bool IsEnd()	const { return *this == (owner.*endp)(); }
		bool IsNotEnd()	const { return *this != (owner.*endp)(); }

		// operators
		this_t& operator =  (base_iterator iter)				{ *(base_iterator*)this = iter; return *this; }
		this_t& operator =  (this_t iter)						{ *(base_iterator*)this = iter; return *this; }
		this_t& operator ++ ()									{ base_iterator::operator ++ (); return *this; }
		this_t& operator -- ()									{ base_iterator::operator -- (); return *this; }
		this_t  operator ++ (int)								{ this_t iter_backup = *this; base_iterator::operator ++ (); return iter_backup; }
		this_t  operator -- (int)								{ this_t iter_backup = *this; base_iterator::operator -- (); return iter_backup; }
		this_t& operator += (index_t offset)					{ base_iterator::operator += (offset); return *this; }
		this_t& operator -= (index_t offset)					{ base_iterator::operator -= (offset); return *this; }
		this_t operator + (index_t offset) const				{ this_t iter(*this); iter += offset; return iter; }
		this_t operator - (index_t offset) const				{ this_t iter(*this); iter -= offset; return iter; }
		index_t		operator - (const this_t& B) const			{ return base_iterator::operator - (B); }

		auto operator <=> (const this_t& B) const				{ return base_iterator::operator <=> (B); }

		const T& operator * () const							{ return *converter::GetPtr(__super::operator*()); }
		const T* operator -> () const							{ return converter::GetPtr(__super::operator*()); }
		//const T& operator * () const							{ return *((__super::operator*()).get()); }
		//const T* operator -> () const							{ return (__super::operator*()).get(); }

		__if_exists(converter::GetPtrWrap) {
			typedef typename owner_t::smart_ptr_t smart_ptr_t;
			operator const smart_ptr_t& () const					{ return converter::GetPtrWrap(__super::operator*()); }
			const smart_ptr_t& GetPtrWrap() const					{ return converter::GetPtrWrap((smart_ptr_t&)(__super::operator*())); }
		}

		operator const T* () const								{ return *this != (owner.*endp)() ? converter::GetPtr(__super::operator*()) : nullptr; }

		const T*	  GetPtr() const							{ return *this != (owner.*endp)() ? converter::GetPtr(__super::operator*()) : nullptr; }
		template < class T_CAST >
		const T_CAST* GetPtr() const							{ return dynamic_cast<const T_CAST*>(GetPtr()); }

		const T& GetRef() const {
			const T* p = GetPtr();
			if (!p)
				throw std::exception(std::invalid_argument(__FMSG("nullptr (GetRef const)")));
			return *p;
		}
		template < class T_CAST >
		const T_CAST& GetRef() const {
			const T_CAST* p = GetPtr<T_CAST>();
			if (!p)
				throw std::exception(std::invalid_argument(__FMSG("nullptr ((Casting) GetRef const)")));
			return *p;
		}

	};


}	// namespace qtl
#pragma pack(pop)

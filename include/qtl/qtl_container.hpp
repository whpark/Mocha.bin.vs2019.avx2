#pragma once

///////////////////////////////////////////////////////////////////////////////
//
// qtl.hpp
//
//			QTL : (Q)RS (T)emplate (L)ibrary
//
//
// extensions of std::containers, ... etc.
//
// 2017.04.23. container : vector<unique_ptr> 추가
// 2017.07.24. container : Sort 제외 IList 기능들 추가.
// 2017.07.28. container : 1 차 완료 예정.
// 2018.03.10. Move Constructor 추가
//             TSmartPtr... --> TUniquePtr... / TSharedPtr... 로 세분화
// 2018.04.01. operator == 추가.
//
// PWH
//
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/*

1. std:: container base (TSmartPtrContainer)

	TSmartPtrVector	: std::vector
	TSmartPtrList	: std::list
	TSmartPtrDeque	: std::deque

	- vector, list 등에 smart pointer 를 합성하여 만든 class.
	- vecotr<unique_ptr<T>> 이런 형태로 사용할 경우,


				vector<unique_ptr<int>> container;
				container.push_back(unique_ptr<int>(new int(3)));		// emplace_back 이 좀 더 간단... container.emplace_back(new int(3));
				container.push_back(unique_ptr<int>(new int(4)));
				....
				container.push_back(unique_ptr<int>(new int(3)));
				container.push_back(unique_ptr<int>(new int(2)));

				for (int i = 0; i < size(); i++) {
					int value = *container[i].get();		// 이런식으로 복잡해진다.
				}

		기본적으로 T == unique_ptr<type> 이 되므로, 소스코드가 좀 복잡해진다.
		T == type 이 되도록 container class 를 확장 함.


				TSmartPtrVector<int> container;
				container.Push(new int(3));
				container.Push(new int(4));
				....
				container.Push(new int(3));
				container.Push(new int(2));

				for (int i = 0; i < size(); i++) {
					int value = container[i];		// T == type 이므로 그대로 사용하면 됨. pointer 를 추가했지만, reference 를 그대로 사용할 수 있음.
				}
		또한, thread-safe 아님.


2. boost::ptr_container base (TPtrContainer)

	TPtrVector	: boost::ptr_vector
	TPtrList	: boost::ptr_list
	TPtrDeque	: boost::ptr_deque

	- std::container<unique_ptr<T>>로 사용하는 것이, boost에서ptr_container 로 확장되어 있다.


				ptr_vector<int> container;
				container.push_back(new int(3));
				container.push_back(new int(4));
				...
				container.push_back(unique_ptr<int>(new int(3)));
				container.push_back(unique_ptr<int>(new int(2)));

				for (int i = 0; i < size(); i++) {
					int value = container[i];		// T == type.
				}


		type == void* 이므로, debug 할 때, 개체를 볼 수 가 없음. 또한 thread-safe 구현하려면, CriticalSection을 걸어줘야 함.

	- TPtrVector, TPtrList, TPtrDeque 등으로 확장함.



3. 기본 함수들 :


	(1) Insert
		- Push			: Container 의 마지막에 개체 추가		ex. { container.Push( new CShapeLine() ); }
		- PushFront		: Container 의 첫 부분에 개체 추가		ex. { container.PushFront( new CShapeLine() ); }
		- InsertAt		: 특정 위치에 개체 추가.				ex. { container.InsertAt( new CShapeLine() ); }


	(2) Remove (Pop/...) :
		- 기본 return type : SmartPtrContainer(std) 일 경우, unique_ptr<> 또는 shared_ptr<>이고,
							PtrContainer(boost) 일 경우, unqiue_ptr<> 임. (원래, boost 는 auto_ptr<>을 리턴함. auto_ptr 은 사용하지 않으므로 unique_ptr<>로 변경)
							smart pointer 를 사용하므로, T* 를 return 할 수는 없음.

				CShapeObject* pObject			 = container.Pop();	// compile 에러 발생함.
				unique_ptr<CShapeObject> rObject = container.Pop(); // ok
				auto rObject					= contaeinr.PopFront();	// ok. auto --> unique_ptr<CShapeObject> ( 또는 shared_ptr<CShapeObject> )


		- Pop			: Conatiner 의 마지막 개체를 꺼내서 리턴. empty일 경우, null return.
		- PopFront		: Container 의 첫 개체를 꺼내서 리턴. empty일 경우, null return.
		- RemoveAt		: Container 의 특정 위치에서...

		- DeleteAll		: 전체 삭제. (shared_ptr 일 경우, reference_count 가 0일 경우에만 지워짐.)


	(3) Iteration

		- index, iterator 를 사용한 iteration 가능.

			ex 1.

				for (int i = 0; i < container.size(); i++) {

					CShapeObject& obj = container[i];

					CShapeLine& line = container.GetRef<CShapeLine>(i);	// CShapeObject -> CShapeLine 으로 형 변형(Type Casting). RTTI 켜져 있을 경우, throw exception.
					
				}

			ex 2.

				for (auto iter = container.begin(); iter != container.end(); iter++) {

					iter->m_pts.AddPoint(CPoint2d(0, 0));

				}


		- vector 는 index, list 는 iterator 를 사용하는게 성능상 유리. (list 에서 index 사용할 경우, 심각하게 느려질 수 있음)
		- iterator 는 T* 처럼 사용 가능.

				auto iter = container.beging();

				CShapeObject& object = *iter;
				CShapeLine& line = iter.GetRef<CShapeLine>();	// CShapeObject -> CShapeLine 으로 형 변환.

		- GetIterator(int iIndex)  : iIndex 를 iterator 로 환산하여 리턴.
									(iIndex == 0) 열 경우 -> begin()
									(iIndex < 0) 일 경우 -> end()

		- iterator, const_iterator, reverse_iterator, const_reverse_iterator 지원.
				iterator				: begin(), end()
				const_iterator			: cbegin(), cend()
				reverse_iterator		: rbegin(), rend()
				const_reverse_iterator	: crbegin(), crend()

		- iterator 에서 바로 형 변환 가능.

				TSmartPtrVector<CShapeObject> container;
				auto iter = container.begin();

				CShapeObject* pObject	= iter.GetPtr();
				CShapeLine* pLine		= iter.GetPtr<CShapeLine>();	// RTTI 일 경우, 개체가 CShapeLine 이 아니면, nullptr 리턴함.
				CShapeObject& object	= iter.GetRef();
				CShapeLine& line		= iter.GetRef<CShapeLine>();	// RTTI 일 경우, 개체가 CShapeLine 이 아니면, exception 발생.

				CShapeObject& object	= *iter;	// GetRef() 대신 *iter 로 써도 됨.


	(4) Find : 

		- FindItemIter  : iterator 사용.
		- FindItem		: index 사용.


	(5) _Sort : TList 에 있던 Sort 를 그대로 사용. (insertion_sort : by KMS)


	(6) Serialize : TList 와 호환 되도록 구현. TList 에서 저장 한 것을 읽어 올 수 있고, vice versa.



*/
///////////////////////////////////////////////////////////////////////////////





#include <list>
#include <array>
#include <vector>
#include <queue>
#include <deque>
#include <memory>
#include <mutex>
#include <atomic>


#include "qtl_lock.hpp"
#include "qtl_iterator.hpp"
//#include "boost_tptr_container.hpp"

#pragma pack(push, 8)
namespace qtl {	// Q(QRS) T(Template) L(library)


	//============================================================================================================================================================
	//============================================================================================================================================================
	// TSmartPtrContainer
	template < class T, class container, class smart_ptr >
	class TSmartPtrContainer : protected container, public CSmartLock {
	public:
		typedef container	container_t;
		typedef T			type_t;
		typedef smart_ptr	smart_ptr_t;
		typedef intptr_t	index_t;	// access with 0-base index

		TSmartPtrContainer() : container() {};
		//TSmartPtrContainer(std::initializer_list<T*> list) {
		//	for (const auto p : list) {
		//		Push(p);
		//	}
		//};
		virtual ~TSmartPtrContainer() {
			//container_t::DeleteAll();
		}

	public:
		template < class T, class smart_ptr >
		class TConverterSmartPtr {
		public:
			static smart_ptr& GetPtrWrap(smart_ptr& r)	{ return r; }
			static		 T* GetPtr(smart_ptr& r)		{ return r.get(); }
			static const T* GetPtr(const smart_ptr& r) 	{ return r.get(); }
		};

		typedef t_iterator<typename container_t::iterator, TSmartPtrContainer, TConverterSmartPtr<T, smart_ptr>>				iterator;
		typedef t_citerator<typename container_t::const_iterator, TSmartPtrContainer, TConverterSmartPtr<T, smart_ptr>>			const_iterator;
		typedef t_iterator<typename container_t::reverse_iterator, TSmartPtrContainer, TConverterSmartPtr<T, smart_ptr>>		reverse_iterator;
		typedef t_citerator<typename container_t::const_reverse_iterator, TSmartPtrContainer, TConverterSmartPtr<T, smart_ptr>>	const_reverse_iterator;

	public:
		T& operator [] (index_t iIndex) requires requires (container_t a) { a[0]; } {
			smart_ptr_t& r = container_t::operator[](iIndex);
			return *r.get();
		}
		T& operator [] (index_t iIndex) requires (! requires (container_t a) { a[0]; } ) {
			return *GetIterator(iIndex);
		}
		const T& operator [] (index_t iIndex) const requires requires (container_t a) { a[0]; } {
			const smart_ptr_t& r = container_t::operator[](iIndex);
			return *r.get();
		}
		const T& operator [] (index_t iIndex) const requires (! requires (container_t a) { a[0]; } ) {
			return *GetIterator(iIndex);
		}


		//--------------------------------------------------------------------------------------------------------------------------------------------------------
		// CopyShareTo
		template < class t_other_container >
		bool CopyShareTo(t_other_container& B) const {		// 주의! DeadLock 걸릴 수 있음... shared_ptr 만 사용 가능.
			_LOCK_GUARD_THIS_;
			__if_exists(t_other_container::Lock) { B.Lock(); }
			B.DeleteAll();
			bool bResult = AddShareTo(B);
			__if_exists(t_other_container::Lock) { B.Unlock(); }
			return bResult;
		}
		template < class t_other_container >
		bool AddShareTo(t_other_container& B) const {		// 주의! DeadLock 걸릴 수 있음... shared_ptr 만 사용 가능.
			_LOCK_GUARD_THIS_;

			if (container_t::empty())
				return true;

			__if_exists(t_other_container::Lock) { B.Lock(); }
			if constexpr (has_Reserve<t_other_container>) {
				B.Reserve(container_t::size() + B.size());
			}

			const auto end = cend();
			for (auto iter = cbegin(); iter != end; iter++) {
				// shared_ptr<T> 이 아닐경우 AddShareTo 사용할 수 없음.
				B.Push(iter.GetPtrWrap());
			}

			__if_exists(t_other_container::Lock) { B.Unlock(); }

			return true;
		}

		//--------------------------------------------------------------------------------------------------------------------------------------------------------
		// Get Iterator (index -> iterator)
		// TPContainerWorker 에서 Virtual 로 선언할 수 도 있지만.....속도 때문에 virtual 로 하지 않음.
	protected:
		template <typename iterator_t>
		iterator_t TGetIterator(index_t iIndex, typename iterator_t::iter_func begin_f, typename iterator_t::iter_func end_f)
			requires (requires (iterator_t iter) { iter + 1; })
		{
			if (iIndex == 0)
				return (this->*begin_f)();
			if (iIndex < 0)
				return (this->*end_f)();
			return (this->*begin_f)()+iIndex;
		}
		template <typename iterator_t>
		iterator_t TGetIterator(index_t iIndex, typename iterator_t::iter_func begin_f, typename iterator_t::iter_func end_f)
			requires (! requires (iterator_t iter) { iter + 1; } and requires (iterator_t iter) { iter += 1; })
		{
			if (iIndex == 0)
				return (this->*begin_f)();
			if (iIndex < 0)
				return (this->*end_f)();

			auto pos = (this->*begin_f)();

			pos += iIndex;
			return pos;
		}
		template <typename iterator_t>
		iterator_t TGetIterator(index_t iIndex, typename iterator_t::iter_func begin_f, typename iterator_t::iter_func end_f)
			requires (! requires (iterator_t iter) { iter + 1; } and ! requires (iterator_t iter) { iter += 1; })
		{
			if (iIndex == 0)
				return (this->*begin_f)();
			if (iIndex < 0)
				return (this->*end_f)();

			auto pos = (this->*begin_f)();

			for (index_t i = 0; i < iIndex; i++)
				pos++;

			return pos;
		}

		template <typename iterator_t>
		iterator_t TGetIterator(index_t iIndex, typename iterator_t::iter_func begin_f, typename iterator_t::iter_func end_f) const
			requires (requires (iterator_t iter) { iter + 1; })
		{
			if (iIndex == 0)
				return (this->*begin_f)();
			if (iIndex < 0)
				return (this->*end_f)();
			return (this->*begin_f)()+iIndex;
		}
		template <typename iterator_t>
		iterator_t TGetIterator(index_t iIndex, typename iterator_t::iter_func begin_f, typename iterator_t::iter_func end_f) const
			requires (! requires (iterator_t iter) { iter + 1; } and requires (iterator_t iter) { iter += 1; })
		{
			if (iIndex == 0)
				return (this->*begin_f)();
			if (iIndex < 0)
				return (this->*end_f)();

			auto pos = (this->*begin_f)();
			pos += iIndex;
			return pos;
		}
		template <typename iterator_t>
		iterator_t TGetIterator(index_t iIndex, typename iterator_t::iter_func begin_f, typename iterator_t::iter_func end_f) const
			requires (! requires (iterator_t iter) { iter + 1; } and ! requires (iterator_t iter) { iter += 1; })
		{
			if (iIndex == 0)
				return (this->*begin_f)();
			if (iIndex < 0)
				return (this->*end_f)();

			auto pos = (this->*begin_f)();

			for (index_t i = 0; i < iIndex; i++)
				pos++;

			return pos;
		}

	public:
		iterator GetIterator(index_t iIndex) {
			return TGetIterator<iterator>(iIndex, &TSmartPtrContainer::begin, &TSmartPtrContainer::end);
		}
		const_iterator GetIterator(index_t iIndex) const {
			return TGetIterator<const_iterator>(iIndex, &TSmartPtrContainer::cbegin, &TSmartPtrContainer::cend);
		}
		reverse_iterator GetRevIterator(index_t iIndex) {
			return TGetIterator<reverse_iterator>(iIndex, &TSmartPtrContainer::rbegin, &TSmartPtrContainer::rend);
		}
		const_reverse_iterator GetRevIterator(index_t iIndex) const {
			return TGetIterator<const_reverse_iterator>(iIndex, &TSmartPtrContainer::crbegin, &TSmartPtrContainer::crend);
		}


		//--------------------------------------------------------------------------------------------------------------------------------------------------------
		// Push (Insert)
	public:
		virtual bool InsertAt(T* pData, index_t iIndex = -1) {
			return InsertAt(smart_ptr_t(pData), GetIterator(iIndex));
		}
		virtual bool InsertAt(smart_ptr_t rData, index_t iIndex = -1) {
			return InsertAt(std::move(rData), GetIterator(iIndex));
		}
		virtual bool InsertAt(smart_ptr_t rData, iterator iter) {
			_LOCK_GUARD_THIS_;

			if (iter == end()) {
				__if_exists(container_t::emplace_back) {
					container_t::emplace_back(std::move(rData));
					return true;
				}
				__if_exists(container_t::push_back) {
					container_t::push_back(std::move(rData));
					return true;
				}
			}

			if (iter == begin()) {
				__if_exists(container_t::emplace_front) {
					container_t::emplace_front(std::move(rData));
					return true;
				}
				__if_exists(container_t::push_front) {
					container_t::push_front(std::move(rData));
					return true;
				}
			}

			__if_exists(container_t::emplace) {
				container_t::emplace(iter, std::move(rData));
				return true;
			}

			__if_exists(container_t::insert) {
				container_t::insert(iter, std::move(rData));
				return true;
			}

			return false;
		}


		virtual bool Push(smart_ptr_t rData) {
			_LOCK_GUARD_THIS_;
			__if_exists(container_t::emplace_back) {
				container_t::emplace_back(std::move(rData));
				return true;
			}
			__if_exists(container_t::push_back) {
				container_t::push_back(std::move(rData));
				return true;
			}
			__if_not_exists(container_t::push_back) {
				return InsertAt(std::move(rData), end());
			}
		}
		virtual bool PushFront(smart_ptr_t rData) {
			_LOCK_GUARD_THIS_;
			__if_exists(container_t::emplace_front) {
				container_t::emplace_front(std::move(rData));
				return true;
			}
			__if_exists(container_t::push_front) {
				container_t::push_front(std::move(rData));
				return true;
			}
			__if_not_exists(container_t::push_front) {
				return InsertAt(std::move(rData), begin());
			}
		}

		virtual bool Push(T* pData) { return Push(smart_ptr_t(pData)); }
		virtual bool PushFront(T* pData) { return PushFront(smart_ptr_t(pData)); }

		//--------------------------------------------------------------------------------------------------------------------------------------------------------
		// Pop (Remove)
	public:
		virtual smart_ptr_t RemoveAt(index_t iIndex = -1) {
			return RemoveAt(GetIterator(iIndex));
		}
		virtual smart_ptr_t RemoveAt(iterator iter) {
			_LOCK_GUARD_THIS_;

			if (container_t::empty())
				return smart_ptr_t(nullptr);

			__if_exists(container_t::pop_back) {
				if ( (iter == end()) || (iter+1 == end()) ) {
					smart_ptr_t r(std::move(__super::back()));
					container_t::pop_back();
					return std::move(r);
				}
			}

			__if_exists(container_t::pop_front) {
				if (iter == begin()) {
					smart_ptr_t r(std::move(__super::front()));
					container_t::pop_front();
					return std::move(r);
				}
			}

			__if_exists(container_t::erase) {
				smart_ptr_t r(std::move(iter.GetPtrWrap()));
				container_t::erase(iter);
				return std::move(r);
			}

			return smart_ptr_t(nullptr);
		}

		virtual smart_ptr_t Pop() {
			_LOCK_GUARD_THIS_;

			if (container_t::empty())
				return smart_ptr_t(nullptr);

			__if_exists(container_t::pop_back) {
				smart_ptr_t r(std::move(__super::back()));
				container_t::pop_back();
				return std::move(r);
			}
			__if_not_exists(container_t::pop_back) {
				return RemoveAt(end());
			}
		}
		virtual smart_ptr_t PopFront() {
			_LOCK_GUARD_THIS_;

			if (container_t::empty())
				return smart_ptr_t(nullptr);

			__if_exists(container_t::pop_front) {
				smart_ptr_t r(std::move(__super::front()));
				container_t::pop_front();
				return std::move(r);
			}
			__if_not_exists(container_t::pop_front) {
				return RemoveAt(begin());
			}
		}
		virtual smart_ptr_t Pop(index_t iIndex)	{ return RemoveAt(GetIterator(iIndex)); }
		virtual smart_ptr_t Pop(iterator iter)	{ return RemoveAt(iter); }

		__if_exists(container_t::erase) {
			virtual bool Delete(index_t iIndex) {
				return Delete(GetIterator(iIndex));
			}
			virtual bool Delete(iterator iter) {
				_LOCK_GUARD_THIS_;
				if (container_t::empty() || (iter == end()))
					return false;
				container_t::erase((typename container_t::iterator&)iter);
				return true;
			}
		}

	public:
		void DeleteAll() {
			_LOCK_GUARD_THIS_;

			__if_exists(container_t::clear) {
				container_t::clear();
				return;
			}

			__if_exists(container_t::erase) {
				container_t::erase(begin(), end());
				return;
			}

			__if_exists(container_t::pop_back) {
				while (container_t::size())
					container_t::pop_back();
				return;
			}

			__if_not_exists(container_t::clear) {
				__if_not_exists(container_t::erase) {
					__if_not_exists(container_t::pop_back) {
						static_assert(false, "Container has no clear function");
					}
				}
			}
		}


		//--------------------------------------------------------------------------------------------------------------------------------------------------------
		// etc
	public:

#if 1 //(backup)
#define qtl__REQUIRE__(expr) requires (requires (container_t c) { c.expr; } )

		void Reserve(size_t nSize) qtl__REQUIRE__(reserve(1)) {
			_LOCK_GUARD_THIS_;
			container_t::reserve(nSize);
		}

		iterator				Begin()											{ return std::move(iterator(*this, &TSmartPtrContainer::end, __super::begin())); }
		iterator				End()											{ return std::move(iterator(*this, &TSmartPtrContainer::end, __super::end())); }

		iterator				begin()											{ return std::move(iterator(*this, &TSmartPtrContainer::end, __super::begin())); }
		iterator				end()											{ return std::move(iterator(*this, &TSmartPtrContainer::end, __super::end())); }
		reverse_iterator		rbegin()			qtl__REQUIRE__(rbegin())	{ return std::move(reverse_iterator(*this, &TSmartPtrContainer::rend, __super::rbegin())); }
		reverse_iterator		rend()				qtl__REQUIRE__(rend())		{ return std::move(reverse_iterator(*this, &TSmartPtrContainer::rend, __super::rend())); }
		const_iterator			begin()		const								{ return std::move(const_iterator(*this, &TSmartPtrContainer::end, __super::begin())); }
		const_iterator			end()		const								{ return std::move(const_iterator(*this, &TSmartPtrContainer::end, __super::end())); }
		const_reverse_iterator	rbegin()	const	qtl__REQUIRE__(rbegin())	{ return std::move(const_reverse_iterator(*this, &TSmartPtrContainer::crend, __super::rbegin())); }
		const_reverse_iterator	rend()		const	qtl__REQUIRE__(rend())		{ return std::move(const_reverse_iterator(*this, &TSmartPtrContainer::crend, __super::rend())); }
		const_iterator			cbegin()	const								{ return std::move(const_iterator(*this, &TSmartPtrContainer::end, __super::begin())); }
		const_iterator			cend()		const								{ return std::move(const_iterator(*this, &TSmartPtrContainer::end, __super::end())); }
		const_reverse_iterator	crbegin()	const	qtl__REQUIRE__(rbegin())	{ return rbegin(); }
		const_reverse_iterator	crend()		const	qtl__REQUIRE__(rend())		{ return rend(); }
		void ShrinkToFit()							qtl__REQUIRE__(shrink_to_fit) { _LOCK_GUARD_THIS_; container_t::shrink_to_fit(); }

		__if_exists(container_t::size)					{ using container_t::size; }
		__if_exists(container_t::max_size)				{ using container_t::max_size; }
		__if_exists(container_t::empty)					{ using container_t::empty; }
		__if_exists(container_t::at)					{ using container_t::at; }
		__if_exists(container_t::data)					{ using container_t::data; }

				T&	front()							qtl__REQUIRE__(front())		{ return *(container_t::front()); }
		const T&	front() const					qtl__REQUIRE__(front())		{ return *(container_t::front()); }
				T&	back()							qtl__REQUIRE__(back())		{ return *(container_t::back()); }
		const T&	back() const					qtl__REQUIRE__(back())		{ return *(container_t::back()); }

#undef qtl__REQUIRE__
		//__if_exists(container_t::front)					{ using container_t::front; }
		//__if_exists(container_t::back)					{ using container_t::back; }

#else //(backup)
		__if_exists(container_t::reserve)				{ void Reserve(size_t nSize) { _LOCK_GUARD_THIS_; container_t::reserve(nSize); } }
		__if_exists(container_t::capacity)				{ using container_t::capacity; }

		__if_exists(container_t::begin)					{ iterator					begin()				{ return std::move(iterator(*this, &TSmartPtrContainer::end, __super::begin())); } }
		__if_exists(container_t::end)					{ iterator					end()				{ return std::move(iterator(*this, &TSmartPtrContainer::end, __super::end())); } }
		__if_exists(container_t::rbegin)				{ reverse_iterator			rbegin()			{ return std::move(reverse_iterator(*this, &TSmartPtrContainer::rend, __super::rbegin())); } }
		__if_exists(container_t::rend)					{ reverse_iterator			rend()				{ return std::move(reverse_iterator(*this, &TSmartPtrContainer::rend, __super::rend())); } }
		__if_exists(container_t::begin)					{ const_iterator			begin()		const	{ return std::move(const_iterator(*this, &TSmartPtrContainer::cend, __super::begin())); } }
		__if_exists(container_t::end)					{ const_iterator			end()		const	{ return std::move(const_iterator(*this, &TSmartPtrContainer::cend, __super::end())); } }
		__if_exists(container_t::rbegin)				{ const_reverse_iterator	rbegin()	const	{ return std::move(const_reverse_iterator(*this, &TSmartPtrContainer::crend, __super::rbegin())); } }
		__if_exists(container_t::rend)					{ const_reverse_iterator	rend()		const	{ return std::move(const_reverse_iterator(*this, &TSmartPtrContainer::crend, __super::rend())); } }
		__if_exists(container_t::cbegin)				{ const_iterator			cbegin()	const	{ return begin(); } }
		__if_exists(container_t::cend)					{ const_iterator			cend()		const	{ return end(); } }
		__if_exists(container_t::crbegin)				{ const_reverse_iterator	crbegin()	const	{ return rbegin(); } }
		__if_exists(container_t::crend)					{ const_reverse_iterator	crend()		const	{ return rend(); } }

		__if_exists(container_t::shrink_to_fit)			{ void ShrinkToFit() { _LOCK_GUARD_THIS_; container_t::shrink_to_fit(); } }
		__if_exists(container_t::size)					{ using container_t::size; }
		__if_exists(container_t::max_size)				{ using container_t::max_size; }
		__if_exists(container_t::empty)					{ using container_t::empty; }
		__if_exists(container_t::at)					{ using container_t::at; }
		__if_exists(container_t::data)					{ using container_t::data; }

		__if_exists(container_t::front)					{		T&	front()								{ return *(__super::front()); } }
		__if_exists(container_t::front)					{ const T&	front() const						{ return *(__super::front()); } }
		__if_exists(container_t::back)					{		T&	back()								{ return *(__super::back()); } }
		__if_exists(container_t::back)					{ const T&	back() const						{ return *(__super::back()); } }

		//__if_exists(container_t::front)					{ using container_t::front; }
		//__if_exists(container_t::back)					{ using container_t::back; }
#endif

		void Reverse() {
			_LOCK_GUARD_THIS_;
			if (container_t::empty())
				return;

			__if_exists(container_t::reverse) {
				container_t::reverse();
			}
			__if_not_exists(container_t::reverse) {
				__if_exists(smart_ptr_t::release) {		// for std::unique_ptr
					std::vector<T*> buf;
					buf.reserve(size());
					while (size())
						buf.push_back(Pop().release());
					for (size_t i = 0; i < buf.size(); i++)
						Push(buf[i]);
				}
				__if_not_exists(smart_ptr_t::release) {	// for std::shared_ptr
					std::vector<smart_ptr_t> buf;
					buf.reserve(size());
					while (size())
						buf.emplace_back(std::move(Pop()));
					for (size_t i = 0; i < buf.size(); i++)
						Push(std::move(buf[i]));
				}
			}
		}

		//--------------------------------------------------------------------------------------------------------------------------------------------------------
		// Swap
		static void Swap(smart_ptr_t& r1, smart_ptr_t& r2) {
			auto rc(std::move(r1));

			r1 = std::move(r2);
			r2 = std::move(rc);
		}
		template < typename iterator_t >
		void Swap(iterator_t& r1, iterator_t& r2) { Swap(r1.GetPtrWrap(), r2.GetPtrWrap()); }

	};


	//============================================================================================================================================================
	//============================================================================================================================================================
	// TPtrContainer
	template < class T, class ptr_container, class base_container, bool bUseExtendedIterator = true >
	class TPtrContainer : protected ptr_container, public CSmartLock {
	public:
		typedef ptr_container	container_t;
		typedef base_container	base_container_t;
		typedef intptr_t		index_t;	// access with 0-base index
		typedef T				type_t;

		TPtrContainer() : ptr_container() {};
		TPtrContainer(std::initializer_list<T*> list) {
			for (const auto p : list) {
				Push(p);
			}
		};
		virtual ~TPtrContainer() {
			container_t::DeleteAll();
		}


	public:
		template < class T >
		class TConverterPtr {
		public:
			static		 T* GetPtr(T& t)		{ return &t; }
			static const T* GetPtr(const T& t)	{ return &t; }
		};

		typedef t_iterator<typename container_t::iterator, TPtrContainer, TConverterPtr<T>>					iterator;
		typedef t_citerator<typename container_t::const_iterator, TPtrContainer, TConverterPtr<T>>			const_iterator;
		typedef t_iterator<typename container_t::reverse_iterator, TPtrContainer, TConverterPtr<T>>			reverse_iterator;
		typedef t_citerator<typename container_t::const_reverse_iterator, TPtrContainer, TConverterPtr<T>>	const_reverse_iterator;


	public:
		T& operator [] (index_t iIndex) requires (requires (container_t iter) { iter[0]; }) {
			return container_t::operator[](iIndex);
		}
		T& operator [] (index_t iIndex) requires (! requires (container_t iter) { iter[0]; })  {
			return *container_t::GetIterator(iIndex);
		}

		const T& operator [] (index_t iIndex) const requires (requires (container_t iter) { iter[0]; }) {
			return container_t::operator[](iIndex);
		}
		const T& operator [] (index_t iIndex) const requires (! requires (container_t iter) { iter[0]; }) {
			return *GetIterator(iIndex);
		}

		//--------------------------------------------------------------------------------------------------------------------------------------------------------
		// Get Iterator (index -> iterator)
		// TPContainerWorker 에서 Virtual 로 선언할 수 도 있지만.....속도 때문에 virtual 로 하지 않음.
	protected:
		template <typename iterator_t>
		iterator_t TGetIterator(index_t iIndex, typename iterator_t::iter_func begin_f, typename iterator_t::iter_func end_f)
			requires (requires (iterator_t iter) { iter+1; })
		{
			if (iIndex == 0)
				return (this->*begin_f)();
			if (iIndex < 0)
				return (this->*end_f)();
			return (this->*begin_f)()+iIndex;
		}
		template <typename iterator_t>
		iterator_t TGetIterator(index_t iIndex, typename iterator_t::iter_func begin_f, typename iterator_t::iter_func end_f)
			requires (!requires (iterator_t iter) { iter+1; } and requires (iterator_t iter) { iter+= 1; })
		{
			if (iIndex == 0)
				return (this->*begin_f)();
			if (iIndex < 0)
				return (this->*end_f)();

			auto pos = (this->*begin_f)();
			pos += iIndex;
			return pos;
		}
		template <typename iterator_t>
		iterator_t TGetIterator(index_t iIndex, typename iterator_t::iter_func begin_f, typename iterator_t::iter_func end_f)
			requires (!requires (iterator_t iter) { iter+1; } and !requires (iterator_t iter) { iter+= 1; })
		{
			if (iIndex == 0)
				return (this->*begin_f)();
			if (iIndex < 0)
				return (this->*end_f)();

			auto pos = (this->*begin_f)();

			for (index_t i = 0; i < iIndex; i++)
				pos++;

			return pos;
		}
		template <typename iterator_t>
		iterator_t TGetIterator(index_t iIndex, typename iterator_t::iter_func begin_f, typename iterator_t::iter_func end_f) const
			requires (requires (iterator_t iter) { iter+1; })
		{
			if (iIndex == 0)
				return (this->*begin_f)();
			if (iIndex < 0)
				return (this->*end_f)();
			return (this->*begin_f)()+iIndex;
		}
		template <typename iterator_t>
		iterator_t TGetIterator(index_t iIndex, typename iterator_t::iter_func begin_f, typename iterator_t::iter_func end_f) const
			requires (!requires (iterator_t iter) { iter+1; } and requires (iterator_t iter) { iter+= 1; })
		{
			if (iIndex == 0)
				return (this->*begin_f)();
			if (iIndex < 0)
				return (this->*end_f)();

			auto pos = (this->*begin_f)();
			pos += iIndex;
			return pos;
		}
		template <typename iterator_t>
		iterator_t TGetIterator(index_t iIndex, typename iterator_t::iter_func begin_f, typename iterator_t::iter_func end_f) const
			requires (!requires (iterator_t iter) { iter+1; } and !requires (iterator_t iter) { iter+= 1; })
		{
			if (iIndex == 0)
				return (this->*begin_f)();
			if (iIndex < 0)
				return (this->*end_f)();

			auto pos = (this->*begin_f)();

			for (index_t i = 0; i < iIndex; i++)
				pos++;

			return pos;
		}


	public:
		iterator GetIterator(index_t iIndex) {
			return TGetIterator<iterator>(iIndex, &TPtrContainer::begin, &TPtrContainer::end);
		}
		const_iterator GetIterator(index_t iIndex) const {
			return TGetIterator<const_iterator>(iIndex, &TPtrContainer::cbegin, &TPtrContainer::cend);
		}
		reverse_iterator GetRevIterator(index_t iIndex) {
			return TGetIterator<reverse_iterator>(iIndex, &TPtrContainer::rbegin, &TPtrContainer::rend);
		}
		const_reverse_iterator GetRevIterator(index_t iIndex) const {
			return TGetIterator<const_reverse_iterator>(iIndex, &TPtrContainer::crbegin, &TPtrContainer::crend);
		}


		//--------------------------------------------------------------------------------------------------------------------------------------------------------
		// Push (Insert)
	public:
		virtual bool InsertAt(T* pData, index_t iIndex = -1) {
			return InsertAt(pData, GetIterator(iIndex));
		}
		virtual bool InsertAt(T* pData, iterator iter) {
			_LOCK_GUARD_THIS_;

			__if_exists(base_container_t::push_back) {
				if (iter == end()) {
					push_back(pData);
					return true;
				}
			}

			__if_exists(base_container_t::push_front) {
				if (iter == begin()) {
					push_front(pData);
					return true;
				}
			}

			__if_exists(base_container_t::insert) {
				insert(iter, pData);
				return true;
			}

			return false;
		}


		virtual bool Push(T* pData) {
			_LOCK_GUARD_THIS_;
			__if_exists(base_container_t::push_back) {
				push_back(pData);
				return true;
			}
			__if_not_exists(base_container_t::push_back) {
				return InsertAt(pData, end());
			}
		}
		virtual bool PushFront(T* pData) {
			_LOCK_GUARD_THIS_;
			__if_exists(base_container_t::push_front) {
				push_front(pData);
				return true;
			}
			__if_not_exists(base_container_t::push_front) {
				return InsertAt(pData, begin());
			}
		}

		//--------------------------------------------------------------------------------------------------------------------------------------------------------
		// Pop (Remove) / Delete
	public:
		virtual std::unique_ptr<T> RemoveAt(index_t iIndex = -1) {
			return RemoveAt(GetIterator(iIndex));
			//_LOCK_GUARD_THIS_;

			//if (empty())
			//	return nullptr;

			//if (iIndex < 0)
			//	iIndex = size()-1;

			//return release(iIndex);
		}
		virtual std::unique_ptr<T> RemoveAt(iterator iter) {
			_LOCK_GUARD_THIS_;
			if (empty())
				return nullptr;
			return std::unique_ptr<T>(release(iter).release());
		}

		__if_exists(base_container_t::pop_back) {
			virtual std::unique_ptr<T> Pop() {
				_LOCK_GUARD_THIS_;
				if (empty())
					return nullptr;
				//return std::move(std::unique_ptr<T>(pop_back().release()));
				return std::unique_ptr<T>(container_t::pop_back().release());
			}
		}
		__if_exists(base_container_t::pop_front) {
			virtual std::unique_ptr<T> PopFront() {
				_LOCK_GUARD_THIS_;
				if (empty())
					return nullptr;
				//return std::move(std::unique_ptr<T>(pop_front().release()));
				return std::unique_ptr<T>(container_t::pop_front().release());
			}
		}
		__if_exists(base_container_t::erase) {
			virtual bool Delete(index_t iIndex) {
				return Delete(GetIterator(iIndex));
			}
			virtual bool Delete(iterator iter) {
				_LOCK_GUARD_THIS_;
				if (empty() || (iter == end()))
					return false;
				base_container_t::erase((typename container_t::iterator&)iter);
				return true;
			}
		}
		virtual std::unique_ptr<T> Pop(index_t iIndex)	{ return RemoveAt(iIndex); }
		virtual std::unique_ptr<T> Pop(iterator iter)	{ return RemoveAt(iter); }

	public:
		void DeleteAll() {
			_LOCK_GUARD_THIS_;

			__if_exists(container_t::clear) {
				clear();
				return;
			}

			__if_exists(container_t::erase) {
				erase(begin(), end());
				return;
			}

			__if_exists(container_t::pop_back) {
				while (size())
					pop_back();
				return;
			}

			__if_not_exists(container_t::clear) {
				__if_not_exists(container_t::erase) {
					__if_not_exists(container_t::pop_back) {
						static_assert(false, "Container has no clear function");
					}
				}
			}
		}


		//--------------------------------------------------------------------------------------------------------------------------------------------------------
		// etc

		// base_container 에 없는데도, ptr_container 에 있는 함수들이 좀 있음.
	public:
		__if_exists(base_container_t::reserve)			{ void Reserve(size_t nSize) { _LOCK_GUARD_THIS_; container_t::reserve(nSize); } }
		__if_exists(base_container_t::capacity)			{ using container_t::capacity; }

		__if_exists(container_t::begin)					{ iterator					begin()				{ return iterator(*this, &TPtrContainer::end, __super::begin()); } }
		__if_exists(container_t::end)					{ iterator					end()				{ return iterator(*this, &TPtrContainer::end, __super::end()); } }
		__if_exists(container_t::rbegin)				{ reverse_iterator			rbegin()			{ return reverse_iterator(*this, &TPtrContainer::rend, __super::rbegin()); } }
		__if_exists(container_t::rend)					{ reverse_iterator			rend()				{ return reverse_iterator(*this, &TPtrContainer::rend, __super::rend()); } }
		__if_exists(container_t::begin)					{ const_iterator			begin()		const	{ return const_iterator(*this, &TPtrContainer::cend, __super::begin()); } }
		__if_exists(container_t::end)					{ const_iterator			end()		const	{ return const_iterator(*this, &TPtrContainer::cend, __super::end()); } }
		__if_exists(container_t::rbegin)				{ const_reverse_iterator	rbegin()	const	{ return const_reverse_iterator(*this, &TPtrContainer::crend, __super::rbegin()); } }
		__if_exists(container_t::rend)					{ const_reverse_iterator	rend()		const	{ return const_reverse_iterator(*this, &TPtrContainer::crend, __super::rend()); } }
		__if_exists(container_t::cbegin)				{ const_iterator			cbegin()	const	{ return begin(); } }
		__if_exists(container_t::cend)					{ const_iterator			cend()		const	{ return end(); } }
		__if_exists(container_t::crbegin)				{ const_reverse_iterator	crbegin()	const	{ return rbegin(); } }
		__if_exists(container_t::crend)					{ const_reverse_iterator	crend()		const	{ return rend(); } }

		__if_exists(container_t::shrink_to_fit)			{ void ShrinkToFit() { _LOCK_GUARD_THIS_; container_t::shrink_to_fit(); } }
		__if_exists(container_t::size)					{ using container_t::size; }
		__if_exists(container_t::max_size)				{ using container_t::max_size; }
		__if_exists(container_t::empty)					{ using container_t::empty; }
		__if_exists(container_t::at)					{ using container_t::at; }
		__if_exists(container_t::data)					{ using container_t::data; }

		//__if_exists(container_t::front)				{		T&	front()								{ return *(__super::front()); } }
		//__if_exists(container_t::front)				{ const T&	front() const						{ return *(__super::front()); } }
		//__if_exists(container_t::back)				{		T&	back()								{ return *(__super::back()); } }
		//__if_exists(container_t::back)				{ const T&	back() const						{ return *(__super::back()); } }
		__if_exists(container_t::front)					{ using container_t::front; }
		__if_exists(container_t::back)					{ using container_t::back; }

		void Reverse() {
			_LOCK_GUARD_THIS_;
			if (empty())
				return;

			__if_exists(base_container_t::reverse) {
				container_t::reverse();
			}
			__if_not_exists(base_container_t::reverse) {
				std::vector<T*> buf;
				buf.reserve(size());
				while (size())
					buf.push_back(Pop().release());
				for (size_t i = 0; i < buf.size(); i++)
					Push(buf[i]);
			}
		}

	};

	template < typename tcontainer >
	concept has_Reserve = requires (tcontainer container) { container.Reserve(1); };


	//============================================================================================================================================================
	//============================================================================================================================================================
	// TContainerWorker
	template < class T, class TPContainer >
	class TContainerWorker : public TPContainer {
	public:
		typedef TPContainer										parent_t;
		typedef typename TPContainer::index_t					index_t;
		typedef typename TPContainer::iterator					iterator;
		typedef typename TPContainer::const_iterator			const_iterator;
		typedef typename TPContainer::reverse_iterator			reverse_iterator;
		typedef typename TPContainer::const_reverse_iterator	const_reverse_iterator;
	public:
		TContainerWorker() {}
		TContainerWorker(std::initializer_list<T*> list) {
			for (const auto p : list) {
				parent_t::Push(p);
			}
		};
		// Copy Constructor
		TContainerWorker(const TContainerWorker& B) {
			B.CopyTo(*this);
		}
		TContainerWorker& operator = (const TContainerWorker& B) {
			B.CopyTo(*this);
			return *this;
		}

		// Move Constructor
		TContainerWorker(TContainerWorker&& B) { *this = B; }
		TContainerWorker& operator = (TContainerWorker&& B) {
			((typename TPContainer::container_t&)*this) = std::move((typename TPContainer::container_t&)B);
			return *this;
		}


		      typename TPContainer::container_t& GetBaseClass()		{ return (typename TPContainer::container_t&)*this; }
		const typename TPContainer::container_t& GetBaseClass() const { return (typename TPContainer::container_t const&)*this; }

		//--------------------------------------------------------------------------------------------------------------------------------------------------------
		// CopyTo,    AddTo    : copies objects to B.
		// CopyRefTo, AddRefTo : copies shared_ptr.
		// CopyTo, CopyRefTo   : 사용 주의. MultiThread에서, 두 개의 리스트 모두에 Insert/Delete 를 동시에 할 경우 DeadLock 걸릴 수 있음.
		// CopyFrom..... 은.. 굳이 안 만들어도 될것 같아서 생략.
	public:
		template < class t_other_container >
		bool CopyTo(t_other_container& B) const {			// 주의! DeadLock 걸릴 수 있음....
			_LOCK_GUARD_THIS_;
			__if_exists(t_other_container::Lock) { B.Lock(); }
			B.DeleteAll();
			bool bResult = AddTo(B);
			__if_exists(t_other_container::Lock) { B.Unlock(); }	// Lock 이 있으면, Unlock 도 있어야 하니까... Lock 이 있는지만 확인하고, Unlock 은 바로 호출해버림. Unlock이 없을 경우, 컴파일타임에 에러 발생.
			return bResult;
		}
		template < class t_other_container >
		bool AddTo(t_other_container& B) const {		// 주의! DeadLock 걸릴 수 있음...
			_LOCK_GUARD_THIS_;

			if (parent_t::empty())
				return true;

			__if_exists(t_other_container::Lock) { B.Lock(); }
			if constexpr (has_Reserve<t_other_container>) {
				B.Reserve(parent_t::size() + B.size());
			}

			const auto end = parent_t::cend();
			for (auto iter = parent_t::cbegin(); iter != end; iter++) {
				__if_exists(T::NewClone) {
					B.Push(iter->NewClone());
				}
				__if_not_exists(T::NewClone) {
					B.Push(new T(*iter));
				}
			}

			__if_exists(t_other_container::Lock) { B.Unlock(); }

			return true;
		}


		//--------------------------------------------------------------------------------------------------------------------------------------------------------
		// operator ==
		bool operator == (const TContainerWorker& B) const {
			if (parent_t::size() != B.size())
				return false;
			auto pos1 = parent_t::cbegin();
			auto pos2 = B.cbegin();

			while (pos1 && pos2) {
				if (!(*pos1++ == *pos2++))
					return false;
			}

			return true;
		}
		template < class TB, class TPContainerB >
		bool operator == (const TContainerWorker<TB, TPContainerB>& B) const {
			if (parent_t::size() != B.size())
				return false;
			auto pos1 = parent_t::cbegin();
			auto pos2 = B.cbegin();

			while (pos1 && pos2) {
				if (*pos1 != *pos2)
					return false;
				pos1++;
				pos2++;
			}

			return true;
		}


		//--------------------------------------------------------------------------------------------------------------------------------------------------------
		// Get Object
	public:
		T*				GetPtr(iterator iter)						{ auto* p = iter.GetPtr(); return p; }
		const T*		GetPtr(const_iterator iter) const			{ auto* p = iter.GetPtr(); return p; }
		template<typename T_CAST>
		T_CAST*			GetPtr(iterator iter)						{ auto* p = iter.GetPtr<T_CAST>(); return p; }
		template<typename T_CAST>
		const T_CAST*	GetPtr(const_iterator iter) const			{ auto* p = iter.GetPtr<T_CAST>(); return p; }

		T*				GetPtr(index_t iIndex)						{ return &((*this)[iIndex]); }
		const T*		GetPtr(index_t iIndex) const				{ return &((*this)[iIndex]); }
		template<typename T_CAST>
		T_CAST*			GetPtr(index_t iIndex)						{ return dynamic_cast<T_CAST*>(GetPtr(iIndex)); }
		template<typename T_CAST>
		const T_CAST*	GetPtr(index_t iIndex) const				{ return dynamic_cast<const T_CAST*>(GetPtr(iIndex)); }


		T&				GetRef(iterator iter)						{ auto* p = iter.GetPtr(); if (!p) throw std::exception(std::invalid_argument(__FMSG("Type mismatch"))); return *p; }
		const T&		GetRef(const_iterator iter) const			{ auto* p = iter.GetPtr(); if (!p) throw std::exception(std::invalid_argument(__FMSG("Type mismatch"))); return *p; }
		template<typename T_CAST>
		T_CAST&			GetRef(iterator iter)						{ auto* p = iter.GetPtr<T_CAST>(); if (!p) throw std::exception(std::invalid_argument(__FMSG("Type mismatch"))); return *p; }
		template<typename T_CAST>
		const T_CAST&	GetRef(const_iterator iter) const			{ auto* p = iter.GetPtr<T_CAST>(); if (!p) throw std::exception(std::invalid_argument(__FMSG("Type mismatch"))); return *p; }

		T&				GetRef(index_t iIndex)						{ return (*this)[iIndex]; }
		const T&		GetRef(index_t iIndex) const				{ return (*this)[iIndex]; }
		template<typename T_CAST>
		T_CAST&			GetRef(index_t iIndex)						{ auto* p = GetPtr<T_CAST>(iIndex); if (!p) throw std::exception(std::invalid_argument(__FMSG("Type mismatch"))); return *p; }
		template<typename T_CAST>
		const T_CAST&	GetRef(index_t iIndex) const				{ auto* p = GetPtr<T_CAST>(iIndex); if (!p) throw std::exception(std::invalid_argument(__FMSG("Type mismatch"))); return *p; }


		//--------------------------------------------------------------------------------------------------------------------------------------------------------
		// Find - Searching Object
	protected:
		template < typename T_CAST, typename T_FIND, typename TIterator >
		TIterator TFindItemIter(TIterator& iter, const T_FIND& B) const {
			_LOCK_GUARD_THIS_;
			for (; iter.IsNotEnd(); iter++) { const T_CAST* p = iter.GetPtr<T_CAST>(); if (p && (*p == B)) return iter++; }
			return iter.end();
		}
		template < typename T_CAST, typename T_FIND, typename INDEX_T >
		INDEX_T TFindItem(INDEX_T& iIndex, const T_FIND& B) const {
			_LOCK_GUARD_THIS_;
			for (; iIndex < (INDEX_T)parent_t::size(); iIndex++) { const T_CAST* p = GetPtr<T_CAST>(iIndex); if (p && (*p == B)) return iIndex++; }
			return -1;
		}
		template < typename T_CAST, typename TIterator >
		TIterator TFindItemIterPtr(TIterator& iter, const T_CAST* pB) const {
			_LOCK_GUARD_THIS_;
			for (; iter.IsNotEnd(); iter++) { const T_CAST* p = iter.GetPtr<T_CAST>(); if (p && (p == pB)) return iter++; }
			return iter.end();
		}
		template < typename T_CAST, typename INDEX_T >
		INDEX_T TFindItemPtr(INDEX_T& iIndex, const T_CAST* pB) const {
			_LOCK_GUARD_THIS_;
			for (; iIndex < (INDEX_T)parent_t::size(); iIndex++) { const T_CAST* p = GetPtr<T_CAST>(iIndex); if (p && (p == pB)) return iIndex++; }
			return -1;
		}

		template < typename T_CAST, typename T_FIND, typename TIterator >
		T* TSearchItemIter(TIterator& iter, const T_FIND& B) {
			_LOCK_GUARD_THIS_;
			for (; iter.IsNotEnd(); iter++) { const T_CAST* p = iter.GetPtr<T_CAST>(); if (p && (*p == B)) { iter++; return p; } }
			return nullptr;
		}
		template < typename T_CAST, typename T_FIND, typename TIterator >
		const T* TSearchItemIter(TIterator& iter, const T_FIND& B) const {
			_LOCK_GUARD_THIS_;
			for (; iter.IsNotEnd(); iter++) { const T_CAST* p = iter.GetPtr<T_CAST>(); if (p && (*p == B)) { iter++; return p; } }
			return nullptr;
		}
		template < typename T_CAST, typename T_FIND, typename INDEX_T >
		T* TSearchItem(INDEX_T& iIndex, const T_FIND& B) {
			_LOCK_GUARD_THIS_;
			for (; iIndex < (INDEX_T)parent_t::size(); iIndex++) { T_CAST* p = GetPtr<T_CAST>(iIndex); if (p && (*p == B)) { iIndex++; return p; } }
			return nullptr;
		}
		template < typename T_CAST, typename T_FIND, typename INDEX_T >
		const T* TSearchItem(INDEX_T& iIndex, const T_FIND& B) const {
			_LOCK_GUARD_THIS_;
			for (; iIndex < (INDEX_T)parent_t::size(); iIndex++) { const T_CAST* p = GetPtr<T_CAST>(iIndex); if (p && (*p == B)) { iIndex++; return p; } }
			return nullptr;
		}

		template < typename T_CAST, typename TIterator >
		T_CAST* TSearchItemIterPtr(TIterator& iter, const T_CAST* pB) {
			_LOCK_GUARD_THIS_;
			for (; iter.IsNotEnd(); iter++) { T_CAST* p = iter.GetPtr<T_CAST>(); if (p && (p == pB)) { iter++; return p; } }
			return nullptr;
		}
		template < typename T_CAST, typename TIterator >
		const T_CAST* TSearchItemIterPtr(TIterator& iter, const T_CAST* pB) const {
			_LOCK_GUARD_THIS_;
			for (; iter.IsNotEnd(); iter++) { const T_CAST* p = iter.GetPtr<T_CAST>(); if (p && (p == pB)) { iter++; return p; } }
			return nullptr;
		}
		template < typename T_CAST, typename INDEX_T >
		T_CAST* TSearchItemPtr(INDEX_T& iIndex, const T_CAST* pB) {
			_LOCK_GUARD_THIS_;
			for (; iIndex < (INDEX_T)parent_t::size(); iIndex++) { T_CAST* p = GetPtr<T_CAST>(iIndex); if (p && (p == pB)) { iIndex++; return p; } }
			return nullptr;
		}
		template < typename T_CAST, typename INDEX_T >
		const T_CAST* TSearchItemPtr(INDEX_T& iIndex, const T_CAST* pB) const {
			_LOCK_GUARD_THIS_;
			for (; iIndex < (INDEX_T)parent_t::size(); iIndex++) { const T_CAST* p = GetPtr<T_CAST>(iIndex); if (p && (p == pB)) { iIndex++; return p; } }
			return nullptr;
		}

	public:
		template < typename T_FIND >										const_iterator FindIter(const T_FIND& B) const								{ return TFindItemIter<T>(parent_t::cbeing(), B); }
		template < typename T_FIND, typename TIterator >					TIterator FindIter(const T_FIND& B, TIterator& iterStart) const				{ return TFindItemIter<T>(iterStart, B); }
		template < typename T_CAST, typename T_FIND, typename TIterator >	TIterator FindIter(const T_FIND& B, TIterator& iterStart) const				{ return TFindItemIter<T_CAST>(iterStart, B); }

		template < typename T_FIND >										index_t Find(const T_FIND& B) const											{ index_t index = 0; return TFindItem<T>(index, B); }
		template < typename T_FIND, typename INDEX_T >						INDEX_T Find(const T_FIND& B, INDEX_T& iterStart) const						{ return TFindItem<T>(iterStart, B); }
		template < typename T_CAST, typename T_FIND, typename INDEX_T >		INDEX_T Find(const T_FIND& B, INDEX_T& iterStart) const						{ return TFindItem<T_CAST>(iterStart, B); }

																			const_iterator FindIterPtr(const T* pB) const								{ return TFindItemIterPtr<T>(parent_t::cbeing(), pB); }
		template < typename TIterator >										TIterator FindIterPtr(const T* pB, TIterator& iterStart) const				{ return TFindItemIterPtr<T>(iterStart, pB); }
		template < typename T_CAST, typename TIterator >					TIterator FindIterPtr(const T_CAST* pB, TIterator& iterStart) const			{ return TFindItemIter<T_CAST>(iterStart, pB); }

																			index_t FindPtr(const T* pB) const											{ index_t index = 0; return TFindItemPtr<T>(index, pB); }
		template < typename T_CAST >										index_t FindPtr(const T_CAST* pB) const										{ index_t index = 0; return TFindItemPtr<T_CAST>(index, pB); }
		template < typename INDEX_T >										INDEX_T FindPtr(const T* pB, INDEX_T& iterStart) const						{ return TFindItemPtr<T>(iterStart, pB); }
		template < typename T_CAST, typename INDEX_T >						INDEX_T FindPtr(const T_CAST* pB, INDEX_T& iterStart) const					{ return TFindItemPtr<T_CAST>(iterStart, pB); }

		template < typename T_FIND, typename TIterator >						  T* SearchIter(const T_FIND& B, TIterator& iterStart) 					{ return TSearchItemIter<T>(iterStart, B); }
		template < typename T_FIND, typename TIterator >					const T* SearchIter(const T_FIND& B, TIterator& iterStart) const			{ return TSearchItemIter<T>(iterStart, B); }

		template < typename T_CAST, typename T_FIND, typename TIterator >		  T* SearchIter(const T_FIND& B, TIterator& iterStart)					{ return TSearchItemIter<T_CAST>(iterStart, B); }
		template < typename T_CAST, typename T_FIND, typename TIterator >	const T* SearchIter(const T_FIND& B, TIterator& iterStart) const			{ return TSearchItemIter<T_CAST>(iterStart, B); }

		template < typename T_FIND >											  T* Search(const T_FIND& B)											{ index_t index = 0; return TSearchItem<T>(index, B); }
		template < typename T_FIND >										const T* Search(const T_FIND& B) const										{ index_t index = 0; return TSearchItem<T>(index, B); }
		template < typename T_FIND, typename INDEX_T >							  T* Search(const T_FIND& B, INDEX_T& iterStart)						{ return TSearchItem<T>(iterStart, B); }
		template < typename T_FIND, typename INDEX_T >						const T* Search(const T_FIND& B, INDEX_T& iterStart) const					{ return TSearchItem<T>(iterStart, B); }
		template < typename T_CAST, typename T_FIND, typename INDEX_T >			  T* Search(const T_FIND& B, INDEX_T& iterStart)						{ return TSearchItem<T_CAST>(iterStart, B); }
		template < typename T_CAST, typename T_FIND, typename INDEX_T >		const T* Search(const T_FIND& B, INDEX_T& iterStart) const					{ return TSearchItem<T_CAST>(iterStart, B); }


		template < typename TIterator >											  T* SearchIterPtr(const T* pB, TIterator& iterStart)					{ return TSearchItemIterPtr<T>(iterStart, pB); }
		template < typename TIterator >										const T* SearchIterPtr(const T* pB, TIterator& iterStart) const				{ return TSearchItemIterPtr<T>(iterStart, pB); }
		template < typename T_CAST, typename TIterator >						  T_CAST* SearchIterPtr(const T_CAST* pB, TIterator& iterStart)			{ return TSearchItemIterPtr<T_CAST>(iterStart, pB); }
		template < typename T_CAST, typename TIterator >					const T_CAST* SearchIterPtr(const T_CAST* pB, TIterator& iterStart) const	{ return TSearchItemIterPtr<T_CAST>(iterStart, pB); }

																				  T* SearchPtr(const T* pB)												{ index_t index = 0; return TSearchItemPtr<T>(index, pB); }
																			const T* SearchPtr(const T* pB) const										{ index_t index = 0; return TSearchItemPtr<T>(index, pB); }
		template < typename T_CAST >											  T_CAST* SearchPtr(const T_CAST* pB)									{ index_t index = 0; return TSearchItemPtr<T_CAST>(index, pB); }
		template < typename T_CAST >										const T_CAST* SearchPtr(const T_CAST* pB) const								{ index_t index = 0; return TSearchItemPtr<T_CAST>(index, pB); }
		template < typename INDEX_T >											  T* SearchPtr(const T* pB, INDEX_T& iterStart)							{ return TSearchItemPtr<T>(iterStart, pB); }
		template < typename INDEX_T >										const T* SearchPtr(const T* pB, INDEX_T& iterStart) const					{ return TSearchItemPtr<T>(iterStart, pB); }
		template < typename T_CAST, typename INDEX_T >							  T_CAST* SearchPtr(const T_CAST* pB, INDEX_T& iterStart)				{ return TSearchItemPtr<T_CAST>(iterStart, pB); }
		template < typename T_CAST, typename INDEX_T >						const T_CAST* SearchPtr(const T_CAST* pB, INDEX_T& iterStart) const			{ return TSearchItemPtr<T_CAST>(iterStart, pB); }

		//--------------------------------------------------------------------------------------------------------------------------------------------------------
		// Container -> vector<const T*>
		//
		__if_exists(TPContainer::smart_ptr_t::release) {
			bool MoveToVector(std::vector<const T*>& vec) {
				_LOCK_GUARD_THIS_;
				vec.clear();
				auto n = parent_t::size();
				if (!n)
					return true;

				vec.assign(n, nullptr);
				for (auto i = n; i; i--) {
					vec[i-1] = parent_t::Pop().release();
				}

				return true;
			}
			bool SetFromVector(std::vector<const T*>& vec, bool bClear = true) {
				_LOCK_GUARD_THIS_;

				for (size_t i = 0; i < vec.size(); i++)
					parent_t::Push((T*)vec[i]);

				if (bClear)
					vec.clear();

				return true;
			}
		}



		//--------------------------------------------------------------------------------------------------------------------------------------------------------
		// Sort
	public:


		//-----------------------
		// Sort - PWH, KMS
		//
		// 현재는, unique_ptr 일 경우에만 Sort 사용할 수 있음..... shared_ptr 일 경우, MoveToVector 함수 구현 안됨. algorithm::sort 사용해야...
		__if_exists(TPContainer::smart_ptr_t::release) {
		public:
			typedef enum { SM_AUTO = -1, SM_QUICK, SM_INTRO, SM_MERGE } eSORTING_METHOD;

			//-----------------------------------------------------------------------------

			bool _Sort(eSORTING_METHOD eSM = SM_AUTO) {
				auto Compare = [] (const T& a, const T& b) { return a == b ? 0 : a < b ? -1 : 1; };
				return _Sort(Compare, eSM);
			}
			template < class Compare >
			bool _Sort(Compare& comp/* CompareFunc(const T& a, const T& b) -> int(-1, 0, 1) */, eSORTING_METHOD eSM = SM_AUTO) {
				if (!parent_t::size())
					return true;

				_LOCK_GUARD_THIS_;

				if (eSM == SM_AUTO) {
					eSM = (parent_t::size() < 500 * 1000) ? SM_INTRO : SM_MERGE;
				}

				std::vector< const T* > backup;
				MoveToVector(backup);

				switch (eSM) {
				case SM_QUICK : QuickSort(backup.data(), 0, parent_t::size()-1, comp); break;
				case SM_INTRO : intro_sort(backup.data(), 0, parent_t::size()-1, parent_t::size()-1, comp); break;
				case SM_MERGE : MergeSort(backup.data(), 0, parent_t::size(), 0, comp); break;
				}

				SetFromVector(backup, false);

				return true;
			}
		}


		//-----------------------------------------------------------------------------
	protected:
		template < class Compare >
		void QuickSort(const T* data[], index_t left, index_t right, Compare& comp) {
			if (left < right) {
				index_t i = left, j = right+1;
				const T** pivot = data+left;
				do {
					do i++; while ((i < right) && (comp(*(data[i]), **pivot) < 0));
					do j--; while ((j >= left) && (comp(*(data[j]), **pivot) > 0));
					if (i < j) {
						const T* temp = data[i];
						data[i] = data[j];
						data[j] = temp;
					}
				} while (i < j);
				const T* temp = data[left];
				data[left] = data[j];
				data[j] = temp;

				QuickSort(data, left, j-1, comp);
				QuickSort(data, j+1, right, comp);
			}
		}

		//-----------------------------------------------------------------------------

		template < class Compare >
		const T* median_of_three(const T* dst[], index_t iElement1, index_t iElement2, Compare& comp) {
			index_t iMid = (iElement1 + iElement2) >> 1;
			if (comp(*dst[iElement1], *dst[iMid]) > 0)
				sort_swap(dst, iElement1, iMid);
	
			if (comp(*dst[iElement1], *dst[iElement2]) > 0)
				sort_swap(dst, iElement1, iElement2);
	
			if (comp(*dst[iMid], *dst[iElement2]) > 0)
				sort_swap(dst, iMid, iElement2);
	
			sort_swap(dst, iMid, iElement2 - 1);
			return dst[iElement2 - 1];
		}

		template < class Compare >
		void heap_down(const T* dst[], index_t i, index_t N, index_t iLeft, Compare& comp) {
			index_t iChild;
			const T* data;
			for (data = dst[i + iLeft]; LeftChild(i) < N; i = iChild - iLeft) {
				iChild = LeftChild(i) + iLeft;
				if (((iChild - iLeft + 1) < N) && comp(*dst[iChild], *dst[iChild + 1]) < 0)
					++iChild;
				if (comp(*data, *dst[iChild]) < 0)
					dst[i + iLeft] = dst[iChild];
				else
					break;
			}
			i += iLeft;
			dst[i] = data;
		}

		template < class Compare >
		void heap_sort(const T* dst[], index_t iLeft, index_t iRight, Compare& comp) {
			index_t N = iRight - iLeft + 1;
			for (index_t i = N / 2; i > 0; )
				heap_down(dst, --i, N, iLeft, comp);

			for (index_t i = N - 1; i >= 1; i--) {
				//const T* temp = dst[iLeft];
				//dst[iLeft] = dst[i + iLeft];
				//dst[i + iLeft] = temp;
				sort_swap(dst, iLeft, i+iLeft);
				heap_down(dst, 0, i, iLeft, comp);
			}
		}

		// intro sort
		inline index_t LeftChild(index_t i) {
			return ( 2 * i + 1) ;
		}

		inline void sort_swap(const T* dst[], index_t i0, index_t i1) {
			if (i0 == i1)
				return;
			const T* temp = dst[i0];
			dst[i0] = dst[i1];
			dst[i1] = temp;
		}

		template < class Compare >
		void insert_sort(const T* dst[], index_t iLeft, index_t iRight,  Compare& comp) {
			iRight++;
			for (index_t i = iLeft + 1; i < iRight; i++) {
				const T* temp = dst[i];
				index_t j = i - 1;
		
				if (j >= iLeft) {
					while (comp(*dst[j], *temp) > 0) {
						dst[j + 1] = dst[j];
						--j;
						if (j < iLeft) break;
					}
				}

				dst[j + 1] = temp;
			}
		}

		// intro_sort()
		// 기본 퀵정렬을 수행하다가, 재귀의 깊이가 깊어져 성능이 저하되면, 언제나 nlogn의 성능을 가지는 힢소트로 전환
		// 단, 정렬구간이 작을 경우 삽입정렬이 가장 빠르므로 삽입정렬 수행
		// ※ 힢소트가 수행되는 경우는 median_of_three()함수로 인해 거의 없음
		template < class Compare >
		void intro_sort(const T* dst[], index_t iLeft, index_t iRight, index_t iMaxDepth, Compare& comp) {
			if (iLeft >= iRight)
				return;
			if ((iRight - iLeft + 1) > 20) {
				if (0 < iMaxDepth) {
					index_t iLow = iLeft;
					index_t iHigh = iRight - 1;
					const T* pivot = median_of_three(dst, iLeft, iRight, comp);
					while (1) {
						while ((iLow < iHigh) && comp(*dst[++iLow], *pivot) < 0);
						while ((iLow < iHigh) && comp(*dst[--iHigh], *pivot) > 0);

						if (iLow >= iHigh) break;
						sort_swap(dst, iLow, iHigh);
					}

					iMaxDepth /= 2, iMaxDepth += iMaxDepth / 2;
					sort_swap(dst, iHigh, iRight - 1);
					intro_sort(dst, iLeft, iLow - 1, iMaxDepth, comp);
					intro_sort(dst, iLow + 1, iRight, iMaxDepth, comp);
				}
				else if (iLeft < iRight)
					heap_sort(dst, iLeft, iRight, comp);
			}
			else
				insert_sort(dst, iLeft, iRight, comp);
		}

		//-----------------------------------------------------------------------------
		template < class Compare >
		bool MergeSort(const T* src[], index_t i0, index_t i1, int nDepth, Compare& comp) {
			if ( (src == NULL) || (i0 >= i1) /*|| (comp == NULL)*/ )
				return false;

			const int nMaxCPUThread = 8;
			const int nMaxItem = 2000;
			const int nThread = 4;

			const SIZE_T nSize = i1-i0;

			if ((nMaxItem < 0) || (nSize <= nMaxItem)) {
				// Sort
				intro_sort(src, i0, i1-1, i1-1, comp);
			} else {
				// Split and Merge
				index_t iFronts[nThread];
				index_t iEnds[nThread];
				index_t ic = i0;

				const SIZE_T iStep = (nSize-1)/nThread + 1;
				for (int iThread = 0; iThread < nThread; iThread++) {
					iFronts[iThread] = ic;
					ic = (_min((index_t)(ic + iStep), i1));
					iEnds[iThread] = ic;
				}

				// 분기 & Sort
				if ( (nDepth+1) * nThread <= nMaxCPUThread ) {
					// Multi Threaded...
					#pragma omp parallel for
					for (int iThread = 0; iThread < nThread; iThread++) {
						MergeSort(src, iFronts[iThread], iEnds[iThread], nDepth+1, comp);
					}
				} else {
					for (int iThread = 0; iThread < nThread; iThread++) {
						MergeSort(src, iFronts[iThread], iEnds[iThread], nDepth+1, comp);
					}
				}

				const T** dst = new const T*[nSize];
				index_t iPos = 0;
				while (true) {
					index_t iMinThread = -1;
					const T* pMin = NULL;

					for (index_t iThread = 0; iThread < nThread; iThread++) {
						if (iFronts[iThread] >= iEnds[iThread])
							continue;
						if ( !pMin || (comp(*src[iFronts[iThread]], *pMin) < 0) ) {
							iMinThread = iThread;
							pMin = src[iFronts[iThread]];
						}
					}

					if (!pMin)
						break;
					dst[iPos++] = pMin;
					iFronts[iMinThread]++;
				}

				memcpy(src+i0, dst, nSize*sizeof(const T*));

				delete [] dst;
			}

			return true;
		}


		//--------------------------------------------------------------------------------------------------------------------------------------------------------
		// Serializing
	public:
		template <class Archive>
		Archive& StoreTo(Archive& ar, bool bFlush = false) const {
			_LOCK_GUARD_THIS_;

		#if _X86_COMPATIBLE_MODE_FOR_IO_
			ar << (uint32_t)parent_t::size();
		#else
			ar << parent_t::size();
		#endif

			if (parent_t::size()) {
				auto iterEnd = parent_t::cend();
				for (auto iter = parent_t::cbegin(); iter != iterEnd; iter++) {
					__if_exists(T::SerializeDynamic) {
						T::SerializeDynamic(ar, (T*)&(*iter));
					}
					__if_not_exists(T::SerializeDynamic) {
						ar << *iter;
					}
				}
			}
			if (bFlush)
				ar.Flush();
			return ar;
		}

		template <class Archive>
		Archive& LoadFrom(Archive& ar) {
			_LOCK_GUARD_THIS_;

			parent_t::DeleteAll();
		#if _X86_COMPATIBLE_MODE_FOR_IO_
			typedef uint32_t index_t;
		#else
			typedef size_t index_t;
		#endif
			index_t n = 0;

			ar >> n;

			__if_exists(Reserve) { Reserve(n); }

			for (index_t i = 0; i < n; i++) {
				T* p = nullptr;
				__if_exists(T::SerializeDynamic) {
					p = T::SerializeDynamic(ar, (T*)nullptr);
				}
				__if_not_exists(T::SerializeDynamic) {
					p = new T;
					ar >> *p;
				}
				parent_t::Push(p);
			}
			return ar;
		}

		template <class Archive>
		friend Archive& operator << (Archive& ar, const TContainerWorker& B) {
			return B.StoreTo(ar);
		}
		template <class Archive>
		friend Archive& operator >> (Archive& ar, TContainerWorker& B) {
			return B.LoadFrom(ar);
		}

		template <class Archive>
		friend Archive& operator || (Archive& ar, const TContainerWorker& B) {
			if (ar.IsStoring())
				return B.StoreTo(ar);
			else
				return B.LoadFrom(ar);
			return *this;
		}


	};


	//============================================================================================================================================================

	template < typename T, class smart_ptr >
	using TSmartPtrVector =		TContainerWorker<T, TSmartPtrContainer<T, std::vector<smart_ptr>, smart_ptr>>;
	template < typename T, class smart_ptr >
	using TSmartPtrList =		TContainerWorker<T, TSmartPtrContainer<T, std::list<smart_ptr>, smart_ptr>>;
	template < typename T, class smart_ptr >
	using TSmartPtrDeque =		TContainerWorker<T, TSmartPtrContainer<T, std::deque<smart_ptr>, smart_ptr>>;

	template < typename T >
	using TUniquePtrVector =	TContainerWorker<T, TSmartPtrContainer<T, std::vector<std::unique_ptr<T>>, std::unique_ptr<T>>>;
	//template < typename T >
	//using TUniquePtrList =		TContainerWorker<T, TSmartPtrContainer<T, std::list<std::unique_ptr<T>>, std::unique_ptr<T>>>;
	template < typename T >
	using TUniquePtrDeque =		TContainerWorker<T, TSmartPtrContainer<T, std::deque<std::unique_ptr<T>>, std::unique_ptr<T>>>;

	template < typename T>
	using TSharedPtrVector =	TContainerWorker<T, TSmartPtrContainer<T, std::vector<std::shared_ptr<T>>, std::shared_ptr<T>>>;
	//template < typename T>
	//using TSharedPtrList =		TContainerWorker<T, TSmartPtrContainer<T, std::list<std::shared_ptr<T>>, std::shared_ptr<T>>>;
	template < typename T>
	using TSharedPtrDeque =		TContainerWorker<T, TSmartPtrContainer<T, std::deque<std::shared_ptr<T>>, std::shared_ptr<T>>>;

	//template < typename T>
	//using TPtrVector =			TContainerWorker<T, TPtrContainer<T, boost::tptr_vector<T>, std::vector<T*>>>;
	template < typename T>
	using TPtrVector =	TUniquePtrVector<T>;

	//template < class T>
	//using TPtrList =			TContainerWorker<T, TPtrContainer<T, boost::tptr_list<T>, std::list<T*>>>;
	//template < class T>
	//using TPtrDeque =			TContainerWorker<T, TPtrContainer<T, boost::tptr_deque<T>, std::deque<T*>>>;
	template < typename T>
	using TPtrDeque =	TUniquePtrDeque<T>;

}	// namespace qtl
#pragma pack(pop)

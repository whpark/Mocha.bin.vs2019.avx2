///////////////////////////////////////////////////////////////////////////////
//
// boost_tptr_container.hpp
//
// extension of boost/ptr_container
//
// 1. 2017.07.25. : container<void*> --> container<T*>
//
// PWH. 2017.07.26
//
///////////////////////////////////////////////////////////////////////////////

//
// Boost.Pointer Container
//
//  Copyright Thorsten Ottosen 2003-2008. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see http://www.boost.org/libs/ptr_container/
//

#pragma once

#if 0

#include <boost/ptr_container/ptr_container.hpp>


#include "default.defines.h"


namespace boost {


	//------------------------------------------------------------------------------------------------------------------------------------------------------------
	// tptr_vector
	//
	template
		< 
		class T, 
		class CloneAllocator = heap_clone_allocator,
		class Allocator      = std::allocator<T*>
		>
		class tptr_vector : public 
		ptr_sequence_adapter< T, 
		std::vector<T*,Allocator>, 
		CloneAllocator >
	{  
		typedef ptr_sequence_adapter< T, 
			std::vector<T*,Allocator>, 
			CloneAllocator > 
			base_class;

		typedef tptr_vector<T,CloneAllocator,Allocator> this_type;

	public:

		BOOST_PTR_CONTAINER_DEFINE_SEQEUENCE_MEMBERS( tptr_vector, 
													 base_class,
													 this_type )

			explicit tptr_vector( size_type n,
								 const allocator_type& alloc = allocator_type() )
			: base_class(alloc)
		{
			this->base().reserve( n );
		}        
	};

	//////////////////////////////////////////////////////////////////////////////
	// clonability

	template< typename T, typename CA, typename A >
	inline tptr_vector<T,CA,A>* new_clone( const tptr_vector<T,CA,A>& r )
	{
		return r.clone().release();
	}

	/////////////////////////////////////////////////////////////////////////
	// swap

	template< typename T, typename CA, typename A >
	inline void swap( tptr_vector<T,CA,A>& l, tptr_vector<T,CA,A>& r )
	{
		l.swap(r);
	}


	//------------------------------------------------------------------------------------------------------------------------------------------------------------
	// tptr_deque
	//
	template
		< 
		class T, 
		class CloneAllocator = heap_clone_allocator,
		class Allocator      = std::allocator<T*>
		>
		class tptr_deque : public 
		ptr_sequence_adapter< T,
		std::deque<T*,Allocator>,     
		CloneAllocator >
	{
		typedef   ptr_sequence_adapter< T,
			std::deque<T*,Allocator>,     
			CloneAllocator >
			base_class;

		typedef tptr_deque<T,CloneAllocator,Allocator> this_type; 

	public:

		BOOST_PTR_CONTAINER_DEFINE_SEQEUENCE_MEMBERS( tptr_deque,
													 base_class,
													 this_type )
	};

	//////////////////////////////////////////////////////////////////////////////
	// clonability

	template< typename T, typename CA, typename A >
	inline tptr_deque<T,CA,A>* new_clone( const tptr_deque<T,CA,A>& r )
	{
		return r.clone().release();
	}

	/////////////////////////////////////////////////////////////////////////
	// swap

	template< typename T, typename CA, typename A >
	inline void swap( tptr_deque<T,CA,A>& l, tptr_deque<T,CA,A>& r )
	{
		l.swap(r);
	}



	//------------------------------------------------------------------------------------------------------------------------------------------------------------
	// tptr_list
	//
	template
		< 
		class T, 
		class CloneAllocator = heap_clone_allocator,
		class Allocator      = std::allocator<void*>
		>
		class tptr_list : public 
		ptr_sequence_adapter< T, 
		std::list<T*,Allocator>, 
		CloneAllocator >
	{
		typedef    ptr_sequence_adapter< T, 
			std::list<T*,Allocator>, 
			CloneAllocator >
			base_class;

		typedef tptr_list<T,CloneAllocator,Allocator>  this_type;

	public:
		BOOST_PTR_CONTAINER_DEFINE_SEQEUENCE_MEMBERS( tptr_list, 
													 base_class,
													 this_type )

			typedef BOOST_DEDUCED_TYPENAME base_class::value_type value_type;

	public:
		using base_class::merge;

		void merge( tptr_list& x )                                 
		{
			merge( x, std::less<T>() );
		}

		template< typename Compare > 
		void merge( tptr_list& x, Compare comp )                   
		{
			this->base().merge( x.base(), void_ptr_indirect_fun<Compare,T>( comp ) ); }

		void sort()                                                    
		{ 
			sort( std::less<T>() ); 
		};

		template< typename Compare > 
		void sort( Compare comp )                             
		{
			this->base().sort( void_ptr_indirect_fun<Compare,T>( comp ) );
		}

		template< class Pred >
		void erase_if( iterator first, iterator last, Pred pred )
		{
			base_class::erase_if( first, last, pred );
		}

		template< class Pred >
		void erase_if( Pred pred )
		{
			this->base().remove_if( BOOST_DEDUCED_TYPENAME base_class:: 
								   BOOST_NESTED_TEMPLATE void_ptr_delete_if<Pred,value_type>
								   (pred) );
		} 

	}; // class 'tptr_list'

	   //////////////////////////////////////////////////////////////////////////////
	   // clonability

	template< typename T, typename CA, typename A >
	inline tptr_list<T,CA,A>* new_clone( const tptr_list<T,CA,A>& r )
	{
		return r.clone().release();
	}

	/////////////////////////////////////////////////////////////////////////
	// swap

	template< typename T, typename CA, typename A >
	inline void swap( tptr_list<T,CA,A>& l, tptr_list<T,CA,A>& r )
	{
		l.swap(r);
	}

}

#endif

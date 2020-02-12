#pragma once
#ifndef _TINYSTL_VECTOR_H_
#define _TINYSTL_VECTOR_H_

#include "allocator.h"

namespace TinySTL
{
	template <class T, class Alloc =  allocator<T>>
	class vector
	{
	public:
		typedef T         value_type;
		typedef Alloc     allocator_type;
		typedef T&        reference;
		typedef const T&  const_reference;
		typedef T*        pointer;
		typedef const T*  const_pointer;
	};
}

#endif /* _TINYSTL_VECTOR_H_ */
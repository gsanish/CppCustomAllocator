#include <iostream>
#include <limits>

namespace codehacks
{

// forward declaration of class template for void specialization
template<typename T> class CustomAllocator;

// explicit specialization for void
template<>
class CustomAllocator<void>
{
public:
	typedef void * pointer;
	typedef const void * const_pointer;
	typedef void value_type;
	template<typename U>
	struct rebind{
		typedef CustomAllocator<U> other;
	};
};

template<typename T>
class CustomAllocator
{
public:
	typedef T value_type;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef T* pointer;
	typedef const T* const_pointer;
	typedef T& reference;
	typedef const T& const_reference;
	//rebind allocator to type U
	template<typename U>
	struct rebind{
		typedef CustomAllocator<U> other;
	};

	CustomAllocator() throw() {}
	CustomAllocator(const CustomAllocator&) throw() {}
	template<typename U>
	CustomAllocator(const CustomAllocator<U>&) throw() {}
	~CustomAllocator() throw() {}

	// allocate memory for 'numberOfObjects' objects of type T, but don't initialize the objects
	pointer allocate(size_type numberOfObjects, CustomAllocator<void>::const_pointer hint=0)
	{
		// allocate memory using global operator new
		size_t bytesToAllocate = numberOfObjects*sizeof(T);
		std::cout << "Allocating " << bytesToAllocate << " bytes of memory" <<std::endl;
		return static_cast<pointer>(::operator new(bytesToAllocate));
	}

	// deallocate memory for 'numberOfObjects' objects of type T,
	// before this those objects should have been destroyed
	void deallocate(pointer p, size_type numberOfObjects) throw()
	{
		size_t bytesToDeallocate = numberOfObjects*sizeof(T);
		std::cout << "De-allocating " << bytesToDeallocate <<" bytes of memory" << std::endl;
		//deallocate using global operator delete
		::operator delete((void *)p);
	}

	pointer address(reference value)
	{
		// return a non-constant pointer to the non-constant value
		return &value;
	}

	const_pointer address(const_reference value)
	{
		// return a const pointer to the const value
		return &value;
	}

	// returns largest value that can be passed to allocate()
	// ie, maximum number of objects of type T that can be allocated
	size_type max_size()
	{
		return std::numeric_limits<size_type>::max() / sizeof(T);
	}

	// construct object at the memory allocated
	void construct(pointer p, const T& value)
	{
		new(p) T(value);
	}
	
	// destroy the object
	void destroy(pointer p)
	{
		// by calling the destructor explicitly
		p->~T();
	}
};

template<typename T>
bool operator==(const CustomAllocator<T> &, const CustomAllocator<T> &) throw()
{
	return true;
}
	
template<typename T1, typename T2>
bool operator!=(const CustomAllocator<T1> &, const CustomAllocator<T2> &) throw()
{
	return false;
}

}

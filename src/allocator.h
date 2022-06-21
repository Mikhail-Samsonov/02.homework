#ifndef CUSTOM_ALLOCATOR_ALLOCATOR_H
#define CUSTOM_ALLOCATOR_ALLOCATOR_H

#include "allocation_strategy.h"

template<typename T, size_t N = CHUNK_SIZE>
struct CustomAllocator
{
	using value_type = T;
	using pointer = T*;
	using const_pointer = const T*;

	template<typename U>
	struct rebind
	{
		using other = CustomAllocator<U>;
	};

	CustomAllocator();

	template<typename U>
	explicit CustomAllocator(const CustomAllocator<U>& a)
	{
	};

	T* allocate(std::size_t n);

	void deallocate(T* p, std::size_t n);

	template<typename U, typename... Args>
	void construct(U* p, Args&& ... args);

	void destroy(T* p);

private:
	std::unique_ptr<AllocationStrategy<T, N>> _allocation_strategy;
};

template<typename T, size_t N>
CustomAllocator<T, N>::CustomAllocator()
{
	_allocation_strategy = std::make_unique<AllocationStrategy<T, N>>();
}

template<typename T, size_t N>
T* CustomAllocator<T, N>::allocate(std::size_t n)
{
	return _allocation_strategy->allocate(n);
}

template<typename T, size_t N>
template<typename U, typename... Args>
void CustomAllocator<T, N>::construct(U* p, Args&& ... args)
{
	new(p) U(std::forward<Args>(args)...);
}

template<typename T, size_t N>
void CustomAllocator<T, N>::destroy(T* p)
{
	p->~T();
}

template<typename T, size_t N>
void CustomAllocator<T, N>::deallocate(T* p, std::size_t n)
{
	_allocation_strategy->deallocate(n);
}


#endif //CUSTOM_ALLOCATOR_ALLOCATOR_H

#ifndef CUSTOM_ALLOCATOR_ALLOCATION_STRATEGY_H
#define CUSTOM_ALLOCATOR_ALLOCATION_STRATEGY_H

#include <iostream>
#include "chunk.h"

template<typename T, std::size_t N>
struct AllocationStrategy
{
	AllocationStrategy();

	~AllocationStrategy();

	T* allocate(std::size_t n);

	void deallocate(std::size_t n);

private:
	T* _memory_pool;
	int _capacity = N;
};

template<typename T, std::size_t N>
AllocationStrategy<T, N>::AllocationStrategy()
{
	auto p = std::malloc(N * sizeof(T));
	if (!p)
		throw std::bad_alloc();

	_memory_pool = reinterpret_cast<T*>(p);
}

template<typename T, std::size_t N>
AllocationStrategy<T, N>::~AllocationStrategy()
{
	std::free(_memory_pool);
}

template<typename T, std::size_t N>
T* AllocationStrategy<T, N>::allocate(std::size_t n)
{
	_capacity -= n;
	if (_capacity < 0)
		throw std::runtime_error("Preallocated memory is empty");
	return _memory_pool + N - _capacity - n;
}

template<typename T, std::size_t N>
void AllocationStrategy<T, N>::deallocate(size_t n)
{
	_capacity += n;
}

#endif //CUSTOM_ALLOCATOR_ALLOCATION_STRATEGY_H

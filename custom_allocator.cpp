#include "chunk.h"
#include "custom_list.h"
#include <iostream>
#include <limits>
#include <map>
#include <memory>
#include <sstream>

int factorial(const int n)
{
    int64_t result{ 1 };
    int maxInt{ std::numeric_limits<int>::max() };

    for (int i = 2; i <= n; i++)
    {
        result *= i;
        if (maxInt < result)
        {
            std::stringstream s;
            s << n;
            std::string number;
            s >> number;

            std::string error_msg = "Int overflow occured while calculating factorial of " + number;

            s >> error_msg;
            throw std::overflow_error(error_msg);
        }
    }

    return result;
}

template <typename T, size_t N>
struct AllocationStrategy
{
    AllocationStrategy()
    {
        auto p = std::malloc(N * sizeof(T));
        if (!p)
            throw std::bad_alloc();

        m_memorypool = reinterpret_cast<T*>(p);
    }

    ~AllocationStrategy()
    {
        std::free(m_memorypool);
    }

    T* allocate(size_t n)
    {
        m_capacity -= n;
        if (m_capacity < 0)
            throw std::runtime_error("Preallocated memory is empty");
        return m_memorypool + N - m_capacity - n;
    }

    void deallocate(size_t n)
    {
        m_capacity += n;
    }

private:
    T* m_memorypool;
    int m_capacity = N;
};

template <typename T, size_t N = CHUNK_SIZE>
struct CustomAllocator
{
    using value_type = T;

    using pointer = T*;
    using const_pointer = const T*;

    template <typename U>
    struct rebind
    {
        using other = CustomAllocator<U>;
    };

    CustomAllocator()
    {
        m_allocationStrategy = std::make_unique<AllocationStrategy<T, N>>();
    };

    template <typename U>
    explicit CustomAllocator(const CustomAllocator<U>& a){};

    T* allocate(std::size_t n)
    {
        return m_allocationStrategy->allocate(n);
    }

    void deallocate(T* p, std::size_t n)
    {
        m_allocationStrategy->deallocate(n);
    };

    template <typename U, typename... Args>
    void construct(U* p, Args&&... args)
    {
        new (p) U(std::forward<Args>(args)...);
    };

    void destroy(T* p) { p->~T(); }

private:
    std::unique_ptr<AllocationStrategy<T, N>> m_allocationStrategy;
};

int main()
{
    try
    {
        // allocator with map
        std::map<int, int> m1;

        for (int i = 0; i < CHUNK_SIZE; ++i)
        {
            m1[i] = factorial(i);
        }

        // custom allocator with map
        using customAllocatorForMap = CustomAllocator<std::pair<const int, int>>;
        using mapWithCustomAllocator = std::map<int, int, std::less<>, customAllocatorForMap>;
        mapWithCustomAllocator m2;

        for (int i = 0; i < CHUNK_SIZE; ++i)
        {
            m2[i] = factorial(i);
        }

        for (const auto& [f, s] : m2)
        {
            std::cout << f << " " << s << std::endl;
        }

        // allocator with custom list
        CustomList<int> l1;

        for (int i = 0; i < CHUNK_SIZE; ++i)
        {
            l1.Add(i);
        }

        // custom allocator with custom list
        CustomList<int, CustomAllocator<int>> l2{};

        for (int i = 0; i < CHUNK_SIZE; ++i)
        {
            l2.Add(i);
        }

        std::cout << l2;
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
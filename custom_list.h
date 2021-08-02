#pragma once

#include <iostream>

template <typename T, typename A = std::allocator<T>>
struct CustomList
{
    struct Node
    {
        T data;
        Node* next;
    };

    ~CustomList();

    void Add(const T& data);

    template <typename U, typename B>
    friend std::ostream& operator<<(std::ostream& stream, const CustomList<U, B>& list);

    class Iterator
    {
    public:
        explicit Iterator(CustomList* list)
            : parent{ list }
            , ptr{ list->head }
        {
        }

        void operator++()
        {
            if (ptr != nullptr)
            {
                ptr = ptr->next;
            }
        }

        typename CustomList::Node& operator*()
        {
            return *ptr;
        }

        typename CustomList::Node* operator->()
        {
            return ptr;
        }

        bool IsEnd() const
        {
            return ptr == nullptr;
        }

    private:
        CustomList* parent;
        typename CustomList::Node* ptr;
    };

    friend class Iterator;

private:
    void Destroy(Node* node);

    Node* m_head = nullptr;
    Node* m_tail = nullptr;

    using AllocatorTraits = std::allocator_traits<A>;
    using RebindAllocNode = typename AllocatorTraits::template rebind_alloc<Node>;

    RebindAllocNode m_rebindAllocator{};
};

template <typename T, typename A>
CustomList<T, A>::~CustomList()
{
    auto ptr = m_head;
    while (ptr != nullptr)
    {
        auto next = ptr->next;
        Destroy(ptr);
        ptr = next;
    }
}

template <typename T, typename A>
void CustomList<T, A>::Destroy(Node* node)
{
    m_rebindAllocator.destroy(node);
    m_rebindAllocator.deallocate(node, 1);
}

template <typename T, typename A>
void CustomList<T, A>::Add(const T& data)
{
    Node* node{ m_rebindAllocator.allocate(1) };
    m_rebindAllocator.construct(node, Node{ data, nullptr });
    if (m_tail != nullptr)
    {
        m_tail->next = node;
    }

    m_tail = node;

    if (m_head == nullptr)
    {
        m_head = node;
    }
}

template <typename T, typename A>
std::ostream& operator<<(std::ostream& stream, const CustomList<T, A>& list)
{
    for (typename CustomList<T, A>::Node* ptr = list.m_head; ptr != nullptr; ptr = ptr->next)
        stream << ptr->data << std::endl;

    return stream;
}
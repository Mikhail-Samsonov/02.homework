#ifndef CUSTOM_ALLOCATOR_LIST_H
#define CUSTOM_ALLOCATOR_LIST_H

#include <iostream>

template<typename T, typename A = std::allocator<T>>
struct CustomList
{
	struct Node
	{
		T data;
		Node* next;
	};

	~CustomList();

	void Add(const T& data);

	template<typename U, typename B>
	friend std::ostream& operator<<(std::ostream& stream, const CustomList<U, B>& list);

	class Iterator
	{
	public:
		explicit Iterator(CustomList* list)
				: parent{ list }, ptr{ list->head }
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

	Node* _head = nullptr;
	Node* _tail = nullptr;

	using AllocatorTraits = std::allocator_traits<A>;
	using RebindAllocNode = typename AllocatorTraits::template rebind_alloc<Node>;

	RebindAllocNode _rebind_allocator{};
};

template<typename T, typename A>
CustomList<T, A>::~CustomList()
{
	auto ptr = _head;
	while (ptr != nullptr)
	{
		auto next = ptr->next;
		Destroy(ptr);
		ptr = next;
	}
}

template<typename T, typename A>
void CustomList<T, A>::Destroy(Node* node)
{
	_rebind_allocator.destroy(node);
	_rebind_allocator.deallocate(node, 1);
}

template<typename T, typename A>
void CustomList<T, A>::Add(const T& data)
{
	Node* node{ _rebind_allocator.allocate(1) };
	_rebind_allocator.construct(node, Node{ data, nullptr });
	if (_tail != nullptr)
	{
		_tail->next = node;
	}

	_tail = node;

	if (_head == nullptr)
	{
		_head = node;
	}
}

template<typename T, typename A>
std::ostream& operator<<(std::ostream& stream, const CustomList<T, A>& list)
{
	for (typename CustomList<T, A>::Node* ptr = list._head; ptr != nullptr; ptr = ptr->next)
		stream << ptr->data << std::endl;

	return stream;
}

#endif //CUSTOM_ALLOCATOR_LIST_H
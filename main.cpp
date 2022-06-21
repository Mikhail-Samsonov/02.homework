#include <iostream>
#include <map>
#include <memory>

#include "chunk.h"
#include "my_list.h"
#include "allocator.h"
#include "factorial.h"


int main()
{
	try
	{
		// map default allocator
		std::map<int, int> map_suite1;

		for (int i = 0; i < CHUNK_SIZE; ++i)
		{
			map_suite1[i] = factorial(i);
		}

		// map custom allocator
		using my_allocator = CustomAllocator<std::pair<const int, int>>;
		using my_map = std::map<int, int, std::less<>, my_allocator>;
		my_map map_suite2;

		for (int i = 0; i < CHUNK_SIZE; ++i)
		{
			map_suite2[i] = factorial(i);
		}

		std::cout << "map with custom allocator\n";
		for (const auto& [f, s]: map_suite2)
		{
			std::cout << f << " " << s << std::endl;
		}

		// list default allocator
		CustomList<int> list_suite1;

		for (int i = 0; i < CHUNK_SIZE; ++i)
		{
			list_suite1.Add(i);
		}

		// list custom allocator
		CustomList<int, CustomAllocator<int>> list_suite2{};

		std::cout << "my_list with custom allocator\n";
		for (int i = 0; i < CHUNK_SIZE; ++i)
		{
			list_suite2.Add(i);
		}

		std::cout << list_suite2;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	return 0;
}
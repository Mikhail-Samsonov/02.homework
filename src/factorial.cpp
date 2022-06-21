#include "factorial.h"

int factorial(int n)
{
	int64_t result{ 1 };
	int max_int{ std::numeric_limits<int>::max() };

	for (int i = 2; i <= n; ++i)
	{
		result *= i;
		if (max_int < result)
		{
			std::stringstream s;
			s << n;
			std::string number;
			s >> number;

			std::string error_msg = "Int overflow occurred while calculating factorial of " + number;

			throw std::overflow_error(error_msg);
		}
	}

	return result;
}
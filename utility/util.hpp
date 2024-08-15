#ifndef UTILITY_UTIL_HPP
#define UTILITY_UTIL_HPP

#include <string>

#include <base_utils/Mem.h>

namespace util
{

	std::string get_parent_path(const std::string& path);

	int compare_dates(const date_t& date1, const date_t& date2);

	template <typename T>
	void mem_free_s(T*& ptr)
	{
		if (ptr)
		{
			MEM_free(ptr);
			ptr = nullptr;
		}
	}

} // namespace util

#endif // UTILITY_UTIL_HPP

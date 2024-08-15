#include "util.hpp"

namespace util
{
	
	std::string get_parent_path(const std::string& path)
	{
		size_t pos = path.find_last_of("\\/");

		if (pos != std::string::npos)
			return path.substr(0, pos);
		else
			return "";
	}

	int compare_dates(const date_t& date1, const date_t& date2)
	{
		if (date1.year < date2.year) return -1;
		if (date1.year > date2.year) return 1;

		if (date1.month < date2.month) return -1;
		if (date1.month > date2.month) return 1;

		if (date1.day < date2.day) return -1;
		if (date1.day > date2.day) return 1;

		if (date1.hour < date2.hour) return -1;
		if (date1.hour > date2.hour) return 1;

		if (date1.minute < date2.minute) return -1;
		if (date1.minute > date2.minute) return 1;

		if (date1.second < date2.second) return -1;
		if (date1.second > date2.second) return 1;

		return 0;
	}

} // namespace util

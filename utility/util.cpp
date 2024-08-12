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

}

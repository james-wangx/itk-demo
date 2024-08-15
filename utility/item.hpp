#ifndef UTILITY_ITEM_HPP
#define UTILITY_ITEM_HPP

#include <vector>

namespace item
{

	tag_t create_item(const char* item_id, const char* item_name);

	tag_t copy_item(const char* source_rev_uid, const char* target_item_id, const char* target_rev_id);

	tag_t get_latest_rev(const char* item_uid);

	tag_t get_latest_released_rev(const char* item_uid);

	int get_workflow_num(const char* item_uid);

	void get_all_revs(const char* item_uid, int& count, tag_t*& revs);

	std::vector<tag_t> get_all_base_revs(const char* item_uid);

} // namespace item

#endif // UTILITY_ITEM_HPP

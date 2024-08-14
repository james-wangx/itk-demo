#ifndef UTILITY_SESSION_CPP
#define UTILITY_SESSION_CPP

#include <string>

#include <unidefs.h>

namespace session
{

	tag_t get_session();

	std::string get_tc_root();

	tag_t get_user();

	tag_t get_group();

	tag_t get_role();

	tag_t get_user_home_folder(tag_t& user);

	tag_t get_user_newstuff_folder(tag_t& user);

	tag_t get_user_mailbox_folder(tag_t& user);

	tag_t get_user_worklist(tag_t& user);

} // namespace session

#endif // UTILITY_SESSION_CPP

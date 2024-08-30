#include <iostream>

#include <pom/pom/pom.h>
#include <sa/sa.h>
#include <tc/tc_util.h>

#include "session.hpp"
#include "util.hpp"

namespace session
{

    tag_t get_session()
    {
        tag_t session = NULL_TAG;
        POM_ask_session(&session);

        return session;
    }

    std::string get_tc_root()
    {
        const char* env = "FMS_HOME1";
        std::string fms_home = TC_getenv(env);

        if (fms_home.empty())
        {
            std::cout << "Unknown environment variable: " << env << std::endl;
            return "";
        }

        return util::get_parent_path(fms_home);
    }

    tag_t get_user()
    {
        char* username = nullptr;
        tag_t user = NULLTAG;

        POM_get_user(&username, &user);

        util::mem_free_s(username);
        return user;
    }

    static tag_t _get_group_member()
    {
        tag_t group_member = NULLTAG;
        SA_ask_current_groupmember(&group_member);

        return group_member;
    }

    tag_t get_group()
    {
        tag_t group_member = _get_group_member();
        tag_t group = NULLTAG;

        SA_ask_groupmember_group(group_member, &group);

        return group;
    }

    tag_t get_role()
    {
        tag_t group_member = _get_group_member();
        tag_t role = NULLTAG;

        SA_ask_groupmember_role(group_member, &role);

        return role;
    }

    tag_t get_user_home_folder(tag_t& user)
    {
        tag_t folder = NULLTAG;
        SA_ask_user_home_folder(user, &folder);

        return folder;
    }

    tag_t get_user_newstuff_folder(tag_t& user)
    {
        tag_t folder = NULLTAG;
        SA_ask_user_newstuff_folder(user, &folder);

        return folder;
    }

    tag_t get_user_mailbox_folder(tag_t& user)
    {
        tag_t folder = NULLTAG;
        SA_ask_user_mailbox(user, &folder);

        return folder;
    }

    tag_t get_user_worklist(tag_t& user)
    {
        tag_t folder = NULLTAG;

        return folder;
    }

} // namespace session

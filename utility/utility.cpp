#include <iostream>
#include <string>

#include <tcinit/tcinit.h>
#include <tc/emh.h>
#include <tc/tc_util.h>
#include <tccore/aom.h>
#include <tccore/aom_prop.h>
#include <tccore/tctype.h>
#include <sa/sa.h>
#include <mld/journal/journal.h>

#include "util.hpp"

const int WRONG_USAGE = 100001;

static inline void output_useage()
{
	std::cout << "\nUSEAGE: utility -u=user -p=password -g=group" << std::endl;

	return;
}

static inline void output_filename()
{
	char* journal_filename;
	char* system_log_filename;

	JOURNAL_ask_file_name(&journal_filename);
	EMH_ask_system_log_filename(&system_log_filename);
	std::cout << "journal file name: " << journal_filename << std::endl;
	std::cout << "system log: " << system_log_filename << std::endl;

	MEM_free(journal_filename);
	MEM_free(system_log_filename);
}

static tag_t get_session()
{
	tag_t session;
	POM_ask_session(&session);

	return session;
}

static std::string get_tc_root()
{
	const char* env = "FMS_HOME";
	const char* fms_home = TC_getenv(env);

	if (!fms_home)
		std::cout << "Unknown environment variable: " << env << std::endl;

	return util::get_parent_path(fms_home);
}

static std::string get_tc_bin()
{
	std::string tc_root = get_tc_root();

	if (tc_root.empty())
		return "";

	return tc_root + "\\" + "bin";
}

static void output_current_user()
{
	char* username;
	tag_t user;
	
	POM_get_user(&username, &user);
	std::cout << "current username: " << username << std::endl;
	
	MEM_free(username);
}

static void output_current_group_role()
{
	tag_t group_member = NULLTAG;
	tag_t group = NULLTAG;
	tag_t role = NULLTAG;
	char* group_name = nullptr;
	char* role_name = nullptr;

	SA_ask_current_groupmember(&group_member);
	SA_ask_groupmember_group(group_member, &group);
	SA_ask_groupmember_role(group_member, &role);
	SA_ask_group_name2(group, &group_name);
	SA_ask_role_name2(role, &role_name);

	std::cout << "current grouo name: " << group_name << std::endl;
	std::cout << "current role name: " << role_name << std::endl;

	MEM_free(group_name);
	MEM_free(role_name);
}

static tag_t get_current_user()
{
	char* username;
	tag_t user;

	POM_get_user(&username, &user);
	std::cout << "current user: " << username << std::endl;

	MEM_free(username);
	return user;
}

static tag_t get_user_home_folder(tag_t &user)
{
	tag_t user_home_folder = NULLTAG;
	
	SA_ask_user_home_folder(user, &user_home_folder);

	return user_home_folder;
}

static tag_t get_user_newstuff_folder(tag_t& user)
{
	tag_t user_newstuff_folder = NULLTAG;

	SA_ask_user_newstuff_folder(user, &user_newstuff_folder);

	return user_newstuff_folder;
}

static tag_t get_user_mailbox_folder(tag_t& user)
{
	tag_t mailbox_folder = NULLTAG;

	SA_ask_user_mailbox(user, &mailbox_folder);

	return mailbox_folder;
}

static tag_t get_user_worklist(tag_t& user)
{
	tag_t worklist = NULLTAG;

	
}

int ITK_user_main(int argc, char** argv)
{
	output_filename();

	char* usr = ITK_ask_cli_argument("-u=");
	char* upw = ITK_ask_cli_argument("-p=");
	char* ugp = ITK_ask_cli_argument("-g=");
	char* help = ITK_ask_cli_argument("-h=");

	if (!usr || !upw || !ugp || help)
	{
		output_useage();
		return WRONG_USAGE;
	}

	std::cout << "usr = " << usr << std::endl;
	std::cout << "upw = " << upw << std::endl;
	std::cout << "ugp = " << ugp << std::endl;

	ITK_initialize_text_services(0);
	int status = ITK_init_module(usr, upw, ugp);

	if (status != ITK_ok)
	{
		char* error;
		EMH_ask_error_text(status, &error);
		std::cout << "Error with ITK_init_module: " << error << std::endl;
		MEM_free(error);
		return status;
	}
	else
		std::cout << "Login to Teamcenter success as " << usr << std::endl;

	// Need env: TC_JOURNAL=FULL
	JOURNAL_comment("Preparing to list tool formats\n");
	TC_write_syslog("Preparing to list tool formats\n");

	/* Call your functions between here */
	//tag_t session = get_session();
	
	//std::string tc_root = get_tc_root();
	//std::string tc_bin = get_tc_bin();
	//std::cout << "tc root: " << tc_root << std::endl;
	//std::cout << "tc_bin: " << tc_bin << std::endl;

	//output_current_user();

	//output_current_group_role();

	//tag_t user = get_current_user();
	//tag_t user_home_folder = get_user_home_folder(user);
	//char* home_folder_name = nullptr;
	//AOM_ask_value_string(user_home_folder, "object_name", &home_folder_name);
	//std::cout << "user home folder name: " << home_folder_name << std::endl;
	//MEM_free(home_folder_name);

	//tag_t user = get_current_user();
	//tag_t user_newstuff_folder = get_user_newstuff_folder(user);
	//char* folder_name = nullptr;
	//AOM_ask_value_string(user_newstuff_folder, "object_name", &folder_name);
	//std::cout << "user newstuff folder name: " << folder_name << std::endl;
	//MEM_free(folder_name);

	tag_t user = get_current_user();
	tag_t mailbox_folder = get_user_mailbox_folder(user);
	char* folder_name = nullptr;
	AOM_ask_value_string(mailbox_folder, "object_name", &folder_name);
	std::cout << "user mailbox folder name: " << folder_name << std::endl;
	MEM_free(folder_name);

	tag_t type = NULLTAG;
	char* class_name = nullptr;
	TCTYPE_ask_object_type(mailbox_folder, &type);
	TCTYPE_ask_class_name2(type, &class_name);
	std::cout << "mailbox type name: " << class_name << std::endl;


	ITK_exit_module(TRUE);

	std::cout << "Press Enter to continue...";
	std::cin.get();

	return status;
}

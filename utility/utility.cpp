#include <iostream>
#include <string>

#include <tcinit/tcinit.h>
#include <tc/emh.h>
#include <tc/tc_util.h>
#include <tccore/aom.h>
#include <tccore/aom_prop.h>
#include <tccore/tctype.h>
#include <tccore/item.h>
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

static tag_t get_user_home_folder(tag_t& user)
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

static void create_item()
{
	// Create form
	tag_t type = NULLTAG;
	TCTYPE_find_type("ItemRevision Master", "ItemRevision Master", &type);

	tag_t create_input = NULLTAG;
	TCTYPE_construct_create_input(type, &create_input);

	const char* form_name[1] = { "1234567/A" };
	TCTYPE_set_create_display_value(create_input, const_cast<char*>("object_name"), 1, form_name);

	tag_t form = NULLTAG;
	TCTYPE_create_object(create_input, &form);
	AOM_save(form);


	// Create item revision
	TCTYPE_find_type("ItemRevision", "ItemRevision", &type);

	tag_t rev_create_input = NULLTAG;
	TCTYPE_construct_create_input(type, &rev_create_input);

	AOM_set_value_tag(rev_create_input, "item_master_tag", form);


	// Create item
	TCTYPE_find_type("Item", "Item", &type);

	tag_t item_create_input = NULLTAG;
	TCTYPE_construct_create_input(type, &item_create_input);

	const char* item_id[1] = { "1234567" };
	TCTYPE_set_create_display_value(item_create_input, const_cast<char*>("item_id"), 1, item_id);

	const char* item_name[1] = { "1234567" };
	TCTYPE_set_create_display_value(item_create_input, const_cast<char*>("object_name"), 1, item_name);

	AOM_set_value_tag(item_create_input, "revision", rev_create_input);

	tag_t item = NULLTAG;
	TCTYPE_create_object(item_create_input, &item);

	ITEM_save_item(item);

	std::cout << "item: " << item_name[0] << " has been created." << std::endl;
}

static void copy_item(const char* source_rev_uid, const char* target_item_id, const char* target_rev_id)
{
	tag_t source_rev = NULLTAG;
	tag_t target_item = NULLTAG;
	tag_t target_rev = NULLTAG;

	ITK__convert_uid_to_tag(source_rev_uid, &source_rev);
	ITKCALL(ITEM_copy_item(source_rev, target_item_id, target_rev_id, &target_item, &target_rev));
}

static tag_t get_latest_rev(const char* item_uid)
{
	tag_t item = NULLTAG;
	tag_t rev = NULLTAG;

	ITK__convert_uid_to_tag(item_uid, &item);
	ITKCALL(ITEM_ask_latest_rev(item, &rev));

	return rev;
}

static int compare_dates(const date_t& date1, const date_t& date2)
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

static tag_t get_latest_released_rev(const char* item_uid)
{
	tag_t item = NULLTAG;
	tag_t* revs = nullptr;
	tag_t latest_released_rev = NULLTAG;
	int rev_count = 0;
	date_t latest_date = { 0 };

	ITK__convert_uid_to_tag(item_uid, &item);
	ITKCALL(ITEM_list_all_revs(item, &rev_count, &revs));

	for (size_t i = 0; i < rev_count; i++)
	{
		date_t rev_date = { 0 };
		ITKCALL(AOM_ask_value_date(revs[i], "date_released", &rev_date));
		if (compare_dates(rev_date, latest_date) >= 0)
		{
			latest_date = rev_date;
			latest_released_rev = revs[i];
		}
	}

	MEM_free(revs);
	return latest_released_rev;
}

static void output_workflow_num(const char* item_uid)
{
	tag_t item = NULLTAG;
	tag_t* workflows = nullptr;
	int num = 0;

	ITK__convert_uid_to_tag(item_uid, &item);
	AOM_ask_value_tags(item, "fnd0AllWorkflows", &num, &workflows);

	std::cout << "find workflow num: " << num << std::endl;

	MEM_free(workflows);
}

static void output_all_revs(const char* item_uid)
{
	tag_t item = NULLTAG;
	tag_t* revs = nullptr;
	int count = 0;
	char* rev_id = nullptr;

	ITK__convert_uid_to_tag(item_uid, &item);
	ITEM_list_all_revs(item, &count, &revs);

	for (size_t i = 0; i < count; i++)
	{
		AOM_ask_value_string(revs[i], "item_revision_id", &rev_id);
		std::cout << "item revision id: " << rev_id << std::endl;
		util::mem_free_s(rev_id);
	}

	util::mem_free_s(revs);
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

	//tag_t user = get_current_user();
	//tag_t mailbox_folder = get_user_mailbox_folder(user);
	//char* folder_name = nullptr;
	//AOM_ask_value_string(mailbox_folder, "object_name", &folder_name);
	//std::cout << "user mailbox folder name: " << folder_name << std::endl;
	//MEM_free(folder_name);

	//tag_t type = NULLTAG;
	//char* class_name = nullptr;
	//TCTYPE_ask_object_type(mailbox_folder, &type);
	//TCTYPE_ask_class_name2(type, &class_name);
	//std::cout << "mailbox type name: " << class_name << std::endl;

	//create_item();

	//copy_item("wWPAAQrt5xMzAD", "000500", "A");

	//char* rev_name = nullptr;
	//ITEM_ask_rev_name2(get_latest_rev("wWLAAQrt5xMzAD"), &rev_name);
	//std::cout << "latest item rev: " << rev_name << std::endl;
	//MEM_free(rev_name);

	//output_workflow_num("wWLAAQrt5xMzAD");

	output_all_revs("wWLAAQrt5xMzAD");


	ITK_exit_module(TRUE);

	std::cout << "Press Enter to continue...";
	std::cin.get();

	return status;
}

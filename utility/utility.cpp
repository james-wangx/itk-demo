#include <iostream>
#include <string>

#include <sa/sa.h>
#include <tc/emh.h>
#include <tc/tc_util.h>
#include <tccore/aom.h>
#include <tccore/aom_prop.h>
#include <tccore/item.h>
#include <tccore/tctype.h>
#include <tcinit/tcinit.h>
#include <mld/journal/journal.h>

#include "item.hpp"
#include "rev.hpp"
#include "session.hpp"
#include "dataset.hpp"
#include "util.hpp"

const int WRONG_USAGE = 100001;
const bool DEBUG = false;

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

int ITK_user_main(int argc, char** argv)
{
    char* usr = ITK_ask_cli_argument("-u=");
    char* upw = ITK_ask_cli_argument("-p=");
    char* ugp = ITK_ask_cli_argument("-g=");
    char* help = ITK_ask_cli_argument("-h=");

    if (!usr || !upw || !ugp || help)
    {
        printf("\nUSEAGE: utility -u=user -p=password -g=group\n");
        return WRONG_USAGE;
    }

    int rcode = ITK_ok;
    tag_t rev = NULLTAG;
    tag_t dataset = NULLTAG;
    char* rev_id = NULL;

    CATCH(ITK_initialize_text_services(0));
    CATCH(ITK_init_module(usr, upw, ugp));
    printf("Login to Teamcenter success as %s", usr);
    output_filename();

    // Need env: TC_JOURNAL=FULL
    JOURNAL_comment("Preparing to list tool formats\n");
    TC_write_syslog("Preparing to list tool formats\n");

    //// Test create rev
    //char* rev_id = nullptr;
    //tag_t rev = rev::create_rev("giFAAQr15xMzAD");
    //ITKCALL_S(ITEM_ask_rev_id2(rev, &rev_id));
    //std::cout << "new rev id: " << rev_id << std::endl;

    //// Test copy rev
    //tag_t rev = rev::copy_rev("giFAAQr15xMzAD");
    //char* item_id;
    //char* rev_id;
    //ITKCALL_S(AOM_ask_value_string(rev, "item_id", &item_id));
    //ITKCALL_S(AOM_ask_value_string(rev, "item_revision_id", &rev_id));
    //std::cout << "item id: " << item_id << "\n"
    //    << "item revision id: " << rev_id << std::endl;

    //// Test revise rev
    //tag_t source;
    //tag_t target;
    //ITK__convert_uid_to_tag("ANMAAYiH5xMzAD", &source);
    //ITK_CALL_S(rev_revise(source, &target));

    //// Test get latest rev
    //ITK_CALL_S(rev_get_latest("giFAAQr15xMzAD", &rev));
    //ITK_CALL_S(ITEM_ask_rev_id2(rev, &rev_id));
    //printf("latest item rev id: %s", rev_id);

    //// Test get latest released rev
    //ITK_CALL_S(rev_get_latest_released("ApHAAYiL5xMzAD", &rev));
    //ITK_CALL_S(ITEM_ask_rev_id2(rev, &rev_id));
    //printf("latest released rev id: %s", rev_id);

    //// Test create dataset
    //TRANCE(dataset_create("gqFAAYiL5xMzAD", &dataset, "PDF", "test name", "test desc", "IMAN_specification"));

    //// Test upload dataset
    //TRANCE(dataset_upload(dataset, "C:\\Users\\Administrator\\Documents\\test.pdf", "PDF_Reference"));

    //// Test replace reference
    //ITK__convert_uid_to_tag("ABAAAgS45xMzAD", &dataset);
    //TRANCE(dataset_replace_ref(dataset, "PDF_Reference", "C:\\Users\\Administrator\\Documents\\replace.pdf"));

    // Test export dataset ref
    ITK__convert_uid_to_tag("ABAAAgS45xMzAD", &dataset);
    TRANCE(dataset_export_ref(dataset, "PDF_Reference", "C:\\Users\\Administrator\\Documents\\export.pdf"))

    /* Call your functions between here */
    //tag_t session = get_session();

    //std::string tc_root = session::get_tc_root();
    //std::string tc_bin = get_tc_bin();
    //if (!tc_root.empty())
    //	std::cout << "tc root: " << tc_root << std::endl;
    //std::cout << "tc_bin: " << tc_bin << std::endl;

    //tag_t user = session::get_user();
    //char* username = nullptr;
    //POM_ask_user_name(user, &username);
    //std::cout << "username: " << username << std::endl;

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

    //item::create_item("000501", "itk-test-2");

    //item::copy_item("AVIAAUrY5xMzAD", "000502", NULL);

    //char* rev_id = nullptr;
    //ITEM_ask_rev_id2(item::get_latest_rev("giFAAQr15xMzAD"), &rev_id);
    //std::cout << "latest item rev: " << rev_id << std::endl;
    //util::mem_free_s(rev_id);

    //char* rev_id = nullptr;
    //tag_t rev = item::get_latest_released_rev("giFAAQr15xMzAD");
    //ITEM_ask_rev_id2(rev, &rev_id);
    //std::cout << "latest released rev: " << rev_id << std::endl;
    //util::mem_free_s(rev_id);

    //int workflow_num = item::get_workflow_num("giFAAQr15xMzAD");
    //std::cout << "work flow num: " << workflow_num << std::endl;

    //tag_t* revs = nullptr;
    //int count = 0;
    //char* rev_id = nullptr;
    //item::get_all_revs("giFAAQr15xMzAD", count, revs);
    //for (size_t i = 0; i < count; i++)
    //{
    //	ITEM_ask_rev_id2(revs[i], &rev_id);
    //	std::cout << "item revision id: " << rev_id << std::endl;
    //	util::mem_free_s(rev_id);
    //}
    //util::mem_free_s(revs);

    //output_all_revs("giFAAQr15xMzAD");

    //std::vector<tag_t> base_rev_vector = item::get_all_base_revs("giFAAQr15xMzAD");
    //char* rev_id = nullptr;
    //for (tag_t base_rev : base_rev_vector)
    //{
    //	ITEM_ask_rev_id2(base_rev, &rev_id);
    //	std::cout << "base rev id: " << rev_id << std::endl;
    //	util::mem_free_s(rev_id);
    //}

CLEANUP:
    MEM_FREE_S(rev_id);
    ITK_exit_module(true);
    return rcode;
}

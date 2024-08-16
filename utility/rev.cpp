#include <tc/tc_startup.h>
#include <tccore/item.h>
#include <tcinit/tcinit.h>
#include <tccore/tctype.h>
#include <tccore/aom.h>
#include <tccore/aom_prop.h>

#include "rev.hpp"
#include "util.hpp"

namespace rev
{

    tag_t create_rev(const char* item_uid)
    {
        tag_t item = NULLTAG;
        tag_t rev = NULLTAG;

        ITK__convert_uid_to_tag(item_uid, &item);
        ITKCALL_S(ITEM_create_rev(item, NULL, &rev));
        ITKCALL_S(ITEM_save_rev(rev));

        return rev;
    }

    tag_t copy_rev(const char* item_uid)
    {
        tag_t item = NULLTAG;
        tag_t source_rev = NULLTAG;
        tag_t target_rev = NULLTAG;

        ITK__convert_uid_to_tag(item_uid, &item);
        ITKCALL_S(ITEM_ask_latest_rev(item, &source_rev));
        ITKCALL_S(ITEM_copy_rev(source_rev, NULL, &target_rev));

        return target_rev;
    }

} // namespace rev

int rev_revise(tag_t source, tag_t* target)
{
    int rcode = ITK_ok;
    tag_t type = NULLTAG;
    tag_t save_as_input = NULLTAG;
    tag_t* deep_copy_data = NULL;
    tag_t* target_copy = NULL;
    int deep_copy_data_num = 0;
    int* ifails = NULL;

    ITK_CALL_S(TCTYPE_ask_object_type(source, &type));
    ITK_CALL_S(TCTYPE_construct_operationinput(type, TCTYPE_OPERATIONINPUT_REVISE, &save_as_input));
    ITK_CALL_S(TCTYPE_ask_deepcopydata(source, TCTYPE_OPERATIONINPUT_REVISE, &deep_copy_data_num, &deep_copy_data));
    ITK_CALL_S(AOM_set_value_string(save_as_input, "object_desc", "I was revised"));
    TCTYPE_revise_objects(1, &source, &save_as_input, &deep_copy_data_num, deep_copy_data, &target_copy, &ifails);

    *target = target_copy[ 0 ];
    rcode = *ifails;

CLEANUP:
    MEM_FREE_S(deep_copy_data);
    MEM_FREE_S(target_copy);
    return rcode;
}

int rev_get_latest(const char* item_uid, tag_t* rev)
{
    int rcode = ITK_ok;
    tag_t item = NULLTAG;

    ITK__convert_uid_to_tag(item_uid, &item);
    ITK_CALL_S(ITEM_ask_latest_rev(item, rev));

CLEANUP:
    return rcode;
}

int rev_get_latest_released(const char* item_uid, tag_t* rev)
{
    int rcode = ITK_ok;
    tag_t item = NULLTAG;
    tag_t* revs = NULL;
    tag_t* rels = NULL;
    int rev_count = 0;
    int rel_count = 0;
    char* rev_id = NULL;

    ITK__convert_uid_to_tag(item_uid, &item);
    ITK_CALL_S(ITEM_list_all_revs(item, &rev_count, &revs));

    for (int i = rev_count - 1; i >= 0; i--)
    {
        ITK_CALL_S(WSOM_ask_release_status_list(revs[ i ], &rel_count, &rels));
        if (rel_count > 0)
        {
            *rev = revs[ i ];
            break;
        }
    }

CLEANUP:
    MEM_FREE_S(rev_id);
    return rcode;
}

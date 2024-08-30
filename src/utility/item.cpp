#include <iostream>

#include <tccore/aom.h>
#include <tccore/aom_prop.h>
#include <tccore/item.h>
#include <tccore/tctype.h>

#include "item.hpp"
#include "util.hpp"

namespace item
{

    tag_t create_item(const char* item_id, const char* item_name)
    {
        tag_t type = NULLTAG;
        tag_t item_create_input = NULLTAG;
        tag_t item = NULLTAG;

        TCTYPE_find_type("Item", "Item", &type);
        TCTYPE_construct_create_input(type, &item_create_input);
        TCTYPE_set_create_display_value(item_create_input, const_cast< char* >( "item_id" ), 1, &item_id);
        TCTYPE_set_create_display_value(item_create_input, const_cast< char* >( "object_name" ), 1, &item_name);
        TCTYPE_create_object(item_create_input, &item);
        ITEM_save_item(item);

        return item;
    }

    tag_t copy_item(const char* source_rev_uid, const char* target_item_id, const char* target_rev_id)
    {
        tag_t source_rev = NULLTAG;
        tag_t target_item = NULLTAG;
        tag_t target_rev = NULLTAG;

        ITK__convert_uid_to_tag(source_rev_uid, &source_rev);
        ITEM_copy_item(source_rev, target_item_id, target_rev_id, &target_item, &target_rev);

        return target_item;
    }

    tag_t get_latest_rev(const char* item_uid)
    {
        tag_t item = NULLTAG;
        tag_t rev = NULLTAG;

        ITK__convert_uid_to_tag(item_uid, &item);
        ITEM_ask_latest_rev(item, &rev);

        return rev;
    }

    tag_t get_latest_released_rev(const char* item_uid)
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
            ITKCALL(AOM_ask_value_date(revs[ i ], "date_released", &rev_date));
            if (util::compare_dates(rev_date, latest_date) >= 0)
            {
                latest_date = rev_date;
                latest_released_rev = revs[ i ];
            }
        }

        MEM_free(revs);
        return latest_released_rev;
    }

    int get_workflow_num(const char* item_uid)
    {
        tag_t item = NULLTAG;
        tag_t* workflows = nullptr;
        int num = 0;

        ITK__convert_uid_to_tag(item_uid, &item);
        AOM_ask_value_tags(item, "fnd0AllWorkflows", &num, &workflows);

        MEM_free(workflows);
        return num;
    }

    void get_all_revs(const char* item_uid, int& count, tag_t*& revs)
    {
        tag_t item = NULLTAG;

        ITK__convert_uid_to_tag(item_uid, &item);
        ITEM_list_all_revs(item, &count, &revs);
    }

    std::vector<tag_t> get_all_base_revs(const char* item_uid)
    {
        int rev_count = 0;
        int rel_count = 0;
        char* rev_id = nullptr;
        tag_t base_rev = NULLTAG;
        tag_t* revs = nullptr;
        tag_t* status = nullptr;
        std::vector<tag_t> base_rev_vector;

        get_all_revs(item_uid, rev_count, revs);

        for (size_t i = 0; i < rev_count; i++)
        {
            WSOM_ask_release_status_list(revs[ i ], &rel_count, &status);
            if (rel_count)
            {
                ITEM_rev_find_base_rev(revs[ i ], &base_rev);
                if (base_rev)
                    base_rev_vector.push_back(base_rev);
                util::mem_free_s(rev_id);
            }
            util::mem_free_s(status);
        }

        util::mem_free_s(revs);
        return base_rev_vector;
    }

} // namespace item

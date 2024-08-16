#include <tc/tc_startup.h>
#include <tccore/item.h>
#include <tcinit/tcinit.h>

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

    tag_t copy_rev(const char* item_uid) throw( ITKException )
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

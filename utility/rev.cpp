#include <tc/tc_startup.h>
#include <tccore/item.h>
#include <tcinit/tcinit.h>

#include "rev.hpp"
#include "util.hpp"

namespace rev
{

    tag_t create_rev(const char* item_uid) noexcept( false )
    {
        tag_t item = NULLTAG;
        tag_t rev = NULLTAG;

        ITK__convert_uid_to_tag(item_uid, &item);
        ITKCALL_S(ITEM_create_rev(item, NULL, &rev));
        ITKCALL_S(ITEM_save_rev(rev));

        return rev;
    }

} // namespace rev

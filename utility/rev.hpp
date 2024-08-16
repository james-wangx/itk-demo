#ifndef UTILITY_REV_HPP
#define UTILITY_REV_HPP

namespace rev
{

    tag_t create_rev(const char* item_uid);

    tag_t copy_rev(const char* item_uid);

} // namespace rev

int rev_revise(tag_t source, tag_t* target);

int rev_get_latest(const char* item_uid, tag_t* rev);

#endif // UTILITY_REV_HPP

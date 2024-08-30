#include <ae/dataset.h>
#include <ae/datasettype.h>
#include <tccore/aom.h>
#include <tccore/grm.h>
#include <sa/tcfile.h>
#include <fclasses/tc_string.h>
#include <res/res_itk.h>

#include "dataset.hpp"
#include "util.hpp"

int dataset_create(const char* rev_uid, tag_t* dataset, const char* dataset_type_name, const char* dataset_name,
    const char* dataset_desc, const char* relation_type_name)
{
    int rcode = ITK_ok;
    tag_t rev = NULLTAG;
    tag_t relation = NULLTAG;
    tag_t dataset_type = NULLTAG;
    tag_t relation_type = NULLTAG;

    ITK__convert_uid_to_tag(rev_uid, &rev);
    CATCH(AE_find_datasettype2(dataset_type_name, &dataset_type));
    CATCH(AE_create_dataset_with_id(dataset_type, dataset_name, dataset_desc, NULL, NULL, dataset));
    CATCH(AOM_save(*dataset));
    CATCH(AOM_lock(rev));
    GRM_find_relation_type(relation_type_name, &relation_type);
    CATCH(GRM_create_relation(rev, *dataset, relation_type, NULL, &relation));
    CATCH(GRM_save_relation(relation));
    CATCH(AOM_unlock(relation));
    CATCH(AOM_unlock(rev));

CLEANUP:
    return rcode;
}

int dataset_import_ref(tag_t dataset, const char* file_path, const char* reference_name)
{
    int rcode = ITK_ok;
    tag_t file = NULLTAG;
    IMF_file_t file_descriptor = NULL;

    CATCH(IMF_import_file(file_path, NULL, SS_BINARY, &file, &file_descriptor));
    CATCH(IMF_close_file(file_descriptor));
    CATCH(AOM_save(file));
    CATCH(AOM_unlock(file));
    CATCH(AOM_lock(dataset));
    CATCH(AE_add_dataset_named_ref2(dataset, reference_name, AE_PART_OF, file));
    CATCH(AE_save_myself(dataset));
    CATCH(AOM_unlock(dataset));

CLEANUP:
    return rcode;
}

int dataset_replace_ref(tag_t dataset, const char* ref_name, const char* target_ref_path)
{
    int rcode = ITK_ok;
    AE_reference_type_t ref_type;
    tag_t ref = NULLTAG;

    CATCH(AE_ask_dataset_named_ref2(dataset, ref_name, &ref_type, &ref));
    CATCH(IMF_replace_file(ref, target_ref_path, false));

CLEANUP:
    return rcode;
}

int dataset_export_ref(tag_t dataset, const char* ref_name, const char* ref_path)
{
    int rcode = ITK_ok;
    AE_reference_type_t ref_type;
    tag_t ref = NULLTAG;

    CATCH(AE_ask_dataset_named_ref2(dataset, ref_name, &ref_type, &ref));
    CATCH(IMF_export_file(ref, ref_path));

CLEANUP:
    return rcode;
}

int dataset_is_checked_out(tag_t dataset, logical* is_checked_out)
{
    int rcode = ITK_ok;

    CATCH(RES_is_checked_out(dataset, is_checked_out));

CLEANUP:
    return rcode;
}

int dataset_checkout(tag_t dataset)
{
    int rcode = ITK_ok;

    CATCH(RES_checkout2(dataset, NULL, NULL, "C:\\temp", RES_EXPORT_FILE));

CLEANUP:
    return rcode;
}

int dataset_checkin(tag_t dataset)
{
    int rcode = ITK_ok;

    CATCH(RES_checkin(dataset));

CLEANUP:
    return rcode;
}

int dataset_cancel_checkout(tag_t dataset, logical save)
{
    int rcode = ITK_ok;

    CATCH(RES_cancel_checkout(dataset, save));

CLEANUP:
    return rcode;
}

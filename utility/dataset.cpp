#include <ae/dataset.h>
#include <ae/datasettype.h>
#include <tccore/aom.h>
#include <tccore/grm.h>

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
    CATCH(AOM_unload(rev));

CLEANUP:
    return rcode;
}

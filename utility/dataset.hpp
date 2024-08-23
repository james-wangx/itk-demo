#ifndef UTILITY_DATASET_HPP
#define UTILITY_DATASET_HPP

int dataset_create(const char* rev_uid, tag_t* dataset, const char* dataset_type_name, const char* dataset_name,
    const char* dataset_desc, const char* relation_type_name);

int dataset_import_ref(tag_t dataset, const char* file_path, const char* reference_name);

int dataset_replace_ref(tag_t dataset, const char* ref_name, const char* target_ref_path);

int dataset_export_ref(tag_t dataset, const char* ref_name, const char* ref_path);

int dataset_is_checked_out(tag_t dataset, logical* is_checked_out);

#endif // UTILITY_DATASET_HPP

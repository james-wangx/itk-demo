#ifndef UTILITY_DATASET_HPP
#define UTILITY_DATASET_HPP

int dataset_create(const char* rev_uid, tag_t* dataset, const char* dataset_type_name, const char* dataset_name,
    const char* dataset_desc, const char* relation_type_name);

int dataset_upload(tag_t dataset, const char* file_path, const char* reference_name);

#endif // UTILITY_DATASET_HPP

#ifndef S3TPC_HASHMAP_H_
#define S3TPC_HASHMAP_H_


#include <stdlib.h>


struct hash_map;

struct hash_map *hm_create(size_t capacity);
void hm_destroy(struct hash_map *hm);

void *hm_insert(struct hash_map *hm, unsigned key, void *value);
void *hm_remove(struct hash_map *hm, unsigned key);
void *hm_find(struct hash_map *hm, unsigned key);

size_t hm_size(struct hash_map *hm);


#endif

#ifndef HASH_MAP_H
#define HASH_MAP_H

#include "linked_list.h"

typedef unsigned int (*hash_map_hash_function)(void *);

typedef struct hash_map {
    int bucket_count;
    linked_list *buckets;
    hash_map_hash_function hash_function;
} hash_map;

void hash_map_init(hash_map *map, int bucket_count,
                   hash_map_hash_function hash_function);
void hash_map_destroy(hash_map *map);
void hash_map_insert(hash_map *map, void *key, void *data);
void hash_map_remove(hash_map *map, void *key);

#endif

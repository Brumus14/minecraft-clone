#include "hash_map.h"

void hash_map_init(hash_map *map, int bucket_count,
                   hash_map_hash_function hash_function) {
    map->bucket_count = bucket_count;
    map->hash_function = hash_function;
}

void hash_map_destroy(hash_map *map) {
}

void hash_map_insert(hash_map *map, void *key, void *data) {
    int index = map->hash_function(key) % map->bucket_count;
    linked_list_insert_end(&map->buckets[index], data);
}

void hash_map_remove(hash_map *map, void *key) {
    int index = map->hash_function(key) % map->bucket_count;
    // linked_list_remove(&map->buckets[index], data);
}

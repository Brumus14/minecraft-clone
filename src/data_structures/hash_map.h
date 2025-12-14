#ifndef HASH_MAP_H
#define HASH_MAP_H

typedef struct hash_map_node hash_map_node;
typedef struct hash_map_node {
    void *key;
    void *value;
    hash_map_node *next;
} hash_map_node;

void hash_map_node_init(hash_map_node *node, void *key, void *value);
void *hash_map_node_destroy(hash_map_node *node);

typedef unsigned int (*hash_map_hash_function)(void *);
typedef void (*hash_map_for_each_function)(void *key, void *value,
                                           void *context);

typedef struct hash_map {
    int bucket_count;
    unsigned long key_size;
    unsigned long value_size;
    hash_map_node **buckets;
    hash_map_hash_function hash_function;
    int iter_bucket;
    hash_map_node *iter_current_node;
} hash_map;

// TODO: Maybe restructure ownership so takes pointer and returns when
// overwritten or removed for user to free
// TODO: Dynamic bucket count that auto increases / decreases
void hash_map_init(hash_map *map, int bucket_count, unsigned long key_size,
                   unsigned long value_size,
                   hash_map_hash_function hash_function);
void hash_map_destroy(hash_map *map);
void *hash_map_put(hash_map *map, void *key, void *value);
void *hash_map_get(hash_map *map, void *key);
void *hash_map_remove(hash_map *map, void *key);
void hash_map_for_each(hash_map *map, hash_map_for_each_function function,
                       void *context);

#endif

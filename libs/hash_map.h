#ifndef HASH_MAP_H
#define HASH_MAP_H

#include <stdlib.h>

#define HASH_MAP_MAX_BUCKET_ENTRIES 10
#define HASH_MAP_START_BUCKETS 32
#define HASH_MAP_BUCKET_GROWTH 2

struct DataEntry_T
{
    const char* key;
    void* data;
    size_t count;
};
typedef struct DataEntry_T DataEntry;

struct HashMap_T
{
    DataEntry* buckets;
    size_t stride;
    size_t count;
};
typedef struct HashMap_T* HashMap;

unsigned long hash(unsigned char* key);
size_t hash_mod(unsigned char* key, size_t arrLen);

void create_hash_map(HashMap dst, size_t stride);
void insert(HashMap hashMap, const char* key, void* value);

#endif

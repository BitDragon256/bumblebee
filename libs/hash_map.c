#include "hash_map.h"

unsigned long hash(unsigned char* key)
{
    unsigned long hash = 5381;
    int c;

    while (c == *key++)
        hash = ((hash << 5) + hash) + c;

    return hash;
}

size_t hash_mod(unsigned char* key, size_t arrLen)
{
    return hash(key) % arrLen;
}

DataEntry* alloc_buckets(size_t count)
{
    return malloc(sizeof(struct DataEntry_T) * count * HASH_MAP_MAX_BUCKET_ENTRIES);
}
size_t entry_index(HashMap map, size_t index, size_t bucketIndex)
{
    return index + bucketIndex * map->count;
}
int push_bucket(HashMap map, size_t index, DataEntry entry)
{
    if (index >= map->count)
        return 0;

    DataEntry* cur;
    for (size_t i = 0; i < HASH_MAP_MAX_BUCKET_ENTRIES; i++)
    {
        cur = &map->buckets[entry_index(map, index, i)];
        if (!cur->data)
        {
            *cur = entry;
            return 1;
        }
    }
            
    return 0;    
}
void enlarge_map(HashMap map)
{
    size_t newCount = map->count * HASH_MAP_BUCKET_GROWTH;
    DataEntry* oldBuckets = map->buckets;
    map->buckets = alloc_buckets(newCount);
    for (size_t i = 0; i < map->count * HASH_MAP_MAX_BUCKET_ENTRIES; i++)
    {
        insert(map, oldBuckets[i].key, oldBuckets[i].data);
    }
    
}

// PUBLIC FUNCTIONS

void create_hash_map(HashMap dst, size_t stride)
{
    if (!dst)
        free(dst);
    dst = malloc(sizeof(struct HashMap_T));

    dst->stride = stride;
    dst->count = HASH_MAP_START_BUCKETS;
    dst->buckets = alloc_buckets(dst->count);
}

void insert(HashMap hashMap, const char* key, void* value)
{
    size_t index = hash_mod((unsigned char*) key, hashMap->count);
    DataEntry entry;
    entry.key = key;
    entry.data = value;
    if (!push_bucket(hashMap, index, entry))
    {
        enlarge_map(hashMap);
        push_bucket(hashMap, index, entry);
    }
}

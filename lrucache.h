#pragma once
#include "lrudriver.h"

typedef struct cache_t cache_t;

void lrucache_init(cache_t**, options_t*);
void lrucache_run_tracefile(cache_t*);
void lrucache_destroy(cache_t*);
uint32_t lrucache_get_hits(cache_t*);
uint32_t lrucache_get_misses(cache_t*);
uint32_t lrucache_get_evictions(cache_t*);


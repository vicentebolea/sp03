#include "lrucache.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <search.h>

typedef struct {
  uint32_t tag;
  uint32_t time;
  bool is_being_used;
} cache_node_t ;

struct cache_t {
  cache_node_t* cache;
  size_t size;
  options_t* opt;
  uint32_t misses;
  uint32_t hits;
  uint32_t evictions;
  uint32_t assoc;
};

int cmp_tags(const void* tag, const void* elem) { 
  cache_node_t* node = (cache_node_t*)elem;
  return !(node->is_being_used && (*(uint32_t*)tag == node->tag));
}
int cmp_empty(const void* dumb, const void* elem) { return ((cache_node_t*)elem)->is_being_used; }
// get_index {{{
// It will compute the index of a given address
static uint32_t get_index (uint32_t addr, uint32_t index_width, uint32_t block_width) {
  if (index_width == 0) {
    return 0;
  }
  uint32_t output = 0;

  //block_width += 4;
  uint32_t index_pos = index_width + block_width;
  uint32_t mask_left  = ((1 << (index_pos)) - 1); //! left = 4  then mask: 0000 1111 1111

  output = addr & mask_left;

  output >>= block_width;

  return output;
}
// }}}
// get_tag {{{
// It will compute the tag of a given address
static uint32_t get_tag (uint32_t addr, uint32_t index_width, uint32_t block_width) {
  uint32_t shift = index_width + block_width ; //word size = 4
  addr >>= shift;

  return addr;
}
// }}}
// cache_increment_time {{{
void cache_increment_time(cache_t* cache, uint32_t offset) {
  int i = 0;
  do {
    cache->cache[offset + i].time++;
  } while (++i < cache->assoc);
}
//}}}
// cache_find_node {{{
//cache_node_t* cache_find_node(cache_t* cache, uint32_t offset, uint32_t tag) {
//  //int i;
//  //cache_node_t to_search = { .tag = tag};
//
//  cache_node_t* node = lsearch(&tag, &cache->cache[offset], (size_t*)&cache->assoc, sizeof(cache_node_t), compare);
//  return node;
//
//  //for (i = 0; i < cache->assoc; i++) {
//  //  cache_node_t* node = &cache->cache[offset + i];
//  //  if (node->tag == tag) {
//  //    return node;
//  //  }
//  //}
//  //return NULL;
//}
//}}}
// cache_evict {{{
cache_node_t* cache_evict (cache_t* cache, uint32_t offset) {
  cache->evictions++;
  int i = 0;                
  uint32_t max = 0;
  int max_index = 0;

  do{
    cache_node_t * node = &cache->cache[offset + i];
    if (node->time > max) {
      max = node->time;
      max_index = i;
    }
  } while (i++ < cache->assoc );

  return &cache->cache[offset + max_index];
}
//}}}
// cache_find_next {{{
//cache_node_t* cache_find_next (cache_t* cache, uint32_t offset) {
//  return lsearch(0, &cache->cache[offset], (size_t*)&cache->assoc, sizeof(cache_node_t), empty);
//  //int i;
//  //for (i = 0; i < cache->assoc; i++) {
//  //  cache_node_t* node = &cache->cache[offset + i];
//  //  if (!node->is_being_used) 
//  //    return node;
//  //}
//  //return NULL;
//}
//}}}
// cache insert {{{
void cache_insert(cache_t* cache, uint32_t tag, uint32_t index) {
  uint32_t offset = index * cache->assoc;
  size_t set_size = cache->assoc;
  cache_node_t* set = &cache->cache[offset];

  cache_node_t* node = lfind(&tag, set, &set_size, sizeof(cache_node_t), cmp_tags);
  if (!node) {
    node = lfind(0, set, &set_size, sizeof(cache_node_t), cmp_empty);
    cache->misses++;

    if (!node) {
      node = cache_evict(cache, offset);
    }
  } else {
    cache->hits++;
  }

  cache_increment_time(cache, offset);

  node->tag = tag;
  node->time = 0;
  node->is_being_used = true;
}
//}}}
// init & destroy{{{
void lrucache_init(cache_t** cache, options_t* opts) {
  *cache = malloc(sizeof(cache_t));
  cache_t* ca = *cache;

  uint32_t sets   = lrudriver_get_sets(opts);
  uint32_t assoc  = lrudriver_get_associativity(opts);

  uint32_t size_of_cache = pow(2,sets)*assoc;
  ca->cache = calloc(sizeof(cache_node_t), size_of_cache);
  ca->hits   = 0;
  ca->misses = 0;
  ca->assoc = assoc;
  ca->opt = opts;
}

void lrucache_destroy(cache_t* cache) {
  free(cache->cache);
  free(cache);
}
// }}}
// run {{{
void lrucache_run_tracefile(cache_t* cache) {
  uint32_t sets   = lrudriver_get_sets(cache->opt);
  uint32_t blocks = lrudriver_get_blocks(cache->opt);

  char* filename;
  lrudriver_get_file(cache->opt, &filename);
  FILE* file = fopen(filename, "r");

  uint32_t addr;
  while (fscanf(file, " %*c %x,%*d", &addr) != EOF) {
    uint32_t tag =   get_tag(addr, sets, blocks);
    uint32_t index = get_index(addr, sets, blocks);
    cache_insert(cache, tag, index);
    printf ("Addr: %u %u %u H:%i M:%i E:%i\n", addr, tag, index, cache->hits, cache->misses, cache->evictions);
  }
  fclose(file);
}
// }}}
// getters {{{
uint32_t lrucache_get_hits(cache_t* cache) {
  return cache->hits;
}
uint32_t lrucache_get_misses(cache_t* cache) {
  return cache->misses;
}
uint32_t lrucache_get_evictions(cache_t* cache) {
  return cache->evictions;
}
//}}}

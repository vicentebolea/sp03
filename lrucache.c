#include "lrucache.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

struct cache_t {
  bool* array;
  size_t size;
  options_t* opt;
};

// init & destroy{{{
void lrucache_init(cache_t** cache, options_t* opts) {
  *cache = malloc(sizeof(cache_t));
  cache_t* ca = *cache;

  uint32_t sets   = lrudriver_get_sets(opts);
  uint32_t assoc  = lrudriver_get_associativity(opts);
  uint32_t blocks = lrudriver_get_blocks(opts);

  uint32_t size_of_cache = sets*assoc*blocks;
  ca->array = malloc(sizeof(bool) * size_of_cache);
}

void lrucache_destroy(cache_t* cache) {
  free(cache->array);
  free(cache);
}
// }}}
// run {{{
void lrucache_run_tracefile(cache_t* cache) {
  char* filename;
  lrudriver_get_file(cache->opt, &filename);
  FILE* file = fopen(filename, "r");

  while (feof(file)) {
    char type;
    uint32_t addr;
    uint32_t addr_size;

    fscanf(file, "%c %x,%d", &type, &addr, &addr_size);
  }
}
// }}}

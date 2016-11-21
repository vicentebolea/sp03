#include "cachelab.h"
#include "lrudriver.h"
#include "lrucache.h"
#include <stdlib.h>

int main(int argc, char** argv) {
  options_t* opts;
  cache_t* cache;

  lrudriver_init(&opts, argc, argv);
  lrucache_init(&cache, opts);

  lrucache_run_tracefile(cache);

  printSummary(lrucache_get_hits(cache), 
      lrucache_get_misses(cache),
      lrucache_get_evictions(cache));

  lrucache_destroy(cache);
  lrudriver_destroy(opts);

  return EXIT_SUCCESS;
}

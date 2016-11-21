#include "cachelab.h"
#include "lrudriver.h"
#include "lrucache.h"

int main(int argc, char** argv)
{
    options_t* opts;
    cache_t* cache;

    lrudriver_init(&opts, argc, argv);
    lrucache_init(&cache, opts);

    lrucache_run_tracefile(cache);

    lrucache_destroy(cache);
    lrudriver_destroy(opts);

    printSummary(0, 0, 0);
    return 0;
}

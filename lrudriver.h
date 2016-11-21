#pragma once
#include <stdint.h>

typedef struct options_t options_t; 

void lrudriver_load_options(options_t*, int, char**);
void lrudriver_get_file(const options_t*, char**);
uint32_t lrudriver_get_sets(const options_t*);
uint32_t lrudriver_get_associativity(const options_t*);
uint32_t lrudriver_get_blocks(const options_t*);

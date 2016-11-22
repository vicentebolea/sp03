/* Author: Vicente Adolfo Bolea Sanchez 20131780 <vicente.bolea@gmail.com>
 * Title: Assignment 3, System Programming, UNIST 
 * License: GPL3
 */
#pragma once
#include <stdint.h>

typedef struct options_t options_t; 

void lrudriver_init(options_t**, int, char**);
void lrudriver_destroy(options_t*);

void lrudriver_get_file(const options_t*, char**);
uint32_t lrudriver_get_sets(const options_t*);
uint32_t lrudriver_get_associativity(const options_t*);
uint32_t lrudriver_get_blocks(const options_t*);

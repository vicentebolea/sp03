/* Author: Vicente Adolfo Bolea Sanchez 20131780 <vicente.bolea@gmail.com>
 * Title: Assignment 3, System Programming, UNIST 
 * License: GPL3
 */
#include "lrudriver.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <stdio.h>

struct options_t {
  uint32_t sets;
  uint32_t assoc;
  uint32_t blocks;
  char file[256];
};

// load {{{
// Creates the option_t ADT and parse the command line input
void lrudriver_init(options_t** opt, int argc, char** argv) {
  *opt = calloc(sizeof(options_t), 1);
  options_t* op = *opt;
 
  char c; 
  while (-1 != (c = getopt(argc, argv, "s:E:b:t:"))) {
    switch(c) {
      case 's':
        op->sets = atoi(optarg);
        break;
      case 'E':
        op->assoc = atoi(optarg);
        break;
      case 'b':
        op->blocks = atoi(optarg);
        break;
      case 't':
        strncpy(op->file, optarg, 256);
        break;
      default:
        perror ("Something is wrong with your options");
        break;
    }
  }
}

void lrudriver_destroy(options_t* opt) {
  free(opt);
}
// }}} 
// getters {{{
// Simple getters 
uint32_t lrudriver_get_sets(const options_t* opt) {
  return opt->sets;
}
uint32_t lrudriver_get_associativity(const options_t* opt) {
  return opt->assoc;
}
uint32_t lrudriver_get_blocks(const options_t* opt){ 
  return opt->blocks;
}
void lrudriver_get_file(const options_t* opt, char** file) { 
  *file = malloc(sizeof(char)*256);
  strncpy(*file, opt->file, 255);
}
// }}}

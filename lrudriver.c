#include "lrudriver.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

struct options_t {
  uint32_t sets;
  uint32_t assoc;
  uint32_t blocks;
  char file[256];
};

// load {{{
void lrudriver_init(options_t** opt, int argc, char** argv) {

}

void lrudriver_destroy(options_t* opt) {
  free(opt);
}
// }}} 
// getters {{{
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
  file = malloc(sizeof(char)*256);
  strncpy(*file, opt->file, 255);
}
// }}}

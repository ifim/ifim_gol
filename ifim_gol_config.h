#ifndef __IFIM_GOL_CONFIG_H
#define __IFIM_GOL_CONFIG_H

#include <stdbool.h>

typedef struct {
  char *board_file;
  char alive_symbol, dead_symbol;
  bool loop;
  int generations;
} ifim_gol_config_t;

ifim_gol_config_t *parse_args(int argc, char **argv);

#endif

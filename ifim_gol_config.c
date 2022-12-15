#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>

#include "ifim_gol_config.h"

ifim_gol_config_t *parse_args(int argc, char **argv) {
  ifim_gol_config_t *config;

  if ((config = malloc(sizeof(ifim_gol_config_t))) == NULL) {
    fprintf(stderr, "error: unable to allocate memory\n");
    exit(EXIT_FAILURE);
  }

  config->loop = false;
  config->alive_symbol = '#';
  config->dead_symbol = ' ';

  const struct option options[] = {
    {"board-file", required_argument, 0, 0},
    {"alive-symbol", required_argument, 0, 0},
    {"dead-symbol", required_argument, 0, 0},
    {"loop", no_argument, 0, 0},
  };

  for (int c, option_index = 0;;) {
    if ((c = getopt_long(argc, argv, "", options, &option_index)) == -1) {
      break;
    }

    switch(c) {
    case 0:
      if (strcmp("board-file", options[option_index].name) == 0) {
        config->board_file = optarg;
      } else if (strcmp("alive-symbol", options[option_index].name) == 0) {
        config->alive_symbol = (optarg == NULL) ? '#' : optarg[0];
      } else if (strcmp("dead-symbol", options[option_index].name) == 0) {
        config->dead_symbol = (optarg == NULL) ? ' ' : optarg[0];
      } else if (strcmp("loop", options[option_index].name) == 0) {
        config->loop = true;
      }

      break;
    }
  }
  
  return config;
}

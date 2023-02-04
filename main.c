#include "ifim_gol_config.h"
#include "ifim_gol_board.h"


int main(int argc, char **argv) {
  ifim_gol_config_t *config = parse_args(argc, argv);

  ifim_gol_board_t *board = board_new_from_file(config->board_file, config->alive_symbol, config->dead_symbol);

  board_run(board, config->loop, config->generations, board_display);

  board_destroy(board);

  return 0;
}

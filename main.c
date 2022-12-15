#include "ifim_gol_config.h"
#include "ifim_gol_board.h"


int main(int argc, char **argv) {
  int width = 20;
  int height = 20;

  ifim_gol_config_t *config = parse_args(argc, argv);

  //board_t *board = board_new(width, height, alive_symbol, dead_symbol);
  ifim_gol_board_t *board = board_new_from_file(config->board_file, config->alive_symbol, config->dead_symbol);

  // Create a "glider"
  // https://en.wikipedia.org/wiki/Glider_(Conway%27s_Life)
  /*
  board_place_cell(board, 6, 6);
  board_place_cell(board, 7, 6);
  board_place_cell(board, 8, 6);
  board_place_cell(board, 6, 7);
  board_place_cell(board, 7, 8);
  */
  // Run it
  board_run(board, config->loop, config->generations, board_display);
  //board_show(board);

  board_destroy(board);

  return 0;
}

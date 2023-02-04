#ifndef __IFIM_GOL_BOARD_H
#define __IFIM_GOL_BOARD_H

typedef struct {
  int **table;
  int width, height;
  char alive_symbol;
  char dead_symbol;
} ifim_gol_board_t;

int *board_row_new(int size);

void board_expand_height(ifim_gol_board_t *board, int units);

ifim_gol_board_t *board_new(int width, int height, char alive_symbol, char dead_symbol);

ifim_gol_board_t *board_new_from_file(char *file_name, char alive_symbol, char dead_symbol);

void board_destroy(ifim_gol_board_t *board);

void board_print(ifim_gol_board_t *board);

int board_get_cell_num_neighbors(ifim_gol_board_t *board, int x, int y);

bool board_cell_lives(ifim_gol_board_t *board, int x, int y);

void board_run(ifim_gol_board_t *current_board, bool loop, int generations, void (*fn)(ifim_gol_board_t *));

void board_display(ifim_gol_board_t *board);

#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "ifim_gol_board.h"

int *board_row_new(int size) {
  int *row = NULL;

  if ((row = calloc(size, sizeof(int))) == NULL) {
    fprintf(stderr, "error: unable to allocate memory\n");
    exit(EXIT_FAILURE);
  }

  return row;
}

void board_expand_height(ifim_gol_board_t *board, int units) {
  board->height += units;
  if ((board->table = realloc(board->table, sizeof(int*) * board->height)) == NULL) {
    fprintf(stderr, "error: unable to reallocate memory\n");
    exit(EXIT_FAILURE);
  }
}

ifim_gol_board_t *board_new(int width, int height, char alive_symbol, char dead_symbol) {
  ifim_gol_board_t *board;

  if ((board = malloc(sizeof(ifim_gol_board_t))) == NULL) {
    fprintf(stderr, "error: unable to allocate memory\n");
    exit(EXIT_FAILURE);
  }

  board->width = width;
  board->height = height;
  board->alive_symbol = alive_symbol;
  board->dead_symbol = dead_symbol;

  if ((board->table = malloc(sizeof(int*) * height)) == NULL) {
    fprintf(stderr, "error: unable to allocate memory\n");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < height; i++) {
    board->table[i] = board_row_new(width);
  }

  return board;
}

ifim_gol_board_t *board_new_from_file(char *file_name, char alive_symbol, char dead_symbol) {
  FILE *stream;
  char *line = NULL;
  size_t length = 0;
  ssize_t nread;

  if ((stream = fopen(file_name, "r")) == NULL) {
    fprintf(stderr, "error: unable to read file\n");
    exit(EXIT_FAILURE);
  }

  int width = 0;
  ifim_gol_board_t *board = NULL;
  if ((nread = getline(&line, &length, stream)) != -1) {
    width = nread;

    board = board_new(width - 1, 1, alive_symbol, dead_symbol);

    for (int x = 0; x < width; x++) {
      switch(line[x]) {
      case '0':
        board->table[0][x] = 0;
        break;
      case '1':
        board->table[0][x] = 1;
        break;
      }
    }
  }

  for (int y = 1; (nread = getline(&line, &length, stream)) != -1; y++) {
    if (nread != width) {
      fprintf(stderr, "error: length mismatch\n");
      exit(EXIT_FAILURE);
    }

    board_expand_height(board, 1);
    board->table[y] = board_row_new(board->width);

    for (int x = 0; x < width; x++) {
      switch(line[x]) {
      case '0':
        board->table[y][x] = 0;
        break;
      case '1':
        board->table[y][x] = 1;
        break;
      }
    }
  }

  return board;
}

void board_destroy(ifim_gol_board_t *board) {
  for (int i = 0; i < board->height; i++) {
    free(board->table[i]);
  }

  free(board->table);

  free(board);
}

void board_print(ifim_gol_board_t *board) {
  for (int i = 0; i < board->height; i++) {
    for (int j = 0; j < board->width; j++) {
      printf("%c", board->table[i][j] ? board->alive_symbol : board->dead_symbol);
    }
    puts("");
  }
}

int board_place_cell(ifim_gol_board_t *board, int x, int y) {
  if (x < 1 || y < 1 || x >= board->width || y >= board->height) {
    return 0;
  }

  board->table[y][x] = 1;

  return 1;
}

int board_get_cell_num_neighbors(ifim_gol_board_t *board, int x, int y) {
  int num_neighbors = 0;

  /*
   * 100
   * 010
   * 000
   */
  if (y > 0 && x > 0) {
    num_neighbors += board->table[y-1][x-1];
  }

  /*
   * 010
   * 010
   * 000
   */
  if (y > 0) {
    num_neighbors += board->table[y-1][x];
  }

  /*
   * 001
   * 010
   * 000
   */
  if (y > 0 && x < (board->width - 1)) {
    num_neighbors += board->table[y-1][x+1];
  }

  /*
   * 000
   * 110
   * 000
   */
  if (x > 0) {
    num_neighbors += board->table[y][x-1];
  }

  /*
   * 000
   * 011
   * 000
   */
  if (x < (board->width - 1)) {
    num_neighbors += board->table[y][x+1];
  }

  /*
   * 000
   * 010
   * 100
   */
  if (y < (board->height - 1) && x > 0) {
    num_neighbors += board->table[y+1][x-1];
  }

  /*
   * 000
   * 010
   * 010
   */
  if (y < (board->height - 1)) {
    num_neighbors += board->table[y+1][x];
  }

  /*
   * 000
   * 010
   * 001
   */
  if (y < (board->height - 1) && x < (board->width - 1)) {
    num_neighbors += board->table[y+1][x+1];
  }

  return num_neighbors;
}

bool board_cell_lives(ifim_gol_board_t *board, int x, int y) {
  int num_neighbors = board_get_cell_num_neighbors(board, x, y);

  if (// If the cell is alive, then it stays alive if it has either 2 or 3 live neighbors
      (board->table[y][x] && (num_neighbors > 1 && num_neighbors < 4))
      ||
      // If the cell is dead, then it springs to life only in the case that it has 3 live neighbors
      (!board->table[y][x] && (num_neighbors == 3))
      ) {
    return true;
  }

  return false;
}

void board_run(ifim_gol_board_t *current_board, bool loop, int generations, void (*fn)(ifim_gol_board_t *)) {
  ifim_gol_board_t *next_board = board_new(current_board->width, current_board->height,
                                  current_board->alive_symbol, current_board->dead_symbol);
  ifim_gol_board_t *aux_board = NULL;

  while (loop || generations--) {
    fn(current_board);

    for (int j = 0; j < current_board->height; j++) {
      for (int k = 0; k < current_board->width; k++) {
        next_board->table[j][k] = board_cell_lives(current_board, k, j);
      }
    }

    aux_board = current_board;
    current_board = next_board;
    next_board = aux_board;
  }
}

void board_display(ifim_gol_board_t *board) {
  system("clear");
  board_print(board);
  nanosleep(&(struct timespec){.tv_sec = 0, .tv_nsec = 75000000}, NULL);
}


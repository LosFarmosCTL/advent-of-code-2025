#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

enum { GRID_SIZE = 140, MAX_NEIGHBORS = 4 };

static void read_grid(bool grid[GRID_SIZE][GRID_SIZE]);
static bool check_position(const bool grid[GRID_SIZE][GRID_SIZE], int row, int col);
static int remove_rolls(bool grid[GRID_SIZE][GRID_SIZE]);

int main(void) {
  static bool grid[GRID_SIZE][GRID_SIZE] = {false};
  read_grid(grid);

  int removable_rolls, removed_rolls = 0;
  do {
    removable_rolls = remove_rolls(grid);
    removed_rolls += removable_rolls;
  } while (removable_rolls > 0);

  printf("%d\n", removed_rolls);

  return 0;
}

static bool check_position(const bool grid[GRID_SIZE][GRID_SIZE], int row, int col) {
  if (row >= 0 && row < GRID_SIZE && col >= 0 && col < GRID_SIZE) {
    return grid[row][col];
  }

  return false;
}

static int remove_rolls(bool grid[GRID_SIZE][GRID_SIZE]) {
  int removed_rolls = 0;

  for (int row = 0; row < GRID_SIZE; ++row) {
    for (int col = 0; col < GRID_SIZE; ++col) {
      if (!grid[row][col]) {
        continue;
      }

      int adjacent_rolls = 0;
      adjacent_rolls += check_position(grid, row + 1, col);
      adjacent_rolls += check_position(grid, row + 1, col + 1);
      adjacent_rolls += check_position(grid, row + 1, col - 1);
      adjacent_rolls += check_position(grid, row - 1, col);
      adjacent_rolls += check_position(grid, row - 1, col + 1);
      adjacent_rolls += check_position(grid, row - 1, col - 1);
      adjacent_rolls += check_position(grid, row, col + 1);
      adjacent_rolls += check_position(grid, row, col - 1);

      if (adjacent_rolls < MAX_NEIGHBORS) {
        ++removed_rolls;
        grid[row][col] = false;
      }
    }
  }

  return removed_rolls;
}

static void read_grid(bool grid[GRID_SIZE][GRID_SIZE]) {
  int row = 0, col = 0;

  int c;
  while ((c = getchar()) != EOF) {
    if (row >= GRID_SIZE) {
      fprintf(stderr, "Too many rows, max %d - got %d\n", GRID_SIZE, row + 1);
      exit(EXIT_FAILURE);
    }

    if (c == '\n') {
      if (col < GRID_SIZE) {
        fprintf(stderr, "Row %d too short, min %d - got %d\n", row, GRID_SIZE, col);
        exit(EXIT_FAILURE);
      }

      col = 0;
      ++row;
    } else if (c == '@' || c == '.') {
      if (col >= GRID_SIZE) {
        fprintf(stderr, "Error: row %d too long, max %d - got %d\n", row, GRID_SIZE, col + 1);
        exit(EXIT_FAILURE);
      }

      grid[row][col++] = (c == '@');
    } else {
      fprintf(stderr, "Unexpected character '%c'\n", c);
      exit(EXIT_FAILURE);
    }
  }

  if (row < GRID_SIZE) {
    fprintf(stderr, "Error: too few rows, expected %d - got %d\n", GRID_SIZE, row);
    exit(EXIT_FAILURE);
  }
}

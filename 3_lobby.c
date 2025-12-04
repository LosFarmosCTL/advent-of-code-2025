#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

enum { BANK_SIZE = 100, NUM_OF_BATTERIES = 12, UNSELECTED = -1 };

long long process_bank(const int bank[], const int bank_size);

int main(void) {
  int c, bank_size = 0;
  long long sum = 0;
  int bank[BANK_SIZE] = {0};

  while ((c = getchar()) != EOF) {
    if (c == '\n') {
      sum += process_bank(bank, bank_size);
      bank_size = 0;
    } else {
      if (c < '0' || c > '9') {
        fprintf(stderr, "Error: failed parsing, expected [0-9] - got %c\n", c);
        return 1;
      }

      if (bank_size >= BANK_SIZE) {
        fprintf(stderr, "Error: bank cannot be bigger than %d\n", BANK_SIZE);
        return 1;
      }

      // convert character to corresponding numeric value
      bank[bank_size++] = c - '0';
    }
  }

  if (bank_size > 0) {
    sum += process_bank(bank, bank_size);
  }

  if (sum == 0) {
    fprintf(stderr, "Warning: no banks processed\n");
  }

  printf("%lld\n", sum);
}

long long process_bank(const int bank[], const int bank_size) {
  if (bank_size < NUM_OF_BATTERIES) {
    fprintf(stderr, "Error: insufficient batteries in bank\n");
    exit(1);
  }

  bool selected[BANK_SIZE] = {false};
  int batteries[NUM_OF_BATTERIES] = {0};

  for (int battery_idx = 0; battery_idx < NUM_OF_BATTERIES; ++battery_idx) {
    int scan_pos = bank_size - 1;
    while (selected[scan_pos])
      --scan_pos;

    int highest_idx = UNSELECTED;
    while (scan_pos >= 0 && !selected[scan_pos]) {
      if (highest_idx == UNSELECTED || bank[scan_pos] >= bank[highest_idx])
        highest_idx = scan_pos;

      --scan_pos;
    }

    batteries[battery_idx] = highest_idx;
    selected[highest_idx] = true;
  }

  for (int sort_idx = 1; sort_idx < NUM_OF_BATTERIES; ++sort_idx) {
    int swap_pos = sort_idx;
    while (swap_pos > 0 && batteries[swap_pos] < batteries[swap_pos - 1]) {
      int tmp = batteries[swap_pos - 1];
      batteries[swap_pos - 1] = batteries[swap_pos];
      batteries[swap_pos--] = tmp;
    }
  }

  long long joltage = 0;
  long long multiplier = 1;
  for (int i = NUM_OF_BATTERIES - 1; i >= 0; --i) {
    joltage += bank[batteries[i]] * multiplier;
    multiplier *= 10;
  }

  return joltage;
}

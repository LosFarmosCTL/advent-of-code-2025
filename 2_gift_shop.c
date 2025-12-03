#include <stdbool.h>
#include <stdio.h>
#include <string.h>

long sum_id_range(const long from, const long to);

enum { BUF_SIZE = 100 };

int main(void) {
  char buf[BUF_SIZE];
  int c, i = 0;
  long id_sum = 0;
  long from_id, to_id;

  while ((c = getchar()) != EOF || i > 0) {
    if (c == ',' || c == EOF) {
      buf[i] = '\0';

      if (sscanf(buf, "%ld-%ld", &from_id, &to_id) != 2) {
        fprintf(stderr, "Error: invalid range format '%s'\n", buf);
        return 1;
      }

      id_sum += sum_id_range(from_id, to_id);
      i = 0;

      if (c == EOF)
        break;
    } else {
      buf[i++] = c;
    }
  }

  printf("%ld\n", id_sum);
}

long sum_id_range(const long from, const long to) {
  long sum = 0;

  for (long i = from; i <= to; ++i) {
    char id[BUF_SIZE];
    int id_len = snprintf(id, BUF_SIZE, "%ld", i);

    // pattern must divide evenly into id length and repeat at least twice
    for (int pattern_len = 1; pattern_len <= id_len / 2; ++pattern_len) {
      if (id_len % pattern_len != 0)
        continue;

      // char pattern[pattern_len + 1];
      // memcpy(pattern, id, pattern_len);
      // pattern[pattern_len] = '\0';
      //
      // bool id_matches = true;
      // for (int j = 1; j < id_len / pattern_len; ++j) {
      //   if (strncmp(id + j * pattern_len, pattern, pattern_len) != 0) {

      bool id_matches = true;
      for (int j = 1; j < id_len / pattern_len; ++j) {
        if (strncmp(id, id + j * pattern_len, pattern_len) != 0) {
          id_matches = false;
          break;
        }
      }

      if (id_matches) {
        sum += i;
        break;
      }
    }
  }

  return sum;
}

#include <stdio.h>

enum { BUF_SIZE = 10 };

int get_line(char buf[], int len);

int main(void) {
  char buf[BUF_SIZE];

  int number_of_zeros = 0;
  int position = 50;

  int len;
  while ((len = get_line(buf, BUF_SIZE)) != 0) {
    char direction;
    int number;
    sscanf(buf, "%c%d", &direction, &number);

    number_of_zeros += number / 100;
    number %= 100;

    if (number == 0)
      continue;

    position += direction == 'R' ? number : -number;

    if (position < 0) {
      // check if we're originating from a zero or actually passing it
      if (position + number != 0) {
        ++number_of_zeros;
      }

      position += 100;
    } else if (position > 99) {
      ++number_of_zeros;

      position -= 100;
    } else if (position == 0) {
      ++number_of_zeros;
    }
  }

  printf("%d\n", number_of_zeros);
}

int get_line(char buf[], int len) {
  int c, i = 0;
  for (; (c = getchar()) != '\n' && c != EOF; ++i) {
    if (i < len) {
      buf[i] = c;
    }
  }

  if (c == '\n') {
    buf[i < len - 1 ? i : len - 2] = '\n';
    ++i;
  }

  buf[i < len ? i : len - 1] = '\0';

  return i;
}

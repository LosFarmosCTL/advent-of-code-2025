#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

enum {
  MAX_LINE = 100,
  MAX_RANGES = 1000,
};

struct Range {
  long long start;
  long long end;
};

static size_t get_line(char line[], size_t max_size);
static long long get_total_fresh_items(struct Range ranges[], size_t len);

static struct Range parse_range(char line[]);

int main(void) {
  struct Range ranges[MAX_RANGES] = {0};
  char line[MAX_LINE] = {0};

  size_t ranges_idx = 0;
  while (get_line(line, MAX_LINE) > 0) {
    if (ranges_idx >= MAX_RANGES) {
      fprintf(stderr, "Error: too many ranges (max %d)\n", MAX_RANGES);
      exit(EXIT_FAILURE);
    }

    ranges[ranges_idx++] = parse_range(line);
  }

  int available_fresh_items = 0;
  while (get_line(line, MAX_LINE) > 0) {
    long long item_id;
    if (sscanf(line, "%lld", &item_id) != 1) {
      fprintf(stderr, "Error: invalid item id: %s\n", line);
      exit(EXIT_FAILURE);
    }

    for (size_t j = 0; j < ranges_idx; ++j) {
      if (item_id >= ranges[j].start && item_id <= ranges[j].end) {
        ++available_fresh_items;
        break;
      }
    }
  }

  long long total_fresh_items = get_total_fresh_items(ranges, ranges_idx);

  printf("Available: %d\n", available_fresh_items);
  printf("Total: %lld\n", total_fresh_items);
}

static struct Range parse_range(char line[]) {
  struct Range range = {0};

  if (sscanf(line, "%lld-%lld", &range.start, &range.end) != 2) {
    fprintf(stderr, "Error: invalid range: %s\n", line);
    exit(EXIT_FAILURE);
  }

  return range;
}

static long long get_total_fresh_items(struct Range ranges[], size_t len) {
  if (len == 0) {
    return 0;
  }

  struct Range merged_ranges[MAX_RANGES] = {ranges[0]};
  size_t merged_idx = 1;
  for (size_t j = 1; j < len; ++j) {
    struct Range range = ranges[j];
    bool is_contained_in_existing = false;
    size_t superseded_ranges[MAX_RANGES];
    size_t superseded_ranges_idx = 0;

    for (size_t k = 0; k < merged_idx; ++k) {
      long long check_start = merged_ranges[k].start;
      long long check_end = merged_ranges[k].end;

      // ignore new range if it's already fully included in another one
      if (range.start >= check_start && range.end <= check_end) {
        is_contained_in_existing = true;
        break;
      }
      // remove ranges that are fully included in the new range
      else if (range.start <= check_start && range.end >= check_end) {
        superseded_ranges[superseded_ranges_idx++] = k;
      }
      // shift new range start/end to outside of the other range, if overlapping
      else if (range.start <= check_end && range.end >= check_end) {
        range.start = check_end + 1;
      } else if (range.end >= check_start && range.start <= check_start) {
        range.end = check_start - 1;
      }
    }

    if (!is_contained_in_existing) {
      merged_ranges[merged_idx++] = range;
    }

    for (size_t k = superseded_ranges_idx; k > 0; --k) {
      size_t idx = superseded_ranges[k - 1];
      merged_ranges[idx] = merged_ranges[--merged_idx];
    }
  }

  long long total_fresh_items = 0;
  for (size_t j = 0; j < merged_idx; ++j) {
    total_fresh_items += merged_ranges[j].end - merged_ranges[j].start + 1;
  }

  return total_fresh_items;
}

static size_t get_line(char line[], size_t max_size) {
  int c;
  size_t pos = 0;
  while ((c = getchar()) != EOF && c != '\n' && pos < max_size) {
    line[pos++] = c;
  }

  if (pos >= max_size) {
    fprintf(stderr, "Error: line too long, max %zu - got %zu\n", max_size, pos);
    exit(EXIT_FAILURE);
  }

  line[pos] = '\0';

  return pos;
}

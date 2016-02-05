#ifndef _OPTION_H
#define _OPTION_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

typedef enum {LAB_MODE_SORT, LAB_MODE_GEN} Mode;

typedef struct {
  Mode mode; // Program mode to operate, default is LAB_MODE_SORT
  int size; // Number of ints to read/gen
  bool flg_size;
  // Options for bounding read/gen
  int min;
  int max;

  // Seed option is set using srand
  bool is_time_seeded;
  unsigned long seed; // WARN: no default

  // Probably change type from char * to FILE *
  FILE *input; // Input file
  FILE *output; // Output file
  FILE *count_out; // Output for counts
} Option;

// Option functions
Option *Option_new(int, char **);
void Option_del(Option *);
void Option_print(Option *);

#endif

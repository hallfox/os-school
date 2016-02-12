#ifndef _OPTION_H
#define _OPTION_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#define OPT_CHILDREN_MAX 3
#define OPT_LEVELS_MAX 4

typedef struct {
  unsigned int levels;
  unsigned int children;
  unsigned int sleep_time;
  bool pause_mode;
} Option;

// Option functions
Option *Option_new(int, char **);
void Option_del(Option *);

#endif

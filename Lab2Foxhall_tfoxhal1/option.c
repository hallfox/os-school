#include "option.h"
#include "debug.h"

#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>


// Option impl

Option *Option_new(int argc, char **argv) {
  char flag;
  unsigned int t;
  bool s_flag = false;
  bool p_flag = false;

  // Initialize new Option struct using argv and getopts
  Option *option = malloc(sizeof(Option));

  // Setting defaults
  option->levels = 1;
  option->children = 1;
  option->sleep_time = 1;
  option->pause_mode = false;

  while ((flag = getopt(argc, argv, "uN:M:ps:")) != -1) {
    switch (flag) {
    case 'u':
    case '?':
      // Just kick out and let main print usage
      goto die;
    case 'N':
      // Max of 4
      t = (unsigned int) strtol(optarg, NULL, 10);
      if (t == 0) t = 1; // shhhhh
      if (t > OPT_LEVELS_MAX) {
        log_err("Maximum number of levels is %d", OPT_LEVELS_MAX);
        goto die;
      }
      option->levels = t;
      break;
    case 'M':
      // Max of 3
      t = (unsigned int) strtol(optarg, NULL, 10);
      if (t > OPT_CHILDREN_MAX) {
        log_err("Maximum number of children is %d", OPT_CHILDREN_MAX);
        goto die;
      }
      option->children = t;
      break;
    case 'p':
      if (s_flag) {
        log_err("-p flag cannot be specified with -s\n");
        goto die;
      }
      option->pause_mode = true;
      p_flag = true;
      break;
    case 's':
      if (p_flag) {
        log_err("-p flag cannot be specified with -s\n");
        goto die;
      }
      t = (unsigned int) strtol(optarg, NULL, 10);
      option->sleep_time = t;
      s_flag = true;
      break;
    default:
      // Just kick out and let main print usage
      goto die;
    }
  }

  return option;

 die:
  Option_del(option);
  return NULL;
}

void Option_del(Option *option) {
  // Close any open files
  free(option);
  option = NULL;
}

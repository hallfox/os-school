#include "option.h"

#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

bool is_in_range(long v, long lower, long upper) {
  if (v < lower || v > upper) {
    fprintf(stderr, "ERR: %ld is outside the bounds [%ld, %ld]\n",
            v, lower, upper);
    return false;
  }
  return true;
}

// Option impl

Option *Option_new(int argc, char **argv) {
  char flag;
  long t;

  // Initialize new Option struct using argv and getopts
  Option *option = malloc(sizeof(Option));

  // Setting defaults
  option->mode = LAB_MODE_SORT; // sort mode
  option->size = 100;
  option->flg_size = false;
  option->min = 1;
  option->max = 255;
  option->is_time_seeded = true;
  option->input = stdin;
  option->output = stdout;
  option->count_out = stdout;

  while ((flag = getopt(argc, argv, "ugn:m:M:s:i:o:c:")) != -1) {
    switch (flag) {
    case 'u':
    case '?':
      // Just kick out and let main print usage
      Option_del(option);
      return NULL;

    case 'g':
      option->mode = LAB_MODE_GEN;
      break;

    case 'n':
      // min = 0
      // max = 1_000_000
      // default = 100
      option->flg_size = true;
      t = strtol(optarg, NULL, 10);

      // bounds check
      if(!is_in_range(t, 0, 1000000)) {
        Option_del(option);
        return NULL;
      }
      option->size = (int)t;
      break;

    case 'm':
      // min = 1
      // max = 1_000_000
      t = strtol(optarg, NULL, 10);

      // bounds check
      if(!is_in_range(t, 1, 1000000)) {
        Option_del(option);
        return NULL;
      }
      option->min = (int)t;
      break;

    case 'M':
      // min = 1
      // max = 1_000_000
      t = strtol(optarg, NULL, 10);

      // bounds check
      if(!is_in_range(t, 1, 1000000)) {
        Option_del(option);
        return NULL;
      }
      option->max = (int)t;
      break;

    case 's':
      option->is_time_seeded = false;
      option->seed = strtol(optarg, NULL, 10);
      break;

    case 'i':
      option->input = fopen(optarg, "r");
      if (!option->input) {
        perror(optarg);
        Option_del(option);
        return NULL;
      }
      break;
    case 'o':
      option->output = fopen(optarg, "w");
      if (!option->output) {
        perror(optarg);
        Option_del(option);
        return NULL;
      }
      break;

    case 'c':
      option->count_out = fopen(optarg, "w");
      if (!option->count_out) {
        perror(optarg);
        Option_del(option);
        return NULL;
      }
      break;

    default:
      // Just kick out and let main print usage
      Option_del(option);
      return NULL;
    }
  }

  // Set the seed
  if (option->is_time_seeded) {
    srand(option->seed);
  }
  else {
    srand(time(0));
  }

  return option;
}

void Option_del(Option *option) {
  // Close any open files
  if (option->input && option->input != stdin) {
    fclose(option->input);
  }
  if (option->output && option->output != stdout) {
    fclose(option->output);
  }
  if (option->count_out && option->count_out != stdout) {
    fclose(option->count_out);
  }
  free(option);
  option = NULL;
}

void Option_print(Option *option) {
  // Displays option object, used for debugging
  printf("Mode: %d\n"
         "Input size: %d\n"
         "Minimum: %d\n"
         "Maximum: %d\n"
         "Seed provided?: %d\n"
         "Seed: %ld\n",
         /* "Input: %p\n" */
         /* "Output: %p\n" */
         /* "Counter Output: %p\n", */
         option->mode, option->size, option->min, option->max, !option->is_time_seeded,
         option->seed); // option->input, option->output, option->count_out);
}

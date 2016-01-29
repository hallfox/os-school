#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> // qsort, malloc, free...
#include <unistd.h> // getopt
#include <time.h> // time

typedef enum {LAB_MODE_SORT, LAB_MODE_GEN} Mode;

typedef struct {
  Mode mode; // Program mode to operate, default is LAB_MODE_SORT
  int size; // Number of ints to read/gen
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

// Error handling
bool is_in_range(long, long, long);

Option *Option_new(int argc, char **argv) {
  char flag;
  long t;

  // Initialize new Option struct using argv and getopts
  Option *option = malloc(sizeof(Option));

  // Setting defaults
  option->mode = LAB_MODE_SORT; // sort mode
  option->size = 100;
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
         "Seed: %ld\n"
         "Input: %p\n"
         "Output: %p\n"
         "Counter Output: %p\n",
         option->mode, option->size, option->min, option->max, !option->is_time_seeded,
         option->seed, option->input, option->output, option->count_out);
}

bool is_in_range(long v, long lower, long upper) {
  if (v < lower || v > upper) {
    fprintf(stderr, "ERR: %ld is outside the bounds [%ld, %ld]\n",
            v, lower, upper);
    return false;
  }
  return true;
}

/* lab1 [-u] [-g] [-n <num-integers>] [-m <min-int>] [-M <max-int>] [-s <seed>]
   [-i <input-file-name>] [-o <output-file-name>] [-c <count-file-name>] */
int main(int argc, char **argv) {
  Option *option = Option_new(argc, argv);

  if (!option) {
    fprintf(stderr, "Usage: lab1 [-u] [-g] [-n <num-integers>] [-m <min-int>] [-M <max-int>] [-s <seed>]"
            "[-i <input-file-name>] [-o <output-file-name>] [-c <count-file-name>]\n");
    exit(1);
  }
  Option_print(option);
  // TODO report time elapsed with decimal precision to the microsecond

  // Cleanup
  Option_del(option);
  return 0;
}

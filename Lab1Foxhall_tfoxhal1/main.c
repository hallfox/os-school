#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> // qsort, malloc, free...
#include <unistd.h> // getopt

typedef struct {
  int mode; // 0 for sort, 1 for gen
  int size; // Number of ints to read/gen

  // gen mode options
  int gen_min; // Minimum number
  int gen_max; // Maximum number

  // Seed option is set using srand

  // Probably change type from char * to FILE *
  char *input; // Input file
  char *output; // Output file
  char *count_out; // Output for counts
} Option;

Option *Option_new(int, char **);
void Option_destroy(Option *);

Option *Option_new(int argc, char **argv) {
  char flag;

  // Initialize new Option struct using argv and getopts
  Option *option = malloc(sizeof(Option));
  option->mode = 0; // sort mode by default

  while ((flag = getopt(argc, argv, "ugn:m:M:s:i:o:c:")) != -1) {
    switch (flag) {
    case 'u':
    case '?':
      // Just kick out and let main print usage
      Option_destroy(option);
      return NULL;
    case 'g':
      option->mode = 1;
      break;
    case 'n':
      option->size = 
    default:
      // Just kick out and let main print usage
      Option_destroy(option);
      return NULL;
    }
  }

  return option;
}

void Option_destroy(Option *option) {
  free(option);
  option = NULL;
}

/* lab1 [-u] [-g] [-n <num-integers>] [-m <min-int>] [-M <max-int>] [-s <seed>]
   [-i <input-file-name>] [-o <output-file-name>] [-c <count-file-name>] */
int main(int argc, char **argv) {
  Option *option = Option_new(argc, argv);

  if (!option) {
    fprintf(stderr, "Usage: lab1 [-u] [-g] [-n <num-integers>] [-m <min-int>] [-M <max-int>] [-s <seed>]"
            "[-i <input-file-name>] [-o <output-file-name>] [-c <count-file-name>]");
    abort();
  }

  // Cleanup
  Option_destroy(option);
  return 0;
}

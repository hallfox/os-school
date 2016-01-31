#include <stdio.h>
#include <stdlib.h> // qsort, malloc, free...
#include "option.h"
#include "mytimer.h"

int compare_ints(const void *a, const void *b) {
  const int v1 = *(const int*)a;
  const int v2 = *(const int*)b;

  if (v1 < v2) return -1;
  else if (v1 > v2) return 1;
  else return 0;
}

/**
 *  \brief finds the number of ints in \a ns that match the character value of \a ch
 *
 *  Could probably be faster if bsearch was used, but I think Don Knuth would call me an ant
 *  or something.
 *
 *  Oh btw it's assumed ns is sorted.
 *
 *  \return int
 *    the number of ints in \a ns that match ch
 */
int match_ascii(char ch, int *ns, int size) {
  int count = 0;
  for (int i = 0; i < size; i++) {
    if (ns[i] == ch) {
      count++;
    }
    else if (ns[i] > ch) {
      // Small optimizations make the world go 'round
      return count;
    }
  }
  return count;
}

int main(int argc, char **argv) {
  Option *option = Option_new(argc, argv);

  if (!option) {
    fprintf(stderr, "Usage: lab1 [-u] [-g] [-n <num-integers>] [-m <min-int>] [-M <max-int>] [-s <seed>]"
            "[-i <input-file-name>] [-o <output-file-name>] [-c <count-file-name>]\n");
    exit(1);
  }

  if (option->mode == LAB_MODE_SORT) {
    // Timer start
    int rc;
    rc = init_mytimer();
    if (rc) {
      fprintf(stderr, "Timer initialization failed.\n");
    }

    // Read ints from option->input
    int n, i;
    int nums[option->size];
    for (i = 0; i < option->size && fscanf(option->input, "%d", &n) != EOF; i++) {
      nums[i] = n;
    }

    // Write sorted input to option->output
    qsort(nums, i, sizeof(int), &compare_ints);
    for (int j = 0; j < i; j++) {
      fprintf(option->output, "%d ", nums[j]);
    }
    fprintf(option->output, "\n");

    // Write the counts of matching ascii and input with username to option->count_out
    char *username = getenv("USER");
    for (int k = 0; username[k] != '\0'; k++) {
      char letter = username[k];
      int count = match_ascii(letter, nums, i);
      fprintf(option->count_out, "%c %d %d\n", letter, letter, count);
    }

    // Timer end
    rc = stop_mytimer();
    if (rc) {
      fprintf(stderr, "Timer halt failed.\n");
    }

    double elapsed;
    get_mytimer(&elapsed);
    fprintf(stderr, "Time elapsed: %.6f seconds\n", elapsed);
  }
  else if (option->mode == LAB_MODE_GEN) {
    // Randomly generate option->size numbers in the range option->min, option->max
    // Write them to option->output
    // NOTE: srand has already been set by Option_new
    for (int i = 0; i < option->size; i++) {
      // Credit to Mary Spencer for variable name
      unsigned int random_ass_number = rand() % (option->max - option->min) + option->min;
      fprintf(option->output, "%d ", random_ass_number);
    }
    fprintf(option->output, "\n");
  }
  else {
    fprintf(stderr, "Invalid mode detected.\n");
    Option_del(option);
    exit(1);
  }

  // Cleanup
  Option_del(option);
  return 0;
}

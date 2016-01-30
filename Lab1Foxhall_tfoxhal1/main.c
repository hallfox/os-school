#include <stdio.h>
#include <stdlib.h> // qsort, malloc, free...
#include "option.h"
#include "mytimer.h"

/* lab1 [-u] [-g] [-n <num-integers>] [-m <min-int>] [-M <max-int>] [-s <seed>]
   [-i <input-file-name>] [-o <output-file-name>] [-c <count-file-name>] */
int main(int argc, char **argv) {
  Option *option = Option_new(argc, argv);

  if (!option) {
    fprintf(stderr, "Usage: lab1 [-u] [-g] [-n <num-integers>] [-m <min-int>] [-M <max-int>] [-s <seed>]"
            "[-i <input-file-name>] [-o <output-file-name>] [-c <count-file-name>]\n");
    exit(1);
  }
  /* Option_print(option); */

  // Timer start
  int rc;
  rc = init_mytimer();
  if (rc) {
    fprintf(stderr, "Timer initialization failed.\n");
  }



  // Timer end
  rc = stop_mytimer();
  if (rc) {
    fprintf(stderr, "Timer halt failed.\n");
  }

  double elapsed;
  get_mytimer(&elapsed);
  fprintf(stderr, "Time elapsed: %.6f seconds\n", elapsed);

  // Cleanup
  Option_del(option);
  return 0;
}

#include <stdio.h>
#include <stdlib.h> // qsort, malloc, free...
#include <assert.h>
#include "option.h"
#include "debug.h"

#define palive(lev, p_pid, c_pid) \
  fprintf(stdout, "ALIVE: Level %d process with pid=%d, child of ppid=%d.", \
          lev, p_pid, c_pid)

#define pdeath(lev, p_pid, c_pid)                                       \
  fprintf(stdout, "EXITING: Level %d process with pid=%d, child of ppid=%d.", \
          lev, p_pid, c_pid)

int main(int argc, char **argv) {
  Option *option = Option_new(argc, argv);

  if (!option) {
    fprintf(stderr, "[USAGE]: lab2 [-u] [-N <num-levels>] [-M <num-children>] [-p] [-s <sleep-time]\n");
    exit(1);
  }

  pid_t pid;

  palive(option->levels, getpid(), getppid());

  assert(option->children <= OPT_CHILDREN_MAX);
  for (int i = 0; i < option->children; i++) {
    pid = fork(); // Fork you

    if (pid < 0) { /* error */
      log_err("Fork failed.");
      exit(EXIT_FAILURE);
    }
    if (pid == 0) { /* child process */
      char next_level[5];
      snprintf(next_level, 3, "-N %d", option->levels - 1);
      execl("./lab2", "lab2", NULL);
    }
  }
  // Good parents wait for their children
  wait(NULL);

  // Cleanup
  pdeath(option->levels, getpid(), getppid());
  Option_del(option);
  return 0;
}

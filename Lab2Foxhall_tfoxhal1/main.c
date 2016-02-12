#include <stdio.h>
#include <stdlib.h> // qsort, malloc, free...
#include <assert.h>
#include "option.h"
#include "debug.h"


#define palive(lev, p_pid, c_pid) \
  fprintf(stdout, "ALIVE: Level %d process with pid=%d, child of ppid=%d.\n", \
          lev, p_pid, c_pid)

#define pdeath(lev, p_pid, c_pid)                                       \
  fprintf(stdout, "EXITING: Level %d process with pid=%d, child of ppid=%d.\n", \
          lev, p_pid, c_pid)

int main(int argc, char **argv) {
  Option *option = Option_new(argc, argv);

  if (!option) {
    fprintf(stderr, "[USAGE]: lab2 [-u] [-N <num-levels>] [-M <num-children>] [-p] [-s <sleep-time]\n");
    exit(1);
  }

  pid_t pid;

  palive(option->levels, getpid(), getppid());

  if (option->levels > 1) {
    assert(option->children <= OPT_CHILDREN_MAX);
    char next_level[2], num_children[2], wait_time[2];
    char *wait_policy;
    if (!option->pause_mode) {
      wait_policy = "-s";
      snprintf(wait_time, 2, "%d", option->sleep_time);
    }
    else {
      wait_policy = "-p";
      wait_time[0] = '\0';
    }
    snprintf(next_level, 2, "%d", option->levels - 1);
    snprintf(num_children, 2, "%d", option->children);

    for (unsigned int i = 0; i < option->children; i++) {
      pid = fork(); // Fork you

      if (pid < 0) {
        log_err("Fork failed.");
        exit(EXIT_FAILURE);
      }
      else if (pid == 0) {
        execl("./lab2", "lab2", "-N", next_level, "-M", option->children,
            wait_policy, wait_time, NULL);
      }
    }
    // Good parents wait for their children
    wait(NULL);
  }
  else {
    if (option->pause_mode) {
      pause();
    }
    else {
      sleep(option->sleep_time);
    }
  }

  // Cleanup
  pdeath(option->levels, getpid(), getppid());
  Option_del(option);
  return 0;
}

#include <stdio.h>
#include <stdlib.h> // qsort, malloc, free...
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

  // Magic
  pid = fork();

  if (pid < 0) { /* error */
    log_err("Fork failed.");
    return 1;
  }
  if (pid == 0) { /* child process */
    execlp("/bin/ls", "ls", NULL);
  }
  else { /* parent process */
    /* parent will wait for the child to complete */
    wait(NULL);
    printf("Child Complete.");
  }

  // Cleanup
  Option_del(option);
  return 0;
}

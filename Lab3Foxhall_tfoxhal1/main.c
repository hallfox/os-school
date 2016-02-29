#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>

#include "debug.h"

#define palive(lev) \
  fprintf(stdout, "ALIVE: Level %d process with pid=%d, child of ppid=%d.\n", \
          lev - 1, getpid(), getppid())

#define pdeath(lev) \
  fprintf(stdout, "EXITING: Level %d process with pid=%d, child of ppid=%d.\n", \
          lev - 1, getpid(), getppid())

void write_to_child(int *fd, unsigned proc_id) {
  close(fd[0]);

  if (write(fd[1], &proc_id, sizeof(unsigned)) <= 0) {
    perror("Parent pipe write");
    exit(1);
  }
}

void read_from_parent(int *fd, unsigned *proc_id) {
  close(fd[1]);

  if (read(fd[0], &proc_id, sizeof(unsigned)) <= 0) {
    logd("Proc failed %u", *proc_id);
    perror("Child read");
    exit(1);
  }
}

void infanticide_handler(int sig) {
  pdeath(1);
  exit(0);
}

int main(int argc, char **argv) {
  unsigned procs;
  if (argc < 2) {
    fprintf(stderr, "[USAGE]: lab3 <num-procs>\n");
    exit(1);
  } else {
    procs = atoi(argv[1]);
    if (procs < 1 || procs > 32) {
      fprintf(stderr, "[USAGE]: <num-procs> must be between 1 and 32\n");
      exit(1);
    }
  }

  int pipefd[2];
  unsigned proc_id = 0;
  pid_t pid;

  const char *named_pipe = "/tmp/lab3";
  int named_fd;
  int named_signal = 0;

  const unsigned shm_size = sizeof(pid_t) * procs;
  const char *shm_name = "PROC_IDS";
  int shm_fd;
  void *shm_ptr;

  const struct sigaction kill_handler = {
    .sa_handler = &infanticide_handler,
    .sa_mask = 0,
    .sa_flags = 0,
  };
  sigaction(SIGTERM, &kill_handler, NULL);

  // Start the chain
  palive(1);

  if (pipe(pipefd) == -1) {
    perror("Pipe creation");
    exit(1);
  }

  if ((pid = fork()) < 0) {
    perror("Fork");
    exit(1);
  } else if (pid == 0) {
    read_from_parent(pipefd, &proc_id);

    // Write to shm at proc_id
    shm_fd = shm_open(shm_name, O_WRONLY, 0666);
    shm_ptr = mmap(0, shm_size, PROT_WRITE, MAP_SHARED, shm_fd, 0);
    ((pid_t *)shm_ptr)[proc_id] = getpid();

    while (proc_id + 1 < procs) {
      if (pipe(pipefd) == -1) {
        perror("Pipe creation");
        exit(1);
      }

      pid = fork();
      if (pid < 0) {
        perror("Child fork");
        exit(1);
      } else if (pid == 0) {
        // Read
        read_from_parent(pipefd, &proc_id);
        // Write pid to shm
        shm_fd = shm_open(shm_name, O_WRONLY, 0666);
        shm_ptr = mmap(0, shm_size, PROT_WRITE, MAP_SHARED, shm_fd, 0);
        ((pid_t *)shm_ptr)[proc_id] = getpid();
      } else {
        // Write and pause
        write_to_child(pipefd, proc_id + 1);
        pause();
      }
    }

    int the_signal = -1;
    mkfifo(named_pipe, 0666);
    named_fd = open(named_pipe, O_WRONLY);
    write(named_fd, &the_signal, sizeof(int));
    close(named_fd);
    unlink(named_pipe);
    pause();
  } else {
    // Create shared memory
    shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, shm_size);
    shm_ptr = mmap(0, shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    // Write to shm
    ((pid_t *)shm_ptr)[proc_id] = getpid();

    // Tell child to write to shm and continue the chain
    write_to_child(pipefd, proc_id + 1);

    // Wait for leaf to finish
    named_fd = open(named_pipe, O_RDONLY);
    read(named_fd, &named_signal, 1);
    close(named_fd);

    // Kill everyone in order
    for (unsigned i = 0; i < procs; i++) {
      if (kill(((pid_t *)shm_ptr)[i], SIGTERM) == -1) {
        perror("Killing children");
        exit(1);
      }
    }
    shm_unlink(shm_name);

    pdeath(1);
  }


  // Cleanup
  return 0;
}

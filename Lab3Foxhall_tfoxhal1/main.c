#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

#include "debug.h"

#define palive(lev) \
  fprintf(stdout, "ALIVE: Level %d process with pid=%d, child of ppid=%d.\n", \
          lev, getpid(), getppid())

#define pdeath(lev) \
  fprintf(stdout, "EXITING: Level %d process with pid=%d, child of ppid=%d.\n", \
          lev, getpid(), getppid())

static unsigned proc_id = 0;

void write_to_child(int *fd, unsigned proc_id) {
  logd("Proc %u ready to write to child", getpid());
  close(fd[0]);

  if (write(fd[1], &proc_id, sizeof(unsigned)) < 0) {
    perror("Parent pipe write");
    exit(1);
  }
  logd("Proc %u success wrote %u to child", getpid(), proc_id);
}

void read_from_parent(int *fd, unsigned *proc_id) {
  logd("Proc %u ready to read from parent", getpid());
  close(fd[1]);

  if (read(fd[0], proc_id, sizeof(unsigned)) < 0) {
    logd("Proc failed %u", getpid());
    perror("Child read");
    exit(1);
  }
  logd("Proc %u success read %u from parent", getpid(), *proc_id);
}

void infanticide_handler(int sig) {
  pdeath(proc_id);
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
  proc_id = 0;
  pid_t pid;

  const char *named_pipe = "/tmp/philipdextreme.com"; // never forget
  int named_fd;
  int named_signal = -1;

  const unsigned shm_size = sizeof(pid_t) * procs;
  const char *shm_name = "PROC_IDS";
  int shm_fd;
  pid_t *shm_ptr;

  struct sigaction kill_handler;
  kill_handler.sa_handler = infanticide_handler;
  kill_handler.sa_flags = 0;
  sigaction(SIGTERM, &kill_handler, NULL);

  // Create shared memory
  if ((shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666)) == -1) {
    perror("Shared memory open");
    exit(1);
  }
  ftruncate(shm_fd, shm_size);
  if ((shm_ptr = (pid_t *)mmap(NULL, shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0)) == MAP_FAILED) {
    perror("Memory mapping");
    exit(1);
  }

  // Create named pipe
  mkfifo(named_pipe, 0666);

  // Start the chain
  palive(proc_id);

  if (pipe(pipefd) == -1) {
    perror("Pipe creation");
    exit(1);
  }

  if ((pid = fork()) < 0) {
    perror("Fork");
    exit(1);
  } else if (pid == 0) {
    read_from_parent(pipefd, &proc_id);
    palive(proc_id);

    // Write to shm at proc_id
    /* shm_fd = shm_open(shm_name, O_WRONLY, 0666); */
    /* logd("Proc %u opened shared memory", getpid()); */
    /* shm_ptr = (pid_t *)mmap(0, shm_size, PROT_WRITE, MAP_SHARED, shm_fd, 0); */
    /* logd("Proc %u mapped shared memory", getpid()); */
    shm_ptr[proc_id] = getpid();
    logd("Proc %u wrote its pid to shared memory", getpid());

    while (proc_id + 1 < procs) {
      if (pipe(pipefd) == -1) {
        perror("Pipe creation");
        exit(1);
      }

      logd("Proc %u about to fork...", getpid());
      pid = fork();
      if (pid < 0) {
        perror("Child fork");
        exit(1);
      } else if (pid == 0) {
        // Read
        read_from_parent(pipefd, &proc_id);
        palive(proc_id);
        // Write pid to shm
        /* shm_fd = shm_open(shm_name, O_WRONLY, 0666); */
        /* shm_ptr = (pid_t *)mmap(0, shm_size, PROT_WRITE, MAP_SHARED, shm_fd, 0); */
        shm_ptr[proc_id] = getpid();
        logd("Proc %d wrote its pid to shared memory", getpid());
      } else {
        // Write and pause
        write_to_child(pipefd, proc_id + 1);
        logd("Proc %d pausing", getpid());
        pause();
      }
    }

    logd("Leaf has been reached: %u", pid);
    int the_signal = 0;
    named_fd = open(named_pipe, O_WRONLY);
    write(named_fd, &the_signal, sizeof(int));
    close(named_fd);
    logd("Leaf %d pausing", getpid());
    pause();
  } else {
    // Write to shm
    shm_ptr[proc_id] = getpid();
    logd("Proc %d wrote pid to shared memory", getpid());

    // Tell child to write to shm and continue the chain
    write_to_child(pipefd, proc_id + 1);

    // Wait for leaf to finish
    named_fd = open(named_pipe, O_RDONLY);
    read(named_fd, &named_signal, sizeof(int));
    close(named_fd);
    unlink(named_pipe);

    // Print everyone in order
    for (unsigned i = 0; i < procs; i++) {
      printf("%u\n", shm_ptr[i]);
    }

    // Kill everyone in order
    for (unsigned i = 1; i < procs; i++) {
      if (kill(shm_ptr[i], SIGTERM) == -1) {
        perror("Killing children");
        exit(1);
      }
      // Wait on process to die
      logd("Send kill to proc %d", shm_ptr[i]);
      waitpid(shm_ptr[i], NULL, 0);
      logd("Waited for proc %d to die", shm_ptr[i]);
    }
    if (shm_unlink(shm_name) == -1) {
      perror("Unlink shared memory");
      exit(1);
    }
  }

  pdeath(proc_id);
  // Cleanup
  return 0;
}

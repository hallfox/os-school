#include "mytimer.h"

#include <stdbool.h>
#include <sys/time.h>
#include <time.h>
#include <stdio.h>


struct timeval start, end;
bool is_timer_running = false;
bool is_timer_init = false;

int init_mytimer() {
  is_timer_init = true;
  if (is_timer_running) {
    // Say something about misusing timer
    return -1;
  }

  is_timer_running = true;
  int err = gettimeofday(&start, NULL);
  if (err) {
    perror("gettimeofday()");
  }

  return err;
}

/**
 *  \brief sets the elapsed time to elapsed_time in microseconds
 *  \param elapsed_time
 *  \return int
 */
int get_mytimer(double *elapsed_time) {
  if (!is_timer_init) {
    return -1;
  }

  struct timeval time;
  int err = 0;
  if (is_timer_running) {
    err = gettimeofday(&time, NULL);
  }
  else {
    time = end;
  }

  // Something's wrong
  if (err) {
    perror("gettimeofday()");
    return err;
  }

  *elapsed_time = difftime(time.tv_sec, start.tv_sec);
  *elapsed_time += (time.tv_usec - start.tv_usec) / 1000000.0;
  return 0;
}

int stop_mytimer() {
  if (!is_timer_running) {
    // Say something about misusing timer
    return 0;
  }

  int err = gettimeofday(&end, NULL);
  is_timer_running = false;
  if (err) {
    perror("gettimeofday()");
  }

  return err;
}

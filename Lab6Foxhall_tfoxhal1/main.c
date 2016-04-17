#include "debug.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define sandwich(a, b, c) ((a) <= (b) && (b) <= (c))

static const char *usage = "./lab6 [number of stocks] [percent change]";
// Shared mutable state with replicated data is one of my triggers
static double stock_value = 0.0;
static double *stock_arr;
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
static int done;

void *stock(void *args);
void *up_watcher(void *args);
void *down_watcher(void *args);

void *stock(void *args) {
  // *args => position in stock_arr
  // Srsly don't touch anything but your slot
  int undyne = *((int *)args);

  while (1) {
    double delta = 2*((double)rand() / RAND_MAX) - 1;
    pthread_mutex_lock(&lock);
    if (done) {
      pthread_mutex_unlock(&lock);
      return 0;
    }
    stock_arr[undyne] += delta;
    stock_value += delta;
    logd("Stock %d changed by %0.2f to %0.2f", undyne, delta, stock_arr[undyne]);
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&lock);
  }

  return 0;
}

void *up_watcher(void *args) {
  // Arg: one double to kill when stock_value <= args[0]
  const double threshold = *((double *)args);

  pthread_mutex_lock(&lock);
  while (!done && stock_value < threshold) {
    pthread_cond_wait(&cond, &lock);
  }
  if (!done) {
    printf("Market Up to %0.2f\n", stock_value);
    // Kill ALL the threads
    done = 1;
    pthread_cond_signal(&cond);
  }
  pthread_mutex_unlock(&lock);

  return 0;
}

void *down_watcher(void *args) {
  // Arg: one double to kill when stock_value <= args[0]
  const double threshold = *((double *)args);

  pthread_mutex_lock(&lock);
  while (!done && stock_value > threshold) {
    pthread_cond_wait(&cond, &lock);
  }
  if (!done) {
    printf("Market Down to %0.2f\n", stock_value);
    // Kill ALL the threads
    done = 1;
    pthread_cond_signal(&cond);
  }
  pthread_mutex_unlock(&lock);

  return 0;
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    loge("Invalid argument numbers: need %s", usage);
    exit(1);
  }
  int stocks = (int)strtol(argv[1], NULL, 10);
  double bound = strtod(argv[2], NULL);

  if (!sandwich(1, stocks, 1000)) {
    loge("Invalid number of stocks: must be between 1 and 1000");
    exit(1);
  } else if (!sandwich(1, bound, 20)) {
    loge("Invalid watcher bounds: must be between 1 and 20");
    exit(1);
  }

  // Initialzing and whatnot
  stock_value = 100 * stocks;
  stock_arr = malloc(sizeof(double) * stocks);
  for (int i = 0; i < stocks; i++) {
    stock_arr[i] = 100.0;
  }
  double upper, lower;
  upper = stock_value * (1+bound/100);
  lower = stock_value * (1-bound/100);
  done = 0;

  pthread_t up, down;

  // Hold the lock so the simulation can't begin until every thread has spawned
  pthread_mutex_lock(&lock);
  if (pthread_create(&up, NULL, up_watcher, &upper) != 0) {
    perror("Up watcher thread");
    free(stock_arr);
    exit(1);
  }
  if (pthread_create(&down, NULL, down_watcher, &lower) != 0) {
    perror("Down watcher thread");
    free(stock_arr);
    exit(1);
  }

  pthread_t stock_threads[stocks];
  int stock_ids[stocks];
  for (int i = 0; i < stocks; i++) {
    stock_ids[i] = i;
    if (pthread_create(&stock_threads[i], NULL, stock, &stock_ids[i]) != 0) {
      perror("Stock thread");
      free(stock_arr);
      exit(1);
    }
  }
  // OPEN THE GATES
  pthread_mutex_unlock(&lock);

  // Wait for the termination, I guess
  pthread_join(up, NULL);
  logd("Up watcher exited");
  pthread_join(down, NULL);
  logd("Down watcher exited");
  for (int i = 0; i < stocks; i++) {
    pthread_join(stock_threads[i], NULL);
    logd("Stock %d exited", i);
  }

  double total = 0;
  for (int i = 0; i < stocks; i++) {
    total += stock_arr[i];
  }

  printf("Total Market Price of %d Stocks: %0.2f\n", stocks, total);

  free(stock_arr);
  return 0;
}

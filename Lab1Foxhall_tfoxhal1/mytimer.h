#ifndef _MYTIMER_H_
#define _MYTIMER_H_

#include <sys/time.h>
#include <stdio.h>

int init_mytimer();
int get_mytimer(double *);
int stop_mytimer();

#endif /* _MYTIMER_H_ */

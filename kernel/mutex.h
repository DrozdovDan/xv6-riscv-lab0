#ifndef MUTEX_H
#define MUTEX_H

#include "spinlock.h"
#include "sleeplock.h"

struct mutex {
  int count;
  struct sleeplock lock;
};

#endif

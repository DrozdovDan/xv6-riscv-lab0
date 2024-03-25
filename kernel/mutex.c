#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "proc.h"
#include "mutex.h"
#include "defs.h"

struct {
  struct spinlock lock;
  struct mutex mutex[NMUTEX];
} mutex_table;

void
mutex_table_init(void) {
  initlock(&mutex_table.lock, "mutex_table");
  for (int i = 0; i < NMUTEX; i++) {
    initsleeplock(&mutex_table.mutex[i].lock, "mutex_i");
  }
}

int
create_mutex(void) {
  int i = 0;
  struct proc* p = myproc();
  acquire(&p->lock);
  acquire(&mutex_table.lock);
  int flag = 0;
  for (i = 0; i < NMUTEX; i++) {
    if (mutex_table.mutex[i].count < 1) {
      mutex_table.mutex[i].count++;
      p->mutex_table[i] = mutex_table.mutex + i;
      flag = 1;
      break;
    }
  }
  if (!flag) {
    release(&mutex_table.lock);
    release(&p->lock);
    return -1;
  }
  release(&mutex_table.lock);
  release(&p->lock);
  return i;
} 

int
mutex_lock(int i) {
  if (i < 0 || i >= NMUTEX || mutex_table.mutex[i].count < 1) {
    return -1;
  }

  struct mutex* m = mutex_table.mutex + i;

  if (m->lock.locked){
    return -2;
  }

  acquiresleep(&m->lock);
  return 0;
}

int
mutex_unlock(int i) {
  if (i < 0 || i >= NMUTEX || mutex_table.mutex[i].count < 1) {
    return -1;
  }

  struct mutex* m = mutex_table.mutex + i;

  if (!m->lock.locked){
    return -2;
  }

  releasesleep(&m->lock);
  return 0;
}

int
mutex_destroy(int i) {
  acquire(&mutex_table.lock);
  if (i < 0 || i >= NMUTEX || mutex_table.mutex[i].count < 1) {
    release(&mutex_table.lock);
    return -1;
  }

  struct mutex* m = mutex_table.mutex + i;

  if (m->lock.locked) {
    releasesleep(&m->lock);
  }

  m->count = 0;
  release(&mutex_table.lock);
  return 0;
}


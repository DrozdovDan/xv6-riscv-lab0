#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "mutex.h"

uint64
sys_create_mutex(void) {
	return create_mutex();
}

uint64
sys_mutex_lock(void) {
	int n;
	argint(0, &n);
	return mutex_lock(n);
}

uint64
sys_mutex_unlock(void) {
	int n;
	argint(0, &n);
	return mutex_unlock(n);
}

uint64
sys_mutex_destroy(void) {
	int n;
	argint(0, &n);
	return mutex_destroy(n);
}
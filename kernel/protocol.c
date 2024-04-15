#include <stdarg.h>

#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "riscv.h"
#include "proc.h"
#include "defs.h"
#include "fs.h"

typedef
{
	struct spinlock lock;
	int syscall_ticks;
	int interrupt_ticks;
	int switch_ticks;
	int exec_ticks;
} protocol;

void
protocol_init(void) {
	initlock(&protocol.lock, "system protocol");
	protocol.syscall_ticks = 0;
	protocol.interrupt_ticks = 0;
	protocol.switch_ticks = 0;
	protocol.exec_ticks = 0;
}

int
enable_protocol(int num, int ticks_) {
	acquire(&protocol.lock);
	acquire(&tickslock);
	if (num == 0) {
		protocol.syscall_ticks = ticks + ticks_;
	}
	else if (num == 1) {
		protocol.interrupt_ticks = ticks + ticks_;
	}
	else if (num == 2) {
		protocol.switch_ticks = ticks + ticks_;
	}
	else if (num == 3) {
		protocol.exec_ticks = ticks + ticks_;
	} else {
		release(&tickslock);
		release(&protocol.lock);
		return -1;
	}
	release(&tickslock);
	release(&protocol.lock);
}

int
disable_protocol(int num) {
	acquire(&protocol.lock);
	if (num == 0) {
		protocol.syscall_ticks = 0;
	}
	else if (num == 1) {
		protocol.interrupt_ticks = 0;
	}
	else if (num == 2) {
		protocol.switch_ticks = 0;
	}
	else if (num == 3) {
		protocol.exec_ticks = 0;
	} else {
		release(&protocol.lock);
		return -1;
	}
	release(&protocol.lock);
}

uint64
sys_enable_protocol(void) {
	uint64 num, ticks_;
	argint(0, num);
	argint(1, ticks_);
	return enable_protocol(num, ticks_);
}

uint64
sys_disable_protocol(void) {
	uint64 num;
	argint(0, num);
	return disable_protocol(num);
}

int
syscall_ticks(void) {
	return protocol.syscall_ticks;
}

int
interrupt_ticks(void) {
	return protocol.interrupt_ticks;
}

int
switch_ticks(void) {
	return protocol.switch_ticks;
}

int
exec_ticks(void) {
	return protocol.exec_ticks;
}

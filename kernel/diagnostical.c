#include <stdarg.h>

#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "riscv.h"
#include "proc.h"
#include "defs.h"
#include "fs.h"

struct
{
	struct spinlock lock;
	char* begin;
	char* end;
	char buf[4096*MAXDBUF];
} diagnostical_buf;

static char digits[] = "0123456789abcdef";

void
diagnostical_buf_init(void) {
	initlock(&diagnostical_buf.lock, "diagnostical buf");
	diagnostical_buf.begin = diagnostical_buf.buf + 4096*MAXDBUF;
	diagnostical_buf.end = diagnostical_buf.buf;
}

void
write_in_buf(char symbol) {
	if (diagnostical_buf.end != diagnostical_buf.begin) {
		*(diagnostical_buf.end) = symbol;
		diagnostical_buf.end++;
	}
	else {
		*(diagnostical_buf.end) = symbol;
		diagnostical_buf.end++;
		diagnostical_buf.begin++;
	}

	if (diagnostical_buf.begin == diagnostical_buf.buf + 4096*MAXDBUF) 
		diagnostical_buf.begin = diagnostical_buf.buf;

	if (diagnostical_buf.end == diagnostical_buf.buf + 4096*MAXDBUF) 
		diagnostical_buf.end = diagnostical_buf.buf;
}

void
write_int(int xx, int base, int sign) {
	char buf[16];
	int i;
	uint x;

  	if(sign && (sign = xx < 0))
    	x = -xx;
  	else
    	x = xx;

  	i = 0;
  	do {
    	buf[i++] = digits[x % base];
  	} while((x /= base) != 0);

  	if(sign)
    	buf[i++] = '-';

	while(--i >= 0)
    write_in_buf(buf[i]);
}

void
write_ptr(uint64 x) {
	int i;
  	write_in_buf('0');
  	write_in_buf('x');
  	for (i = 0; i < (sizeof(uint64) * 2); i++, x <<= 4)
    write_in_buf(digits[x >> (sizeof(uint64) * 8 - 4)]);
}

void 
pr_msg(char* fmt, ...) {
	va_list ap;
	int i, c;
	char *s;

	if (fmt == 0)
	panic("null fmt");

	acquire(&diagnostical_buf.lock);

	acquire(&tickslock);

	write_in_buf('[');
	write_int(ticks, 10, 1);
	write_in_buf(']');
	write_in_buf(' ');

	release(&tickslock);

	va_start(ap, fmt);
	for(i = 0; (c = fmt[i] & 0xff) != 0; i++){
	if(c != '%'){
	  write_in_buf(c);
	  continue;
	}
	c = fmt[++i] & 0xff;
	if(c == 0)
	  break;
	switch(c){
	case 'd':
	  write_int(va_arg(ap, int), 10, 1);
	  break;
	case 'x':
	  write_int(va_arg(ap, int), 16, 1);
	  break;
	case 'p':
	  write_ptr(va_arg(ap, uint64));
	  break;
	case 's':
	  if((s = va_arg(ap, char*)) == 0)
	    s = "(null)";
	  for(; *s; s++)
	    write_in_buf(*s);
	  break;
	case '%':
	  write_in_buf('%');
	  break;
	default:
	  // Print unknown % sequence to draw attention.
	  write_in_buf('%');
	  write_in_buf(c);
	  break;
	}
	}
	va_end(ap);

	write_in_buf('\n');

	release(&diagnostical_buf.lock);
}

int
dmesg(char* user_buf) {

	if (user_buf == 0)
		return -1;

	acquire(&diagnostical_buf.lock);

	char* sym = diagnostical_buf.begin;
	int i = 0;

	do {
		if (sym == diagnostical_buf.buf + 4096*MAXDBUF)
			break;
		if (copyout(myproc()->pagetable, (uint64)(user_buf + i), sym, 1) < 0) {
			release(&diagnostical_buf.lock);
			return -1;
		}
		i++;
		sym++;
		if (sym == diagnostical_buf.buf + 4096*MAXDBUF) 
			sym = diagnostical_buf.buf;
	} while (sym != diagnostical_buf.end);

	char* end = "\0";

	if (copyout(myproc()->pagetable, (uint64)(user_buf + i), end, 1) < 0) {
		release(&diagnostical_buf.lock);
		return -1;
	}

	release(&diagnostical_buf.lock);

	return 0;
}

uint64
sys_dmesg(void) {
	uint64 user_buf;
	argaddr(0, &user_buf);
	return dmesg((char*)user_buf);
}
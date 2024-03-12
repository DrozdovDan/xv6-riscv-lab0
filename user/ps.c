#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"
#include "kernel/spinlock.h"
#include "kernel/riscv.h"
#include "kernel/proc.h"

int 
main(int argc, char *argv[]) {
	struct procinfo info[NPROC];
	int n = ps_listinfo(info, NPROC);
	if (n == -1) {
		printf("Something went wrong with process...\n");
		exit(100);
	}
	char* state;
	static char *states[] = {
	  [UNUSED]    "unused",
	  [USED]      "used",
	  [SLEEPING]  "sleep ",
	  [RUNNABLE]  "runble",
	  [RUNNING]   "run   ",
	  [ZOMBIE]    "zombie"
	};
	printf("%d\n", n);
	for (int i = 0; i < n; ++i)
	{
		if(info[i].state >= 0 && states[info[i].state])
	      state = states[info[i].state];
	    else
	      state = "???";
		printf("%s %s %d\n", info[i].name, state, info[i].parentid);
	}
	exit(0);
}
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"
#include "kernel/spinlock.h"
#include "kernel/riscv.h"
#include "kernel/proc.h"

int 
main(int argc, char *argv[]) {
	if (argc < 2) {
		printf("I don't know what to do...\n");
		exit(999);
	}
	if (strcmp(argv[1], "0") == 0 || strcmp(argv[1], "0\n") == 0) {
		struct procinfo *info = (struct procinfo *)malloc(12 * sizeof(struct procinfo));
		int n = ps_listinfo(info, 999);
		if (n < 0) {
			printf("Something went wrong with process in test 0...(return %d)\n", n);
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
	} else if (strcmp(argv[1], "1") == 0 || strcmp(argv[1], "1\n") == 0) {
		struct procinfo *info = 0;
		int n = ps_listinfo(info, 999);
		if (n == -1) {
			printf("Something went wrong with process in test 1...\n");
			exit(101);
		}
		printf("%d\n", n);
		exit(0);
	} else if (strcmp(argv[1], "2") == 0 || strcmp(argv[1], "2\n") == 0) {
		struct procinfo *info = (struct procinfo *)malloc(12 * sizeof(struct procinfo));
		int n = ps_listinfo(info, 1);
		if (n == -1) {
			printf("Not enough lim as planned...\n");
			exit(0);
		} else {
			printf("Something went wrong with process in test 2...(n=%d)\n", n);
			exit(102);
		}
	} else if (strcmp(argv[1], "3") == 0 || strcmp(argv[1], "3\n") == 0) {
		struct procinfo *info = (struct procinfo *)malloc(1 * sizeof(struct procinfo));
		int n = ps_listinfo(info, 10);
		if (n == -2) {
			printf("Not enough bufsize as planned...\n");
			exit(0);
		} else {
			printf("Something went wrong with process in test 3...(n=%d)\n", n);
			exit(102);
		}
	} else if (strcmp(argv[1], "4") == 0 || strcmp(argv[1], "4\n") == 0) {
		struct procinfo *info = (struct procinfo *)malloc(10 * sizeof(struct procinfo));
		int n = ps_listinfo(info + 100, 10);
		if (n == -2) {
			printf("Wrong address as planned...\n");
			exit(0);
		} else {
			printf("Something went wrong with process in test 3...(n=%d)\n", n);
			exit(102);
		}
	}
	printf("Wrong flag...\n");
	exit(321);
}
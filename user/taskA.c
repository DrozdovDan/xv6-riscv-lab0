#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
	int pid = fork();
	if (pid == -1) {
		printf("Can't create a new process...\n");
		exit(1);
	}
	if (pid > 0) {
		printf("%d %d\n", getpid(), pid);
	}
	if (pid == 0) {
		sleep(50);
		exit(1);
	}
	int status;
	wait(&status);
	printf("%d %d\n", pid, status);
	exit(0);
}
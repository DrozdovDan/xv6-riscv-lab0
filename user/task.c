#include "kernel/types.h"
#include "user/user.h"

#define BUF_SIZE 100

int main(int argc, char *argv[]) {
	int p[2];
	int status = pipe(p);
	if (status < 0) {
		printf("Can't create pipe...\n");
		exit(1);
	}
	int pid = fork();
	if (pid < 0) {
		printf("Can't create process...\n");
		exit(1);
	}
	if (pid > 0) {
		close(p[0]);
		for (int i = 1; i < argc; i++) {
			write(p[1], argv[i], strlen(argv[i]));
			write(p[1], "\n", 1);
		}
		close(p[1]);
		int status;
		wait(&status);
		exit(0);

	}
	if (pid == 0) {
		close(0);
		dup(p[0]);
		close(p[0]);
		close(p[1]);
		char *wc_argv[] = {"/wc", 0};
		status = exec("/wc", wc_argv);
		if (status < 0) {
			printf("Can't exec...\n");
			exit(1);
		}
		exit(0);
	}
	exit(0);
}
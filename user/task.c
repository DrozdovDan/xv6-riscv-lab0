#include "kernel/types.h"
#include "user/user.h"

#define BUF_SIZE 100

int main(int argc, char *argv[]) {
	int p[2];
	pipe(p);
	int pid = fork();
	if (pid == -1) {
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
		close(p[1]);
		char buf[BUF_SIZE];
		for (int i = 0; i < (argc - 1) * 2; i++) {
			int n = read(p[0], buf, BUF_SIZE);
			write(1, buf, n);
		}
		close(p[0]);
		exit(0);
	}
	exit(0);
}
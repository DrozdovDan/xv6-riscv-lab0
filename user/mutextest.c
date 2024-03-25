#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  if (argc < 2) {
    printf("Not enough params...\n");
    exit(1);
  }

  int p1[2];
  int p2[2];

  int n = 0;
  n = pipe(p1);

  if (n < 0) {
    printf("Failed to create a pipe...\n");
    exit(2);
  }

  n = pipe(p2);

  if (n < 0) {
    printf("Failed to create a pipe...\n");
    exit(2);
  }

  int mid = 0;
  mid = create_mutex();
  if (mid < 0) {
    printf("Failed to create a mutex...\n");
    exit(2);
  }

  int pid = 0;
  pid = fork();

  if (pid < 0) {
    printf("Failed to create a child process...\n");
    close(p1[0]);
    close(p1[1]);
    close(p2[0]);
    close(p2[1]);
    exit(2);
  }

  if (pid == 0) {
    close(p1[1]);
    close(p2[0]);
    char buf[1];

    while(1 == 1) {
      n = read(p1[0], buf, 1);

      if (n < 0) {
        printf("Can't read from the pipe...\n");
        close(p1[0]);
        close(p2[1]);
        exit(2);
      }

      if (n == 0) {
        break;
      }

      do {
        n = mutex_lock(mid);
      } while (n < 0);

      printf("%d: received %s\n", getpid(), buf);

      n = mutex_unlock(mid);
      if (n < 0) {
        printf("Can't unlock the mutex...\n");
        close(p1[0]);
        close(p2[1]);
        exit(2);
      }

      n = write(p2[1], buf, 1);

      if (n < 0) {
        printf("Can't write to the pipe...\n");
        close(p1[0]);
        close(p2[1]);
        exit(2);
      }

    }

    close(p1[0]);
    close(p2[1]);
    exit(0);
  } 

  if (pid > 0) {
    close(p1[0]);
    close(p2[1]);
    n = write(p1[1], argv[1], strlen(argv[1]));

    if (n < 0) {
      printf("Can't write to the pipe...\n");
      close(p2[0]);
      close(p1[1]);
      exit(2);
    }

    close(p1[1]);

    char buf[1];

    while(1 == 1) {

      n = read(p2[0], buf, 1);

      if (n < 0) {
        printf("Can't read from the pipe...\n");
        close(p2[0]);
        exit(2);
      }

      if (n == 0) {
        break;
      }

      do {
        n = mutex_lock(mid);
      } while (n < 0);

      printf("%d: received %s\n", getpid(), buf);

      n = mutex_unlock(mid);
      if (n < 0) {
        printf("Can't unlock the mutex...\n");
        close(p2[0]);
        exit(2);
      }
    }

    close(p2[0]);
    wait(&n);
    n = mutex_destroy(mid);
    if (n < 0) {
        printf("Can't destroy the mutex...\n");
        exit(2);
    }
    exit(0);
  }

  exit(0);
}
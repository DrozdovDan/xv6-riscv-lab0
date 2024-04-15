#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int n_buf = 1;
  int n = 0;
  char *c = 0;
  do {
    if (c) 
      free(c);
    c = (char*) malloc(n_buf * sizeof(char));
    n = dmesg(c);
    n_buf *= 2;
  } while (n == -1);
  printf("%s\n", c);
  free(c);
  exit(0);
}

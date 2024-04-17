#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  if (argc == 3) {
    int num = -1;
    num = atoi(argv[1]);
    int ticks_num = -1;
    ticks_num = atoi(argv[2]);
  	if (ticks_num <= 0) {
      int status = disable_protocol(num);
      if (status < 0) {
        printf("Wrong arguments...\n");
        exit(1);
      }
  	} else {
      int status = enable_protocol(num, ticks_num);
      if (status < 0) {
        printf("Wrong arguments...\n");
        exit(1);
      }
    }
  } else {
    printf("Wrong number of arguments...\n");
    exit(1);
  }
  exit(0);
}

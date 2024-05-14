#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "kernel/param.h"
#include "kernel/fs.h"
#include "user/user.h"

#define N_CASES 10
#define BUFSIZE 512

char buf[BUFSIZE];

char* cases[N_CASES] = {
    "000\n",
    "111\n",
    "222\n",
    "333\n",
    "444\n",
    "555\n",
    "666\n",
    "777\n",
    "888\n",
    "999\n"
};

int generate_file(char* path, char* content) {
    int file = open(path, O_CREATE | O_RDWR);
    if (file < 0) return -1;
    write(file, content, sizeof(content));
    close(file);
    return 0;
}

int main(int argc, char const *argv[])
{
    int n;
    n = mkdir("/d1");
    if (n < 0) {
        fprintf(2, "ERROR...\n");
        exit(1);
    }
    n = mkdir("/d1/d2");
    if (n < 0) {
        fprintf(2, "ERROR...\n");
        exit(1);
    }
    n = mkdir("/d1/d2/d3");
    if (n < 0) {
        fprintf(2, "ERROR...\n");
        exit(1);
    }
    n = mkdir("/d1/d2/d3/d4");
    if (n < 0) {
        fprintf(2, "ERROR...\n");
        exit(1);
    }
    n = mkdir("/d1/d2/d3/d4/d5");
    if (n < 0) {
        fprintf(2, "ERROR...\n");
        exit(1);
    }

    n = generate_file("/d1/d2/d3/f1", cases[0]);
    if (n < 0) {
        fprintf(2, "ERROR...\n");
        exit(1);
    }
    n = generate_file("/d1/f2", cases[1]);
    if (n < 0) {
        fprintf(2, "ERROR...\n");
        exit(1);
    }
    n = generate_file("/d1/d2/d3/d4/d5/f3", cases[2]);
    if (n < 0) {
        fprintf(2, "ERROR...\n");
        exit(1);
    }

    n = symlink("/d1/d2/d3/f1", "/d1/d2/d3/l1");
    if (n < 0) {
        fprintf(2, "ERROR...\n");
        exit(1);
    }
    n = symlink("./f1", "/d1/d2/d3/l2");
    if (n < 0) {
        fprintf(2, "ERROR...\n");
        exit(1);
    }
    n = symlink("./d4/d5/f3", "/d1/d2/d3/l3");
    if (n < 0) {
        fprintf(2, "ERROR...\n");
        exit(1);
    }
    n = symlink("../../f2", "/d1/d2/d3/l4");
    if (n < 0) {
        fprintf(2, "ERROR...\n");
        exit(1);
    }
    n = symlink("/d1/d2/d3/l1", "/d1/d2/d3/l5");
    if (n < 0) {
        fprintf(2, "ERROR...\n");
        exit(1);
    }
    n = symlink("/d1/d2/d3/l2", "/d1/d2/d3/l6");
    if (n < 0) {
        fprintf(2, "ERROR...\n");
        exit(1);
    }
    n = symlink("./l2", "/d1/d2/d3/l7");
    if (n < 0) {
        fprintf(2, "ERROR...\n");
        exit(1);
    }
    n = symlink("/d1/d2/d3/l8", "/d1/d2/d3/l8");
    if (n < 0) {
        fprintf(2, "ERROR...\n");
        exit(1);
    }
    n = symlink("/d1/d2/d3/l9", "/d1/d2/d3/l10");
    if (n < 0) {
        fprintf(2, "ERROR...\n");
        exit(1);
    }
    n = symlink("/d1/d2/d3/l10", "/d1/d2/d3/l9");
    if (n < 0) {
        fprintf(2, "ERROR...\n");
        exit(1);
    }
    n = symlink("/d1/d2/d3/w", "/d1/d2/d3/l11");
    if (n < 0) {
        fprintf(2, "ERROR...\n");
        exit(1);
    }
    n = symlink("./f2", "/d1/d2/d3/l12");
    if (n < 0) {
        fprintf(2, "ERROR...\n");
        exit(1);
    }
    n = symlink("./d4/d5/f1", "/d1/d2/d3/l13");
    if (n < 0) {
        fprintf(2, "ERROR...\n");
        exit(1);
    }
    n = symlink("../../f1", "/d1/d2/d3/l14");
    if (n < 0) {
        fprintf(2, "ERROR...\n");
        exit(1);
    }
    exit(0);
}

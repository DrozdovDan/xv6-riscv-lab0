#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

#define MULTIPLIER 1442695040888963407ULL

#define MODULUS 6364136223846793005ULL

#define INCREMENT 1

#define BUFSIZE 2048

#define MAXSIZE (11 + 256 + 256 * 256) * 1024 / sizeof(uint)

#define FILENAME "BIG FILE"


void generate_next(uint *seed) {
    *seed = (MULTIPLIER * (*seed) + INCREMENT) % MODULUS;
}

int write_file(uint size, uint data) {
    uint* buf = malloc(sizeof(uint) * BUFSIZE);
    if (buf == 0) {
        fprintf(2, "Can't malloc...\n");
        return 2;
    }
    int fd = open(FILENAME, O_CREATE | O_WRONLY);
    if (fd < 0) {
        fprintf(2, "Can't create a file...\n");
        return 3;
    }
    for (int i = 0; i < size; i++) {
        buf[i % BUFSIZE] = data;
        if (i == size - 1 || (i + 1) % BUFSIZE == 0) {
            int write_size = size % BUFSIZE;
            if ((i + 1) % BUFSIZE == 0)
                write_size = BUFSIZE;
            if (write(fd, buf, sizeof(uint) * write_size) != sizeof(uint) * write_size) {
                close(fd);
                fprintf(2, "Incorrect write in file...\n");
                return 4;
            }
        }
        generate_next(&data);
    }
    close(fd);
    free(buf);
    return 0;
}

int check_identity(uint size, uint data) {
    uint* buf = malloc(sizeof(uint) * BUFSIZE);
    if (buf == 0) {
        fprintf(2, "Can't malloc...\n");
        return 2;
    }
    int fd = open(FILENAME, O_RDONLY);
    if (fd < 0) {
        fprintf(2, "Can't open the file...\n");
        return 5;
    }
    for (int i = 0; i < size / BUFSIZE; i++) {
        if (read(fd, buf, sizeof(uint) * BUFSIZE) != sizeof(uint) * BUFSIZE) {
            close(fd);
            fprintf(2, "Incorrect read from file...\n");
            return 6;
        }
        for (int j = 0; j < BUFSIZE; j++) {
            if (data != buf[j]) {
                close(fd);
                printf("Mismatch at position %d: expected %d, got %d instead\n", i * BUFSIZE + j, data, buf[j]);
                return 1;
            }
            generate_next(&data);
        }
    }
    if (size % BUFSIZE != 0) {
        if (read(fd, buf, sizeof(uint) * (size % BUFSIZE)) != sizeof(uint) * (size % BUFSIZE)) {
            close(fd);
            fprintf(2, "Incorrect read from file...\n");
            return 6;
        }
        for (int i = 0; i < size % BUFSIZE; i++) {
            if (data != buf[i]) {
                close(fd);
                printf("Mismatch at position %d: expected %d, got %d instead\n", size - (size % BUFSIZE) + i, data, buf[i]);
                return 1;
            }
            generate_next(&data);
        }
    }
    close(fd);
    free(buf);
    return 0;
}

int main(int argc, char** argv) {
    int size = atoi(argv[1]);
    int seed = atoi(argv[2]);
    size /= sizeof(uint);
    if (size <= 0 || size > MAXSIZE) {
        fprintf(2, "Wrong size...\n");
        exit(1);
    }
    int status = write_file(size, seed);
    if (status != 0)
        exit(status);
    status = check_identity(size, seed);
    if (status > 1)
        exit(status);
    if (status == 0)
        printf("It is done!\n");
    exit(0);
}
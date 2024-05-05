#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

#define MULTIPLIER 1442695040888963407ULL

#define MODULUS 6364136223846793005ULL

#define INCREMENT 1

#define MAXSIZE (11 + 256 + 256 * 256) * 1024 / sizeof(uint)


uint64_t generate_next(uint64_t *seed) {
    *seed = (MULTIPLIER * (*seed) + INCREMENT) % MODULUS;
    return *seed;
}

bool verify_file(const char *filename, uint64_t seed, off_t file_size) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("open");
        return false;
    }

    uint64_t buffer[BUFFER_SIZE / sizeof(uint64_t)];
    uint64_t expected seed = seed;
    off_t remaining_bytes = file_size;

    while (remaining_bytes > 0) {
        size_t bytes_to_read = (remaining_bytes > BUFFER_SIZE) ? BUFFER_SIZE : remaining_bytes;
        ssize_t bytes_read = read(fd, buffer, bytes_to_read);
        if (bytes_read == -1) {
            perror("read");
            close(fd);
            return false;
        }

        for (size_t i = 0; i < bytes_read / sizeof(uint64_t); i++) {
            uint64_t expected_value = generate_next(&expected_seed);
            if (buffer[i] != expected_value) {
                fprintf(stderr, "Mismatch at position %ld: expected %lu, got %lu\n", (long)(remaining_bytes - bytes_read + i * sizeof(uint64_t)), expected_value, buffer[i]);
                close(fd);
                return false;
            }
        }

        remaining_bytes -= bytes_read;
    }

    close(fd);
    return true;
}


int main(int argc, char** argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <file_size> <seed>\n", argv[0]);
        exit(1);
    }

    off_t file_size = atoll(argv[1]);
    uint64_t seed = strtoull(argv[2], NULL, 10);
    const char *filename = "test_file";
    int fd = open(filename, OWRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open");
        close(1);
    }

    uint64_t buffer[BUFFER_SIZE / sizeof(uint64_t)];
    off_t remaining_bytes = file_size;

    while (remaining_bytes > 0) {
        size_t bytes_to_write = (remaining_bytes > BUFFER_SIZE) ? BUFFER_SIZE : remaining_bytes;
        for (size_t i = 0; i < bytes_to_write / sizeof(uint64_t); i++) {
            buffer[i] = generate_next(&seed);
        }
        uint64_t bytes_written = write(fd, buffer, bytes_to_write);
        if (bytes_written == -1) {
            perror("write");
            close(fd);
            close(1);
        }
        remaining_bytes -= bytes_written;
    }

    close(fd);

    bool verification_result = verify_file(filename, seed, file_size);
    if (verification_result) {
        printf("Verification successful\n");
    } else {
        printf("Verification failed\n");
    }

    exit(0);
}
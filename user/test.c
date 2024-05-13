#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "kernel/param.h"
#include "kernel/fs.h"
#include "user/user.h"

#define MAX_DEPTH 10
#define N_CASES 14
#define BUFSIZE 512

char buf[BUFSIZE];

char* cases[N_CASES] = {
    "Корректная абсолютная ссылка на файл",
    "Корректная относительная ссылка на файл того же каталога",
    "Корректная относительная ссылка на файл каталога на 2-3 уровня выше",
    "Корректная относительная ссылка на файл каталога на 2-3 уровня ниже",
    "Корректная абсолютная ссылка на абсолютную символическую ссылку",
    "Корректная абсолютная ссылка на относительную символическую ссылку",
    "Корректная относительная ссылка на относительную символическую ссылку",
    "Ссылка на себя (бесконечная рекурсия)",
    "Косвенная ссылка на себя (бесконечная рекурсия через 2-3 перехода)",
    "Абсолютная ссылка на несуществующий файл",
    "Относительная ссылка на несуществующий файл того же каталога",
    "Относительная ссылка на несуществующий файл каталога на 2-3 уровня выше",
    "Относительная ссылка на несуществующий файл каталога на 2-3 уровня ниже",
    0
};

int prepare_cases() {
    if (open("aa", O_CREATE | O_WRONLY) < 0) return 1;
    if (open("bb", O_CREATE | O_WRONLY) < 0) return 1;
    if (open("cc", O_CREATE | O_WRONLY) < 0) return 1;
    if (open("dd", O_CREATE | O_WRONLY) < 0) return 1;

    if (mkdir("dir1") < 0) return 1;
    if (mkdir("dir1/dir2") < 0) return 1;
    if (mkdir("dir1/dir2/dir3") < 0) return 1;

    if (symlink("aa", "link1") < 0) return 1;                     // 0
    if (symlink("bb", "link2") < 0) return 1;                     // 1
    if (symlink("../bb", "dir1/link3") < 0) return 1;             // 2
    if (symlink("../dir1/bb", "dir1/dir2/link4") < 0) return 1;   // 3
    if (symlink("/link1", "dir1/dir2/dir3/link5") < 0) return 1;  // 4
    if (symlink("/dir1/link3", "dir1/dir2/link6") < 0) return 1;  // 5
    if (symlink("link4", "dir1/dir2/link7") < 0) return 1;        // 6
    if (symlink("link7", "dir1/dir2/link8") < 0) return 1;        // 7
    if (symlink("link6", "dir1/dir2/link9") < 0) return 1;        // 8
    if (symlink("ee", "link10") < 0) return 1;                    // 9
    if (symlink("ff", "dir1/link11") < 0) return 1;               // 10
    if (symlink("../gg", "dir1/link12") < 0) return 1;            // 11
    if (symlink("../dir1/hh", "dir1/dir2/link13") < 0) return 1;  // 12

    return 0;
}

int test_case(char* path, int case_id) {
    char buf[BUFSIZE];
    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        printf("Ошибка открытия %s\n", path);
        return 1;
    }
    int n = read(fd, buf, sizeof(buf));
    if (n < 0) {
        printf("Ошибка чтения %s\n", path);
        close(fd);
        return 1;
    }
    buf[n] = 0;
    printf("Случай %d: %s\nСодержимое: %s\n", case_id, cases[case_id], buf);
    close(fd);
    return 0;
}

int main(int argc, char *argv[]) {
    prepare_cases();

    int i, ret = 0;
    for (i = 0; cases[i]; i++) {
        ret |= test_case(".", i);

        if (chdir("..") == 0) {
            ret |= test_case("..", i);
            chdir(".");
        }

        if (chdir("..") == 0) {
            ret |= test_case("..", i);
            chdir("..");
        }
    }

    if (ret > 0)
    	fprintf(2, "Something isn't normal...\n");
   	else
   		fprintf(2, "Everything is ok!\n");
    exit(ret);
}

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#define WANTED_PATH "/home/challenge04/creating/files/and/directories/is/easy"

int main(void)
{
    struct stat statbuf;
    int status = stat(WANTED_PATH, &statbuf);

    if (-1 == status) {
        fprintf(stderr, "File %s does not exist.\nCreate it and run the command again :)\n", WANTED_PATH);
        return 1;
    }

    if (S_ISDIR(statbuf.st_mode)) {
        fprintf(stderr, "Path %s is supposed to point to a file, not a directory\n", WANTED_PATH);
        return 2;
    }

    printf("Awesome job! The flag is: {{flag}}\n");

    return 0;
}

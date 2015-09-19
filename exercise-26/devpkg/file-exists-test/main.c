#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <errno.h>

#include <stdio.h>
#include <assert.h>

int main(void)
{
    /*
    printf("hallo!\n");
    
    int fd = open("./test.txt", O_RDONLY);
    assert(fd >= 0);
    printf("fd is %d\n", fd);
    
    struct stat fileStats;
    errno = 0;
    int fstat_rc = fstat(fd, &fileStats);
    assert(fstat_rc == 0);
    */
    
    
    struct stat fileStats;
    int stat_rc = 0;

    stat_rc = stat("./testsss.txt", &fileStats);
    
    if (stat_rc == 0) {
        printf("file exists\n");
    } else {
        printf("file does not exit\n");
    }
    
    
    // @TODO execute
    
    
    /*
        http://pubs.opengroup.org/onlinepubs/9699919799/functions/open.html
        http://stackoverflow.com/questions/230062/whats-the-best-way-to-check-if-a-file-exists-in-c-cross-platform
        http://stackoverflow.com/questions/4629643/implementing-a-unix-shell-in-c-check-if-file-is-executable
    */
    
    return 0;
}

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUFFER_LENGTH 1024
int main() {
    //open
    int fd = open("/dev/ntychannel", O_RDWR);
    if (fd < 0) {
        printf("open failed: errno --> %d\n", errno);
        return -1;
    }
    //select
    char buffer[BUFFER_LENGTH] = {0};
    fd_set rds;
    FD_ZERO(&rds);
    FD_SET(fd, &rds);
    while(1) {

        int ret = select(fd+1, &rds, NULL, NULL, NULL);
        if (ret < 0) {
            printf("select error\n");
            return -1;
        }

        if (FD_ISSET(fd, &rds)) {
            read(fd, buffer, BUFFER_LENGTH);
            printf("channel data : %s\n", buffer);
        }
    }
    close(fd);
    //read
    // write echo "0voice" > /dev/ntychannel
    return 0;
}
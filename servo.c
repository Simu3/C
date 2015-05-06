#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int deviceOpen(char*, char*);
void deviceClose(int);

int main(int argc, char*argv[]){
    char data;

    fd = deviceOpen(argv[1], argv[2]);
    if(fd == -1){
        printf("%s doesn't open.\n", argv[1]);
        return 0;
    }

    data = 97;
    write(fd, &data, sizeof(data));
    printf("write\n");

    deviceClose(fd);

    return 0;
}

int deviceOpen(char *device, char *baudrate){
    int fd;
    struct termios oldtio, newtio;
    
    fd = open(device, O_RDWR | O_NOCTTY | O_NONBLOCK);

    if(fd < 0){
        return -1;
    }
    
    tcgetattr(fd, &oldtio);
    newtio = oldtio;
    cfsetspeed(&newtio, baudrate);
    tcflush(fd, TCIFLUSH);
    tcsetattr(fd, TCSANOW, &newtio);

    return fd;
}

void deviceClose(int fd){ 
    tcsetattr(fd, TCSANOW, &oldtio);
    close(fd);
}

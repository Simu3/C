#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEVICE   "/dev/cu.usbserial-FTH7PKJS"
#define BAUDRATE B9600

int deviceOpen(void);
void deviceClose(int);

int main(void){
    char data;

    fd = deviceOpen();
    if(fd == -1){
        printf("%s doesn't open.\n", DEVICE);
        return 0;
    }

    data = 97;
    write(fd, &data, sizeof(data));
    printf("write\n");

    deviceClose(fd);

    return 0;
}

int deviceOpen(void){
    int fd;
    struct termios oldtio, newtio;
    
    fd = open(DEVICE, O_RDWR | O_NOCTTY | O_NONBLOCK);

    if(fd < 0){
        return -1;
    }
    
    tcgetattr(fd, &oldtio);
    newtio = oldtio;
    cfsetspeed(&newtio, BAUDRATE);
    tcflush(fd, TCIFLUSH);
    tcsetattr(fd, TCSANOW, &newtio);

    return fd;
}

void deviceClose(int fd){ 
    tcsetattr(fd, TCSANOW, &oldtio);
    close(fd);
}

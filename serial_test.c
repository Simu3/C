#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEVICE   "/dev/tty.usbserial-FTGCKD3V"
#define BAUDRATE B115200

int main(void){
    int fd;
    struct termios oldtio, newtio;
    char buf[20];

    fd = open(DEVICE, O_RDWR | O_NOCTTY | O_NONBLOCK);

    if(fd < 0){
        printf("%s doesn't open.\n", DEVICE);
        return 0;
    }

    tcgetattr(fd, &oldtio);
    newtio = oldtio;
    cfsetspeed(&newtio, BAUDRATE);
    tcflush(fd, TCIFLUSH);
    tcsetattr(fd, TCSANOW, &newtio);

    strcpy(buf, "hello");
    write(fd, buf, sizeof(buf));
    printf("write\n");

    tcsetattr(fd, TCSANOW, &oldtio);
    close(fd);

    return 0;
}

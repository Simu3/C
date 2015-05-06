#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEVICE   "/dev/cu.usbserial-FTH7PKJS"
#define BAUDRATE B115200
#define ID       0

int main(void){
    int fd;
    struct termios oldtio, newtio;
    struct timespec ts;
    int data = 7500;
    unsigned char cmd;
    unsigned char pos_h, pos_l;

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

    /* User's program */
    ts.tv_sec = 0;
    ts.tv_nsec = 10000000;

    cmd = 0x80 | ID;
    pos_h = (unsigned char)((data >> 7) & 0x7F);
    pos_l = (unsigned char)(data & 0x7F);
    write(fd, &cmd, sizeof(cmd));
    nanosleep(&ts, NULL);
    printf("write\n");
    write(fd, &pos_h, sizeof(pos_h));
    nanosleep(&ts, NULL);
    printf("write\n");
    write(fd, &pos_l, sizeof(pos_l));
    nanosleep(&ts, NULL);
    printf("write\n");
    /* end */

    tcsetattr(fd, TCSANOW, &oldtio);
    close(fd);

    return 0;
}

/*
 * main.c
 *
 *  Created on: 23.12.2012
 *      Author: dzusan
 */


#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>


int main(int argc, char* argv[])
{
        int fd;
        fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY);

        char tx[100] = "ABCD\n";
        char rx[100] = "";

        write(fd, tx, 5);
        read(fd, rx, 4);
        printf("TX -> %sRX <- %s\n", tx, rx);

        close(fd);

        return 1;
}

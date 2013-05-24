#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/signal.h>
#include <sys/types.h>
#include <stdint.h>
#include <linux/kernel.h>
#include <time.h>

int OpenPort(void);
void ClosePort(int fd);
void WritePort(int fd);
void ReadPort(int fd);
void ConfPort(int fd);
void ConfPort(int fd);

int main(void)
{
    int fd;
    uint8_t buf[10];
    int status;
    int i;
    uint16_t eqvTmp;
    int16_t eqv;
    int16_t zero[3];
    clock_t startTime;

    fd = OpenPort();

    printf("Calibrating...\n");

    startTime = clock();
    while ((double)(clock() - startTime) / CLOCKS_PER_SEC < 1);

    status = write(fd, "12345678", 8);
    status = read(fd, buf, 10);
    for(i = 0; i < 6; i += 2)
    {
        eqvTmp = (uint16_t)(buf[i] << 8) | (uint16_t)buf[i + 1];
        zero[i / 2] = *(&eqvTmp);
        printf("%8d ", zero[i / 2]);
    }

    printf("\nCalibrating done\n");

    startTime = clock();
    while ((double)(clock() - startTime) / CLOCKS_PER_SEC < 1);

    while(1)
    {
        status = write(fd, "12345678", 8);
        status = read(fd, buf, 10);

        for(i = 0; i < 6; i += 2)
        {
            eqvTmp = (uint16_t)(buf[i] << 8) | (uint16_t)buf[i + 1];
            eqv = *(&eqvTmp) - zero[i / 2];
            printf("%8d ", eqv);
        }

        printf("\n");

        startTime = clock();
        while ((double)(clock() - startTime) / CLOCKS_PER_SEC < 1);
    }

    close(fd);

    return 0;
}

int OpenPort(void)
{
  int fd; /* Файловый дескриптор для порта */

  fd = open("/dev/ttyUSB3", O_RDWR | O_NOCTTY | O_NDELAY);

  if (fd == -1) perror("Unable to open port");
  else fcntl(fd, F_SETFL, 0);

  return (fd);
}

void ClosePort(int fd)
{
    close(fd);
}

void WritePort(int fd)
{
    int n;
    n = write(fd, "ADF\r", 4);
    if (n < 0) printf("Write failed!\n");//fputs("Write failed!\n", stderr);
}

void ReadPort(int fd)
{
    int status;
    status = fcntl(fd, F_SETFL, 0);
    if(status == 0) printf("Nothing to read!\n");
}

void ConfPort(int fd)
{
    struct termios options;
    int status;

    tcgetattr(fd, &options);

    cfsetispeed(&options, B9600);
    cfsetospeed(&options, B9600);

    options.c_cflag |= (CLOCAL | CREAD);

    options.c_cflag &= ~CSIZE; /* Маскирование битов размера символов */
    options.c_cflag |= CS8;    /* Установка 8 битов данных */

    options.c_cflag |= PARENB;
    options.c_cflag &= ~PARODD;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;

    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

    status = tcsetattr(fd, TCSANOW, &options);
}

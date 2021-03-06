//
// arduino-serial-lib -- simple library for reading/writing serial ports
//
// 2006-2013, Tod E. Kurt, http://todbot.com/blog/
//

#include "control.h"

#include <stdio.h>    // Standard input/output definitions 
#include <unistd.h>   // UNIX standard function definitions 
#include <fcntl.h>    // File control definitions 
#include <errno.h>    // Error number definitions 
#include <termios.h>  // POSIX terminal control definitions 
#include <string.h>   // String function definitions 
#include <sys/ioctl.h>

// uncomment this to debug reads
//#define SERIALPORTDEBUG 

// takes the string name of the serial port (e.g. "/dev/tty.usbserial","COM1")
// and a baud rate (bps) and connects to that port at that speed and 8N1.
// opens the port in fully raw mode so you can send binary data.
// returns valid fd, or -1 on error
Conn serial_init(char* serialport, int baud)
{
    struct termios toptions;
    Conn c, error;
    error.fd = -1;
    
    //fd = open(serialport, O_RDWR | O_NOCTTY | O_NDELAY);
    c.fd = open(serialport, O_RDWR | O_NONBLOCK );
    
    if(c.fd == -1)  {
        perror("serial_init: Unable to open port ");
        return error;
    }
    
    //int iflags = TIOCM_DTR;
    //ioctl(c.fd, TIOCMBIS, &iflags);     // turn on DTR
    //ioctl(c.fd, TIOCMBIC, &iflags);    // turn off DTR

    if(tcgetattr(c.fd, &toptions) < 0) {
        perror("serial_init: Couldn't get term attributes");
        return error;
    }
    speed_t brate = baud; // let you override switch below if needed
    switch(baud) {
    case 4800:   brate=B4800;   break;
    case 9600:   brate=B9600;   break;
#ifdef B14400
    case 14400:  brate=B14400;  break;
#endif
    case 19200:  brate=B19200;  break;
#ifdef B28800
    case 28800:  brate=B28800;  break;
#endif
    case 38400:  brate=B38400;  break;
    case 57600:  brate=B57600;  break;
    case 115200: brate=B115200; break;
    }
    cfsetispeed(&toptions, brate);
    cfsetospeed(&toptions, brate);

    // 8N1
    toptions.c_cflag &= ~PARENB;
    toptions.c_cflag &= ~CSTOPB;
    toptions.c_cflag &= ~CSIZE;
    toptions.c_cflag |= CS8;
    // no flow control
    toptions.c_cflag &= ~CRTSCTS;

    //toptions.c_cflag &= ~HUPCL; // disable hang-up-on-close to avoid reset

    toptions.c_cflag |= CREAD | CLOCAL;  // turn on READ & ignore ctrl lines
    toptions.c_iflag &= ~(IXON | IXOFF | IXANY); // turn off s/w flow ctrl

    toptions.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // make raw
    toptions.c_oflag &= ~OPOST; // make raw

    // see: http://unixwiz.net/techtips/termios-vmin-vtime.html
    toptions.c_cc[VMIN]  = 0;
    toptions.c_cc[VTIME] = 0;
    //toptions.c_cc[VTIME] = 20;
    
    tcsetattr(c.fd, TCSANOW, &toptions);
    if(tcsetattr(c.fd, TCSAFLUSH, &toptions) < 0) {
        perror("init_serial: Couldn't set term attributes");
        return error;
    }

    return c;
}

int bad_init(Conn c) {
    return c.fd == -1;
}

int serial_close(Conn c)
{
    return close(c.fd);
}

// reads until newline or BUFFER_SIZE characters
int serial_read(Conn c) {
    char buffer[BUFFER_SIZE];
    int n = 0;
    while(n < BUFFER_SIZE) {
        int nread  = read(c.fd, buffer+n, 1);
        if(nread < 1)
            return 0;
        if(buffer[n] == '\n')
            return 0;
        n += nread;
    }
    return 1;
}

int serial_write(Conn c, char* str)
{
    int len = strlen(str);
    int n = write(c.fd, str, len);
    if( n!=len ) {
        perror("serial_write: couldn't write whole string\n");
        return -1;
    }
    //return serial_read(c);
    return 0;
}

int serial_write_profile(Conn c, Profile profile) {
    if(profile.type == STATIC) {
        char buffer[16];
        memset(buffer, 0, 16);
        sprintf(buffer, "%d %hhu %hhu %hhu\n", STATIC, profile.r, profile.g, profile.b);
        return serial_write(c, buffer);
    }
    return 0;
}

void wait() {
    pause();
}

void hide_console() {
    
}
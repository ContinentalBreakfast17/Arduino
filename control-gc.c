#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include "control.h"

void INThandler(int);

int main(void) {
	Conn c = serial_init(getenv("SERIAL_PORT"), BAUD_RATE);
	if(bad_init(c)) {
		printf("Can't connect to serial device (startup)\n");
		return -1;
	}
	serial_write(c, "on\n");
	serial_close(c);

	printf("on\n");

	signal(SIGINT, INThandler);
	while (1)
		wait();
	return 0;
}

void INThandler(int sig) {
	Conn c = serial_init(getenv("SERIAL_PORT"), BAUD_RATE);
	if(bad_init(c)) {
		printf("Can't connect to serial device (shutdown)\n");
	} else {
		serial_write(c, "off\n");
		printf("off\n");
		serial_close(c);
	}

	exit(sig);
}
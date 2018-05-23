#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include "control.h"

void sigHandler(int);

int main(void) {
	hide_console();
	Conn c = serial_init(getenv("SERIAL_PORT"), BAUD_RATE);
	if(bad_init(c)) {
		printf("Can't connect to serial device (startup)\n");
		return -1;
	}
	serial_write(c, "on\n");
	serial_close(c);

	printf("on\n");

	signal(SIGBREAK, sigHandler);
	signal(SIGINT, sigHandler);
	while (1)
		wait();
	return 0;
}

void sigHandler(int sig) {
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
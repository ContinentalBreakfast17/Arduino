#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "control.h"

Conn serial_init(char* serialport, int baud) {
	Conn c, error;
	error.h = NULL;
	c.h = CreateFile(serialport,  
	                   GENERIC_READ | GENERIC_WRITE, 
	                   0, 
	                   0, 
	                   OPEN_EXISTING,
	                   0,
	                   0);
	if (c.h == INVALID_HANDLE_VALUE) {
		return error;
	}

	DCB dcb;
	memset(&dcb, 0, sizeof(DCB));
	dcb.DCBlength = sizeof(DCB);
	int success = GetCommState(c.h, &dcb);
	if(!success) return error;
	//printf("%d %d %d %d\n", dcb.BaudRate, dcb.ByteSize, dcb.Parity, dcb.StopBits);
	dcb.BaudRate = baud;
	dcb.ByteSize = 8;
	dcb.Parity = NOPARITY;
	dcb.StopBits = ONESTOPBIT;
	success = SetCommState(c.h, &dcb);
	if(!success) return error;
	success = GetCommState(c.h, &dcb);
	if(!success) return error;
	//printf("%d %d %d %d\n", dcb.BaudRate, dcb.ByteSize, dcb.Parity, dcb.StopBits);

	return c;
}

int bad_init(Conn c) {
	return c.h == NULL;
}

int serial_close(Conn c) {
	return !CloseHandle(c.h);
}

int serial_read(Conn c) {
	char buffer[BUFFER_SIZE];
	memset(buffer, 0, BUFFER_SIZE);
	DWORD length;
	int result = !ReadFile(c.h, buffer, BUFFER_SIZE, &length, NULL);
	if(result) return 1;
	return 0;
}

int serial_write(Conn c, char* str) {
	DWORD length;
	if(!WriteFile(c.h, str, strlen(str), &length, NULL))
		return 1;
	return serial_read(c);
}

int serial_write_profile(Conn c, Profile profile) {
    if(profile.type == STATIC) {
        char buffer[32];
        memset(buffer, 0, 32);
        sprintf(buffer, "%d %hhu %hhu %hhu\n", STATIC, profile.r, profile.g, profile.b);
        return serial_write(c, buffer);
    }
    return 0;
}

void wait() {
	Sleep(1000*3);
}
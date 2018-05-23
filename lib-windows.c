#include <windows.h>
#include <stdio.h>
#include <string.h>

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

	return c;
}

int bad_init(Conn c) {
	return c.h == NULL;
}

int serial_close(Conn c) {
	return !CloseHandle(c.h);
}

int serial_read(Conn c) {
	char buffer[BUFFER_SZ];
	int length;
	return !ReadFile(c.h, buffer, BUFFER_SZ, &length, NULL);
}

int serial_write(Conn c, char* str) {
	int length;
	if(!WriteFile(c.h, str, len(str), &length, NULL))
		return 0;
	return serial_read(c);
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
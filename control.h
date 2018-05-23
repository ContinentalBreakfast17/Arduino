//
// arduino-serial-lib -- simple library for reading/writing serial ports
//
// 2006-2013, Tod E. Kurt, http://todbot.com/blog/
//


#ifndef __CONTROL_H__
#define __CONTROL_H__

#include <stdint.h>

#define STATIC 0

typedef struct profile_data {
	unsigned char 	r;
	unsigned char 	g;
	unsigned char 	b;
	unsigned char 	type;
} Profile;

typedef struct profile_tracker {
	Profile* 		list;
	unsigned int 	size;
	unsigned int 	current;
} Profiles;

int serial_init(char* serialport, int baud);
int serial_close(int fd);
int serial_write(int fd, char* str);
int serial_write_profile(int fd, Profile profile);
int serial_flush(int fd);

#endif
//
// arduino-serial-lib -- simple library for reading/writing serial ports
//
// 2006-2013, Tod E. Kurt, http://todbot.com/blog/
//


#ifndef __CONTROL_H__
#define __CONTROL_H__

#include <stdint.h>

#define BUFFER_SZ 128
#define STATIC 0

typedef union connection {
	int 			fd;
	void* 			h;
} Conn;

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

Conn serial_init(char* serialport, int baud);
int bad_init(Conn c);
int serial_close(Conn c);
int serial_write(Conn c, char* str);
int serial_write_profile(Conn c, Profile profile);

#endif
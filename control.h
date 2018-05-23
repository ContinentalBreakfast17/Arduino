//
// arduino-serial-lib -- simple library for reading/writing serial ports
//
// 2006-2013, Tod E. Kurt, http://todbot.com/blog/
//


#ifndef __ARDUINO_SERIAL_LIB_H__
#define __ARDUINO_SERIAL_LIB_H__

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

/*int serial_init(const char* serialport, int baud);
int serial_close(int fd);
int serial_write(int fd, const char* str);
int serial_writeprofile(int fd, Profile profile)
int serial_flush(int fd);
*/
#endif
#ifndef __CONTROL_H__
#define __CONTROL_H__

//#include <stdint.h>

#define READ_ERROR 		1
#define INIT_ERROR 		2
#define ADD_ERROR		3
#define GET_LINE_ERROR 	4

#define BUFFER_SIZE 	128
#define BAUD_RATE		9600

#define STATIC 			0


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

void hide_console();
Conn serial_init(char* serialport, int baud);
int bad_init(Conn c);
int serial_close(Conn c);
int serial_write(Conn c, char* str);
int serial_write_profile(Conn c, Profile profile);
void wait();

#endif
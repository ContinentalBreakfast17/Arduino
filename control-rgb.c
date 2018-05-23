#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "control.h"

#define READ_ERROR 		1
#define INIT_ERROR 		2
#define ADD_ERROR		3
#define GET_LINE_ERROR 	4
#define SERIAL_PORT 	"COM3"
#define BAUD_RATE		9600

Profiles* read_profiles() {
	// open profiles file
	char* file = getenv("RGB_PROFILES");
	FILE* fp = fopen(file, "r");
	if(fp == NULL) {
		printf("An error occured opening profiles: %s\n", strerror(errno));
		return NULL;
	}

	// allocate profiles, then read starting profile and size, then allocate list
	Profiles* profiles = (Profiles*)malloc(sizeof(Profiles));
	if(profiles == NULL) {
		printf("An error occured allocating profiles struct: %s\n", strerror(errno));
		return NULL;
	}
	int result = fscanf(fp, "%u %u\n", &profiles->size, &profiles->current);
	if(result != 2) {
		printf("Profiles file does not start with proper header\n");
		return NULL;
	}
	profiles->list = (Profile*)malloc(sizeof(Profile)*profiles->size);
	if(profiles->list == NULL) {
		printf("An error occured allocating profiles list: %s\n", strerror(errno));
		return NULL;
	}

	// read all the profiles
	for(int i = 0; i < profiles->size; i++) {
		result = fscanf(fp, "%hhu %hhu %hhu %hhu\n", &profiles->list[i].r, &profiles->list[i].g, &profiles->list[i].b, &profiles->list[i].type);
		if(result != 4) {
			printf("Improper formatting in profiles file\n");
			return NULL;
		}
	}

	fclose(fp);
	return profiles;
}

int save_profiles(Profiles* profiles) {
	char* file = getenv("RGB_PROFILES");
	FILE* fp = fopen(file, "w");
	if(fp == NULL) return 1;

	fprintf(fp, "%u %u\n", profiles->size, profiles->current);
	for(int i = 0; i < profiles->size; i++) {
		fprintf(fp, "%hhu %hhu %hhu %hhu\n", profiles->list[i].r, profiles->list[i].g, profiles->list[i].b, profiles->list[i].type);
	}

	fclose(fp);
}

int add_profile(Profiles* profiles) {
	profiles->size++;
	profiles->list = (Profile*)realloc(profiles->list, profiles->size*sizeof(Profile));
	if(profiles->list == NULL) {
		printf("Failed to reallocate profiles list: %s\n", strerror(errno));
		return ADD_ERROR;
	}

	profiles->current = profiles->size - 1;
	profiles->list[profiles->current].r = rand();
	profiles->list[profiles->current].g = rand();
	profiles->list[profiles->current].b = rand();
	profiles->list[profiles->current].type = STATIC;
	return 0;
}

void get_color(char* color, unsigned char* pointer) {
	printf("Enter a value for %s: ", color);
	while(scanf("%hhu", pointer) != 1) {
		while(fgetc(stdin) != '\n');
		printf("Invalid input. Please enter a number in the range [0,255]\n");
		printf("Enter a value for %s: ", color);
	}
	while(fgetc(stdin) != '\n');
}

void edit_profile(Profile* profile) {
	get_color("Red", &profile->r);
	get_color("Green", &profile->g);
	get_color("Blue", &profile->b);
}

void switch_profile(Profiles* profiles) {
	printf("Profile to switch to: ");
	while(scanf("%u", &profiles->current) != 1) {
		while(fgetc(stdin) != '\n');
		printf("Invalid input. Please enter a number in the range [1,%d]\n", profiles->size);
		printf("Profile to switch to: ");
	}
	while(fgetc(stdin) != '\n');
	profiles->current--;
}

void help() {
	printf("Commands (omit apostrophe's):\n");
	printf("'edit': edits current profile\n");
	printf("'switch': switch profiles\n");
	printf("'add': adds and switches to a new profile\n");
	printf("'quit': terminate program\n");
}

void shutdown(Profiles* profiles) {
	int error = save_profiles(profiles);
	if(error) {
		printf("Profiles unable to be saved: %s\n\n", strerror(errno));
		printf("Profile dump:\n");
		printf("%u %u\n", profiles->size, profiles->current);
		for(int i = 0; i < profiles->size; i++) {
			printf("%hhu %hhu %hhu %hhu\n", profiles->list[i].r, profiles->list[i].g, profiles->list[i].b, profiles->list[i].type);
		}
	}
	free(profiles->list);
	free(profiles);
}

void check_arduino(int n) {
	if(n) {
		printf("Arduino may be disconnected\n");
	}
}

int main(int argc, char** argv) {
	Profiles* profiles = read_profiles();
	if(profiles == NULL) exit(READ_ERROR);

	Conn c = serial_init(SERIAL_PORT, BAUD_RATE);
	if(bad_init(c)) {
		printf("Can't connect to serial device\n");
		shutdown(profiles);
		return INIT_ERROR;
	}

	if(argc >= 2 && strcmp(argv[1], "init") == 0) {
		check_arduino(serial_write_profile(c, profiles->list[profiles->current]));
		shutdown(profiles);
		return 0;
	}

	srand(time(NULL));

	Profile profile;
	char* line = NULL;
	size_t line_size = 0;
	int error = 0, save = 0, n = 0;
	while(1) {
		if(save > 3) {
			save_profiles(profiles);
			save = 0;
		}

		profile = profiles->list[profiles->current];
		printf("Currently using profile %u out of %u: \nR - %hhu, G - %hhu, B - %hhu\n\n", profiles->current+1, profiles->size, profile.r, profile.g, profile.b);
		printf("--> ");
		n = getline(&line, &line_size, stdin);
		if(n < 0) {
			free(line);
			shutdown(profiles);
			return GET_LINE_ERROR;
		}
		line[n-1] = 0;
		
		if(strcmp(line, "quit") == 0 || strcmp(line, "exit") == 0) {
			break;
		} else if(strcmp(line, "add") == 0) {
			error = add_profile(profiles);
		} else if(strcmp(line, "switch") == 0) {
			switch_profile(profiles);
		} else if(strcmp(line, "edit") == 0) {
			edit_profile(&profiles->list[profiles->current]);
		} else {
			help();
		}

		if(error) {
			free(line);
			shutdown(profiles);
			return error;
		}

		check_arduino(serial_write_profile(c, profiles->list[profiles->current]));
		printf("\n");
	}

	free(line);
	shutdown(profiles);
	return 0;
}
CC=gcc
CFLAGS=-g  

all: control-rgb
	
control-rgb: control-rgb.o arduino-serial-lib.o
	$(CC) -o $@ $^
	rm $^

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -rf *.o control-rgb

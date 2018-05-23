CC = gcc
CFLAGS = -std=gnu99  

ifeq "$(OS)" "Windows_NT"
	EXE_SUFFIX = .exe
	LIB_SUFFIX = windows
else
	UNAME_S := $(shell uname -s)
	ifeq "$(UNAME_S)" "Linux"
	EXE_SUFFIX = 
	LIB_SUFFIX = unix
	endif
endif


all: control-rgb
	
control-rgb: control-rgb.o lib-$(LIB_SUFFIX).o
	$(CC) -o $@$(EXE_SUFFIX) $^
	rm $^

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -rf *.o control-rgb

CC = gcc
CFLAGS = -std=gnu99  

ifeq "$(OS)" "Windows_NT"
	EXE_SUFFIX = .exe
	LIB_SUFFIX = windows
	RM = del
	RM_ARGS = 
else
	UNAME_S := $(shell uname -s)
	ifeq "$(UNAME_S)" "Linux"
	EXE_SUFFIX = 
	LIB_SUFFIX = unix
	RM = rm
	RM_ARGS = -rf
	endif
endif


all: control-rgb control-gc rmo
	
control-rgb: control-rgb.o lib-$(LIB_SUFFIX).o
	$(CC) -o $@$(EXE_SUFFIX) $^

control-gc: control-gc.o lib-$(LIB_SUFFIX).o
	$(CC) -o $@$(EXE_SUFFIX) $^

rmo:
	$(RM) $(RM_ARGS) *.o

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	$(RM) $(RM_ARGS) *.o control-rgb  *.exe

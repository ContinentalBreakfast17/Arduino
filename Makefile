CC = gcc
CFLAGS = -std=gnu99  

ifeq "$(OS)" "Windows_NT"
	EXE_SUFFIX = .exe
	LIB_SUFFIX = windows
	RM = del
	RM_ARGS = 
	GEN_RES_GC = windres icon.rc -O coff -o icon.res
	RES_GC = icon.res
else
	UNAME_S := $(shell uname -s)
	ifeq "$(UNAME_S)" "Linux"
	EXE_SUFFIX = 
	LIB_SUFFIX = unix
	RM = rm
	RM_ARGS = -rf
	GEN_RES_GC = 
	RES_GC = 
	endif
endif


all: control-rgb control-gc rmo
	
control-rgb: control-rgb.o lib-$(LIB_SUFFIX).o
	$(CC) -o $@$(EXE_SUFFIX) $^

control-gc: control-gc.o lib-$(LIB_SUFFIX).o
	$(GEN_RES_GC)
	$(CC) -o $@$(EXE_SUFFIX) $^ $(RES_GC)

rmo:
	$(RM) $(RM_ARGS) *.o

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	$(RM) $(RM_ARGS) *.o control-rgb  *.exe

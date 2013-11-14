CC=g++

FOLDER=$(shell pwd)
OBJECTS=joystick.o
LIB=libjoystick++.so
CPPFLAGS=-fPIC

all: library

clean:
	@echo $(FOLDER)	
	@echo "Cleaning up directory."
	rm -f *.o *.so *~

# Applications:
library: $(OBJECTS)
	$(CC) $(CPPFLAGS) -shared -Wl,-soname,$(LIB) -o $(LIB) $(OBJECTS)

install:
	cp $(LIB) /usr/lib
	cp joystick.h /usr/


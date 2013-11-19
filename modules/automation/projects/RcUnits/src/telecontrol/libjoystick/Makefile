# URL for libjoystick: http://libjoystick.sourceforge.net/
CC=g++

OBJECTS=joystick.o
LIB=libjoystick++.so
CPPFLAGS=-fPIC

all: library

clean:
	@echo "Cleaning up build directory."
	rm -rf build

# Applications:
library: $(OBJECTS)
	$(CC) $(CPPFLAGS) -shared -Wl,-soname,$(LIB) -o $(LIB) $(OBJECTS)

install:
	mkdir -p build build/lib build/include
	cp $(LIB) build/lib
	cp joystick.h build/include


CC = g++

# Pre-additions for platform specific libraries
ifeq ($(OS),Windows_NT)
	LIBS = -lmingw32 -lSDL2main
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		# Linux params
	else
		# MAC params
	endif
endif

CFLAGS = -c -std=c++11 -Wall
INCLUDES = -Iinclude
# Extra libraries: -lSDL2_ttf (fonts)
LIBS += -lSDL2 -lSDL2_image -lSDL2_mixer
# WINDOWS only?! Helps the cmd not appear when running the app separately.
# This stops all output to the command line though...
# -Wl,-subsystem,windows
LNFLAGS=-Wl,-O1
EXECUTABLE=bin/Univursa

# Post-additions for platform specific libraries
ifeq ($(OS),Windows_NT)
	# Windows params
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		# Linux params
	else
		# MAC params
	endif
endif

SOURCES=src/Helpers.cc \
        src/Main.cc \
        src/Application.cc \
        src/Game/Frame.cc \
        src/Game/Sprite.cc

OBJECTS=$(SOURCES:.cc=.o)


all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LNFLAGS) -o $@ $(OBJECTS) $(LIBS)

.cc.o:
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@

.PHONY: clean

clean:
	$(RM) $(OBJECTS) $(EXECUTABLE)*

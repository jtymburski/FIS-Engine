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

CFLAGS = -c -std=c++11 -Wall -Wextra
INCLUDES = -Iinclude

LIBS += -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf
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

SOURCES=src/Application.cc \
        src/Frame.cc \
        src/Game/Game.cc \
        src/Game/Player/Action.cc \
        src/Game/Player/Skill.cc \
        src/Helpers.cc \
        src/Main.cc \
        src/Music.cc \
        src/Options.cc \
        src/Sound.cc \
        src/Sprite.cc \
        src/Text.cc \
        src/TinyXML.cc \
        src/TitleScreen.cc

OBJECTS=$(SOURCES:.cc=.o)


all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LNFLAGS) -o $@ $(OBJECTS) $(LIBS)

.cc.o:
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@

.PHONY: clean

clean:
	$(RM) $(OBJECTS) $(EXECUTABLE)*

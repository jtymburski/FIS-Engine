CC := g++

# TODO: Test in windows. -p not supported, builds directory trees by default
MKDIR_P := mkdir -p

# Pre-additions for platform specific libraries
ifeq ($(OS),Windows_NT)
	# Windows parameters
	LIBS := -lmingw32 -lSDL2main
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		# Linux parameters
	else
		# MAC parameters
	endif
endif

# Add -g for additional debugging options in 'gdb'
CFLAGS := -c -std=c++1y -Wextra -Wno-unused-variable -Wno-narrowing

# Post-additions for platform specific libraries
ifeq ($(OS),Windows_NT)
	UNAME_S := Windows

	# Windows parameters
	LIBS += -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf
	LNFLAGS := -Wl,-O1
else
	UNAME_S := $(shell uname -s)

	ifeq ($(UNAME_S),Linux)
		# Linux parameters
		LIBS += -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf
		LNFLAGS := -Wl,-O1
	else
		# MAC parameters
		LIBS += -framework SDL2 -framework SDL2_image -framework SDL2_mixer -framework SDL2_ttf -framework Cocoa
	endif
endif

INCLUDES := -Iinclude

BUILD_DIR := bin
EXEC_GENERIC := $(BUILD_DIR)/FISE
EXEC_OS := $(EXEC_GENERIC)-$(UNAME_S)

SRC_DIR := src
SRC_FILE_DIRS := $(SRC_DIR) \
                 $(SRC_DIR)/Game \
                 $(SRC_DIR)/Game/Battle \
                 $(SRC_DIR)/Game/Map \
                 $(SRC_DIR)/Game/Player \
                 $(SRC_DIR)/GFX
SOURCES := $(foreach dir,$(SRC_FILE_DIRS),$(wildcard $(dir)/*.cc))

OBJ_DIR := $(BUILD_DIR)/obj
OBJECTS := $(patsubst $(SRC_DIR)/%.cc,$(OBJ_DIR)/%.o,$(SOURCES))

all: $(EXEC_OS)

$(EXEC_OS): $(OBJECTS)
	$(CC) $(LNFLAGS) -g -o $@ $(OBJECTS) $(LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc
	@$(MKDIR_P) $(@D)
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@

.PHONY: clean

clean:
	$(RM) $(OBJECTS) $(EXEC_GENERIC)*

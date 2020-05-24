CC := g++

# TODO: Port for windows
CP_RF := cp -rf
MKDIR_P := mkdir -p
RM_RF := rm -rf

# Pre-additions for platform specific libraries
ifeq ($(OS),Windows_NT)
	# Windows parameters
	LIBS := -lmingw32 -lSDL2main
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		# Linux parameters
	else
		# OSX parameters
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

	# *nix parameters
	LIBS += -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf

	ifeq ($(UNAME_S),Linux)
		# Linux parameters
		LNFLAGS := -Wl,-O1
	else
		# OSX parameters
	endif
endif

BUILD_DIR := bin
EXEC_GENERIC := $(BUILD_DIR)/FISE
EXEC_OS := $(EXEC_GENERIC)-$(UNAME_S)

ASSETS_DIR := assets

INCLUDES := -Iinclude

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
	@$(CP_RF) $(ASSETS_DIR) $(BUILD_DIR)
	$(CC) $(LNFLAGS) -g -o $@ $(OBJECTS) $(LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc
	@$(MKDIR_P) $(@D)
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@

.PHONY: clean

clean:
	$(RM) $(OBJECTS) $(EXEC_GENERIC)*

deepclean: clean
	$(RM_RF) $(BUILD_DIR)

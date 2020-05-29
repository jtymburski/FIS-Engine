CC := g++

CP_RF := cp -rf
MKDIR_P := mkdir -p
RM_RF := rm -rf

# Add -g for additional debugging options in 'gdb'
CFLAGS := -c -std=c++1y `sdl2-config --cflags`
CFLAGS_LIB := $(CFLAGS) -w
CFLAGS_SRC := $(CFLAGS) -Wextra -Wno-unused-variable -Wno-narrowing

# External Library Configuration
EXT_LIBS := -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf
ifeq ($(OS),Windows_NT)
	UNAME_S := Windows

	# Windows parameters
	EXT_LIBS += -lmingw32 -lSDL2main
	LNFLAGS := -Wl,-O1
else
	UNAME_S := $(shell uname -s)

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

INCLUDES := -Iinclude \
            -Ilib/sdl2_gfx \
            -Ilib/tinyxml2

LIB_DIR := lib
LIB_C_SOURCES := $(LIB_DIR)/sdl2_gfx/SDL2_gfxPrimitives.c \
                 $(LIB_DIR)/sdl2_gfx/SDL2_rotozoom.c
LIB_CPP_SOURCES := $(LIB_DIR)/tinyxml2/tinyxml2.cpp

SRC_DIR := src
SRC_FILE_DIRS := $(SRC_DIR) \
                 $(SRC_DIR)/Game \
                 $(SRC_DIR)/Game/Battle \
                 $(SRC_DIR)/Game/Map \
                 $(SRC_DIR)/Game/Player \
                 $(SRC_DIR)/GFX
SOURCES := $(foreach dir,$(SRC_FILE_DIRS),$(wildcard $(dir)/*.cc))

OBJ_DIR := $(BUILD_DIR)/obj
OBJECTS := $(patsubst %.cc,$(OBJ_DIR)/%.o,$(SOURCES)) \
           $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(LIB_CPP_SOURCES)) \
           $(patsubst %.c,$(OBJ_DIR)/%.o,$(LIB_C_SOURCES))

all: $(EXEC_OS)

$(EXEC_OS): $(OBJECTS)
	@$(CP_RF) $(ASSETS_DIR) $(BUILD_DIR)
	$(CC) $(LNFLAGS) -g -o $@ $(OBJECTS) $(EXT_LIBS)

$(OBJ_DIR)/$(LIB_DIR)/%.o: $(LIB_DIR)/%.c*
	@$(MKDIR_P) $(@D)
	$(CC) $(CFLAGS_LIB) $(INCLUDES) $< -o $@

$(OBJ_DIR)/$(SRC_DIR)/%.o: $(SRC_DIR)/%.c*
	@$(MKDIR_P) $(@D)
	$(CC) $(CFLAGS_SRC) $(INCLUDES) $< -o $@

.PHONY: clean

clean:
	$(RM) $(OBJECTS) $(EXEC_GENERIC)*

deepclean: clean
	$(RM_RF) $(BUILD_DIR)

CP_RF := cp -rf
MKDIR_P := mkdir -p
RM_RF := rm -rf

# Architecture Specific Configuration
ifeq ($(ARCH),windows)
  # Windows Config
  SDL2_CONFIG := /usr/x86_64-w64-mingw32/bin/sdl2-config

  CC := x86_64-w64-mingw32-g++
  CFLAGS_ARCH := `$(SDL2_CONFIG) --cflags`
  EXT_LIBS_ARCH := -static-libgcc -static-libstdc++ `$(SDL2_CONFIG) --libs`
  LNFLAGS := -Wl,-O1
else
  # *nix Config
  SDL2_CONFIG := sdl2-config

  CC := g++
  CFLAGS_ARCH := `$(SDL2_CONFIG) --cflags`
  EXT_LIBS_ARCH := `$(SDL2_CONFIG) --libs`

  ifeq ($(ARCH),linux)
    # Linux Config
    LNFLAGS := -Wl,-O1
  else
    # OSX Config
    ARCH := osx
  endif
endif

# Add -g for additional debugging options in 'gdb'
CFLAGS := -c -std=c++1y $(CFLAGS_ARCH)
CFLAGS_LIB := $(CFLAGS) -w
CFLAGS_SRC := $(CFLAGS) -Wextra -Wno-unused-variable -Wno-narrowing

EXT_LIBS := $(EXT_LIBS_ARCH) -lSDL2_image -lSDL2_mixer -lSDL2_ttf

BUILD_DIR := bin
EXEC_GENERIC := $(BUILD_DIR)/FISE
EXEC_OS := $(EXEC_GENERIC)-$(ARCH)

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

OBJ_DIR := $(BUILD_DIR)/obj/$(ARCH)
OBJECTS := $(patsubst %.cc,$(OBJ_DIR)/%.o,$(SOURCES)) \
           $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(LIB_CPP_SOURCES)) \
           $(patsubst %.c,$(OBJ_DIR)/%.o,$(LIB_C_SOURCES))

# Phony targets

all: linux windows

.PHONY: all clean cleansingle deepclean executable linux osx windows

clean:
	@$(MAKE) cleansingle ARCH=linux
	@$(MAKE) cleansingle ARCH=osx
	@$(MAKE) cleansingle ARCH=windows

cleansingle:
	$(RM) $(OBJECTS) $(EXEC_OS)*

deepclean: clean
	$(RM_RF) $(BUILD_DIR)

executable: $(EXEC_OS)

linux:
	@$(MAKE) executable ARCH=linux

osx:
	@$(MAKE) executable ARCH=osx

windows:
	@$(MAKE) executable ARCH=windows

# File targets

$(EXEC_OS): $(OBJECTS)
	@$(CP_RF) $(ASSETS_DIR) $(BUILD_DIR)
	$(CC) $(LNFLAGS) -g -o $@ $(OBJECTS) $(EXT_LIBS)

$(OBJ_DIR)/$(LIB_DIR)/%.o: $(LIB_DIR)/%.c*
	@$(MKDIR_P) $(@D)
	$(CC) $(CFLAGS_LIB) $(INCLUDES) $< -o $@

$(OBJ_DIR)/$(SRC_DIR)/%.o: $(SRC_DIR)/%.c*
	@$(MKDIR_P) $(@D)
	$(CC) $(CFLAGS_SRC) $(INCLUDES) $< -o $@

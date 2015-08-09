CC = g++

# Pre-additions for platform specific libraries
ifeq ($(OS),Windows_NT)
	# Windows parameters
	LIBS = -lmingw32 -lSDL2main
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		# Linux parameters
	else
		# MAC parameters
	endif
endif

CFLAGS = -c -std=c++1y -Wall -Wextra
INCLUDES = -Iinclude

# WINDOWS only?! Helps the cmd not appear when running the app separately.
# This stops all output to the command line though...
# -Wl,-subsystem,windows
#LNFLAGS=-Wl,-O1
EXECUTABLE=Univursa

# Post-additions for platform specific libraries
ifeq ($(OS),Windows_NT)
	# Windows parameters
	LIBS += -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf
	LNFLAGS=-Wl,-O1
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		# Linux parameters
		LIBS += -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf
		LNFLAGS=-Wl,-O1
	else
		# MAC parameters
		LIBS += -framework SDL2 -framework SDL2_image -framework SDL2_mixer -framework SDL2_ttf -framework Cocoa
	endif
endif

SOURCES=src/Application.cc \
        src/FileHandler.cc \
        src/Frame.cc \
        src/Game/EventHandler.cc \
        src/Game/KeyHandler.cc \
        src/Game/Game.cc \
        src/Game/Battle/AIModule.cc \
        src/Game/Battle/AIModuleTester.cc \
        src/Game/Battle/Battle.cc \
        src/Game/Battle/BattleDisplay.cc \
        src/Game/Battle/BattleMenu.cc \
        src/Game/Battle/Buffer.cc \
        src/Game/Battle/EventBuffer.cc \
        src/Game/Battle/RenderElement.cc \
        src/Game/Map/ItemStore.cc \
        src/Game/Map/Map.cc \
        src/Game/Map/MapDialog.cc \
        src/Game/Map/MapInteractiveObject.cc \
        src/Game/Map/MapItem.cc \
        src/Game/Map/MapNPC.cc \
        src/Game/Map/MapPerson.cc \
        src/Game/Map/MapState.cc \
        src/Game/Map/MapThing.cc \
        src/Game/Map/MapViewport.cc \
        src/Game/Map/SpriteMatrix.cc \
        src/Game/Map/Tile.cc \
        src/Game/Map/TileSprite.cc \
        src/Game/Player/Action.cc \
        src/Game/Player/Ailment.cc \
        src/Game/Player/AttributeSet.cc \
        src/Game/Player/Bubby.cc \
        src/Game/Player/Category.cc \
        src/Game/Player/Equipment.cc \
        src/Game/Player/Flavour.cc \
        src/Game/Player/Inventory.cc \
        src/Game/Player/Item.cc \
        src/Game/Player/Party.cc \
        src/Game/Player/Person.cc \
        src/Game/Player/Player.cc \
        src/Game/Player/Signature.cc \
        src/Game/Player/Skill.cc \
        src/Game/Player/SkillSet.cc \
        src/Helpers.cc \
        src/Main.cc \
        src/Md5.cc \
        src/Music.cc \
        src/Options.cc \
        src/Sound.cc \
        src/Sprite.cc \
				src/TestBattle.cc \
        src/Text.cc \
        src/TinyXML.cc \
        src/TitleScreen.cc \
        src/XmlData.cc

OBJECTS=$(SOURCES:.cc=.o)


all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LNFLAGS) -g -o $@ $(OBJECTS) $(LIBS)

.cc.o:
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@

.PHONY: clean

clean:
	$(RM) $(OBJECTS) $(EXECUTABLE) $(EXECUTABLE).exe

#################################################
# Made by Jordan. Do not edit. Only for testing
# Date: December 10, 2012
#################################################

TARGET = Univursa
TEMPLATE = app

CONFIG += qt
DESTDIR = ./bin
win32 {
  LIBS += -lmingw32 -l SDLmain
}
LIBS += -lSDL -lSDL_mixer
QT += phonon # network xml, core and gui are default
RESOURCES = Resources.qrc

SOURCES += \
    ./src/Main.cc \
    ./src/MainWindow.cc \
    ./src/Application.cc \
    ./src/Options.cc \
    ./src/Sound.cc \
    ./src/TitleScreen.cc \
    ./src/Game/Game.cc \
    ./src/Game/Frame.cc \
    ./src/Game/Sprite.cc \
    ./src/Game/VictoryScreen.cc \
    ./src/Game/Weather.cc \
    ./src/Game/Battle/Battle.cc \
    ./src/Game/Battle/BattleInfoBar.cc \
    ./src/Game/Battle/BattleMenu.cc \
    ./src/Game/Battle/BattleStatusBar.cc \
    ./src/Game/Battle/PersonStatusBar.cc \
    ./src/Game/Map/Map.cc \
    ./src/Game/Map/MapBubby.cc \
    ./src/Game/Map/MapEquipment.cc \
    ./src/Game/Map/MapFoliage.cc \
    ./src/Game/Map/MapInteractiveObject.cc \
    ./src/Game/Map/MapItem.cc \
    ./src/Game/Map/MapMenu.cc \
    ./src/Game/Map/MapNPC.cc \
    ./src/Game/Map/MapPerson.cc \
    ./src/Game/Map/MapStatusBar.cc \
    ./src/Game/Map/MapThing.cc \
    ./src/Game/Map/MapWalkOver.cc \
    ./src/Game/Map/Sector.cc \
    ./src/Game/Map/SectorAction.cc \
    ./src/Game/Map/Tile.cc \
    ./src/Game/Player/Player.cc \
    ./src/Game/Player/Action.cc \
    ./src/Game/Player/Bubby.cc \
    ./src/Game/Player/Category.cc \
    ./src/Game/Player/Equipment.cc \
    ./src/Game/Player/Inventory.cc \
    ./src/Game/Player/Item.cc \
    ./src/Game/Player/Party.cc \
    ./src/Game/Player/Person.cc \
    ./src/Game/Player/Race.cc

HEADERS += \
    ./include/Application.h \
    ./include/MainWindow.h \
    ./include/EnumDatabase.h \
    ./include/Options.h \
    ./include/Sound.h \
    ./include/TitleScreen.h \
    ./include/Game/Game.h \
    ./include/Game/Frame.h \
    ./include/Game/Sprite.h \
    ./include/Game/VictoryScreen.h \
    ./include/Game/Weather.h \
    ./include/Game/Battle/Battle.h \
    ./include/Game/Battle/BattleInfoBar.h \
    ./include/Game/Battle/BattleMenu.h \
    ./include/Game/Battle/BattleStatusBar.h \
    ./include/Game/Battle/PersonStatusBar.h \
    ./include/Game/Map/Map.h \
    ./include/Game/Map/MapBubby.h \
    ./include/Game/Map/MapEquipment.h \
    ./include/Game/Map/MapFoliage.h \
    ./include/Game/Map/MapInteractiveObject.h \
    ./include/Game/Map/MapItem.h \
    ./include/Game/Map/MapMenu.h \
    ./include/Game/Map/MapNPC.h \
    ./include/Game/Map/MapPerson.h \
    ./include/Game/Map/MapStatusBar.h \
    ./include/Game/Map/MapThing.h \
    ./include/Game/Map/MapWalkOver.h \
    ./include/Game/Map/Sector.h \
    ./include/Game/Map/SectorAction.h \
    ./include/Game/Map/Tile.h \
    ./include/Game/Player/Player.h \
    ./include/Game/Player/Action.h \
    ./include/Game/Player/Bubby.h \
    ./include/Game/Player/Category.h \
    ./include/Game/Player/Equipment.h \
    ./include/Game/Player/Inventory.h \
    ./include/Game/Player/Item.h \
    ./include/Game/Player/Party.h \
    ./include/Game/Player/Person.h \
    ./include/Game/Player/Race.h

INCLUDEPATH += ./include

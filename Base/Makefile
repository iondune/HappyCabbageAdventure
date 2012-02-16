ROOT_CABBAGE_DIR=../
SOURCES=main.cpp CGameState.cpp CPlayerView.cpp Materials.cpp FreeType.cpp CGameplayManager.cpp CGameEventReceiver.cpp ./irrxml-1.2/src/irrXML.cpp CBadGuy.cpp EApple.cpp
OBJECTS=$(SOURCES:%.cpp=%.o) ../StateMainMenu/CMainMenuState.o
CABBAGE= -L../lib/ -lCabbageSound -lCabbageCollider -lCabbageFramework -lCabbageScene -lCLWIB -lOverworldState

COMPILER_VERSION=-std=c++0x
INCLUDE=-I../CabbageCore

#Chris Code.  Adding check for 32 or 64 bit
LBITS:=$(shell getconf LONG_BIT)
ifeq ($(LBITS),64)
MIXLIB=-L../lib/x64 -lSDL_mixer -Wl,-rpath=../lib/x64
else
MIXLIB=-L../lib/x86 -lSDL_mixer -Wl,-rpath=../lib/x86
endif

ifeq (${__MINGW__},1)
TARGET=mingw
else
TARGET=cabbage
endif

all: ${TARGET} 

cabbage: ${OBJECTS} ../lib/libCabbageCollider.a ../lib/libCabbageScene.a ../lib/libCabbageFramework.a ../lib/libCLWIB.a ../lib/libOverworldState.a
	g++ ${COMPILER_VERSION} ${OBJECTS} -DGL_GLEXT_PROTOTYPES -lGL -lGLU -lSDL `sdl-config --cflags --libs` ${CABBAGE} ${MIXLIB} `freetype-config --libs` -g -o $@

./irrxml-1.2/src/irrXML.o: ./irrxml-1.2/src/irrXML.cpp  
	g++ -I../irrxml-1.2/src/ -o $@ -c ./irrxml-1.2/src/irrXML.cpp

main.o: main.cpp
	g++ $(INCLUDE) `sdl-config --cflags` `freetype-config --cflags` -c main.cpp -DGL_GLEXT_PROTOTYPES -lGL -lGLU
CGameState.o: CGameState.cpp header.h
	g++ $(INCLUDE) `sdl-config --cflags` `freetype-config --cflags` -c CGameState.cpp -DGL_GLEXT_PROTOTYPES -lGL -lGLU
#sound.o: sound.cpp sound.h
#	g++ `sdl-config --cflags` -c sound.cpp -DGL_GLEXT_PROTOTYPES
CPlayerView.o: CPlayerView.cpp CPlayerView.h
	g++ $(INCLUDE) -c CPlayerView.cpp -DGL_GLEXT_PROTOTYPES -lGL -lGLU
CGameEventReceiver.o: CGameEventReceiver.cpp CGameEventReceiver.h
	g++ $(INCLUDE) `sdl-config --cflags` -c CGameEventReceiver.cpp -DGL_GLEXT_PROTOTYPES -lGL -lGLU
CGameplayManager.o: CGameplayManager.cpp CGameplayManager.h
	g++ $(INCLUDE) `sdl-config --cflags` -c CGameplayManager.cpp -DGL_GLEXT_PROTOTYPES -lGL -lGLU
Materials.o: Materials.cpp Materials.h
	g++ -c Materials.cpp
FreeType.o: FreeType.cpp FreeType.h
	g++ `freetype-config --cflags` -c FreeType.cpp
CBadGuy.o: CBadGuy.cpp CBadGuy.h
	g++ $(INCLUDE) `sdl-config --cflags` -c CBadGuy.cpp -DGL_GLEXT_PROTOTYPES -lGL -lGLU
EApple.o: EApple.cpp EApple.h CBadGuy.h
	g++ $(INCLUDE) `sdl-config --cflags` -c EApple.cpp -DGL_GLEXT_PROTOTYPES -lGL -lGLU

qclean:
	rm cabbage.exe cabbage 2>/dev/null
clean:
	rm *.o a.out cabbage cabbage.exe 2>/dev/null

mingw: cabbage.exe

cabbage.exe: ${OBJECTS} ../lib/libCabbageSound.a ../lib/libCabbageCollider.a ../lib/libCabbageScene.a ../lib/libCabbageFramework.a ../lib/libCLWIB.a ../lib/libOverworldState.a
	g++ ${COMPILER_VERSION} `sdl-config --cflags` -mwindows ${OBJECTS} -DGL_GLEXT_PROTOTYPES ${CABBAGE} ./*.dll -lSDL_mixer -lopengl32 -lglu32 -lglut32 `sdl-config --libs` `freetype-config --libs` -g -L/lib/ -o $@ -lglew32

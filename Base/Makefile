ROOT_CABBAGE_DIR=../
SOURCES=main.cpp CGameState.cpp CPlayerView.cpp CGameplayManager.cpp CGameEventReceiver.cpp ./irrxml-1.2/src/irrXML.cpp CBadGuy.cpp EApple.cpp EOrange.cpp EKiwi.cpp EFlame.cpp EBlade.cpp PKiwi.cpp CItem.cpp CIHealth.cpp CIEnergy.cpp CBiggerBlock.cpp
OBJECTS=$(SOURCES:%.cpp=%.o) ../StateMainMenu/CMainMenuState.o ../StateOverworld/COverworldState.o
CABBAGE= -L../lib/ -lCabbageSound -lCabbageCollider -lCabbageFramework -lCabbageScene -lCLWIB -lOverworldState -lCabbageParticles -lCabbageGUI
CC=g++ -O1 -s

COMPILER_VERSION=-std=c++0x
INCLUDE=-I../CabbageCore -I../CabbageScene -I../CabbageFramework -I../CabbageCollider -I../CabbageSound -I../CabbageParticles -I../StateLWIB -I../CabbageGUI

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

cabbage: ${OBJECTS} ../lib/libCabbageCollider.a ../lib/libCabbageScene.a ../lib/libCabbageFramework.a ../lib/libCLWIB.a ../lib/libOverworldState.a ../lib/libCabbageParticles.a ../lib/libCabbageGUI.a
	$(CC) ${COMPILER_VERSION} ${OBJECTS} -DGL_GLEXT_PROTOTYPES -lGL -lGLU -lSDL `sdl-config --cflags --libs` ${CABBAGE} ${MIXLIB} `freetype-config --libs` -o $@

./irrxml-1.2/src/irrXML.o: ./irrxml-1.2/src/irrXML.cpp  
	$(CC) -I../irrxml-1.2/src/ -o $@ -c ./irrxml-1.2/src/irrXML.cpp

main.o: main.cpp
	$(CC) $(INCLUDE) `sdl-config --cflags` `freetype-config --cflags` -c main.cpp -DGL_GLEXT_PROTOTYPES -lGL -lGLU
CGameState.o: CGameState.cpp header.h draw.h CPlayerView.h
	$(CC) $(INCLUDE) `sdl-config --cflags` `freetype-config --cflags` -c CGameState.cpp -DGL_GLEXT_PROTOTYPES -lGL -lGLU
#sound.o: sound.cpp sound.h
#	$(CC) `sdl-config --cflags` -c sound.cpp -DGL_GLEXT_PROTOTYPES
CPlayerView.o: CPlayerView.cpp CPlayerView.h
	$(CC) $(INCLUDE) -c CPlayerView.cpp -DGL_GLEXT_PROTOTYPES -lGL -lGLU
CGameEventReceiver.o: CGameEventReceiver.cpp CGameEventReceiver.h
	$(CC) $(INCLUDE) `sdl-config --cflags` -c CGameEventReceiver.cpp -DGL_GLEXT_PROTOTYPES -lGL -lGLU
CGameplayManager.o: CGameplayManager.cpp CGameplayManager.h
	$(CC) $(INCLUDE) `sdl-config --cflags` -c CGameplayManager.cpp -DGL_GLEXT_PROTOTYPES -lGL -lGLU
CBadGuy.o: CBadGuy.cpp CBadGuy.h
	$(CC) $(INCLUDE) `sdl-config --cflags` -c CBadGuy.cpp -DGL_GLEXT_PROTOTYPES -lGL -lGLU
EApple.o: EApple.cpp EApple.h CBadGuy.h
	$(CC) $(INCLUDE) `sdl-config --cflags` -c EApple.cpp -DGL_GLEXT_PROTOTYPES -lGL -lGLU
EOrange.o: EOrange.cpp EOrange.h CBadGuy.h
	$(CC) $(INCLUDE) `sdl-config --cflags` -c EOrange.cpp -DGL_GLEXT_PROTOTYPES -lGL -lGLU
EKiwi.o: EKiwi.cpp EKiwi.h CBadGuy.h
	$(CC) $(INCLUDE) `sdl-config --cflags` -c EKiwi.cpp -DGL_GLEXT_PROTOTYPES -lGL -lGLU
EFlame.o: EFlame.cpp EFlame.h CBadGuy.h
	$(CC) $(INCLUDE) `sdl-config --cflags` -c EFlame.cpp -DGL_GLEXT_PROTOTYPES -lGL -lGLU
EBlade.o: EBlade.cpp EBlade.h CBadGuy.h
	$(CC) $(INCLUDE) `sdl-config --cflags` -c EBlade.cpp -DGL_GLEXT_PROTOTYPES -lGL -lGLU
PKiwi.o: PKiwi.cpp PKiwi.h CBadGuy.h
	$(CC) $(INCLUDE) `sdl-config --cflags` -c PKiwi.cpp -DGL_GLEXT_PROTOTYPES -lGL -lGLU
CIHealth.o: CIHealth.cpp CIHealth.h CItem.h
	$(CC) $(INCLUDE) `sdl-config --cflags` -c CIHealth.cpp -DGL_GLEXT_PROTOTYPES -lGL -lGLU
CIEnergy.o: CIEnergy.cpp CIEnergy.h CItem.h
	$(CC) $(INCLUDE) `sdl-config --cflags` -c CIEnergy.cpp -DGL_GLEXT_PROTOTYPES -lGL -lGLU
CItem.o: CItem.cpp CItem.h
	$(CC) $(INCLUDE) `sdl-config --cflags` -c CItem.cpp -DGL_GLEXT_PROTOTYPES -lGL -lGLU

CBiggerBlock.o: CBiggerBlock.cpp CBiggerBlock.h
	$(CC) $(INCLUDE) `sdl-config --cflags` -c CBiggerBlock.cpp -DGL_GLEXT_PROTOTYPES -lGL -lGLU

qclean:
	rm cabbage.exe cabbage 2>/dev/null
clean:
	rm *.o a.out cabbage cabbage.exe 2>/dev/null

mingw: cabbage.exe

cabbage.exe: ${OBJECTS} ../lib/libCabbageSound.a ../lib/libCabbageCollider.a ../lib/libCabbageScene.a ../lib/libCabbageFramework.a ../lib/libCLWIB.a ../lib/libOverworldState.a ../lib/libCabbageParticles.a ../lib/libCabbageGUI.a
	$(CC) ${COMPILER_VERSION} `sdl-config --cflags` ${OBJECTS} -DGL_GLEXT_PROTOTYPES ${CABBAGE} ./*.dll -lSDL_mixer -lopengl32 -lglu32 -lglut32 `sdl-config --libs` `freetype-config --libs` -L/lib/ -o $@ -lglew32

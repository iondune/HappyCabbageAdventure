ROOT_CABBAGE_DIR=../
SOURCES=main.cpp
OBJECTS=$(SOURCES:%.cpp=%.o)
CABBAGE= -L../lib/ ../StateMainMenu/CMainMenuState.o -lOverworldState -lCabbageFramework -lStateGame -lCLWIB -lCabbageGame -lCabbageGUI -lCabbageParticles -lCabbageScene -lCabbageCollider
ELEMENTSHEADER=$(shell find ../CabbageGame/Elements -type d | awk 'BEGIN{}{print "-I./"$$1}')
INCLUDE = -I../CabbageCore -I../CabbageScene -I../CabbageFramework -I../CabbageCollider -I../CabbageSound -I../CabbageParticles -I../StateLWIB -I../CabbageGUI -I../CabbageGame -I ../CabbageGame/wmlwriter_src/ $(ELEMENTSHEADER) -I../StateLWIB/ `sdl-config --cflags` -I../StateGame -I../StateMainMenu
#INCLUDE = -I../CabbageCore -I../CabbageScene -I../CabbageFramework -I../CabbageCollider -I../CabbageSound -I../CabbageParticles -I../StateLWIB -I../CabbageGUI `sdl-config --cflags` `freetype-config --cflags` -I../irrxml-1.2/src/ -I./Elements/ -I. -I../CabbageGame -I../StateGame -I../CabbageGame/Elements -I../CabbageGame/Elements/Blocks -I../StateMainMenu
LIBFILES=../lib/libCabbageParticles.a ../lib/libCabbageCollider.a ../lib/libCabbageScene.a ../lib/libCabbageFramework.a ../lib/libCabbageGame.a ../lib/libStateGame.a ../lib/libCabbageGUI.a ../lib/libCLWIB.a ../lib/libOverworldState.a
CC=g++ -O1 -s
COMPILER_VERSION= -std=c++0x 

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

ifeq (${__CYGWIN__},1)
TARGET=cygwin
else
endif


all: ${TARGET} 

.cpp.o:
	$(CC) -c $(INCLUDE) -DGL_GLEXT_PROTOTYPES $< -o $@

cabbage: ${OBJECTS} ${LIBFILES} 
	$(CC) ${COMPILER_VERSION} ${OBJECTS} -lGL -lGLU -lSDL `sdl-config --libs` ${CABBAGE} ${MIXLIB} `freetype-config --libs` -o $@

qclean:
	rm -rf cabbage.exe cabbage 2>/dev/null
clean:
	rm -rf *.o a.out cabbage cabbage.exe 2>/dev/null

mingw: cabbage.exe

cabbage.exe: ${OBJECTS} ${LIBFILES}
	$(CC) ${COMPILER_VERSION} `sdl-config --cflags` ${OBJECTS} -DGL_GLEXT_PROTOTYPES ${CABBAGE} ./*.dll -lSDL_mixer -lopengl32 -lglu32 -lglut32 `sdl-config --libs` `freetype-config --libs` -L/lib/ -o $@ -lglew32



cygwin: cabbage2.exe
cabbage2.exe: ${OBJECTS} ../lib/libCabbageSound.a ../lib/libCabbageCollider.a ../lib/libCabbageScene.a ../lib/libCabbageFramework.a ../lib/libCLWIB.a ../lib/libOverworldState.a ../lib/libCabbageParticles.a ../lib/libCabbageGUI.a 
	$(CC) ${COMPILER_VERSION} ${OBJECTS} ${CABBAGE} -lSDL_mixer -lGL -lGLU `sdl-config --libs` `freetype-config --libs` -L/lib/ -o $@ -lglew32
#	$(CC) ${COMPILER_VERSION} `sdl-config --cflags` ${OBJECTS} -DGL_GLEXT_PROTOTYPES ${CABBAGE} ./*.dll -lSDL_mixer -lGL -lGLU `sdl-config --libs` -L/lib/ -o $@ -lglew32



#LIBS=-lGL -lGLU -L/lib/ -lglew32 `sdl-config --libs`

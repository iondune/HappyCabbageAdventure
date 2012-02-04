ROOT_CABBAGE_DIR=../
SOURCES=main.cpp CPlayerView.cpp Materials.cpp FreeType.cpp CGameplayManager.cpp sound.cpp
OBJECTS=$(SOURCES:%.cpp=%.o)
CABBAGE=-L../lib/ -lCabbageCollider -lCabbageFramework -lCabbageScene
COMPILER_VERSION=-std=c++0x

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

cabbage: ${OBJECTS}
	g++ ${COMPILER_VERSION} ${OBJECTS} -DGL_GLEXT_PROTOTYPES -lGL -lGLU -lSDL -lglut `sdl-config --cflags --libs` ${CABBAGE} ${MIXLIB} `freetype-config --libs` -g -o $@

main.o: main.cpp draw.h header.h
	g++ `sdl-config --cflags` `freetype-config --cflags` -c main.cpp -DGL_GLEXT_PROTOTYPES -lGL -lGLU
sound.o: sound.cpp sound.h
	g++ `sdl-config --cflags` -c sound.cpp -DGL_GLEXT_PROTOTYPES
CPlayerView.o: CPlayerView.cpp CPlayerView.h
	g++ -c CPlayerView.cpp -DGL_GLEXT_PROTOTYPES -lGL -lGLU
CGameplayManager.o: CGameplayManager.cpp CGameplayManager.h
	g++ `sdl-config --cflags` -c CGameplayManager.cpp -DGL_GLEXT_PROTOTYPES -lGL -lGLU
Materials.o: Materials.cpp Materials.h
	g++ -c Materials.cpp
FreeType.o: FreeType.cpp FreeType.h
	g++ `freetype-config --cflags` -c FreeType.cpp

qclean:
	rm cabbage.exe cabbage 2>/dev/null
clean:
	rm *.o a.out cabbage cabbage.exe 2>/dev/null

mingw: cabbage.exe

cabbage.exe: ${OBJECTS}
	g++ ${COMPILER_VERSION} `sdl-config --cflags` -mwindows ${OBJECTS} -DGL_GLEXT_PROTOTYPES ${CABBAGE} ./*.dll -lSDL_mixer -lopengl32 -lglu32 -lglut32 `sdl-config --libs` `freetype-config --libs` -g -L/lib/ -o $@ -lglew32

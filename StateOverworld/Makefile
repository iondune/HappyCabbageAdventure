ROOT_CABBAGE_DIR=../
SOURCES=COverworldState.cpp ../Base/sound.cpp
OBJECTS=$(SOURCES:%.cpp=%.o)
COMPILER_VERSION=-std=c++0x
INCLUDE=-I../CabbageCore -I../base

#Chris Code.  Adding check for 32 or 64 bit
LBITS:=$(shell getconf LONG_BIT)
ifeq ($(LBITS),64)
MIXLIB=-L../lib/x64 -lSDL_mixer -Wl,-rpath=../lib/x64
else
MIXLIB=-L../lib/x86 -lSDL_mixer -Wl,-rpath=../lib/x86
endif

TARGET=../lib/libOverworldState.a

all: ${TARGET} 

../lib/libOverworldState.a: ${OBJECTS}
	rm -f $@
	mkdir -p ../lib
	ar cq $@ $(OBJECTS)

COverworldState.o: COverworldState.cpp header.h
	g++ $(INCLUDE) `freetype-config --cflags` `sdl-config --cflags` -c COverworldState.cpp -DGL_GLEXT_PROTOTYPES -lGL -lGLU
sound.o: sound.cpp sound.h
	g++ `sdl-config --cflags` -c sound.cpp -DGL_GLEXT_PROTOTYPES

qclean:
	rm cabbage.exe cabbage 2>/dev/null
clean:
	rm *.o a.out cabbage cabbage.exe 2>/dev/null

mingw: cabbage.exe

cabbage.exe: ${OBJECTS} ../lib/libCabbageCollider.a ../lib/libCabbageScene.a ../lib/libCabbageFramework.a ../lib/libCLWIB.a
	g++ ${COMPILER_VERSION} `sdl-config --cflags` -mwindows ${OBJECTS} -DGL_GLEXT_PROTOTYPES ${CABBAGE} ./*.dll -lSDL_mixer -lopengl32 -lglu32 -lglut32 `sdl-config --libs` `freetype-config --libs` -g -L/lib/ -o $@ -lglew32

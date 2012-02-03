ROOT_CABBAGE_DIR=../
SOURCES=main.cpp CPlayerView.cpp Materials.cpp FreeType.cpp
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

all: cabbage 

cabbage: ${OBJECTS}
	g++ ${COMPILER_VERSION} ${OBJECTS} -DGL_GLEXT_PROTOTYPES -lGL -lGLU -lSDL -lglut `sdl-config --cflags --libs` ${CABBAGE} ${MIXLIB} `freetype-config --libs` -g -o $@

main.o: main.cpp draw.h header.h
	g++ `sdl-config --cflags` `freetype-config --cflags` -c main.cpp -DGL_GLEXT_PROTOTYPES -lGL -lGLU
CPlayerView.o: CPlayerView.cpp CPlayerView.h
	g++ -c CPlayerView.cpp
Materials.o: Materials.cpp Materials.h
	g++ -c Materials.cpp
FreeType.o: FreeType.cpp FreeType.h
	g++ `freetype-config --cflags` -c FreeType.cpp

clean:
	rm *.o a.out cabbage

mingw: cabbage.exe

cabbage.exe: ${OBJECTS}
	g++ `sdl-config --cflags` -mwindows ${OBJECTS} -DGL_GLEXT_PROTOTYPES -lopengl32 -lglu32 -lglut32 `sdl-config --libs` ${CABBAGE} `freetype-config --libs` -g -L/lib/ -lglew32 -o $@ 

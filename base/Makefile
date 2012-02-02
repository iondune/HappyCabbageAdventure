ROOT_CABBAGE_DIR=../
SOURCES=MainFull.cpp CPlayerView.cpp Materials.cpp 3dsloader/3dsloader.cpp FreeType.cpp
OBJECTS=$(SOURCES:%.cpp=%.o)
CABBAGE=-L../lib/ -lCabbageCollider

#Chris Code.  Adding check for 32 or 64 bit
LBITS:=$(shell getconf LONG_BIT)
ifeq ($(LBITS),64)
MIXLIB=-L../lib/x64 -lSDL_mixer -Wl,-rpath=../lib/x64
else
MIXLIB=-L../lib/x86 -lSDL_mixer -Wl,-rpath=../lib/x86
endif

all: cabbage 

cabbage: ${OBJECTS}
	g++ ${OBJECTS} -DGL_GLEXT_PROTOTYPES -lGL -lGLU -lSDL -lglut `sdl-config --cflags --libs` ${CABBAGE} ${MIXLIB} `freetype-config --libs` -g -o $@

MainFull.o: MainFull.cpp draw.h header.h
	g++ `sdl-config --cflags` `freetype-config --cflags` -c MainFull.cpp
CPlayerView.o: CPlayerView.cpp CPlayerView.h
	g++ -c CPlayerView.cpp
Materials.o: Materials.cpp Materials.h
	g++ -c Materials.cpp
3dsloader.o: 3dsloader/3dsloader.cpp 3dsloader/3dsloader.h
	g++ `sdl-config --cflags` -c 3dsloader/3dsloader.cpp
3dsloader/3dsloader.o: 3dsloader.o
	cp 3dsloader.o 3dsloader/3dsloader.o
FreeType.o: FreeType.cpp FreeType.h
	g++ `freetype-config --cflags` -c FreeType.cpp

clean:
	rm *.o a.out cabbage 3dsloader/3dsloader.o

mingw: cabbage.exe

cabbage.exe: ${OBJECTS}
	g++ `sdl-config --cflags` -mwindows ${OBJECTS} -DGL_GLEXT_PROTOTYPES -lopengl32 -lglu32 -lglut32 `sdl-config --libs` ${CABBAGE} `freetype-config --libs` -g -L/lib/ -lglew32 -o $@ 

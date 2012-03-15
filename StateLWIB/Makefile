ROOT_CABBAGE_DIR=../
SOURCES=CLWIBState.cpp Materials.cpp ./wmlwriter_src/xmlwriter.cpp CBlock.cpp CEnemy.cpp CCabbage.cpp CFlag.cpp CPItem.cpp 
OBJECTS=$(SOURCES:%.cpp=%.o)
CABBAGE=-L../lib/ -lCabbageCollider -lCabbageFramework -lCabbageScene
COMPILER_VERSION=-std=c++0x
INCLUDE = -I../CabbageCore -I../CabbageScene -I../CabbageFramework -I../CabbageCollider -I../CabbageSound -I../CabbageParticles -I../StateLWIB -I../CabbageGUI
CC=g++ -g 

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
lib: ../lib/libCLWIB.a $(OBJECTS)

../lib/libCLWIB.a: $(OBJECTS)
	rm -f $@
	mkdir -p ../lib
	ar cq $@ $(OBJECTS)

cabbage: ${OBJECTS} ../lib/libCabbageCollider.a ../lib/libCabbageScene.a ../lib/libCabbageFramework.a
	$(CC) ${COMPILER_VERSION} ${OBJECTS} -DGL_GLEXT_PROTOTYPES -lGL -lGLU -lSDL `sdl-config --cflags --libs` ${CABBAGE} ${MIXLIB} `freetype-config --libs` -g -o $@

./wmlwriter_src/xmlwriter.o: ./wmlwriter_src/xmlwriter.cpp
	$(CC) -o $@ -c ./wmlwriter_src/xmlwriter.cpp
CBlock.o: CBlock.cpp CBlock.h CPlaceable.h
	$(CC) $(INCLUDE) -DGL_GLEXT_PROTOTYPES `sdl-config --cflags` -c CBlock.cpp
CEnemy.o: CEnemy.cpp CEnemy.h CPlaceable.h
	$(CC) $(INCLUDE) -DGL_GLEXT_PROTOTYPES `sdl-config --cflags` -c CEnemy.cpp
CPItem.o: CPItem.cpp CPItem.h CPlaceable.h
	$(CC) $(INCLUDE) -DGL_GLEXT_PROTOTYPES `sdl-config --cflags` -c CPItem.cpp
CCabbage.o: CCabbage.cpp CCabbage.h CPlaceable.h
	$(CC) $(INCLUDE) -DGL_GLEXT_PROTOTYPES `sdl-config --cflags` -c CCabbage.cpp
CFlag.o: CFlag.cpp CFlag.h CPlaceable.h
	$(CC) $(INCLUDE) -DGL_GLEXT_PROTOTYPES `sdl-config --cflags` -c CFlag.cpp
main.o: main.cpp
	$(CC) $(INCLUDE) `sdl-config --cflags` `freetype-config --cflags` -c main.cpp -DGL_GLEXT_PROTOTYPES -lGL -lGLU
CLWIBState.o: CLWIBState.cpp header.h
	$(CC) $(INCLUDE) `sdl-config --cflags` `freetype-config --cflags` -c CLWIBState.cpp -DGL_GLEXT_PROTOTYPES -lGL -lGLU
Materials.o: Materials.cpp Materials.h
	$(CC) -c Materials.cpp

qclean:
	rm cabbage.exe cabbage 2>/dev/null
clean:
	rm -f $(OBJECTS)
	rm -f ../lib/libCLWIB.a

mingw: cabbage.exe

cabbage.exe: ${OBJECTS} ../lib/libCabbageCollider.a ../lib/libCabbageScene.a ../lib/libCabbageFramework.a
	$(CC) ${COMPILER_VERSION} `sdl-config --cflags` -mwindows ${OBJECTS} -DGL_GLEXT_PROTOTYPES ${CABBAGE} ./*.dll -lCabbageSound -lSDL_mixer -lopengl32 -lglu32 -lglut32 `sdl-config --libs` `freetype-config --libs` -g -L/lib/ -o $@ -lglew32

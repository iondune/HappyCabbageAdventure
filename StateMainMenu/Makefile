ROOT_CABBAGE_DIR=../
SOURCES=CMainMenuState.cpp
OBJECTS=$(SOURCES:%.cpp=%.o)
CABBAGE=-L../lib/ -lCabbageCollider -lCabbageFramework -lCabbageScene
COMPILER_VERSION=-std=c++0x
ELEMENTSHEADER=$(shell find ../CabbageGame/Elements -type d | awk 'BEGIN{}{print "-I./"$$1}')
INCLUDE = -I../CabbageCore -I../CabbageScene -I../CabbageFramework -I../CabbageCollider -I../CabbageSound -I../CabbageParticles -I../StateLWIB -I../CabbageGUI -I../CabbageGame -I ../CabbageGame/wmlwriter_src/ $(ELEMENTSHEADER) -I../StateLWIB/ -I../StateGame -I.

CC=g++ -O1 -s

all: $(OBJECTS)

CMainMenuState.o: CMainMenuState.cpp CMainMenuState.h
	$(CC) $(INCLUDE) -DGL_GLEXT_PROTOTYPES `sdl-config --cflags` -c CMainMenuState.cpp

clean:
	rm -f $(OBJECTS)

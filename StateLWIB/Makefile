ROOT_CABBAGE_DIR=../
SOURCES=CLWIBState.cpp
OBJECTS=$(SOURCES:%.cpp=%.o)
COMPILER_VERSION=-std=c++0x
ELEMENTSHEADER=$(shell find ../CabbageGame/Elements -type d | awk 'BEGIN{}{print "-I./"$$1}')
INCLUDE = -I../CabbageCore -I../CabbageScene -I../CabbageFramework -I../CabbageCollider -I../CabbageSound -I../CabbageParticles -I../StateLWIB -I../CabbageGUI -I../CabbageGame -I ../CabbageGame/wmlwriter_src/ $(ELEMENTSHEADER)
CC=g++ -O1 -s 

all: lib 
lib: ../lib/libCLWIB.a $(OBJECTS)

.cpp.o:
	$(CC) -c $(INCLUDE) -DGL_GLEXT_PROTOTYPES `sdl-config --cflags` $< -o $@

../lib/libCLWIB.a: $(OBJECTS)
	rm -f $@
	mkdir -p ../lib
	ar cq $@ $(OBJECTS)

qclean:
	rm cabbage.exe cabbage 2>/dev/null
clean:
	rm -f $(OBJECTS)
	rm -f ../lib/libCLWIB.a

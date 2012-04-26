ROOT_CABBAGE_DIR=../
SOURCES=CLWIBState.cpp Materials.cpp ./wmlwriter_src/xmlwriter.cpp CBlock.cpp CEnemy.cpp CCabbage.cpp CFlag.cpp CPItem.cpp CPFriends.cpp
OBJECTS=$(SOURCES:%.cpp=%.o)
COMPILER_VERSION=-std=c++0x
INCLUDE = -I../CabbageCore -I../CabbageScene -I../CabbageFramework -I../CabbageCollider -I../CabbageSound -I../CabbageParticles -I../StateLWIB -I../CabbageGUI
CC=g++ -g 

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

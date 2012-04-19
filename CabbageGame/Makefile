FILES = CDecorManager.cpp CGameLevel.cpp CGameplayManager.cpp CGameLevelLoader.cpp CBiggerBlock.cpp
ELEMENTS := $(wildcard ./Elements/*.cpp) $(wildcard ./Elements/Blocks/*.cpp) $(wildcard ./Elements/Enemies/*.cpp)
CC=g++ -O1 -s

OFILES = $(FILES:%.cpp=%.o) $(ELEMENTS:%.cpp=%.o)

DIRECTORIES = -I../CabbageCore -I../CabbageScene -I../CabbageFramework -I../CabbageCollider -I../CabbageSound -I../CabbageParticles -I../StateLWIB -I../CabbageGUI -I./Elements/ -I./ -I./Elements/Blocks -I./Elements/Enemies

all: ../lib/libCabbageGame.a

../lib/libCabbageGame.a: $(OFILES)
	rm -f $@
	mkdir -p ../lib
	ar cq $@ $(OFILES)

.cpp.o:
	$(CC) -c $(DIRECTORIES) -DGL_GLEXT_PROTOTYPES $< -o $@

clean:
	rm -f $(OFILES)
	rm -f ../lib/libCabbageGame.a

FILES = CDecorManager.cpp CGameLevel.cpp CGameplayManager.cpp CGameLevelLoader.cpp CBiggerBlock.cpp $(wildcard irrxml-1.2/src/*.cpp) $(wildcard wmlwriter_src/*.cpp)
ELEMENTS := $(wildcard ./Elements/*.cpp) $(wildcard ./Elements/Blocks/*.cpp) $(wildcard ./Elements/Enemies/*.cpp) $(wildcard ./Elements/Player/*.cpp)
CC=g++ -g

OFILES = $(FILES:%.cpp=%.o) $(ELEMENTS:%.cpp=%.o)

DIRECTORIES = -I../CabbageCore -I../CabbageScene -I../CabbageFramework -I../CabbageCollider -I../CabbageSound -I../CabbageParticles -I../StateLWIB -I../CabbageGUI -I./Elements/ -I./ -I./Elements/Blocks -I./Elements/Enemies -I./Elements/Player

all: ../lib/libCabbageGame.a

../lib/libCabbageGame.a: $(OFILES)
	rm -f $@
	mkdir -p ../lib
	ar cq $@ $(OFILES)

.cpp.o:
	$(CC) -c $(DIRECTORIES) -DGL_GLEXT_PROTOTYPES $< -o $@

clean:
	rm -f $(OFILES) Driver.o
	rm -f ../lib/libCabbageGame.a

LIBRARIES= -L../lib/ -lCabbageFramework -lCabbageGUI -lCabbageParticles -lCabbageScene -lStateGame -lCabbageCore -lCabbageGame -lCabbageCollider 
test-game: Driver.o ${OFILES}
	$(CC) Driver.o ${COMPILER_VERSION} ${OFILES} -lGL -lGLU -lSDL `sdl-config --libs` ${LIBRARIES} `freetype-config --libs` -o $@
test: test-game

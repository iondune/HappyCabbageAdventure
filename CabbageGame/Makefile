FILES = CDecorManager.cpp CGameLevel.cpp CGameplayManager.cpp CGameLevelLoader.cpp CBiggerBlock.cpp $(wildcard irrxml-1.2/src/*.cpp) $(wildcard wmlwriter_src/*.cpp)
ELEMENTS := $(wildcard ./Elements/*.cpp) $(wildcard ./Elements/*/*.cpp)
CC=g++ -g

OFILES = $(FILES:%.cpp=%.o) $(ELEMENTS:%.cpp=%.o)
ELEMENTSHEADER=$(shell find Elements -type d | awk 'BEGIN{}{print "-I./"$$1}')
DIRECTORIES = -I../CabbageCore -I../CabbageScene -I../CabbageFramework -I../CabbageCollider -I../CabbageSound -I../CabbageParticles -I../StateLWIB -I../CabbageGUI -I. $(ELEMENTSHEADER)
all: ../lib/libCabbageGame.a

../lib/libCabbageGame.a: $(OFILES)
	rm -f $@
	mkdir -p ../lib
	ar cq $@ $(OFILES)

print:
	echo $(OFILES:%.o=.dependencies/%.d)

-include $(OFILES:%.o=.dependencies/%.d)

directories:
	mkdir -p `find . -type d | sed -e '/sdlmixer/d' -e '/todo/d' -e '/old/d' -e 's/\.\/*//' -e '/.dependencies/d' | awk 'BEGIN {} {print ".dependencies/"$$1} END{}'`

.cpp.o: directories
	$(CC) -c $(DIRECTORIES) -DGL_GLEXT_PROTOTYPES $< -o $@
	$(CC) -MM $(DIRECTORIES) -DGL_GLEXT_PROTOTYPES $< > .dependencies/$*.d


clean:
	rm -f $(OFILES) Driver.o
	rm -f ../lib/libCabbageGame.a
	rm -rf .dependencies

LIBRARIES= -L../lib/ -lCabbageGame -lCabbageFramework -lCabbageGUI -lCabbageParticles -lCabbageScene -lStateGame -lCabbageCore -lCabbageCollider 
test-game: Driver.o ../lib/libCabbageGame.a
	$(CC) Driver.o ${COMPILER_VERSION} -lGL -lGLU -lSDL `sdl-config --libs` ${LIBRARIES} `freetype-config --libs` -o $@
test: test-game

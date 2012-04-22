FILES = CDecorManager.cpp CGameLevel.cpp CGameplayManager.cpp CGameLevelLoader.cpp CBiggerBlock.cpp $(wildcard irrxml-1.2/src/*.cpp) $(wildcard wmlwriter_src/*.cpp)
ELEMENTS := $(wildcard ./Elements/*.cpp) $(wildcard ./Elements/*/*.cpp)
CC=g++ -g

OFILES = $(FILES:%.cpp=%.o) $(ELEMENTS:%.cpp=%.o)
ELEMENTSHEADER=$(shell find Elements -type d | awk 'BEGIN{}{print "-I./"$$1}')
DIRS=$(shell find . -type d | sed -e '/sdlmixer/d' -e '/todo/d' -e '/old/d' -e 's/\.\/*//' -e '/.dependencies/d' | awk 'BEGIN {} {print ".dependencies/"$$1} END{}')
DIRECTORIES = -I../CabbageCore -I../CabbageScene -I../CabbageFramework -I../CabbageCollider -I../CabbageSound -I../CabbageParticles -I../StateLWIB -I../CabbageGUI -I. $(ELEMENTSHEADER)
all: ../lib/libCabbageGame.a

../lib/libCabbageGame.a: $(DIRS) $(OFILES)
	rm -f $@
	mkdir -p ../lib
	ar cq $@ $(OFILES)

print:
	echo $(OFILES:%.o=.dependencies/%.d)

-include $(OFILES:%.o=.dependencies/%.d)

$(DIRS):
	mkdir -p `find . -type d | sed -e '/sdlmixer/d' -e '/todo/d' -e '/old/d' -e 's/\.\/*//' -e '/.dependencies/d' | awk 'BEGIN {} {print ".dependencies/"$$1} END{}'`

.dependencies/%.d: %.cpp $(DIRS)
	$(CC) -MM $(DIRECTORIES) -DGL_GLEXT_PROTOTYPES $< > $@

.cpp.o:
	$(CC) -c $(DIRECTORIES) -DGL_GLEXT_PROTOTYPES $< -o $@


clean:
	rm -f $(OFILES) Driver.o
	rm -f ../lib/libCabbageGame.a
	rm -rf .dependencies dirs.file

LIBRARIES= -L../lib/ -lCabbageGame -lCabbageFramework -lCabbageGUI -lCabbageParticles -lCabbageScene -lStateGame -lCabbageCore -lCabbageCollider 
test-game: Driver.o ../lib/libCabbageGame.a
	$(CC) Driver.o ${COMPILER_VERSION} -lGL -lGLU -lSDL `sdl-config --libs` ${LIBRARIES} `freetype-config --libs` -o $@
test: test-game

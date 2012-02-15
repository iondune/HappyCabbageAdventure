FILES = Sound.cpp

OFILES = $(FILES:%.cpp=%.o)

ifeq (${__MINGW__},1)
OPENGL = `sdl-config --cflags` -DGL_GLEXT_PROTOTYPES `sdl-config --libs`
else
OPENGL = -DGL_GLEXT_PROTOTYPES
endif


all: ../lib/libCabbageSound.a

Sound.o: sound.cpp *.h
	g++ -o $@ sound.cpp -c $(OPENGL)


../lib/libCabbageSound.a: $(OFILES)
	rm -f $@
	mkdir -p ../lib
	ar cq $@ $(OFILES)

clean:
	rm -f $(OFILES)
	rm -f ../lib/libCabbageSound.a
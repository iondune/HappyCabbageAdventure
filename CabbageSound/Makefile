FILES = Sound.cpp
CC=g++ -O1 -s

OFILES = $(FILES:%.cpp=%.o)

OPENGL = `sdl-config --cflags` -DGL_GLEXT_PROTOTYPES `sdl-config --libs`

all: ../lib/libCabbageSound.a

Sound.o: sound.cpp *.h
	$(CC) -o $@ sound.cpp -c $(OPENGL)

../lib/libCabbageSound.a: $(OFILES)
	rm -f $@
	mkdir -p ../lib
	ar cq $@ $(OFILES)

clean:
	rm -f $(OFILES)
	rm -f ../lib/libCabbageSound.a

FILES = CApplication.cpp CEventManager.cpp CStateManager.cpp CFadeOutState.cpp
CC=g++ -O1 -s

OFILES = $(FILES:%.cpp=%.o)

DIRECTORIES=-I../CabbageCore -I../CabbageScene -I../CabbageFramework -I../CabbageCollider -I../CabbageSound -I../CabbageParticles -I../StateLWIB -I../CabbageGUI

ifeq (${__MINGW__},1)
OPENGL = `sdl-config --cflags` -DGL_GLEXT_PROTOTYPES `sdl-config --libs`
else
OPENGL = -DGL_GLEXT_PROTOTYPES
endif


all: ../lib/libCabbageFramework.a

CApplication.o: CApplication.cpp *.h
	$(CC) -o $@ CApplication.cpp -c $(DIRECTORIES) $(OPENGL)

CFadeOutState.o: CFadeOutState.cpp *.h
	$(CC) -o $@ CFadeOutState.cpp -c $(DIRECTORIES) $(OPENGL)

CEventManager.o: CEventManager.cpp *.h
	$(CC) -o $@ CEventManager.cpp -c $(DIRECTORIES) $(OPENGL)

CStateManager.o: CStateManager.cpp *.h
	$(CC) -o $@ CStateManager.cpp -c $(DIRECTORIES) $(OPENGL)

../lib/libCabbageFramework.a: $(OFILES)
	rm -f $@
	mkdir -p ../lib
	ar cq $@ $(OFILES)

clean:
	rm -f $(OFILES)
	rm -f ../lib/libCabbageFramework.a


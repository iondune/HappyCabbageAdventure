FILES = CApplication.cpp CEventManager.cpp CStateManager.cpp CFadeOutState.cpp
CC=g++ -O1 -s

OFILES = $(FILES:%.cpp=%.o)

DIRECTORIES=-I../CabbageCore -I../CabbageScene -I../CabbageFramework -I../CabbageCollider -I../CabbageSound -I../CabbageParticles -I../StateLWIB -I../CabbageGUI

all: ../lib/libCabbageFramework.a

.cpp.o:
	$(CC) -c $(DIRECTORIES) -DGL_GLEXT_PROTOTYPES $< -o $@

../lib/libCabbageFramework.a: $(OFILES)
	rm -f $@
	mkdir -p ../lib
	ar cq $@ $(OFILES)

clean:
	rm -f $(OFILES)
	rm -f ../lib/libCabbageFramework.a


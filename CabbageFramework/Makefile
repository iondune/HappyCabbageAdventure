FILES = CActor.cpp CCollideable.cpp CObject.cpp

OFILES = $(FILES:%.cpp=%.o)

DIRECTORIES = -I../CabbageCore/:../CabbageScene/

OPENGL = -DGL_GLEXT_PROTOTYPES

all: ../lib/libCabbageFramework.a

CApplication.o: CApplication.cpp *.h
	g++ -o $@ CApplication.cpp -c $(DIRECTORIES) $(OPENGL)

CEventManager.o: CEventManager.cpp *.h
	g++ -o $@ CEventManager.cpp -c $(DIRECTORIES) $(OPENGL)

CStateManager.o: CStateManager.cpp *.h
	g++ -o $@ CStateManager.cpp -c $(DIRECTORIES) $(OPENGL)

../lib/libCabbageFramework.a: $(OFILES)
	rm -f $@
	mkdir -p ../lib
	ar cq $@ $(OFILES)

clean:
	rm -f $(OFILES)
	rm -f ../lib/libCabbageFramework.a


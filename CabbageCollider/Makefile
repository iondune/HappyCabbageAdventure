FILES = CActor.cpp CCollideable.cpp CObject.cpp

OFILES = $(FILES:%.cpp=%.o)

DIRECTORIES = -I../CabbageCore/

OPENGL = -DGL_GLEXT_PROTOTYPES

all: ../lib/libCabbageCollider.a

CActor.o: CActor.cpp *.h
	g++ -o $@ CActor.cpp -c $(DIRECTORIES) $(OPENGL)

CCollideable.o: CCollideable.cpp *.h
	g++ -o $@ CCollideable.cpp -c $(DIRECTORIES) $(OPENGL)

CObject.o: CObject.cpp *.h
	g++ -o $@ CObject.cpp -c $(DIRECTORIES) $(OPENGL)

../lib/libCabbageCollider.a: $(OFILES)
	rm -f $@
	mkdir -p ../lib
	ar cq $@ $(OFILES)

clean:
	rm -f $(OFILES)
	rm -f ../lib/libCabbageCollider.a


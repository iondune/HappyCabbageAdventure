FILES = CActor.cpp CCollideable.cpp CObject.cpp 
CC=g++ -g

OFILES = $(FILES:%.cpp=%.o)

DIRECTORIES = -I../CabbageCore/

OPENGL = -DGL_GLEXT_PROTOTYPES

all: ../lib/libCabbageCollider.a

CActor.o: CActor.cpp *.h
	$(CC) -o $@ CActor.cpp -c $(DIRECTORIES) $(OPENGL)

CCollideable.o: CCollideable.cpp *.h
	$(CC) -o $@ CCollideable.cpp -c $(DIRECTORIES) $(OPENGL)

CObject.o: CObject.cpp *.h
	$(CC) -o $@ CObject.cpp -c $(DIRECTORIES) $(OPENGL)

../lib/libCabbageCollider.a: $(OFILES)
	rm -f $@
	mkdir -p ../lib
	ar cq $@ $(OFILES)

clean:
	rm -f $(OFILES)
	rm -f ../lib/libCabbageCollider.a


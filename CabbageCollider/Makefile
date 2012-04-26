FILES = CCollisionEngine.cpp CCollisionActor.cpp CCollideable.cpp CCollisionObject.cpp 
CC=g++ -g

OFILES = $(FILES:%.cpp=%.o)

DIRECTORIES = -I../CabbageCore/

all: ../lib/libCabbageCollider.a

.cpp.o:
	$(CC) -c $(DIRECTORIES) -DGL_GLEXT_PROTOTYPES $< -o $@

../lib/libCabbageCollider.a: $(OFILES)
	rm -f $@
	mkdir -p ../lib
	ar cq $@ $(OFILES)

clean:
	rm -f $(OFILES)
	rm -f ../lib/libCabbageCollider.a


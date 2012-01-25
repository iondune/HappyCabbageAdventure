FILES = Main.cpp

DIRECTORIES = -L../lib/

OPENGL = -DGL_GLEXT_PROTOTYPES -lGL -lGLU -lSDL
CABBAGE = -lCabbageScene -lCabbageCollider

CabbageColliderDemo: $(FILES)
	g++ -o $@ $(FILES) $(DIRECTORIES) $(OPENGL) $(CABBAGE)

clean:
	rm -f CabbageColliderDemo


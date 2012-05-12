FILES = Main.cpp

DIRECTORIES = -L../lib/

OPENGL = -DGL_GLEXT_PROTOTYPES -lGL -lGLU -lSDL
CABBAGE = -lCabbageScene -lCabbageCollider -lCabbageGUI
INCLUDE = -I../CabbageScene/ -I../CabbageFramework/ -I../CabbageGUI -I../CabbageCore

CabbageColliderDemo: $(FILES)
	g++ $(INCLUDE) -o $@ $(FILES) $(DIRECTORIES) $(OPENGL) $(CABBAGE)

clean:
	rm -f CabbageColliderDemo


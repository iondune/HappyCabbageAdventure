FILES = Main.cpp

DIRECTORIES = -I../CabbageCore/:../CabbageScene/ -L../lib/

OPENGL = -DGL_GLEXT_PROTOTYPES -lGL -lGLU -lSDL
CABBAGE = -lCabbageScene

MeshLoaderDemo: $(FILES)
	g++ -o $@ $(FILES) $(DIRECTORIES) $(OPENGL) $(CABBAGE)


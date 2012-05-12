FILES = Main.cpp FreeType.cpp

DIRECTORIES = -I../CabbageCore/:../CabbageScene/ -L../lib/
INCLUDE = -I../CabbageCore -I../CabbageGUI -I../CabbageScene -I../CabbageFramework -I../CabbageCollider -I../CabbageSound -I../CabbageParticles -I../StateLWIB

OPENGL = -DGL_GLEXT_PROTOTYPES -lGL -lGLU -lSDL
CABBAGE = -lCabbageScene -lCabbageFramework -lCabbageGUI

MeshLoaderDemo: $(FILES)
	g++ -o $@ $(FILES) $(DIRECTORIES) $(CABBAGE) $(OPENGL) $(INCLUDE) `freetype-config --cflags --libs`

clean:
	rm -f MeshLoaderDemo

MeshLoaderDemo2: $(FILES)
	g++ -I../CabbageCore/ `freetype-config --cflags` `sdl-config --cflags` -mwindows -o $@ $(FILES) $(DIRECTORIES) $(CABBAGE) -DGL_GLEXT_PROTOTYPES -lopengl32 -lglu32 -lglut32 `sdl-config --libs` `freetype-config --libs` -g -L/lib/ -lglew32

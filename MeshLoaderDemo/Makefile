FILES = Main.cpp

DIRECTORIES = -I../CabbageCore/:../CabbageScene/ -L../lib/

OPENGL = -DGL_GLEXT_PROTOTYPES -lGL -lGLU -lSDL
CABBAGE = -lCabbageScene -lCabbageFramework

MeshLoaderDemo: $(FILES)
	g++ -o $@ $(FILES) $(DIRECTORIES) $(OPENGL) $(CABBAGE)

clean:
	rm -f MeshLoaderDemo

MeshLoaderDemo2: $(FILES)
	g++ `sdl-config --cflags` -mwindows -o $@ $(FILES) $(DIRECTORIES) $(CABBAGE) -DGL_GLEXT_PROTOTYPES -lopengl32 -lglu32 -lglut32 `sdl-config --libs` `freetype-config --libs` -g -L/lib/ -lglew32

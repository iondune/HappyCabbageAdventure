ROOT_CABBAGE_DIR=../

all:
	g++ -I${ROOT_CABBAGE_DIR}/CabbageCollider `freetype-config --cflags` MainFull.cpp FreeType.cpp -DGL_GLEXT_PROTOTYPES -lGL -lGLU -lSDL -lglut `freetype-config --libs` -g `find ${ROOT_CABBAGE_DIR}/CabbageCollider | grep 'Actor.cpp$$'`

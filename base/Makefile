ROOT_CABBAGE_DIR=../

all:
	g++ `freetype-config --cflags` 3dsloader/3dsloader.cpp MainFull.cpp FreeType.cpp -DGL_GLEXT_PROTOTYPES -lGL -lGLU -lSDL -lglut `sdl-config --cflags` `freetype-config --libs` -g ../CabbageCollider/CActor.cpp 

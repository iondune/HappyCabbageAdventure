ROOT_CABBAGE_DIR=../
FILES=3dsloader/3dsloader.cpp MainFull.cpp FreeType.cpp ../CabbageCollider/CActor.cpp ../CabbageCollider/CCollideable.cpp ../CabbageCollider/CObject.cpp CPlayerView.cpp Materials.cpp

all:
	g++ `freetype-config --cflags` ${FILES} -DGL_GLEXT_PROTOTYPES -lGL -lGLU -lSDL -lglut `sdl-config --cflags` `freetype-config --libs` -g 

mingw:
	g++ `freetype-config --cflags` `sdl-config --cflags` -mwindows ${FILES} -DGL_GLEXT_PROTOTYPES -lopengl32 -lglu32 -lglut32 `sdl-config --libs` `freetype-config --libs` -g -L/lib/ -lglew32

#mingw:
#g++ -std=c++0x `freetype-config --cflags` -I./mingw_glfw/include/ -Wall -O2 -mwindows ${FILES} ./mingw_glfw/lib/win32/libglfw.a -lglu32 -s `freetype-config --libs` -lopengl32 -lglut32 -lm -L/lib/ -lglew32


ROOT_CABBAGE_DIR=../
SOURCES=MainFull.cpp ../CabbageCollider/CActor.cpp ../CabbageCollider/CCollideable.cpp ../CabbageCollider/CObject.cpp CPlayerView.cpp Materials.cpp 3dsloader/3dsloader.cpp FreeType.cpp
OBJECTS=`echo "${SOURCES}" | sed "s/\.cpp/\.o/g" | sed "s/ [^ ]*\// /g"`

all: MainFull.o CActor.o CCollideable.o CObject.o CPlayerView.o Materials.o 3dsloader.o FreeType.o
	g++ ${OBJECTS} -DGL_GLEXT_PROTOTYPES -lGL -lGLU -lSDL -lglut `sdl-config --cflags` `freetype-config --libs` -g 

#For printing out ${OBJECTS} to paste them after all:
print:
	echo ${OBJECTS}

MainFull.o: MainFull.cpp draw.h header.h
	g++ `freetype-config --cflags` -c MainFull.cpp
CActor.o: ../CabbageCollider/CActor.cpp ../CabbageCollider/CActor.h
	g++ -c ../CabbageCollider/CActor.cpp
CCollideable.o: ../CabbageCollider/CCollideable.cpp ../CabbageCollider/CCollideable.h
	g++ -c ../CabbageCollider/CCollideable.cpp
CObject.o: ../CabbageCollider/CObject.cpp ../CabbageCollider/CObject.h 
	g++ -c ../CabbageCollider/CObject.cpp 
CPlayerView.o: CPlayerView.cpp CPlayerView.h
	g++ -c CPlayerView.cpp
Materials.o: Materials.cpp Materials.h
	g++ -c Materials.cpp
3dsloader.o: 3dsloader/3dsloader.cpp 3dsloader/3dsloader.h
	g++ -c 3dsloader/3dsloader.cpp
FreeType.o: FreeType.cpp FreeType.h
	g++ `freetype-config --cflags` -c FreeType.cpp

clean:
	rm *.o a.out

mingw:
	g++ `freetype-config --cflags` `sdl-config --cflags` -mwindows ${SOURCES} -DGL_GLEXT_PROTOTYPES -lopengl32 -lglu32 -lglut32 `sdl-config --libs` `freetype-config --libs` -g -L/lib/ -lglew32

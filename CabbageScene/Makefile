FILES = *.cpp

DIRECTORIES = -I../CabbageCore/ -L../lib

OPENGL = -DGL_GLEXT_PROTOTYPES -lGL -lGLU -lSDL

../lib/CabbageScene.o: FILES
	g++ -o ../lib/CabbageScene.o -c $(FILES) $(DIRECTORIES) $(OPENGL)

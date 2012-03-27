FILES = CGUIEngine.cpp CGUIImageWidget.cpp CGUIFontWidget.cpp CGUIWidget.cpp liboglft/OGLFT.cpp
CC=g++ -O1 -s

OFILES = $(FILES:%.cpp=%.o)

DIRECTORIES = -I../CabbageCore -I../CabbageScene -I../CabbageFramework -I../CabbageCollider -I../CabbageSound -I../CabbageParticles -I../StateLWIB -I../CabbageGUI `freetype-config --cflags`

ifeq (${__MINGW__},1)
OPENGL = -mwindows -lopengl32 -lglew32 `sdl-config --cflags` -lglu32 -DGL_GLEXT_PROTOTYPES `sdl-config --libs`
else
OPENGL = -DGL_GLEXT_PROTOTYPES -lGL -lGLU -lSDL
endif


all: ../lib/libCabbageGUI.a

liboglft/OGLFT.o: liboglft/OGLFT.cpp
	$(CC) -o $@ liboglft/OGLFT.cpp -c $(DIRECTORIES) $(OPENGL)
CGUIEngine.o: CGUIEngine.cpp
	$(CC) -o $@ CGUIEngine.cpp -c $(DIRECTORIES) $(OPENGL)
CGUIImageWidget.o: CGUIImageWidget.cpp
	$(CC) -o $@ CGUIImageWidget.cpp -c $(DIRECTORIES) $(OPENGL)
CGUIFontWidget.o: CGUIFontWidget.cpp
	$(CC) -o $@ CGUIFontWidget.cpp -c $(DIRECTORIES) $(OPENGL)
CGUIWidget.o: CGUIWidget.cpp
	$(CC) -o $@ CGUIWidget.cpp -c $(DIRECTORIES) $(OPENGL)


../lib/libCabbageGUI.a: $(OFILES)
	rm -f $@
	mkdir -p ../lib
	ar cq $@ $(OFILES)

clean:
	rm -f $(OFILES)
	rm -f ../lib/libCabbageGUI.a

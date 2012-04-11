FILES = CGUIEngine.cpp CGUIImageWidget.cpp CGUIFontWidget.cpp CGUIDialogWidget.cpp CGUIWidget.cpp liboglft/OGLFT.cpp
CC=g++ -O1 -s

OFILES = $(FILES:%.cpp=%.o)

DIRECTORIES = -I../CabbageCore -I../CabbageScene -I../CabbageFramework -I../CabbageCollider -I../CabbageSound -I../CabbageParticles -I../StateLWIB -I../CabbageGUI `freetype-config --cflags`

ifeq (${__MINGW__},1)
OPENGL = -mwindows -lopengl32 -lglew32 `sdl-config --cflags` -lglu32 -DGL_GLEXT_PROTOTYPES `sdl-config --libs`
else
OPENGL = -DGL_GLEXT_PROTOTYPES -lGL -lGLU -lSDL
endif


all: ../lib/libCabbageGUI.a

../lib/libCabbageGUI.a: $(OFILES)
	rm -f $@
	mkdir -p ../lib
	ar cq $@ $(OFILES)

.cpp.o:
	$(CC) -c $(DIRECTORIES) -DGL_GLEXT_PROTOTYPES $< -o $@

clean:
	rm -f $(OFILES)
	rm -f ../lib/libCabbageGUI.a

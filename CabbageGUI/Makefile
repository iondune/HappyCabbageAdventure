FILES = CGUIEngine.cpp CGUIImageWidget.cpp CGUIFontWidget.cpp CGUIDialogWidget.cpp CGUIWidget.cpp liboglft/OGLFT.cpp
CC=g++ -g

OFILES = $(FILES:%.cpp=%.o)

DIRECTORIES = -I../CabbageCore -I../CabbageScene -I../CabbageFramework -I../CabbageCollider -I../CabbageSound -I../CabbageParticles -I../StateLWIB -I../CabbageGUI `freetype-config --cflags`

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

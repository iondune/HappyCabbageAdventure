FILES = CMesh.cpp CMeshLoader.cpp CRenderable.cpp CShader.cpp CTexture.cpp CTextureLoader.cpp

OFILES = $(FILES:%.cpp=%.o)

DIRECTORIES = -I../CabbageCore/

OPENGL = -DGL_GLEXT_PROTOTYPES

all: ../lib/libCabbageScene.a

CMesh.o: CMesh.cpp
	g++ -o CMesh.o CMesh.cpp -c $(DIRECTORIES) $(OPENGL)

CMeshLoader.o: CMeshLoader.cpp
	g++ -o CMeshLoader.o CMeshLoader.cpp -c $(DIRECTORIES) $(OPENGL)

CRenderable.o: CRenderable.cpp
	g++ -o CRenderable.o CRenderable.cpp -c $(DIRECTORIES) $(OPENGL)

CShader.o: CShader.cpp
	g++ -o CShader.o CShader.cpp -c $(DIRECTORIES) $(OPENGL)

CTexture.o: CTexture.cpp
	g++ -o CTexture.o CTexture.cpp -c $(DIRECTORIES) $(OPENGL)

CTextureLoader.o: CTextureLoader.cpp
	g++ -o CTextureLoader.o CTextureLoader.cpp -c $(DIRECTORIES) $(OPENGL)

../lib/libCabbageScene.a: $(OFILES)
	rm -f $@
	mkdir -p ../lib
	ar cq $@ $(OFILES)


FILES = CMesh.cpp CMeshLoader.cpp CRenderable.cpp CShader.cpp CTexture.cpp CImageLoader.cpp CImage.cpp CShaderLoader.cpp CCamera.cpp CShaderContext.cpp

OFILES = $(FILES:%.cpp=%.o)

DIRECTORIES = -I../CabbageCore/

OPENGL = -DGL_GLEXT_PROTOTYPES

all: ../lib/libCabbageScene.a

CMesh.o: CMesh.cpp
	g++ -o CMesh.o CMesh.cpp -c $(DIRECTORIES) $(OPENGL)

CShaderContext.o: CShaderContext.cpp
	g++ -o CShaderContext.o CShaderContext.cpp -c $(DIRECTORIES) $(OPENGL)

CShaderLoader.o: CShaderLoader.cpp
	g++ -o CShaderLoader.o CShaderLoader.cpp -c $(DIRECTORIES) $(OPENGL)

CCamera.o: CCamera.cpp
	g++ -o CCamera.o CCamera.cpp -c $(DIRECTORIES) $(OPENGL)

CImage.o: CImage.cpp
	g++ -o CImage.o CImage.cpp -c $(DIRECTORIES) $(OPENGL)

CMeshLoader.o: CMeshLoader.cpp
	g++ -o CMeshLoader.o CMeshLoader.cpp -c $(DIRECTORIES) $(OPENGL)

CRenderable.o: CRenderable.cpp
	g++ -o CRenderable.o CRenderable.cpp -c $(DIRECTORIES) $(OPENGL)

CShader.o: CShader.cpp
	g++ -o CShader.o CShader.cpp -c $(DIRECTORIES) $(OPENGL)

CTexture.o: CTexture.cpp
	g++ -o CTexture.o CTexture.cpp -c $(DIRECTORIES) $(OPENGL)

CImageLoader.o: CImageLoader.cpp
	g++ -o CImageLoader.o CImageLoader.cpp -c $(DIRECTORIES) $(OPENGL)

../lib/libCabbageScene.a: $(OFILES)
	rm -f $@
	mkdir -p ../lib
	ar cq $@ $(OFILES)

clean:
	rm -f $(OFILES)
	rm -f ../lib/libCabbageScene.a


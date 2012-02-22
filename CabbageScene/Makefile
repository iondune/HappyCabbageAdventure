FILES = CMesh.cpp CMeshLoader.cpp CRenderable.cpp CShader.cpp CTexture.cpp CImageLoader.cpp CImage.cpp CShaderLoader.cpp CCamera.cpp CShaderContext.cpp CSceneManager.cpp CMeshSceneObject.cpp ISceneObject.cpp CSceneObject.cpp CMaterial.cpp CMeshSceneObject.cpp

OFILES = $(FILES:%.cpp=%.o)

DIRECTORIES = -I../CabbageCore/

ifeq (${__MINGW__},1)
OPENGL = -mwindows -lopengl32 -lglew32 `sdl-config --cflags` -lglu32 -DGL_GLEXT_PROTOTYPES `sdl-config --libs`
else
OPENGL = -DGL_GLEXT_PROTOTYPES -lGL -lGLU -lSDL
endif


all: ../lib/libCabbageScene.a

ISceneObject.o: ISceneObject.cpp
	g++ -o ISceneObject.o ISceneObject.cpp -c $(DIRECTORIES) $(OPENGL)

CMaterial.o: CMaterial.cpp
	g++ -o CMaterial.o CMaterial.cpp -c $(DIRECTORIES) $(OPENGL)
CSceneObject.o: CSceneObject.cpp
	g++ -o CSceneObject.o CSceneObject.cpp -c $(DIRECTORIES) $(OPENGL)

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

CSceneManager.o: CSceneManager.cpp
	g++ -o $@ CSceneManager.cpp -c $(DIRECTORIES) $(OPENGL)

CMeshSceneObject.o: CMeshSceneObject.cpp
	g++ -o $@ CMeshSceneObject.cpp -c $(DIRECTORIES) $(OPENGL)

../lib/libCabbageScene.a: $(OFILES)
	rm -f $@
	mkdir -p ../lib
	ar cq $@ $(OFILES)

clean:
	rm -f $(OFILES)
	rm -f ../lib/libCabbageScene.a


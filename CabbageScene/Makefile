FILES = CMesh.cpp CMeshLoader.cpp CRenderable.cpp CShader.cpp CTexture.cpp CImageLoader.cpp CImage.cpp CShaderLoader.cpp CCamera.cpp CShaderContext.cpp CSceneManager.cpp CMeshSceneObject.cpp ISceneObject.cpp CSceneObject.cpp CMaterial.cpp CMeshSceneObject.cpp ImageLoaders/Texture.cpp

OFILES = $(FILES:%.cpp=%.o)

DIRECTORIES = -I../CabbageCore -I../CabbageScene -I../CabbageFramework -I../CabbageCollider -I../CabbageSound -I../CabbageParticles -I../StateLWIB

ifeq (${__MINGW__},1)
OPENGL = -mwindows -lopengl32 -lglew32 `sdl-config --cflags` -lglu32 -DGL_GLEXT_PROTOTYPES `sdl-config --libs`
else
OPENGL = -DGL_GLEXT_PROTOTYPES -lGL -lGLU -lSDL
endif


all: ../lib/libCabbageScene.a

ISceneObject.o: ISceneObject.cpp
	g++ -O1 -o ISceneObject.o ISceneObject.cpp -c $(DIRECTORIES) $(OPENGL)

CMaterial.o: CMaterial.cpp
	g++ -O1 -o CMaterial.o CMaterial.cpp -c $(DIRECTORIES) $(OPENGL)
CSceneObject.o: CSceneObject.cpp
	g++ -O1 -o CSceneObject.o CSceneObject.cpp -c $(DIRECTORIES) $(OPENGL)

CMesh.o: CMesh.cpp
	g++ -O1 -o CMesh.o CMesh.cpp -c $(DIRECTORIES) $(OPENGL)

CShaderContext.o: CShaderContext.cpp
	g++ -O1 -o CShaderContext.o CShaderContext.cpp -c $(DIRECTORIES) $(OPENGL)

CShaderLoader.o: CShaderLoader.cpp
	g++ -O1 -o CShaderLoader.o CShaderLoader.cpp -c $(DIRECTORIES) $(OPENGL)

CCamera.o: CCamera.cpp
	g++ -O1 -o CCamera.o CCamera.cpp -c $(DIRECTORIES) $(OPENGL)

CImage.o: CImage.cpp
	g++ -O1 -o CImage.o CImage.cpp -c $(DIRECTORIES) $(OPENGL)

CMeshLoader.o: CMeshLoader.cpp
	g++ -O1 -o CMeshLoader.o CMeshLoader.cpp -c $(DIRECTORIES) $(OPENGL)

CRenderable.o: CRenderable.cpp
	g++ -O1 -o CRenderable.o CRenderable.cpp -c $(DIRECTORIES) $(OPENGL)

CShader.o: CShader.cpp
	g++ -O1 -o CShader.o CShader.cpp -c $(DIRECTORIES) $(OPENGL)

ImageLoaders/Texture.o: ImageLoaders/Texture.cpp ImageLoaders/Texture.h
	g++ -O1 -o $@ ImageLoaders/Texture.cpp -c $(DIRECTORIES) $(OPENGL)

CTexture.o: CTexture.cpp
	g++ -O1 -o CTexture.o CTexture.cpp -c $(DIRECTORIES) $(OPENGL)

CImageLoader.o: CImageLoader.cpp
	g++ -O1 -o CImageLoader.o CImageLoader.cpp -c $(DIRECTORIES) $(OPENGL)

CSceneManager.o: CSceneManager.cpp
	g++ -O1 -o $@ CSceneManager.cpp -c $(DIRECTORIES) $(OPENGL)

CMeshSceneObject.o: CMeshSceneObject.cpp
	g++ -O1 -o $@ CMeshSceneObject.cpp -c $(DIRECTORIES) $(OPENGL)

../lib/libCabbageScene.a: $(OFILES)
	rm -f $@
	mkdir -p ../lib
	ar cq $@ $(OFILES)

clean:
	rm -f $(OFILES)
	rm -f ../lib/libCabbageScene.a


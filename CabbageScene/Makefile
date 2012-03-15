FILES = CMesh.cpp CMeshLoader.cpp CRenderable.cpp CShader.cpp CTexture.cpp CImageLoader.cpp CImage.cpp CShaderLoader.cpp CCamera.cpp CShaderContext.cpp CSceneManager.cpp CMeshSceneObject.cpp ISceneObject.cpp CSceneObject.cpp CMaterial.cpp CMeshSceneObject.cpp ImageLoaders/Texture.cpp CSceneEffectManager.cpp CFrameBufferObject.cpp CRenderBufferObject.cpp
CC=g++ -g

OFILES = $(FILES:%.cpp=%.o)

DIRECTORIES = -I../CabbageCore -I../CabbageScene -I../CabbageFramework -I../CabbageCollider -I../CabbageSound -I../CabbageParticles -I../StateLWIB -I../CabbageGUI

ifeq (${__MINGW__},1)
OPENGL = -mwindows -lopengl32 -lglew32 `sdl-config --cflags` -lglu32 -DGL_GLEXT_PROTOTYPES `sdl-config --libs`
else
OPENGL = -DGL_GLEXT_PROTOTYPES -lGL -lGLU -lSDL
endif


all: ../lib/libCabbageScene.a

ISceneObject.o: ISceneObject.cpp
	$(CC) -o ISceneObject.o ISceneObject.cpp -c $(DIRECTORIES) $(OPENGL)

CMaterial.o: CMaterial.cpp
	$(CC) -o CMaterial.o CMaterial.cpp -c $(DIRECTORIES) $(OPENGL)
CSceneObject.o: CSceneObject.cpp
	$(CC) -o CSceneObject.o CSceneObject.cpp -c $(DIRECTORIES) $(OPENGL)

CMesh.o: CMesh.cpp
	$(CC) -o CMesh.o CMesh.cpp -c $(DIRECTORIES) $(OPENGL)

CShaderContext.o: CShaderContext.cpp
	$(CC) -o CShaderContext.o CShaderContext.cpp -c $(DIRECTORIES) $(OPENGL)

CShaderLoader.o: CShaderLoader.cpp
	$(CC) -o CShaderLoader.o CShaderLoader.cpp -c $(DIRECTORIES) $(OPENGL)

CCamera.o: CCamera.cpp
	$(CC) -o CCamera.o CCamera.cpp -c $(DIRECTORIES) $(OPENGL)

CImage.o: CImage.cpp
	$(CC) -o CImage.o CImage.cpp -c $(DIRECTORIES) $(OPENGL)

CMeshLoader.o: CMeshLoader.cpp
	$(CC) -o CMeshLoader.o CMeshLoader.cpp -c $(DIRECTORIES) $(OPENGL)

CRenderable.o: CRenderable.cpp
	$(CC) -o CRenderable.o CRenderable.cpp -c $(DIRECTORIES) $(OPENGL)

CShader.o: CShader.cpp
	$(CC) -o CShader.o CShader.cpp -c $(DIRECTORIES) $(OPENGL)

ImageLoaders/Texture.o: ImageLoaders/Texture.cpp ImageLoaders/Texture.h
	$(CC) -o $@ ImageLoaders/Texture.cpp -c $(DIRECTORIES) $(OPENGL)

CTexture.o: CTexture.cpp
	$(CC) -o CTexture.o CTexture.cpp -c $(DIRECTORIES) $(OPENGL)

CImageLoader.o: CImageLoader.cpp
	$(CC) -o CImageLoader.o CImageLoader.cpp -c $(DIRECTORIES) $(OPENGL)

CSceneManager.o: CSceneManager.cpp
	$(CC) -o $@ CSceneManager.cpp -c $(DIRECTORIES) $(OPENGL)

CSceneEffectManager.o: CSceneEffectManager.cpp
	$(CC) -o $@ CSceneEffectManager.cpp -c $(DIRECTORIES) $(OPENGL)

CFrameBufferObject.o: CFrameBufferObject.cpp
	$(CC) -o $@ CFrameBufferObject.cpp -c $(DIRECTORIES) $(OPENGL)

CRenderBufferObject.o: CRenderBufferObject.cpp
	$(CC) -o $@ CRenderBufferObject.cpp -c $(DIRECTORIES) $(OPENGL)

CMeshSceneObject.o: CMeshSceneObject.cpp
	$(CC) -o $@ CMeshSceneObject.cpp -c $(DIRECTORIES) $(OPENGL)

../lib/libCabbageScene.a: $(OFILES)
	rm -f $@
	mkdir -p ../lib
	ar cq $@ $(OFILES)

clean:
	rm -f $(OFILES)
	rm -f ../lib/libCabbageScene.a


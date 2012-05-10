FILES = CMesh.cpp CMeshLoader.cpp CRenderable.cpp CShader.cpp CTexture.cpp CImageLoader.cpp CImage.cpp CShaderLoader.cpp CCamera.cpp CShaderContext.cpp CSceneManager.cpp CMeshSceneObject.cpp ISceneObject.cpp CSceneObject.cpp CMaterial.cpp CMeshSceneObject.cpp ImageLoaders/Texture.cpp CSceneEffectManager.cpp CFrameBufferObject.cpp CRenderBufferObject.cpp CPointLightSceneObject.cpp CDeferredShadingManager.cpp CDirectionalLightSceneObject.cpp
CC=g++ -g

OFILES = $(FILES:%.cpp=%.o)

DIRECTORIES = -I../CabbageCore -I../CabbageScene -I../CabbageFramework -I../CabbageCollider -I../CabbageSound -I../CabbageParticles -I../StateLWIB -I../CabbageGUI

all: ../lib/libCabbageScene.a

.cpp.o:
	$(CC) -c $(DIRECTORIES) -DGL_GLEXT_PROTOTYPES $< -o $@

../lib/libCabbageScene.a: $(OFILES)
	rm -f $@
	mkdir -p ../lib
	ar cq $@ $(OFILES)

clean:
	rm -f $(OFILES)
	rm -f ../lib/libCabbageScene.a


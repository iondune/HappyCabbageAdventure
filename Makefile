
all: CabbageScene MeshLoaderDemo CabbageColliderDemo

CabbageScene:
	@(cd ./CabbageScene && $(MAKE))

MeshLoaderDemo: CabbageScene
	@(cd ./MeshLoaderDemo && $(MAKE))

CabbageColliderDemo:
	@(cd ./CabbageColliderDemo && $(MAKE))

all: CabbageScene MeshLoaderDemo

CabbageScene:
	@(cd ./CabbageScene && $(MAKE))

MeshLoaderDemo: CabbageScene
	@(cd ./MeshLoaderDemo && $(MAKE))

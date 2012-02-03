
all: build-CabbageFramework build-CabbageScene build-MeshLoaderDemo build-CabbageColliderDemo

build-CabbageScene:
	cd ./CabbageScene && $(MAKE)

build-CabbageCollider:
	cd ./CabbageCollider && $(MAKE)

build-MeshLoaderDemo: build-CabbageScene
	cd ./MeshLoaderDemo && $(MAKE)

build-CabbageColliderDemo: build-CabbageCollider build-CabbageScene
	cd ./CabbageColliderDemo && $(MAKE)

build-base: build-CabbageCollider build-CabbageScene build-CabbageFramework
	cd ./base && $(MAKE)

build-CabbageFramework:
	cd ./CabbageFramework && $(MAKE)

clean:
	cd ./CabbageScene && $(MAKE) $@ && cd ../CabbageCollider && $(MAKE) $@ && cd ../MeshLoaderDemo && $(MAKE) $@ && cd ../CabbageColliderDemo && $(MAKE) $@


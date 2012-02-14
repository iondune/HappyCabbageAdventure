all: build-CabbageFramework build-CabbageScene build-CabbageCollider build-CLWIB build-MainMenu build-base build-MeshLoaderDemo build-CabbageColliderDemo

build-CLWIB:
	cd ./StateLWIB && $(MAKE) lib

build-MainMenu:
	cd ./StateMainMenu && ./que

build-CabbageScene:
	cd ./CabbageScene && $(MAKE)

build-CabbageCollider:
	cd ./CabbageCollider && $(MAKE)

build-MeshLoaderDemo: build-CabbageScene
	cd ./DemoMeshLoader && $(MAKE)

build-CabbageColliderDemo: build-CabbageCollider build-CabbageScene
	cd ./DemoCabbageCollider && $(MAKE)

build-base: build-CabbageCollider build-CabbageScene build-CabbageFramework
	cd ./Base && $(MAKE)

build-CabbageFramework:
	cd ./CabbageFramework && $(MAKE)

clean:
	cd ./CabbageScene && $(MAKE) $@ && cd ../CabbageCollider && $(MAKE) $@ && cd ../DemoMeshLoader && $(MAKE) $@ && cd ../DemoCabbageCollider && $(MAKE) $@ && cd ../CabbageFramework && $(MAKE) $@


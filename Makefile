all: build-Sound build-CabbageFramework build-CabbageScene build-CabbageCollider build-CLWIB build-MainMenu build-Overworld build-base build-MeshLoaderDemo build-CabbageColliderDemo

build-Sound:
	cd ./CabbageSound && $(MAKE)

build-CLWIB:
	cd ./StateLWIB && $(MAKE) lib

build-MainMenu:
	cd ./StateMainMenu && ./que
	
build-Overworld:
	cd ./StateOverworld && $(MAKE)
	
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
	cd ./CabbageScene && $(MAKE) $@ && cd ../StateOverworld && $(MAKE) $@ && cd ../StateLWIB && $(MAKE) $@ && cd ../CabbageSound && $(MAKE) $@ && cd ../CabbageCollider && $(MAKE) $@ && cd ../DemoMeshLoader && $(MAKE) $@ && cd ../DemoCabbageCollider && $(MAKE) $@ && cd ../CabbageFramework && $(MAKE) $@


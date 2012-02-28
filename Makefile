OBJECT_FILES=`find | grep '\.o$$' | sed "/sdlmixer/d"`


all: build-Sound build-CabbageFramework build-CabbageParticles build-CabbageScene build-CabbageCollider build-CLWIB build-MainMenu build-Overworld build-base build-MeshLoaderDemo build-CabbageColliderDemo

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

build-base: build-CabbageCollider build-CabbageScene build-CabbageFramework build-CabbageParticles
	cd ./Base && $(MAKE)

build-CabbageFramework:
	cd ./CabbageFramework && $(MAKE)

build-CabbageParticles:
	cd ./CabbageParticles && $(MAKE)
	
	
clean:
	rm -f $(OBJECT_FILES)
	rm -f Base/cabbage DemoCabbageCollider/CabbageColliderDemo DemoMeshLoader/MeshLoaderDemo
	rm -f lib/libCLWIB.a lib/libCabbageCollider.a lib/libCabbageFramework.a lib/libCabbageParticles.a lib/libCabbageScene.a lib/libCabbageSound.a lib/libOverworldState.a

#	cd ./CabbageScene && $(MAKE) $@ && cd ../StateOverworld && $(MAKE) $@ && cd ../StateLWIB && $(MAKE) $@ && cd ../CabbageSound && $(MAKE) $@ && cd ../CabbageCollider && $(MAKE) $@ && cd ../DemoMeshLoader && $(MAKE) $@ && cd ../DemoCabbageCollider && $(MAKE) $@ && cd ../CabbageFramework && $(MAKE) $@



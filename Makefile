OBJECT_FILES=`find | grep '\.o$$' | sed "/sdlmixer/d"`
CABBAGE_GAME_DEPENDENCY_FILES=./CabbageGame/.dependencies

all: build-Sound build-CabbageFramework build-CabbageParticles build-CabbageScene build-CabbageCollider build-base build-CabbageGUI build-CabbageGame build-StateGame build-CLWIB build-MainMenu build-Overworld

#build-MeshLoaderDemo build-CabbageColliderDemo

build-StateGame:
	cd ./StateGame && make -j

build-CabbageGame:
	cd ./CabbageGame && make -j

build-Sound:
	cd ./CabbageSound && make -j

build-CLWIB:
	cd ./StateLWIB && make -j lib

build-MainMenu:
	cd ./StateMainMenu && make -j
	
build-Overworld:
	cd ./StateOverworld && make -j
	
build-CabbageScene:
	cd ./CabbageScene && make -j

build-CabbageCollider:
	cd ./CabbageCollider && make -j

build-CabbageGUI:
	cd ./CabbageGUI && make -j

build-MeshLoaderDemo: build-CabbageScene
	cd ./DemoMeshLoader && make -j

build-CabbageColliderDemo: build-CabbageCollider build-CabbageScene
	cd ./DemoCabbageCollider && make -j

build-base: build-StateGame build-CabbageGame build-CabbageCollider build-CabbageScene build-CabbageFramework build-CabbageParticles build-CabbageGUI build-CLWIB build-MainMenu build-Overworld
	cd ./Base && make -j

build-CabbageFramework:
	cd ./CabbageFramework && make -j

build-CabbageParticles:
	cd ./CabbageParticles && make -j
	
clean:
	rm -f $(OBJECT_FILES)
	rm -rf $(CABBAGE_GAME_DEPENDENCY_FILES)
	rm -f Base/cabbage DemoCabbageCollider/CabbageColliderDemo DemoMeshLoader/MeshLoaderDemo
	rm -f lib/libCLWIB.a lib/libCabbageCollider.a lib/libCabbageFramework.a lib/libCabbageParticles.a lib/libCabbageScene.a lib/libCabbageSound.a lib/libOverworldState.a lib/libCabbageGUI.a lib/libCabbageGame.a lib/libStateGame.a

#	cd ./CabbageScene && make -j $@ && cd ../StateOverworld && make -j $@ && cd ../StateLWIB && make -j $@ && cd ../CabbageSound && make -j $@ && cd ../CabbageCollider && make -j $@ && cd ../DemoMeshLoader && make -j $@ && cd ../DemoCabbageCollider && make -j $@ && cd ../CabbageFramework && make -j $@



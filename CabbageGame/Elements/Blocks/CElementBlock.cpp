#include "CElementBlock.h"
#include "Player/CPlayerAbility.h"
#include "CGameLevel.h"

CElementBlock::CElementBlock(SRect2f nArea, int depth, int texture)
: CGameplayElement((CCollideable *&)PhysicsEngineObject, (ISceneObject *&)SceneObject, nArea), Depth(depth), Texture(texture) {
}

void CElementBlock::update(float time) {
   if(ParticleEngine) {
      ParticleEngine->step(time);
      //ParticleEngine->setCenterPos(time);
      if(ParticleEngine->dead) {
         removeFromGame();
      }
   }
   else
      updateSceneObject(time);
}

void CElementBlock::updatePhysicsEngineObject(float time) {
}

void CElementBlock::updateSceneObject(float time) {
}

void CElementBlock::OnCollision(const SCollisionEvent& Event) {
   return;
}

void CElementBlock::writeXML(xmlwriter *l) {
   std::stringstream xValue, yValue, widthValue, heightValue, tagValue, rangeValue, speedValue, depthValue, textureType;
   xValue << Area.Position.X;
   yValue << Area.Position.Y;
   widthValue << Area.Size.X;
   heightValue << Area.Size.Y;
   depthValue << Depth;
   textureType << Texture;

   tagValue << "CBlock";

   l->AddAtributes("texture ", textureType.str());
   l->AddAtributes("depth ", depthValue.str());
   l->AddAtributes("width ", widthValue.str());
   l->AddAtributes("height ", heightValue.str());
   l->AddAtributes("Y ", yValue.str());
   l->AddAtributes("X ", xValue.str());
   l->Createtag(tagValue.str());
   l->CloseLasttag();
}

void CElementBlock::setupPhysicsEngineObject() {
   /*
    * Doing this in the consolidation phase in CGameState
    * Thus, there will be no physics engine object that this can refer to! TODO: Check if this is good enough or not
    CObject *engBlock = Engine->addObject();
    engBlock->setArea(SRect2f(x, y, Area.Size.X, Area.Size.Y));
    */
   //Point of this is so any functions (superclass) that use the physics engine object pointer won't break
   PhysicsEngineObject = Level.getPhysicsEngine().addObject();
   PhysicsEngineObject->setArea(Area);
   /*
   PhysicsEngineObject->setCollisionResponder(NULL);
   PhysicsEngineObject->setArea(Area);
   PhysicsEngineObject->setVisualDepth(Depth);
   */
   Level.getPhysicsEngine().removeCollideable(PhysicsEngineObject);
}

void CElementBlock::setupSceneObject() {
   CMeshSceneObject *tempBlock = new CMeshSceneObject();
   CMesh *mesh;

   mesh = CMeshLoader::load3dsMesh("Base/cuberock3.3ds");

   if(mesh) {
         mesh->resizeMesh(SVector3f(1));
         mesh->centerMeshByExtents(SVector3f(0));
         mesh->calculateNormalsPerFace();
         //mesh->calculateNormalsPerVertex();
   }

   else {
      fprintf(stderr, "COULD NOT LOAD BLOCK MESH.\n");
   }

   tempBlock->setMesh(mesh);
   if (Level.getEnvironment() == Env::FOREST) {
      switch(Texture) {
      case 0:
         tempBlock->setTexture("Base/grass.bmp");
         break; 
      case 1:
         tempBlock->setTexture("Base/dirt.bmp");
         break;
      case 2:
         tempBlock->setTexture("Base/rock.bmp");
         break;
      case -5:
         mesh = CMeshLoader::load3dsMesh("Base/levelBlock.3ds");
         if (mesh) {
            mesh->resizeMesh(SVector3f(1));
            mesh->centerMeshByExtents(SVector3f(0));
            mesh->calculateNormalsPerVertex();
         }
         else {
            fprintf(stderr, "Failed to load the mesh\n");
         }
         tempBlock->setMesh(mesh);
         tempBlock->setTexture("Base/GrassyGrass.bmp", 2);
         tempBlock->setTexture("Base/DirtyDirt.bmp", 3);
         break;
      default:
         fprintf(stderr, "Unknown texture type %d\n", Texture);
         break;
      }
   }
   else if (Level.getEnvironment() == Env::DESERT) {
      switch(Texture) {
      case -5:
         mesh = CMeshLoader::load3dsMesh("Base/levelBlock.3ds");
         if (mesh) {
            mesh->resizeMesh(SVector3f(1.0));
            mesh->centerMeshByExtents(SVector3f(0));
            mesh->calculateNormalsPerVertex();
         }
         else {
            fprintf(stderr, "Failed to load the mesh\n");
         }
         tempBlock->setMesh(mesh);
         tempBlock->setTexture("Base/DesertyDesert.tga", 2);
         tempBlock->setTexture("Base/DirtyDirt.bmp", 3);
         break;
      case 0:
      case 1:
      case 2:
         tempBlock->setTexture("Base/desert.bmp");
         break;
      default:
         fprintf(stderr, "Unknown texture type %d\n", Texture);
         break;
      }
   }

   else if (Level.getEnvironment() == Env::WATER) {
         switch(Texture) {
         case 0:
            tempBlock->setTexture("Base/grass.bmp");
            break;
         case 1:
            tempBlock->setTexture("Base/dirt.bmp");
            break;
         case 2:
            tempBlock->setTexture("Base/rock.bmp");
            break;
         case -5:
			 switch (7)//rand() % 7)
			 {
			 case 0:
				mesh = CMeshLoader::load3dsMesh("Base/waterLevelBlock_1.3ds");
				break;
			 case 1:
				mesh = CMeshLoader::load3dsMesh("Base/waterLevelBlock_2.3ds");
				break;
			 case 2:
				mesh = CMeshLoader::load3dsMesh("Base/waterLevelBlock_3.3ds");
				break;
			 case 3:
				mesh = CMeshLoader::load3dsMesh("Base/waterLevelBlock_4.3ds");
				break;
			 case 4: case 5: case 6:
				mesh = CMeshLoader::load3dsMesh("Base/waterLevelBlock_5.3ds");
				break;
			 case 7:
				mesh = CMeshLoader::load3dsMesh("Base/levelBlock.3ds");
				break;
			 }
            if (mesh) {
               mesh->resizeMesh(SVector3f(1));
               mesh->centerMeshByExtents(SVector3f(0));
               mesh->calculateNormalsPerVertex();
            }
            else {
               fprintf(stderr, "Failed to load the mesh\n");
            }
            tempBlock->setMesh(mesh);

			// If you want to get these textures to work properly, you'll need to sort out which indices to use for each model
            tempBlock->setTexture("Colors/White.bmp");
            tempBlock->setTexture("Base/WateryWater.bmp", 2);
            tempBlock->setTexture("Base/WetRocksMmmmm.bmp", 3);
            break;
         default:
            fprintf(stderr, "Unknown texture type %d\n", Texture);
            break;
         }
      }

   tempBlock->setShader(ERP_DEFAULT, CShaderLoader::loadShader("ToonTexture"));
   tempBlock->setShader(ERP_DEFERRED_OBJECTS, CShaderLoader::loadShader("Deferred/Textured"));
   if(Texture != -5) {
      tempBlock->setTranslation(SVector3f((Area.Position.X+(Area.Position.X+Area.Size.X))/2, (Area.Position.Y+(Area.Position.Y+Area.Size.Y))/2, 0));
      
      //if(Area.Size.Y > 1)
         tempBlock->setScale(SVector3f(Area.Size.X*1.075f, Area.Size.Y*1.075f, (float) Depth));
      //else
      //   tempBlock->setScale(SVector3f(Area.Size.X, Area.Size.Y, (float) Depth));
   }
   else /* For ground blocks */{
      tempBlock->setTranslation(SVector3f((Area.Position.X+(Area.Position.X+Area.Size.X))/2, Area.Position.Y+.6f*Area.Size.Y, 0));
      tempBlock->setScale(SVector3f(5.0f)); //Area.Size.X, Area.Size.Y, Depth
   }

   // Don't ask me
   bool You_Want_The_Program_To_Crash = false;
   if (You_Want_The_Program_To_Crash)
      tempBlock->setRotation(SVector3f(Texture==-5?-90.f:0, 0, 90));
   else
	  tempBlock->setRotation(SVector3f(Texture==-5?-90.f:0, 0, 0));

   // I was trying to rotate the block's Z axis to find which side has stuff on them
   // like this:
   // tempBlock->setRotation(SVector3f(Texture==-5?-90.f:0, 0, (Level.getEnvironment() == Env::WATER ? 90.f : 0.f)));

   SceneObject = tempBlock;
   if(Level.isLoaded())
      CApplication::get().getSceneManager().addImmobileSceneObject(tempBlock, THIS_OBJECT_WILL_NEVER_MOVE_AND_ITS_BOUNDING_BOX_IS_CORRECT);
   else
      CApplication::get().getSceneManager().addSceneObject(tempBlock);
}

void CElementBlock::printInformation() {
   printf("CElementBlock; Area: [[%0.0f, %0.0f],[%0.0f, %0.0f]]; Depth: %d; Texture: %d\n", Area.Position.X, Area.Position.Y, Area.Size.X, Area.Size.Y, Depth, Texture);
}

void CElementBlock::reactToAbility(Abilities::EAbilityType ability) {
   //Default behavior to react to abilities:
   return;
}

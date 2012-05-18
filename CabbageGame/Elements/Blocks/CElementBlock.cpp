#include "CElementBlock.h"
#include "Player/CPlayerAbility.h"
#include "CGameLevel.h"

CElementBlock::CElementBlock(SRect2 nArea, int depth, int texture)
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
   if(Texture != -5) {
      SceneObject->setTranslation(SVector3((Area.Position.X+(Area.Position.X+Area.Size.X))/2, (Area.Position.Y+(Area.Position.Y+Area.Size.Y))/2, 0));
      SceneObject->setScale(SVector3(Area.Size.X, Area.Size.Y, (float) Depth));
   }
}

void CElementBlock::OnCollision(CCollideable *Object) {
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
    engBlock->setArea(SRect2(x, y, Area.Size.X, Area.Size.Y));
    */
   //Point of this is so any functions (superclass) that use the physics engine object pointer won't break
   PhysicsEngineObject = Level.getPhysicsEngine().addObject();
   PhysicsEngineObject->setArea(Area);
   /*
   PhysicsEngineObject->setCollisionResponder(NULL);
   PhysicsEngineObject->setArea(Area);
   PhysicsEngineObject->setDepth(Depth);
   */
   Level.getPhysicsEngine().removeObject(PhysicsEngineObject);
}

void CElementBlock::setupSceneObject() {
   CMeshSceneObject *tempBlock = new CMeshSceneObject();
   CMesh *mesh;

   mesh = CMeshLoader::load3dsMesh("Base/cuberock.3ds");

   if(mesh) {
         mesh->resizeMesh(SVector3(1));
         mesh->centerMeshByExtents(SVector3(0));
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
            mesh->resizeMesh(SVector3(1));
            mesh->centerMeshByExtents(SVector3(0));
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
            mesh->resizeMesh(SVector3(1.0));
            mesh->centerMeshByExtents(SVector3(0));
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
            mesh = CMeshLoader::load3dsMesh("Base/levelBlock.3ds");
            if (mesh) {
               mesh->resizeMesh(SVector3(1));
               mesh->centerMeshByExtents(SVector3(0));
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

   tempBlock->setShader(ERP_DEFAULT, CShaderLoader::loadShader("ToonTexture"));
   tempBlock->setShader(ERP_DEFERRED_OBJECTS, CShaderLoader::loadShader("Deferred/Textured"));
   if(Texture != -5) {
      tempBlock->setTranslation(SVector3((Area.Position.X+(Area.Position.X+Area.Size.X))/2, (Area.Position.Y+(Area.Position.Y+Area.Size.Y))/2, 0));
      tempBlock->setScale(SVector3(Area.Size.X, Area.Size.Y, (float) Depth));
   }
   else /* For ground blocks */{
      tempBlock->setTranslation(SVector3((Area.Position.X+(Area.Position.X+Area.Size.X))/2, Area.Position.Y+.6f*Area.Size.Y, 0));
      tempBlock->setScale(SVector3(5.0f)); //Area.Size.X, Area.Size.Y, Depth
   }

   tempBlock->setRotation(SVector3(Texture==-5?-90.f:0, 0, 0));

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

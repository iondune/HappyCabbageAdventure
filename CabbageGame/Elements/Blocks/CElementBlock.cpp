#include "CElementBlock.h"
#include "CGameLevel.h"

CElementBlock::CElementBlock(SRect2 nArea, int depth, int texture)
: CGameplayElement(PhysicsEngineObject, SceneObject, nArea), Depth(depth), Texture(texture) {
}

void CElementBlock::update(float f) {
   return;
}

void CElementBlock::OnCollision(CCollideable *Object, CCollideable *With) {
   return;
}

void CElementBlock::writeXML(xmlwriter *l) {
   std::stringstream xValue, yValue, widthValue, heightValue, tagValue, isMovingValue, rangeValue, speedValue, depthValue, textureType;
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

void CElementBlock::setupObjects() {
   CMeshSceneObject *tempBlock = new CMeshSceneObject();
   /*
    * Doing this in the consolidation phase in CGameState
    * Thus, there will be no physics engine object that this can refer to! TODO: Check if this is good enough or not
    CObject *engBlock = Engine->addObject();
    engBlock->setArea(SRect2(x, y, Area.Size.X, Area.Size.Y));
    */

   CMesh *mesh;

   mesh = CMeshLoader::createCubeMesh();
   mesh->calculateNormalsPerVertex();
   tempBlock->setMesh(mesh);
   if (Level.getEnvironment() == 0) {
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
            mesh->resizeMesh(SVector3(1.0));
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
   else if (Level.getEnvironment() == 1) {
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
   tempBlock->setShader(ERP_DEFAULT, CShaderLoader::loadShader("ToonTexture"));
   tempBlock->setShader(ERP_DEFERRED_OBJECTS, CShaderLoader::loadShader("Deferred/Textured"));
   if(Texture != -5) {
      tempBlock->setTranslation(SVector3((Area.Position.X+(Area.Position.X+Area.Size.X))/2, (Area.Position.Y+(Area.Position.Y+Area.Size.Y))/2, 0));
      tempBlock->setScale(SVector3(Area.Size.X, Area.Size.Y, (float) Depth));
   }
   else /* For ground blocks */{
      tempBlock->setTranslation(SVector3((Area.Position.X+(Area.Position.X+Area.Size.X))/2, Area.Position.Y+.84f*Area.Size.Y, 0));
      tempBlock->setScale(SVector3(Area.Size.X, (float) Depth, Area.Size.Y)); //Area.Size.X, Area.Size.Y, Depth
   }

   tempBlock->setRotation(SVector3(Texture==-5?-90.f:0, 0, 0));

   CApplication::get().getSceneManager().addImmobileSceneObject(tempBlock, THIS_OBJECT_WILL_NEVER_MOVE_AND_ITS_BOUNDING_BOX_IS_CORRECT);
}

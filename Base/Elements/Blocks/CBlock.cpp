#include "CBlock.h"

CBlock(SRect2 nArea, int depth, int texture)
: CGameplayElement(PhysicsEngineObject, SceneObject), Area(nArea), Depth(depth), Texture(texture) {

}

void CBlock::writeXML(xmlwriter *l) {
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

void setupObjects() {
   CMeshSceneObject *tempBlock = new CMeshSceneObject();
   /*
    * Doing this in the consolidation phase in CGameState
    * Thus, there will be no physics engine object that this can refer to! TODO: Check if this is good enough or not
     Cabbage::Collider::CObject *engBlock = Engine->addObject();
     engBlock->setArea(SRect2(x, y, (float) w, (float) h));
   */

   CMesh *mesh;


   mesh = CMeshLoader::createCubeMesh();
   mesh->calculateNormalsPerVertex();
   tempBlock->setMesh(mesh);
   if (env == 0) {
      if (t == 0) {
           tempBlock->setTexture("Base/grass.bmp");
      }
      else if (t == 1) {
          tempBlock->setTexture("Base/dirt.bmp");
      }
      else if (t == 2) {
         tempBlock->setTexture("Base/rock.bmp");
      }

      else if (t == -5) {
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
      }
      else {
           printf("texture not found\n" );
      }
   }

   else if (env == 1) {
      if (t == 0) {
           tempBlock->setTexture("Base/desert.bmp");
      }
      else if (t == 1) {
          tempBlock->setTexture("Base/desert.bmp");
      }
      else if (t == 2) {
         tempBlock->setTexture("Base/desert.bmp");
      }

      else if (t == -5) {
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
      }
      else {
           printf("texture not found\n" );
      }
   }
   tempBlock->setShader(ERP_DEFAULT, shader);
   tempBlock->setShader(ERP_DEFERRED_OBJECTS, dShader);
   if(t != -5) {
      tempBlock->setTranslation(SVector3((x+(x+w))/2, (y+(y+h))/2, 0));
      tempBlock->setScale(SVector3((float) w, (float) h, (float) z));
   }
   else /* For ground blocks */{
      tempBlock->setTranslation(SVector3((x+(x+w))/2, y+.84f*h, 0));
      tempBlock->setScale(SVector3((float) w, (float) z, (float) h)); //w, h, z
   }

   tempBlock->setRotation(SVector3(t==-5?-90.f:0, 0, 0));

   if(isMovingPlatform) {
      CApplication::get().getSceneManager().addSceneObject(tempBlock);
   }
   else {
      //CApplication::get().getSceneManager().addSceneObject(tempBlock);
      CApplication::get().getSceneManager().addImmobileSceneObject(tempBlock, THIS_OBJECT_WILL_NEVER_MOVE_AND_ITS_BOUNDING_BOX_IS_CORRECT);
   }

   return tempBlock;

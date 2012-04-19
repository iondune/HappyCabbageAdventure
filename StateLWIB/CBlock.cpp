#include "CBlock.h"

CBlock::CBlock(float nx, float ny, int width, int height, int depth, int texture, int moving, int env)
{
   x = nx; y = ny;
   w = width; h = height;
   Speed = 0;
   Range = 0;
   z = depth;
   t = texture;
   isMovingPlatform = 0;
   this->env = env;
}

void CBlock::writeXML(xmlwriter *l) {
    std::stringstream xValue, yValue, widthValue, heightValue, tagValue, isMovingValue, rangeValue, speedValue, depthValue, textureType;
    xValue << x;
    yValue << y;
    widthValue << w;
    heightValue << h;
    depthValue << z;
    textureType << t;
    isMovingValue << isMovingPlatform;
    rangeValue << Range;
    speedValue << Speed;

    tagValue << "CBlock";

    l->AddAtributes("speed ", speedValue.str());
    l->AddAtributes("range ", rangeValue.str());
    l->AddAtributes("isMoving ", isMovingValue.str());
    l->AddAtributes("texture ", textureType.str());
    l->AddAtributes("depth ", depthValue.str());
    l->AddAtributes("width ", widthValue.str());
    l->AddAtributes("height ", heightValue.str());
    l->AddAtributes("Y ", yValue.str());
    l->AddAtributes("X ", xValue.str());
    l->Createtag(tagValue.str());
    l->CloseLasttag();
}
void CBlock::moveTo(float x,float y) {
   //For usage in LWIB
}

CMeshSceneObject * CBlock::setupItem(CShader * shader, CShader * dShader, Cabbage::Collider::CCollisionEngine *Engine, CGameplayManager *GameplayManager /* For enemy handling */) {
   CMeshSceneObject *tempBlock = new CMeshSceneObject();
   if(isMovingPlatform) {
      Cabbage::Collider::CElevator *engBlock = Engine->addElevator();
      engBlock->setArea(SRect2(x, y, (float) w, (float) h));
      engBlock->Speed = (float) Speed;
      engBlock->Range = (float) Range;
      engBlock->setRenderable(tempBlock);
      elevator = engBlock;
   }
   else {
      /* Doing this in the consolidation phase in CGameState
      Cabbage::Collider::CObject *engBlock = Engine->addObject();
      engBlock->setArea(SRect2(x, y, (float) w, (float) h));
      */
   }

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
}

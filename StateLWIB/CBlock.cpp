#include "CBlock.h"

CBlock::CBlock(float nx, float ny, int width, int height, int depth, int texture)
{
   x = nx; y = ny;
   w = width; h = height;
   Speed = 0;
   Range = 0;
   z = depth;
   t = texture;
   isMovingPlatform = 0;
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

CMeshSceneObject * CBlock::setupItem(CShader * shader, Cabbage::Collider::CEngine *Engine, CGameplayManager *GameplayManager /* For enemy handling */) {
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
      Cabbage::Collider::CObject *engBlock = Engine->addObject();
      engBlock->setArea(SRect2(x, y, (float) w, (float) h));
   }

   CMesh *mesh = CMeshLoader::loadAsciiMesh("Cube");
   mesh->calculateNormalsPerVertex();
   tempBlock->setMesh(mesh);
   if (t == 0) {
        tempBlock->setTexture("Textures/grass.bmp");
   }
   else if (t == 1) {
        tempBlock->setTexture("Textures/dirt.bmp");
   }
   else if (t == 2) {
        tempBlock->setTexture("Textures/rock.bmp");
   }
   else {
        printf("texture not found\n" );   
   }
   tempBlock->setShader(shader);
   tempBlock->setTranslation(SVector3((x+(x+w))/2, (y+(y+h))/2, 0));
   tempBlock->setScale(SVector3((float) w, (float) h, (float) z));
   tempBlock->setRotation(SVector3(0, 0, 0));
   CApplication::get().getSceneManager().addSceneObject(tempBlock);

   return tempBlock;
}

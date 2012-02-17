#include "CEnemy.h"
#include "../Base/CBadGuy.h"

CEnemy::CEnemy(float nx, float ny, int width, int height)
{
   x = nx; y = ny;
   w = width; h = height;
   isMovingPlatform = 0;
}

void CEnemy::writeXML(xmlwriter *l) {
    std::stringstream xValue, yValue, widthValue, heightValue, tagValue;
    xValue << x;
    yValue << y;
    widthValue << w;
    heightValue << h;
    tagValue << "CEnemy";

    l->AddAtributes("width ", widthValue.str());
    l->AddAtributes("height ", heightValue.str());
    l->AddAtributes("Y ", yValue.str());
    l->AddAtributes("X ", xValue.str());
    l->Createtag(tagValue.str());
    l->CloseLasttag();
}

void CEnemy::moveTo(float x,float y) {
   //For usage in LWIB
}

void CEnemy::setShader(CShader * chad) {
   shader = chad;
}
CMeshRenderable * CEnemy::setupItem(CShader * chad, Cabbage::Collider::CEngine *Engine, CGameplayManager *GameplayManager /* For enemy handling */) {
   //Rough code plan
    CBadGuy::makeBadGuy(x, y, w, h, CBadGuy::apple, GameplayManager);

    return new CMeshRenderable();

/*
   CMeshRenderable *tempEnemy = new CMeshRenderable();
   CMesh *mesh = CMeshLoader::load3dsMesh("Models/appleEnemy.3ds");
   if(mesh) {
      mesh->resizeMesh(SVector3(1));
      mesh->centerMeshByExtents(SVector3(0));
      mesh->calculateNormalsPerFace();
   }

   tempEnemy->setMesh(mesh);
   //tempEnemy->getMaterial().Texture = CImageLoader::loadTexture("Textures/dirt.bmp");
   tempEnemy->getMaterial().Shader = shader;
   tempEnemy->setTranslation(SVector3((x+(x+1))/2, (y+(y+1))/2, 0));
   tempEnemy->setScale(SVector3(1, 1, 1));
   tempEnemy->setRotation(SVector3(-90, 0, 0));

   CApplication::get().getSceneManager().addRenderable(tempEnemy);
   GameplayManager->addEnemy(SVector2(x, y), tempEnemy);
   return tempEnemy;*/
}

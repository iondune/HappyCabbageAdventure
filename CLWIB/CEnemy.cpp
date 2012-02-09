#include "CEnemy.h"

CEnemy::CEnemy(float nx, float ny, int width, int height)
{
   x = nx; y = ny;
   w = width; h = height;
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
CMeshRenderable * CEnemy::setupItem(CShader * shader, Cabbage::Collider::CEngine *Engine, CGameplayManager *GameplayManager /* For enemy handling */) {
   Cabbage::Collider::CObject *engBlock = Engine->addObject();
   engBlock->setArea(SRect2(x, y, w, h));

   CMeshRenderable *tempBlock = new CMeshRenderable();
   CMesh *mesh = CMeshLoader::loadAsciiMesh("Cube");
   mesh->calculateNormalsPerFace();
   tempBlock->setMesh(mesh);
   tempBlock->getMaterial().Texture = CImageLoader::loadTexture("Textures/dirt.bmp");
   tempBlock->getMaterial().Shader = shader;
   tempBlock->setTranslation(SVector3((x+(x+w))/2, (y+(y+h))/2, 0));
   tempBlock->setScale(SVector3(w, h, 1));
   tempBlock->setRotation(SVector3(0, 0, 0));
   CApplication::get().getSceneManager().addRenderable(tempBlock);

   return tempBlock;
}

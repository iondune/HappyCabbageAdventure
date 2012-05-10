#include "CFlag.h"

CFlag::CFlag(float nx, float ny, int width, int height,int secret)
{
   x = nx; y = ny;
   w = width; h = height;
   isMovingPlatform = 0;
   isSecret = secret;
}
void CFlag::writeXML(xmlwriter *l) {
    std::stringstream xValue, yValue, widthValue, heightValue, tagValue, isMovingValue, secretFlag;
    xValue << x;
    yValue << y;
    widthValue << w;
    heightValue << h;
    secretFlag << isSecret;
    isMovingValue << isMovingPlatform;

    tagValue << "CFlag";

    l->AddAtributes("isMoving ", isMovingValue.str());
    l->AddAtributes("secret ", secretFlag.str());
    l->AddAtributes("width ", widthValue.str());
    l->AddAtributes("height ", heightValue.str());
    l->AddAtributes("Y ", yValue.str());
    l->AddAtributes("X ", xValue.str());
    l->Createtag(tagValue.str());
    l->CloseLasttag();
}
void CFlag::moveTo(float x,float y) {
   //For usage in LWIB
}

CMeshSceneObject * CFlag::setupItem(CShader * shader, CShader * dShader, CCollisionEngine *Engine, CGameplayManager *GameplayManager){
//    Engine->addActor()->setArea(SRect2f(CPlaceable::x,y,(float)w,(float)h));
    return new CMeshSceneObject(); // not sure to really return here
}
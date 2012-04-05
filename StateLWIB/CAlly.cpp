#include "CAlly.h"

CAlly::CAlly(float nx, float ny, int width, int height, int type)
{
   x = nx; y = ny;
   w = width; h = height;
   Speed = 0;
   Range = 0;
   isMovingPlatform = 0;
}

void CAlly::writeXML(xmlwriter *l) {
    std::stringstream xValue, yValue, widthValue, heightValue, tagValue, type;
    xValue << x;
    yValue << y;
    widthValue << w;
    heightValue << h;
    type << t;

    tagValue << "CAlly";

    l->AddAtributes("type ", type.str());
    l->AddAtributes("width ", widthValue.str());
    l->AddAtributes("height ", heightValue.str());
    l->AddAtributes("Y ", yValue.str());
    l->AddAtributes("X ", xValue.str());
    l->Createtag(tagValue.str());
    l->CloseLasttag();
}
void CAlly::moveTo(float x,float y) {
   //For usage in LWIB
}

CMeshSceneObject * CAlly::setupItem(CShader * shader, CShader * dShader, Cabbage::Collider::CEngine *Engine, CGameplayManager *GameplayManager /* For enemy handling */) {

}

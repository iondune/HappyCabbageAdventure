#include "CPFriends.h"

CPFriends::CPFriends(float nx, float ny, int type)
{
   x = nx; y = ny;
   Speed = 0;
   Range = 0;
   isMovingPlatform = 0;
}

void CPFriends::writeXML(xmlwriter *l) {
    std::stringstream xValue, yValue, widthValue, heightValue, tagValue, type;
    xValue << x;
    yValue << y;
    widthValue << w;
    heightValue << h;
    type << t;

    tagValue << "CPFriends";

    l->AddAtributes("type ", type.str());
    l->AddAtributes("Y ", yValue.str());
    l->AddAtributes("X ", xValue.str());
    l->Createtag(tagValue.str());
    l->CloseLasttag();
}
void CPFriends::moveTo(float x,float y) {
   //For usage in LWIB
}

CMeshSceneObject * CPFriends::setupItem(CShader * shader, CShader * dShader, Cabbage::Collider::CEngine *Engine, CGameplayManager *GameplayManager /* For enemy handling */) {
    return NULL;
}

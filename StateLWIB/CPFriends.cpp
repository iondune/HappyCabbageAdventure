#include "CPFriends.h"

CPFriends::CPFriends(float nx, float ny, int type)
{
   x = nx; y = ny;
   Speed = 0;
   Range = 0;
   t = type;
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
    l->AddAtributes("y ", yValue.str());
    l->AddAtributes("x ", xValue.str());
    l->Createtag(tagValue.str());
    l->CloseLasttag();
}
void CPFriends::moveTo(float x,float y) {
   //For usage in LWIB
}

CMeshSceneObject * CPFriends::setupItem(CShader * shader, CShader * dShader, Cabbage::Collider::CCollisionEngine *Engine, CGameplayManager *GameplayManager /* For enemy handling */) {

    if (t == 1)
        CFriends::makeFriend((float)x, (float)y, 1, 1, CFriends::Derp, GameplayManager);
    else
        printf("unknown friend type\n");
    
    return NULL;
}

#include "CEnemy.h"
#include "../Base/CBadGuy.h"

CEnemy::CEnemy(float nx, float ny, int width, int height, int type )
{
   num = type;
   x = nx; y = ny;
   w = width; h = height;
   isMovingPlatform = 0;
}

void CEnemy::writeXML(xmlwriter *l) {
    std::stringstream xValue, yValue, widthValue, heightValue, tagValue, eType;
    xValue << x;
    yValue << y;
    widthValue << w;
    heightValue << h;
    tagValue << "CEnemy";
    eType << num;
    //put code for type
    // 1: apple 2: orange
    l->AddAtributes("type ", eType.str());
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
    if (num == 0)
        CBadGuy::makeBadGuy(x, y, w, h, CBadGuy::apple, GameplayManager);
    else if (num == 1)
        CBadGuy::makeBadGuy(x, y, w, h, CBadGuy::orange, GameplayManager);
    else
        printf("Unknown enemy type received.\n");
    return new CMeshRenderable();
}

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
    CBadGuy::makeBadGuy(x, y, w, h, CBadGuy::orange, GameplayManager);

    return new CMeshRenderable();
}

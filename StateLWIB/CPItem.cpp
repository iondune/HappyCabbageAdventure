#include "CPItem.h"

CPItem::CPItem(float nx, float ny, int type)
{
   x = nx; y = ny;
   t = type;
   isMovingPlatform = 0;
}

void CPItem::writeXML(xmlwriter *l) {
    std::stringstream xValue, yValue, isMovingValue, itemType, tagValue;
    xValue << x;
    yValue << y;
    itemType << t;
    isMovingValue << isMovingPlatform;

    tagValue << "CPItem";

    l->AddAtributes("isMoving ", isMovingValue.str());
    l->AddAtributes("texture ", itemType.str());
    l->AddAtributes("Y ", yValue.str());
    l->AddAtributes("X ", xValue.str());
    l->Createtag(tagValue.str());
    l->CloseLasttag();
}
void CPItem::moveTo(float x,float y) {
   //For usage in LWIB
}

CMeshSceneObject * CPItem::setupItem(CShader * shader, CShader * dShader, Cabbage::Collider::CEngine *Engine, CGameplayManager *GameplayManager) {
    if (t == 0) // health
        CItem::makeItem(x, y, 1, 1, CItem::health, GameplayManager);
    else if (t == 1) // energy
        CItem::makeItem(x, y, 1, 1, CItem::energy, GameplayManager);
    else if (t == 2) // life
        CItem::makeItem(x, y, 1, 1, CItem::life, GameplayManager);
    else if (t == 3) // powerup
        CItem::makeItem(x, y, 1, 1, CItem::life, GameplayManager);
    else 
        printf("item not found");
    return new CMeshSceneObject();
}
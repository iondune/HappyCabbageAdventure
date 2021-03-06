#include "CPItem.h"

CPItem::CPItem(float nx, float ny, int type)
{
   x = nx; y = ny;
   t = type;
   w = 1;
   h = 1;
   isMovingPlatform = 0;
}

void CPItem::writeXML(xmlwriter *l) {
    std::stringstream xValue, yValue, isMovingValue, itemType, tagValue;
    xValue << Area.Position.X;
    yValue << Area.Position.Y;
    itemType << Type;
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

CMeshSceneObject * CPItem::setupItem(CShader * shader, CShader * dShader, CCollisionEngine *Engine, CGameplayManager *GameplayManager) {
    if (t == 0) // health
        CItem::makeItem(x, y, 1, 1, CItem::health, GameplayManager);
    else if (t == 1) // energy
        CItem::makeItem(x, y, 1, 1, CItem::energy, GameplayManager);
    else if (t == 2) // life
        CItem::makeItem(x, y, 1, 1, CItem::life, GameplayManager);
    else if (t == 3) // seeds
        CItem::makeItem(x, y, 1, 1, CItem::seed, GameplayManager);
    else 
        printf("item not found");
    return new CMeshSceneObject();
}

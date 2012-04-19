#include "CEnemy.h"
#include "../Base/CBadGuy.h"

CEnemy::CEnemy(float nx, float ny, int width, int height, int type, int env)
{
   num = type;
   x = nx; y = ny;
   w = width; h = height;
   isMovingPlatform = 0;

   this->env = env;
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

CMeshSceneObject * CEnemy::setupItem(CShader * chad, CShader * dShader, Cabbage::Collider::CEngine *Engine, CGameplayManager *GameplayManager) {
    if (num == 0)
       CBadGuy::makeBadGuy(x, y, (float) w, (float) h, CBadGuy::apple, GameplayManager, env);
    else if (num == 1)
       CBadGuy::makeBadGuy(x, y, (float) w, (float) h, CBadGuy::orange, GameplayManager, env);
    else if (num == 2)
       CBadGuy::makeBadGuy(x, y, (float) w, (float) h, CBadGuy::kiwi, GameplayManager, env);
    else if (num == 3)
       CBadGuy::makeBadGuy(x, y, (float) w, (float) h, CBadGuy::grape, GameplayManager, env);
    else if (num == 4)
       CBadGuy::makeBadGuy(x, y, (float) w, (float) h, CBadGuy::flame, GameplayManager, env);
    else if (num == 5)
       CBadGuy::makeBadGuy(x, y, 2.0f, 2.0f/*(float)w, (float)h*/, CBadGuy::blade, GameplayManager, env);
    else
        fprintf(stderr, "Unknown enemy type received.\n");

    //BS return.  Either need to make useful or make this void.
    return new CMeshSceneObject();
}

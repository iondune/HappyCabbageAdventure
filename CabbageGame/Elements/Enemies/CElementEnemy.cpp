#include "CElementEnemy.h"

//Generic enemy, for usage in the LWIB, I guess.
CElementEnemy::CElementEnemy(SRect2 nArea, int type)
: CGameplayElement(PhysicsEngineObject, SceneObject, nArea), Type(type) {
}

//Enemy created by factory
CElementEnemy *CEnemyLoader::LoadEnemy(SRect2 nArea, int type, int texture) {
   switch(type) {
   case 0:
      return NULL;//new CElementEnemyApple(nArea, type);
      break;
   case 1:
      return NULL;//new CElementEnemyOrange(nArea, type);
      break;
   case 2:
      return NULL;//new CElementEnemyKiwi(nArea, type);
      break;
   case 3:
      return NULL;//new CElementEnemyGrape(nArea, type);
      break;
   case 4:
      return NULL;//new CElementEnemyFlame(nArea, type);
      break;
   case 5:
      return NULL;//new CElementEnemyBlade(nArea, type);
      break;
   default:
      fprintf(stderr, "Error: unknown enemy type %d\n", type);
      exit(1);
      break;
   }
}

void CElementEnemy::writeXML(xmlwriter *l) {
    std::stringstream xValue, yValue, widthValue, heightValue, tagValue, eType;
    xValue << Area.Position.X;
    yValue << Area.Position.Y;
    widthValue << Area.Size.X;
    heightValue << Area.Size.Y;
    tagValue << "CEnemy";
    eType << Type;
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

void CElementEnemy::OnCollision(CCollideable *Object, CCollideable *With) {
   fprintf(stderr, "Error: collision on generic enemy type %d (perhaps the CElementEnemy::OnCollision function wasn't overridden?).\n", Type);
   exit(1);
}

void CElementEnemy::update(float f) {
   fprintf(stderr, "Error: update on generic enemy type %d (perhaps the CElementEnemy::update function wasn't overridden?).\n", Type);
   exit(1);
}

void CElementEnemy::setupObjects() {
   fprintf(stderr, "Error: setupObjects on generic enemy type %d (perhaps the CElementEnemy::setupObjects function wasn't overridden?).\n", Type);
   exit(1);
}

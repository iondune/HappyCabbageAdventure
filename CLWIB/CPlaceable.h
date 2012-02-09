#ifndef __CPLACEABLE_H____
#define __CPLACEABLE_H____
#include <sstream>
#include <string>
#include "../CabbageScene/CabbageScene.h"
#include "../CabbageFramework/CabbageFramework.h"
#include "../base/CGameplayManager.h"
#include "../CabbageCollider/CEngine.h"
#include "../CabbageCollider/CObject.h"
#include "../CabbageCollider/CActor.h"

class CPlaceable {
   public:
   float x,y;
   int w,h;

      virtual void printXML() =0;
      virtual void moveTo(float,float) =0;
      virtual std::string tag();
      virtual CMeshRenderable* setupItem(CShader*, Cabbage::Collider::CEngine*, CGameplayManager*);
      std::string getX();
      std::string getY();
      std::string getWidth();
      std::string getHeight();
};
#endif

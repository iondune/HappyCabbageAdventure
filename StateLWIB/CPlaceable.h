#ifndef __CPLACEABLE_H____
#define __CPLACEABLE_H____

#include <stdio.h>

#include "../CabbageScene/CabbageScene.h"
#include "../CabbageFramework/CabbageFramework.h"
#include "../Base/CGameplayManager.h"
#include "../CabbageCollider/CCollisionEngine.h"
#include "../CabbageCollider/CCollisionObject.h"
#include "../CabbageCollider/CActor.h"
#include "./wmlwriter_src/xmlwriter.h"
#include <sstream>
#include <string>


class CPlaceable {
   public:
      float x,y;
      int w,h;
      int isMovingPlatform;

      virtual void writeXML(xmlwriter *) =0;
      virtual void moveTo(float,float) =0;
      virtual CMeshSceneObject* setupItem(CShader*, CShader*, CCollisionEngine*, CGameplayManager*)=0;

};
#endif

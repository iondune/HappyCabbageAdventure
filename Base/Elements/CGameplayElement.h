#ifndef ___CGAMEPLAYELEMENT__HEADER
#define ___CGAMEPLAYELEMENT__HEADER

#include "IUpdater.h"
#include "CCollideable.h"
#include "CEngine.h"
#include "CabbageScene.h"

#include "./wmlwriter_src/xmlwriter.h"
using namespace Cabbage::Collider;

class CGameplayElement : public IUpdater, public ICollisionResponder {
   private:
      CCollideable* PhysicsEngineObject;
      ISceneObject* SceneObject;
      float x, y, w, h;

   public:
      //Functions for CGameplayManager 
      virtual void update(float);
      virtual bool OnCollision()=0;

      //Functions for CLWIBState
      virtual void writeXML(xmlwriter *)=0;
};

#endif

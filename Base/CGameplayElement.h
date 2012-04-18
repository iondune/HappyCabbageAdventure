#ifndef ___CGAMEPLAYELEMENT__HEADER
#define ___CGAMEPLAYELEMENT__HEADER

#include "IUpdater.h"
#include "CCollideable.h"
#include "CEngine.h"
#include "CabbageScene.h"

using namespace Cabbage::Collider;

class CGameplayElement : public IUpdater, public ICollisionResponder {
   private:
      CCollideable* PhysicsEngineObject;
      ISceneObject* SceneObject;

   public:
      virtual void update(float);
      virtual bool OnCollision()=0;
};

#endif

#ifndef ___CGAMEPLAYELEMENT__HEADER
#define ___CGAMEPLAYELEMENT__HEADER

#include "IUpdater.h"
#include "CCollideable.h"
#include "CEngine.h"
#include "CabbageScene.h"

class CGameLevel;
#include "./wmlwriter_src/xmlwriter.h"
using namespace Cabbage::Collider;

class CGameplayElement : public IUpdater, public ICollisionResponder {
   private:
      CCollideable *& PhysicsEngineObject;
      //A top-level object in the scene manager. Subclasses may create new pointers of a more specific subclass.
      ISceneObject *& SceneObject;
      //Make sure level in CGameState is set before initializing any CGameplayElements
      CGameLevel & Level;
      SRect2 Area;

   public:
      //Functions for CGameplayManager 
      virtual void update(float);
      virtual void setupObjects()=0;
      virtual void OnCollision(CCollideable *Object, CCollideable *With)=0;

      //Functions for CLWIBState
      virtual void writeXML(xmlwriter *)=0;
      CGameplayElement(CCollideable * const c, ISceneObject * const s);
      virtual ~CGameplayElement();
};

#endif

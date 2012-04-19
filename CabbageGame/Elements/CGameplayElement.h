#ifndef ___CGAMEPLAYELEMENT__HEADER
#define ___CGAMEPLAYELEMENT__HEADER

#include "IUpdater.h"
#include "CCollideable.h"
#include "CCollisionEngine.h"
#include "CabbageScene.h"

class CGameLevel;
#include "./wmlwriter_src/xmlwriter.h"
#include <sstream>
#include <string>

class CGameplayElement : public IUpdater, public ICollisionResponder {
   protected:
      CCollideable *& PhysicsEngineObject;
      //A top-level object in the scene manager. Subclasses may create new pointers of a more specific subclass.
      ISceneObject *& SceneObject;
      //If CGameLevelLoader isn't being used to load a level, this value will be wrong when created.
      CGameLevel & Level;
      SRect2 Area;

   public:
      //Functions for CGameplayManager 
      //virtual void update(float)=0;
      virtual void setupObjects()=0;
      virtual void OnCollision(CCollideable *Object, CCollideable *With)=0;

      //Functions for CLWIBState
      virtual void writeXML(xmlwriter *)=0;
      CGameplayElement(CCollideable * const c, ISceneObject * const s, SRect2 a);
};

#endif

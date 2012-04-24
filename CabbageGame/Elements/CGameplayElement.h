#ifndef ___CGAMEPLAYELEMENT__HEADER
#define ___CGAMEPLAYELEMENT__HEADER

#include "IUpdater.h"
#include "CCollideable.h"
#include "CCollisionEngine.h"
#include "CabbageScene.h"
#include "Player/CPlayerAbility.h"

class CElementPlayer;
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
      float ElapsedTime;
      bool Dead;

   public:
      //Functions for CGameplayManager 
      virtual void update(float); //By default it just updates both objects, but can be overridden
      void setupObjects();
      virtual void setupPhysicsEngineObject()=0;
      virtual void setupSceneObject()=0;
      virtual void OnCollision(CCollideable *Object)=0;

      //Functions for subclasses
      virtual void updatePhysicsEngineObject(float)=0;
      virtual void updateSceneObject(float)=0;

      //Functions for CLWIBState
      virtual void writeXML(xmlwriter *)=0;

      virtual void removeFromEngines();
      virtual void reactToAbility(Abilities::EAbilityType ability); 

      //Constructor
      CGameplayElement(CCollideable *& c, ISceneObject *& s, SRect2 a);
      SRect2 & getArea();

      //Function for debugging
      virtual void printInformation();

      CCollideable *& getPhysicsEngineObject();
};

#endif

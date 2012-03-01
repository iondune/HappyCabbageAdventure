#ifndef _______CITEM_H_
#define _______CITEM_H_

#include "../CabbageScene/CabbageScene.h"
#include "../CabbageFramework/CabbageFramework.h"
#include "../CabbageCollider/CEngine.h"
#include "../CabbageSound/sound.h"

class CGameplayManager;

class CItem {
public:

   enum ItemType {
      health,
      energy,
      life,
      powerup
   };

   float x, y, w, h;
   ItemType Type;
   CGameplayManager* Manager;
   Cabbage::Collider::CActor* Actor;
   CMeshSceneObject* Renderable;
   //Currently the Shader stuff appears to be acting up.
   CShader* Shader;

   void setShader(CShader* shader);
   static void makeItem(float x, float y, float w, float h, ItemType item, CGameplayManager* manager);

   //Methods that must be implemented in subclasses
   virtual void update(float const TickTime) =0;
   virtual void loadMesh() =0;
   virtual void loadActor() =0;
   virtual void doRenderable();
};

#endif /* CITEM_H_ */

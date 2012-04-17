#ifndef _______CFRIENDS_H_
#define _______CFRIENDS_H_

#include "../CabbageScene/CabbageScene.h"
#include "../CabbageFramework/CabbageFramework.h"
#include "../CabbageCollider/CEngine.h"
#include "../CabbageSound/sound.h"
#include "../CabbageGUI/CabbageGUI.h"
#include "CCollideable.h"

class CGameplayManager;

class CFriends {
public:

   enum FriendType {
      Derp,
      Patatoes,
      broccoli
   };

   float x, y, w, h;
   FriendType Type;
   CGameplayManager* Manager;
   Cabbage::Collider::CActor* Actor;
   CMeshSceneObject* Renderable;
   //Currently the Shader stuff appears to be acting up.
   CShader* Shader;

   void setShader(CShader* shader);
   static void makeFriend(float x, float y, float w, float h, FriendType item, CGameplayManager* manager);

   //Methods that must be implemented in subclasses
   virtual void update(float const TickTime) =0;
   virtual void loadMesh() =0;
   virtual void loadActor() =0;
   virtual void doRenderable();
};

#endif /* CITEM_H_ */

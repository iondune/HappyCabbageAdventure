#ifndef __CFLAG_H____
#define __CFLAG_H____

#include "../CGameplayElement.h"
#include "CElementBlock.h"


class CElementBlockFlag : public CElementBlock {
   public:
      enum FlagRole {
         NEXT         = 0,
         SECRET      = 1
      };
   private:
      FlagRole Role;

   public:
      virtual void OnCollision(const SCollisionEvent& Event);
      virtual void setupPhysicsEngineObject();
      virtual void setupSceneObject();
      virtual void writeXML(xmlwriter *);

      CElementBlockFlag(SRect2f, FlagRole);
      CElementBlockFlag(SRect2f nArea, int role); //So we don't need a block loader

      virtual void printInformation();
      CElementBlockFlag::FlagRole getRole();
};

#endif

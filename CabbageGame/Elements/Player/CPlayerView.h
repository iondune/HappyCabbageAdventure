#ifndef CPLAYERVIEW_HEADER_GUARD
#define CPLAYERVIEW_HEADER_GUARD

#include "CElementPlayer.h"
//#include "../ISquishable.h"

#define NUM_CABBAGE_MESH_STATES 5

class CGameLevel;
class CParticleEngine;
class CPlayerView {
   private:
      ISceneObject *SceneObject; //Contains the next 4 things listed here
      CCollisionActor *PhysicsEngineObject;

      ISceneObject *CabbageSceneObject;
      ISceneObject *NormalCabbageContainer, *HurtCabbageContainer; //Contain all of the cabbage meshes
      CMeshSceneObject *ShadowLeft, *ShadowRight;

      CParticleEngine *Bubbles;

      int CabbageIndex; //Current index of renderables array
      CMeshSceneObject *HurtCabbageRenderables[NUM_CABBAGE_MESH_STATES];
      CMeshSceneObject *NormalCabbageRenderables[NUM_CABBAGE_MESH_STATES];

      //Attributes relating to the player
      bool Hurt, UseCamera;
      int UseSubView;
      float tiltValue;
      CElementPlayer::EDirection & Direction;
      CElementPlayer::EAction & Action;
      SRect2f & Area;
      float LeftShadowCutoffValue, LeftShadowStartValue, RightShadowCutoffValue, RightShadowStartValue;
      float yLeftShadow, yRightShadow, cutOffPoint;
      float ySineValue;
      CPointLightSceneObject *Light;

      SVector3f & ShakeFactor;

     SVector2f CurrentCameraPosition, TargetCameraPosition;
     SVector2f CurrentLookPosition, TargetLookPosition;

     void updateCameraPosition(float const ElapsedTime);

   public:
      void updateShadow(float time = 0);
      void setCabbageScale(SVector3f);
      void setCutoffPoint(SRect2f left, SRect2f right);
      void setShadowHeights(float, float);
      void addLeaf();
      void removeLeaf();
      void setHurt(bool);
      void setVisible(bool);
      void updateView(float time);
      void translateCabbage(float time);
      void setTilt(float t);
      ISceneObject & getCabbageSceneObject();
      CPlayerView(ISceneObject *, CElementPlayer::EDirection &, CElementPlayer::EAction &, int, SRect2f &, SVector3f & sf, CCollisionActor *, bool uC, CGameLevel &Level);

      int getSubView();
      void useSubView(int subview);
};

#endif

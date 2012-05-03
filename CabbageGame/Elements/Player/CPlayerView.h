#ifndef CPLAYERVIEW_HEADER_GUARD
#define CPLAYERVIEW_HEADER_GUARD

#include "CElementPlayer.h"
//#include "../ISquishable.h"

#define NUM_CABBAGE_MESH_STATES 5

class CPlayerView {
   private:
      ISceneObject *SceneObject; //Contains the next 4 things listed here
      CCollisionActor *PhysicsEngineObject;

      ISceneObject *CabbageSceneObject;
      ISceneObject *NormalCabbageContainer, *HurtCabbageContainer; //Contain all of the cabbage meshes
      CMeshSceneObject *ShadowLeft, *ShadowRight;

      int CabbageIndex; //Current index of renderables array
      CMeshSceneObject *HurtCabbageRenderables[NUM_CABBAGE_MESH_STATES];
      CMeshSceneObject *NormalCabbageRenderables[NUM_CABBAGE_MESH_STATES];

      //Attributes relating to the player
      bool Hurt;
      CElementPlayer::EDirection & Direction;
      CElementPlayer::EAction & Action;
      SRect2 & Area;
      float LeftShadowCutoffValue, LeftShadowStartValue, RightShadowCutoffValue, RightShadowStartValue;
      float yLeftShadow, yRightShadow, cutOffPoint;
      float ySineValue;

      SVector3 & ShakeFactor;

	  SVector2 CurrentCameraPosition, TargetCameraPosition, CameraVelocity;

	  void updateCameraPosition(float const ElapsedTime);

   public:
      void updateShadow(float time = 0);
      void setCabbageScale(SVector3);
      void setCutoffPoint(SRect2 left, SRect2 right);
      void setShadowHeights(float, float);
      void addLeaf();
      void removeLeaf();
      void setHurt(bool);
      void setVisible(bool);
      void updateView(float time);
      void translateCabbage(float time);
      ISceneObject & getCabbageSceneObject();
      CPlayerView(ISceneObject *, CElementPlayer::EDirection &, CElementPlayer::EAction &, int, SRect2 &, SVector3 & sf, CCollisionActor *);
};

#endif

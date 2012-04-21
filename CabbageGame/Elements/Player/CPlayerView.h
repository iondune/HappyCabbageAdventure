#ifndef CPLAYERVIEW_HEADER_GUARD
#define CPLAYERVIEW_HEADER_GUARD

#include "CElementPlayer.h"

#define NUM_CABBAGE_MESH_STATES 5

class CPlayerView {
   private:
      ISceneObject *SceneObject; //Contains the next 4 things listed here

      ISceneObject *NormalCabbageContainer, *HurtCabbageContainer; //Contain all of the cabbage meshes
      CMeshSceneObject *ShadowLeft, *ShadowRight;

      int CabbageIndex; //Current index of renderables array
      CMeshSceneObject *HurtCabbageRenderables[NUM_CABBAGE_MESH_STATES];
      CMeshSceneObject *NormalCabbageRenderables[NUM_CABBAGE_MESH_STATES];

      //Attributes relating to the player
      bool Hurt;
      CElementPlayer::EDirection & Direction;
      CElementPlayer::EAction & Action;
      float LeftShadowCutoffValue, LeftShadowStartValue, RightShadowCutoffValue, RightShadowStartValue; 

   public:
      void addLeaf();
      void removeLeaf();
      CPlayerView(ISceneObject *, CElementPlayer::EDirection &, CElementPlayer::EAction &, int);
};

#endif

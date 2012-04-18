#ifndef ___CDECORMANAGER______INCLUDE_FILE_H_STUFF
#define ___CDECORMANAGER______INCLUDE_FILE_H_STUFF

#include "IUpdater.h"

#include "CabbageScene.h"
#include "CApplication.h"

#include <stdio.h>

class CGroundBlock {
   private:
   public:
      float x, y, w, h, d;
      CGroundBlock(float nx, float ny, float nw, float nh, float nd);
};

class CDecorManager : public IUpdater {
   private:
      ISceneObject *setupObject(float,float,float,float,CMesh*);
      void GenerateForestPlants(CGroundBlock*);
      void GenerateDesertPlants(CGroundBlock*, bool genTree);

      bool oldFern;
      //Chris wtf is all this!
      CMesh *basicTreeMesh, *cabbageMesh, *cabbageDamage, *christmasTreeMesh, *cubeMesh, *discMesh,
            *blueFlwrMesh, *whiteFlwrMesh, *ficusMesh, *fernMesh, *enemyMesh, *flagMesh, *derpMesh,
            *purpleFlwrMesh, *yellowFlwrMesh, *tealFlwrMesh, *whiteSunflwrMesh, *cactus1Mesh,
            *cactusBushMesh, *cactusBush2Mesh, *cactus2Mesh;

      CShader *Shader, *Flat, *Diffuse, *BlackShader, *DiffuseTexture, *normalColor, *Toon, *ToonTexture, *ToonBright;
      CShader *DeferredToon, *DeferredTexture, *DeferredDiffuse, *DeferredToonBright, *DeferredToonTexture , *DeferredFlat;

      void PrepMeshes();
      void PrepShaders();

   public:
      CDecorManager(std::vector<CGroundBlock*>, int);
      void update(float);
};

#endif

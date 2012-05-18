#ifndef ___CDECORMANAGER______INCLUDE_FILE_H_STUFF
#define ___CDECORMANAGER______INCLUDE_FILE_H_STUFF

#include "IUpdater.h"
class CParticleEngine;

class ISceneObject;
class CMesh;
class CShader;

#include <stdio.h>

class CGameLevel;

class CGroundBlock {
   private:
   public:
      float x, y, w, h, d;
      CGroundBlock(float nx, float ny, float nw, float nh, float nd);
      void SetupSky();
};

class CDecorManager : public IUpdater {
   private:
      int env;
      bool oldFern, night;
      CParticleEngine *StarEngine;

      //These functions setup and add things to the scene manager
      ISceneObject *SetupObject(float,float,float,float,CMesh*);
      void GenerateForestPlants(CGroundBlock*);
      void GenerateDesertPlants(CGroundBlock*, bool genTree);
      void SetupSky();
      void SetupClouds();

      //These functions initialize the following lists of attributes. Yuck!
      void PrepMeshes();
      void PrepShaders();

      CMesh *basicTreeMesh, *cabbageMesh, *cabbageDamage, *christmasTreeMesh, *cubeMesh, *discMesh,
            *blueFlwrMesh, *whiteFlwrMesh, *ficusMesh, *fernMesh, *enemyMesh, *flagMesh, *derpMesh,
            *purpleFlwrMesh, *yellowFlwrMesh, *tealFlwrMesh, *whiteSunflwrMesh, *cactus1Mesh,
            *cactusBushMesh, *cactusBush2Mesh, *cactus2Mesh;

      CShader *Shader, *Flat, *Diffuse, *BlackShader, *DiffuseTexture, *normalColor, *Toon, *ToonTexture, *ToonBright;
      CShader *DeferredToon, *DeferredTexture, *DeferredDiffuse, *DeferredToonBright, *DeferredToonTexture , *DeferredFlat;
   public:
      CDecorManager(CGameLevel& level);
      void update(float);
};

#endif

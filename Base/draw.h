#ifndef __DRAW_HEADER_
#define __DRAW_HEADER_
#include "../CabbageCore/SVector3.h"

//Variables need to create VBOs of meshes, textures, and shaders
CShader *Shader, *Flat, *Diffuse, *BlackShader, *DiffuseTexture, *normalColor, *Toon, *ToonTexture, *ToonBright;  //Use Diffuse for trees (doesn't need texture)

CShader * DeferredToon, * DeferredTexture, * DeferredDiffuse, * DeferredToonBright, * DeferredToonTexture , *DeferredFlat;
CImage *skyImg, *flagImg, *cactusImg;

CTexture *skyTxt, *flagTxt, *cactusTxt;

CMesh *basicTreeMesh, *cabbageMesh, *cabbageDamage, *christmasTreeMesh, *cubeMesh, *discMesh,
      *blueFlwrMesh, *whiteFlwrMesh, *ficusMesh, *fernMesh, *enemyMesh, *flagMesh, *derpMesh,
      *purpleFlwrMesh, *yellowFlwrMesh, *tealFlwrMesh, *whiteSunflwrMesh, *cactusMesh,
      *cactusBushMesh;

CMesh *mCab0[2], *mCab1[2], *mCab2[2], *mCab3[2], *mCab4[2], *mCab5[2],
      *mCabOw0[2], *mCabOw1[2], *mCabOw2[2], *mCabOw3[2], *mCabOw4[2], *mCabOw5[2];

CMeshSceneObject *renderWinCabbage, *playerRenderable, *tempRender, *renderFlag, *flagLogo, *renderSecretFlag, *flagLogo2, *renderDerp;

ISceneObject *cabbage5, *cabbage4, *cabbage3, *cabbage2, 
  *cabbage1, *cabbage0, *cabbageHurt5, *cabbageHurt4, *cabbageHurt3, *cabbageHurt2,
  *cabbageHurt1, *cabbageHurt0;

std::vector<ISceneObject *> LODObjects[2];



CPointLightSceneObject * playerLight2;

void drawModel(float x, float y, float z, float scale, CApplication app, CMesh* model) {

   if (y == -1) {
      y = scale / 2.0f;
   }

   CMeshSceneObject *render;

   render = new CMeshSceneObject();
   render->setMesh(model);
   render->setShader(ERP_DEFAULT, Toon);
   render->setShader(ERP_DEFERRED_OBJECTS, DeferredToon);
   render->setTranslation(SVector3(x, y, z));
   render->setScale(SVector3(scale));
   render->setRotation(SVector3(-90, 0, 0));

   if (model == fernMesh) {
      render->setRotation(SVector3(-90, 0, -115));
   }
   else if (model == whiteFlwrMesh) {
      render->setRotation(SVector3(-90, 0, -80));
   }
   else if (model == cactusMesh) {
      render->setShader(ERP_DEFAULT, ToonTexture);
      render->setTexture(cactusTxt);
      render->setShader(ERP_DEFERRED_OBJECTS, DeferredToonTexture);

      render->setRotation(SVector3(-90, 0, rand()%179 - 90));
   }

   //if (model == fernMesh || model == whiteFlwrMesh)
   //else
   //render->setRotation(SVector3(-90, 0, 0));

   app.getSceneManager().addImmobileSceneObject(render, THIS_OBJECT_WILL_NEVER_MOVE_AND_ITS_BOUNDING_BOX_IS_CORRECT);
}
#endif

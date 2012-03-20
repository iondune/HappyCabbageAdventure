#ifndef __DRAW_HEADER_
#define __DRAW_HEADER_
#include "../CabbageCore/SVector3.h"

//Variables need to create VBOs of meshes, textures, and shaders
CShader *Shader, *Flat, *Diffuse, *BlackShader, *DiffuseTexture, *normalColor, *Toon, *ToonTexture, *ToonBright;  //Use Diffuse for trees (doesn't need texture)

CShader * DeferredToon, * DeferredTexture, * DeferredDiffuse, * DeferredToonBright, * DeferredToonTexture , *DeferredFlat;
CImage *grassImg, *skyImg, *dirtImg, *blueFlwrImg, *whiteFlwrImg, *ficusImg, *flagImg;

CTexture *grassTxt, *skyTxt, *dirtTxt, *blueFlwrTxt, *whiteFlwrTxt, *ficusTxt, *flagTxt;

CMesh *basicTreeMesh, *cabbageMesh, *cabbageDamage, *christmasTreeMesh, *cubeMesh, *discMesh,
      *blueFlwrMesh, *whiteFlwrMesh, *ficusMesh, *fernMesh, *enemyMesh,
      *flagMesh, *derpMesh, *purpleFlwrMesh, *yellowFlwrMesh, *tealFlwrMesh, *whiteSunflwrMesh;

CMesh *mCab0[2], *mCab1[2], *mCab2[2], *mCab3[2], *mCab4[2], *mCab5[2],
      *mCabOw0[2], *mCabOw1[2], *mCabOw2[2], *mCabOw3[2], *mCabOw4[2], *mCabOw5[2];

CMeshSceneObject *renderWinCabbage, *playerRenderable, *renderChristmasTree,
  *renderBasicTree, *renderBlueFlwr, *renderWhiteFlwr, *renderFicus, *renderFern,
  *tempRender, *renderFlag, *flagLogo, *renderDerp, *renderPurpleFlwr, *renderYellowFlwr,
  *renderTealFlwr, *renderWhiteSunflwr;
ISceneObject *cabbage5, *cabbage4, *cabbage3, *cabbage2, 
  *cabbage1, *cabbage0, *cabbageHurt5, *cabbageHurt4, *cabbageHurt3, *cabbageHurt2,
  *cabbageHurt1, *cabbageHurt0;

std::vector<ISceneObject *> LODObjects[2];

std::vector<CMeshSceneObject*> blocks, enemies, blueFlwrs, whiteFlwrs, ficuses,
  basicTrees, xmasTrees, ferns, tealFlwrs, purpleFlwrs, yellowFlwrs, whiteSunflwrs;

CPointLightSceneObject * playerLight2;

void drawYellowFlwr(float x, float y, float z, float scale, CApplication app) {

   if (y == -1) {
      y = scale / 2.0f;
   }

   yellowFlwrs.push_back(tempRender = new CMeshSceneObject());

   renderYellowFlwr = new CMeshSceneObject();
   renderYellowFlwr->setMesh(yellowFlwrMesh);
   renderYellowFlwr->setShader(ERP_DEFAULT, Toon);
   renderYellowFlwr->setShader(ERP_DEFERRED_OBJECTS, DeferredToon);
   renderYellowFlwr->setTranslation(SVector3(x, y, z));
   renderYellowFlwr->setScale(SVector3(scale));
   renderYellowFlwr->setRotation(SVector3(-90, 0, 0));

   //app.getSceneManager().addSceneObject(renderYellowFlwr);
   app.getSceneManager().addImmobileSceneObject(renderYellowFlwr, THIS_OBJECT_WILL_NEVER_MOVE_AND_ITS_BOUNDING_BOX_IS_CORRECT);
}

void drawTealFlwr(float x, float y, float z, float scale, CApplication app) {

   if (y == -1) {
      y = scale / 2.0f;
   }

   tealFlwrs.push_back(tempRender = new CMeshSceneObject());

   renderTealFlwr = new CMeshSceneObject();
   renderTealFlwr->setMesh(tealFlwrMesh);
   renderTealFlwr->setShader(ERP_DEFAULT, Toon);
   renderTealFlwr->setShader(ERP_DEFERRED_OBJECTS, DeferredToon);
   renderTealFlwr->setTranslation(SVector3(x, y, z));
   renderTealFlwr->setScale(SVector3(scale));
   renderTealFlwr->setRotation(SVector3(-90, 0, 0));

   //app.getSceneManager().addSceneObject(renderTealFlwr);
   app.getSceneManager().addImmobileSceneObject(renderTealFlwr, THIS_OBJECT_WILL_NEVER_MOVE_AND_ITS_BOUNDING_BOX_IS_CORRECT);
}

void drawPurpleFlwr(float x, float y, float z, float scale, CApplication app) {

   if (y == -1) {
      y = scale / 2.0f;
   }

   purpleFlwrs.push_back(tempRender = new CMeshSceneObject());

   renderPurpleFlwr = new CMeshSceneObject();
   renderPurpleFlwr->setMesh(purpleFlwrMesh);
   renderPurpleFlwr->setShader(ERP_DEFAULT, Toon);
   renderPurpleFlwr->setShader(ERP_DEFERRED_OBJECTS, DeferredToon);
   renderPurpleFlwr->setTranslation(SVector3(x, y, z));
   renderPurpleFlwr->setScale(SVector3(scale));
   renderPurpleFlwr->setRotation(SVector3(-90, 0, 0));

   //app.getSceneManager().addSceneObject(renderPurpleFlwr);
   app.getSceneManager().addImmobileSceneObject(renderPurpleFlwr, THIS_OBJECT_WILL_NEVER_MOVE_AND_ITS_BOUNDING_BOX_IS_CORRECT);
}

void drawWhiteSunflwr(float x, float y, float z, float scale, CApplication app) {

   if (y == -1) {
      y = scale / 2.0f;
   }

   whiteSunflwrs.push_back(tempRender = new CMeshSceneObject());

   renderWhiteSunflwr = new CMeshSceneObject();
   renderWhiteSunflwr->setMesh(whiteSunflwrMesh);
   renderWhiteSunflwr->setShader(ERP_DEFAULT, Toon);
   renderWhiteSunflwr->setShader(ERP_DEFERRED_OBJECTS, DeferredToon);
   renderWhiteSunflwr->setTranslation(SVector3(x, y, z));
   renderWhiteSunflwr->setScale(SVector3(scale));
   renderWhiteSunflwr->setRotation(SVector3(-90, 0, 0));

   //app.getSceneManager().addSceneObject(renderWhiteSunflwr);
   app.getSceneManager().addImmobileSceneObject(renderWhiteSunflwr, THIS_OBJECT_WILL_NEVER_MOVE_AND_ITS_BOUNDING_BOX_IS_CORRECT);
}

void drawBlueFlwr(float x, float y, float z, float scale, CApplication app) {

   if (y == -1) {
      y = scale / 2.0f;
   }

   blueFlwrs.push_back(tempRender = new CMeshSceneObject());

   renderBlueFlwr = new CMeshSceneObject();
   renderBlueFlwr->setMesh(blueFlwrMesh);
   renderBlueFlwr->setShader(ERP_DEFAULT, Toon);
   renderBlueFlwr->setShader(ERP_DEFERRED_OBJECTS, DeferredToon);
   renderBlueFlwr->setTranslation(SVector3(x, y, z));
   renderBlueFlwr->setScale(SVector3(scale));
   renderBlueFlwr->setRotation(SVector3(-90, 0, 0));

   //app.getSceneManager().addSceneObject(renderBlueFlwr);
   app.getSceneManager().addImmobileSceneObject(renderBlueFlwr, THIS_OBJECT_WILL_NEVER_MOVE_AND_ITS_BOUNDING_BOX_IS_CORRECT);
}

void drawWhiteFlwr(float x, float y, float z, float scale, CApplication app) {

   if (y == -1) {
      y = scale / 2.0f;
   }

   whiteFlwrs.push_back(tempRender = new CMeshSceneObject());

   renderWhiteFlwr = new CMeshSceneObject();
   renderWhiteFlwr->setMesh(whiteFlwrMesh);
   renderWhiteFlwr->setShader(ERP_DEFAULT, ToonBright);
   renderWhiteFlwr->setShader(ERP_DEFERRED_OBJECTS, DeferredToonBright);
   renderWhiteFlwr->setTranslation(SVector3(x, y, z));
   renderWhiteFlwr->setScale(SVector3(scale));
   renderWhiteFlwr->setRotation(SVector3(-90, 0, -80));

   //app.getSceneManager().addSceneObject(renderWhiteFlwr);
   app.getSceneManager().addImmobileSceneObject(renderWhiteFlwr, THIS_OBJECT_WILL_NEVER_MOVE_AND_ITS_BOUNDING_BOX_IS_CORRECT);
}

void drawFern(float x, float y, float z, float scale, CApplication app) {

   if (y == -1) {
      y = scale / 2.0f;
   }

   ferns.push_back(tempRender = new CMeshSceneObject());

   renderFern = new CMeshSceneObject();
   renderFern->setMesh(fernMesh);
   renderFern->setShader(ERP_DEFAULT, Toon);
   renderFern->setShader(ERP_DEFERRED_OBJECTS, DeferredToon);
   renderFern->setTranslation(SVector3(x, y, z));
   renderFern->setScale(SVector3(scale));
   renderFern->setRotation(SVector3(-90, 0, -115));

   //app.getSceneManager().addSceneObject(renderFern);
   app.getSceneManager().addImmobileSceneObject(renderFern, THIS_OBJECT_WILL_NEVER_MOVE_AND_ITS_BOUNDING_BOX_IS_CORRECT);
}

void drawFicus(float x, float y, float z, float scale, CApplication app) {

   if (y == -1) {
      y = scale / 2.0f;
   }

   ficuses.push_back(tempRender = new CMeshSceneObject());

   renderFicus = new CMeshSceneObject();
   renderFicus->setMesh(ficusMesh);
   renderFicus->setShader(ERP_DEFAULT, Toon);
   renderFicus->setShader(ERP_DEFERRED_OBJECTS, DeferredToon);
   renderFicus->setTranslation(SVector3(x, y, z));
   renderFicus->setScale(SVector3(scale));
   renderFicus->setRotation(SVector3(-90, 0, 0));

   //app.getSceneManager().addSceneObject(renderFicus);
   app.getSceneManager().addImmobileSceneObject(renderFicus, THIS_OBJECT_WILL_NEVER_MOVE_AND_ITS_BOUNDING_BOX_IS_CORRECT);
}

void drawBasicTree(float x, float y, float z, float scale, CApplication app) {

   if (y == -1) {
      y = scale / 2.0f;
   }

   basicTrees.push_back(tempRender = new CMeshSceneObject());

   renderBasicTree = new CMeshSceneObject();
   renderBasicTree->setMesh(basicTreeMesh);
   //renderBasicTree->getMaterial().Texture = (basicTreeTxt);
   renderBasicTree->setShader(ERP_DEFAULT, Toon);
   renderBasicTree->setShader(ERP_DEFERRED_OBJECTS, DeferredToon);
   renderBasicTree->setScale(SVector3(scale));
   renderBasicTree->setRotation(SVector3(-90, 0, 0));
   renderBasicTree->setTranslation(SVector3(x, y + scale/4.0f, z));

   //app.getSceneManager().addSceneObject(renderBasicTree);
   app.getSceneManager().addImmobileSceneObject(renderBasicTree, THIS_OBJECT_WILL_NEVER_MOVE_AND_ITS_BOUNDING_BOX_IS_CORRECT);
}

void drawChristmasTree(float x, float y, float z, float scale, CApplication app) {

   if (y == -1) {
      y = scale / 2.0f;
   }

   xmasTrees.push_back(tempRender = new CMeshSceneObject());

   renderChristmasTree = new CMeshSceneObject();
   renderChristmasTree->setMesh(christmasTreeMesh);
   renderChristmasTree->setShader(ERP_DEFAULT, Toon);
   renderChristmasTree->setShader(ERP_DEFERRED_OBJECTS, DeferredToon);
   renderChristmasTree->setTranslation(SVector3(x, y, z));
   renderChristmasTree->setScale(SVector3(scale));
   renderChristmasTree->setRotation(SVector3(-90, 0, 0));

   //app.getSceneManager().addSceneObject(renderChristmasTree);
   app.getSceneManager().addImmobileSceneObject(renderChristmasTree, THIS_OBJECT_WILL_NEVER_MOVE_AND_ITS_BOUNDING_BOX_IS_CORRECT);
}
#endif

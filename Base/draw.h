#ifndef __DRAW_HEADER_
#define __DRAW_HEADER_
#include "../CabbageCore/SVector3.h"

//Variables need to create VBOs of meshes, textures, and shaders
CShader *Shader, *Flat, *Diffuse, *BlackShader, *DiffuseTexture, *normalColor, *Toon, *ToonTexture, *ToonBright;  //Use Diffuse for trees (doesn't need texture)

CImage *grassImg, *skyImg, *dirtImg, *blueFlwrImg, *whiteFlwrImg, *ficusImg, *flagImg;

CTexture *grassTxt, *skyTxt, *dirtTxt, *blueFlwrTxt, *whiteFlwrTxt, *ficusTxt, *flagTxt;

CMesh *basicTreeMesh, *cabbageMesh, *cabbageDamage, *christmasTreeMesh, *cubeMesh, *discMesh,
      *blueFlwrMesh, *whiteFlwrMesh, *ficusMesh, *fernMesh, *enemyMesh,
      *flagMesh, *derpMesh, *purpleFlwrMesh, *yellowFlwrMesh, *tealFlwrMesh, *whiteSunflwrMesh;

CMeshSceneObject *renderWinCabbage, *renderShadow, *playerRenderable, *normalCabbage, *damageCabbage, *renderChristmasTree,
  *renderBasicTree, *renderBlueFlwr, *renderWhiteFlwr, *renderFicus, *renderFern,
  *tempRender, *renderFlag, *flagLogo, *renderDerp, *renderPurpleFlwr, *renderYellowFlwr,
  *renderTealFlwr, *renderWhiteSunflwr;

std::vector<CMeshSceneObject*> blocks, enemies, blueFlwrs, whiteFlwrs, ficuses,
  basicTrees, xmasTrees, ferns, tealFlwrs, purpleFlwrs, yellowFlwrs, whiteSunflwrs;



void drawYellowFlwr(float x, float y, float z, float scale, CApplication app) {

   if (y == -1) {
      y = scale / 2.0f;
   }

   yellowFlwrs.push_back(tempRender = new CMeshSceneObject());

   renderYellowFlwr = new CMeshSceneObject();
   renderYellowFlwr->setMesh(yellowFlwrMesh);
   renderYellowFlwr->setShader(Toon);
   renderYellowFlwr->setTranslation(SVector3(x, y, z));
   renderYellowFlwr->setScale(SVector3(scale));
   renderYellowFlwr->setRotation(SVector3(-90, 0, 0));

   app.getSceneManager().addSceneObject(renderYellowFlwr);
}

void drawTealFlwr(float x, float y, float z, float scale, CApplication app) {

   if (y == -1) {
      y = scale / 2.0f;
   }

   tealFlwrs.push_back(tempRender = new CMeshSceneObject());

   renderTealFlwr = new CMeshSceneObject();
   renderTealFlwr->setMesh(tealFlwrMesh);
   renderTealFlwr->setShader(Toon);
   renderTealFlwr->setTranslation(SVector3(x, y, z));
   renderTealFlwr->setScale(SVector3(scale));
   renderTealFlwr->setRotation(SVector3(-90, 0, 0));

   app.getSceneManager().addSceneObject(renderTealFlwr);
}

void drawPurpleFlwr(float x, float y, float z, float scale, CApplication app) {

   if (y == -1) {
      y = scale / 2.0f;
   }

   purpleFlwrs.push_back(tempRender = new CMeshSceneObject());

   renderPurpleFlwr = new CMeshSceneObject();
   renderPurpleFlwr->setMesh(purpleFlwrMesh);
   renderPurpleFlwr->setShader(Toon);
   renderPurpleFlwr->setTranslation(SVector3(x, y, z));
   renderPurpleFlwr->setScale(SVector3(scale));
   renderPurpleFlwr->setRotation(SVector3(-90, 0, 0));

   app.getSceneManager().addSceneObject(renderPurpleFlwr);
}

void drawWhiteSunflwr(float x, float y, float z, float scale, CApplication app) {

   if (y == -1) {
      y = scale / 2.0f;
   }

   whiteSunflwrs.push_back(tempRender = new CMeshSceneObject());

   renderWhiteSunflwr = new CMeshSceneObject();
   renderWhiteSunflwr->setMesh(whiteSunflwrMesh);
   renderWhiteSunflwr->setShader(Toon);
   renderWhiteSunflwr->setTranslation(SVector3(x, y, z));
   renderWhiteSunflwr->setScale(SVector3(scale));
   renderWhiteSunflwr->setRotation(SVector3(-90, 0, 0));

   app.getSceneManager().addSceneObject(renderWhiteSunflwr);
}

void drawBlueFlwr(float x, float y, float z, float scale, CApplication app) {

   if (y == -1) {
      y = scale / 2.0f;
   }

   blueFlwrs.push_back(tempRender = new CMeshSceneObject());

   renderBlueFlwr = new CMeshSceneObject();
   renderBlueFlwr->setMesh(blueFlwrMesh);
   renderBlueFlwr->setShader(Toon);
   renderBlueFlwr->setTranslation(SVector3(x, y, z));
   renderBlueFlwr->setScale(SVector3(scale));
   renderBlueFlwr->setRotation(SVector3(-90, 0, 0));

   app.getSceneManager().addSceneObject(renderBlueFlwr);
}

void drawWhiteFlwr(float x, float y, float z, float scale, CApplication app) {

   if (y == -1) {
      y = scale / 2.0f;
   }

   whiteFlwrs.push_back(tempRender = new CMeshSceneObject());

   renderWhiteFlwr = new CMeshSceneObject();
   renderWhiteFlwr->setMesh(whiteFlwrMesh);
   renderWhiteFlwr->setShader(ToonBright);
   renderWhiteFlwr->setTranslation(SVector3(x, y, z));
   renderWhiteFlwr->setScale(SVector3(scale));
   renderWhiteFlwr->setRotation(SVector3(-90, 0, -80));

   app.getSceneManager().addSceneObject(renderWhiteFlwr);
}

void drawFern(float x, float y, float z, float scale, CApplication app) {

   if (y == -1) {
      y = scale / 2.0f;
   }

   ferns.push_back(tempRender = new CMeshSceneObject());

   renderFern = new CMeshSceneObject();
   renderFern->setMesh(fernMesh);
   renderFern->setShader(Toon);
   renderFern->setTranslation(SVector3(x, y, z));
   renderFern->setScale(SVector3(scale));
   renderFern->setRotation(SVector3(-90, 0, -115));

   app.getSceneManager().addSceneObject(renderFern);
}

void drawFicus(float x, float y, float z, float scale, CApplication app) {

   if (y == -1) {
      y = scale / 2.0f;
   }

   ficuses.push_back(tempRender = new CMeshSceneObject());

   renderFicus = new CMeshSceneObject();
   renderFicus->setMesh(ficusMesh);
   renderFicus->setShader(Toon);
   renderFicus->setTranslation(SVector3(x, y, z));
   renderFicus->setScale(SVector3(scale));
   renderFicus->setRotation(SVector3(-90, 0, 0));

   app.getSceneManager().addSceneObject(renderFicus);
}

void drawBasicTree(float x, float y, float z, float scale, CApplication app) {

   if (y == -1) {
      y = scale / 2.0f;
   }

   basicTrees.push_back(tempRender = new CMeshSceneObject());

   renderBasicTree = new CMeshSceneObject();
   renderBasicTree->setMesh(basicTreeMesh);
   //renderBasicTree->getMaterial().Texture = (basicTreeTxt);
   renderBasicTree->setShader(Toon);
   renderBasicTree->setScale(SVector3(scale));
   renderBasicTree->setRotation(SVector3(-90, 0, 0));
   renderBasicTree->setTranslation(SVector3(x, y + scale/4.0f, z));

   app.getSceneManager().addSceneObject(renderBasicTree);
}

void drawChristmasTree(float x, float y, float z, float scale, CApplication app) {

   if (y == -1) {
      y = scale / 2.0f;
   }

   xmasTrees.push_back(tempRender = new CMeshSceneObject());

   renderChristmasTree = new CMeshSceneObject();
   renderChristmasTree->setMesh(christmasTreeMesh);
   renderChristmasTree->setShader(Toon);
   renderChristmasTree->setTranslation(SVector3(x, y, z));
   renderChristmasTree->setScale(SVector3(scale));
   renderChristmasTree->setRotation(SVector3(-90, 0, 0));

   app.getSceneManager().addSceneObject(renderChristmasTree);
}
#endif

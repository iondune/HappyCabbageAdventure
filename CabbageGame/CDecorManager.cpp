#include "CDecorManager.h"
#include "CGameLevel.h"

#include "CParticleEngine.h"

CGroundBlock::CGroundBlock(float nx, float ny, float nw, float nh, float nd) : x(nx), y(ny), w(nw), h(nh), d(nd) {
}

ISceneObject *CDecorManager::SetupObject(float x, float y, float z, float scale, CMesh* model) {
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
   else if (model == cactus1Mesh) {
      render->setShader(ERP_DEFAULT, ToonTexture);
      render->setTexture(CImageLoader::loadTexture("Base/cactus.bmp", true));
      render->setShader(ERP_DEFERRED_OBJECTS, DeferredToonTexture);

      render->setRotation(SVector3(-90, 0, rand()%179 - 90.f));
   }
   else if (model == cactus2Mesh || model == cactusBush2Mesh) {
      render->setRotation(SVector3(-90, 0, rand()%179 - 90.f));
   }

   CApplication::get().getSceneManager().addImmobileSceneObject(render, THIS_OBJECT_WILL_NEVER_MOVE_AND_ITS_BOUNDING_BOX_IS_CORRECT);
   return render;
}

void CDecorManager::update(float f) {
   if(StarEngine)
      StarEngine->step(f);

}

CDecorManager::CDecorManager(CGameLevel & level) {
   std::vector<CGroundBlock*> & groundBlocks = level.getGroundBlocks();
   env = level.getEnvironment();
   night = level.isNight();
   PrepShaders();
   PrepMeshes();
   SetupSky();
   StarEngine = NULL;
   if(night) {
      StarEngine = new CParticleEngine(SVector3(-30.0f, 15.0f, 0.0f), 100, -1, STAR_PARTICLE);
      SetupClouds();
      for (int x = -5; x < 200; ++ x)
         for (int y = -0; y < 2; ++ y)
         {
            CPointLightSceneObject * point = new CPointLightSceneObject(1.5f, x % 2 ? SColor(1, 0, 0) : SColor(0, 1, 1));
            point->setTranslation(SVector3(x * 3.f, y * 3.f, 0.f));
            CApplication::get().getSceneManager().addSceneObject(point);
         }
   }
   oldFern = false;

   CGroundBlock *curBlock;
   int drawTree = rand()%2 + 1;
   int NumTreeTypes = 2;
   int NumFlowerTypes = 2;
   for(unsigned int i = 0; i < groundBlocks.size(); i++) {
      curBlock = groundBlocks[i];
      if (env == 0)
         GenerateForestPlants(curBlock);
      else if (env == 1) {
         drawTree--;
         if (drawTree == 0) {
            drawTree = rand()%2 + 1;
            GenerateDesertPlants(curBlock, true);
         }
         else
            GenerateDesertPlants(curBlock, false);
      }
      delete curBlock;
   }
}

void CDecorManager::GenerateDesertPlants(CGroundBlock* block, bool genTree) {
   float x = block->x,
         y = block->y,
         w = block->w,
         h = block->h,
         d = block->d;
   int numForeground, numBackground;
   int plantType;
   float randScale, randDepth, randDist;
   float yVal = y + h;

   if (w > 0.5f && w < 1.5f)  //If block size roughly 1, don't draw any trees
      numForeground = numBackground = 0;
   else { //Calculate the number of plants to draw in the foreground and background
      numForeground = rand()%2 + 1;
      numBackground = rand()%2 + 1;
   }

   //Draw Tree in background
   if (genTree) {
      plantType = rand()%3;
      randDist = w * 0.1f + (rand() / (float)RAND_MAX) * 0.7f * w;  //.1, .7 to make sure doesn't overlap with other randomly drawn cacti
      if (plantType < 2)
         SetupObject(x + randDist, yVal + 1.5f, -d/2.0f + .4f, 5.5f, cactus2Mesh);
      else if (plantType == 2)
         SetupObject(x + randDist, yVal + 1.5f, -d/2.0f + .4f, 5.5f, cactus1Mesh);
   }

   //Draw shrubbery in background
   for (int n = 0; n < numBackground; n++) {
      plantType = rand()%5;

      randScale = (float) (rand()%10);
      randScale = randScale * .2f;

      randDepth = (float) (rand()%2);
      randDepth = (float) randDepth*.25f;

      if (n == 0) {
         randDist = (rand() / (float)RAND_MAX) * 0.4f * w;
      }
      else {
         randDist = w * 0.5f + (rand() / (float)RAND_MAX) * 0.4f * w;
      }

      if (plantType == 0)
         SetupObject(x + randDist, yVal + 1.6f, d/2.0f - 0.6f, 5.5f, cactus2Mesh);
      else if (plantType < 5 && randScale > .5f)
         SetupObject(x + randDist, yVal + randScale/4.5f, d/2.0f - 0.6f, randScale, cactusBush2Mesh);
   }

   for (int n = 0; n < numForeground; n++) {
      plantType = rand()%6;

      randScale = (float) (rand()%20);
      randScale = randScale * .025f;

      randDepth = (float) (rand()%2);
      randDepth = (float) randDepth*0.25f;

      if (n == 0) {
         randDist = w * 0.1f + (rand() / (float)RAND_MAX) * 0.4f * w;
      }
      else {
         randDist += w * 0.4f + (rand() / (float)RAND_MAX) * 0.1f * w;
      }

      if (plantType == 0) {
         printf ("Draw a shrubbery!\n");
      }
   }
}

void CDecorManager::GenerateForestPlants(CGroundBlock* block) {
   float x = block->x,
         y = block->y,
         w = block->w,
         h = block->h,
         d = block->d;
   int numForeground, numBackground;
   int random;
   float randScale, randDepth;
   float div, yVal = y + h;

   if (w > 0.5f && w < 1.5f)  //If block size roughly 1, don't draw any trees
      numForeground = numBackground = 0;
   else {
      numForeground = (int) w / 2;
      numBackground = (int) w / 2;
   }

   div =  w/(float)numBackground;

   //Check how many tree-type objects we should draw in the background
   for (int n = 0; n < numBackground; n++) {
      random = rand()%2;

      if (random == 0)
         SetupObject(x + (n)*div + div/2.0f, yVal + 1.8f, -d/2.0f + .4f, 8.0f, basicTreeMesh);
      else if (random == 1)
         SetupObject(x + (n)*div + div/2.0f, yVal + 1.4f, -d/2.0f + .4f, 6.0f, christmasTreeMesh);
   }

   //Draw flower-type plants in background
   for (int n = 0; n < w; n++) {
      random = rand()%6;
      float subrand = (float) (rand()%5);
      randScale = (float) (rand()%20);
      randScale = randScale * .025f;
      randDepth = (float) (rand()%2);
      randDepth = (float) randDepth*.25f;

      if (random == 0 || random == 1)
         SetupObject(x + n + .5f, yVal + .2f, -d/2.0f + 1.6f + randDepth, .7f, whiteFlwrMesh);
      else if (random == 2 || random == 3 || random == 4) {
         if (subrand == 0)
            SetupObject(x + n + .5f, yVal + .3f, -d/2.0f + 1.5f + randDepth, .7f + randScale, yellowFlwrMesh);
         else if (subrand == 1)
            SetupObject(x + n + .5f, yVal + .3f, -d/2.0f + 1.5f + randDepth, .7f + randScale, blueFlwrMesh);
         else if (subrand == 2)
            SetupObject(x + n + .5f, yVal + .3f, -d/2.0f + 1.5f + randDepth, .7f + randScale, whiteSunflwrMesh);
         else if (subrand == 3)
            SetupObject(x + n + .5f, yVal + .3f, -d/2.0f + 1.5f + randDepth, .7f + randScale, tealFlwrMesh);
         else if (subrand == 4)
            SetupObject(x + n + .5f, yVal + .3f, -d/2.0f + 1.5f + randDepth, .7f + randScale, purpleFlwrMesh);
      }
      else if (random == 5 && !oldFern) {
         SetupObject(x + n + .5f, yVal + .2f, -d/2.0f + 1.5f + randDepth, 1.f + randScale, fernMesh);
         oldFern = true;
      }
      else if (random == 5 && oldFern)
         n--;

      if (random != 5)
         oldFern = false;
   }

   //Draw flower-type plants in foreground
   for (int n = 0; n < w; n++) {
      random = rand()%6;
      float subrand = (float) (rand()%5);
      randScale = (float) (rand()%10);
      randScale = randScale * 0.04f;
      randDepth = (float) (rand()%2);
      randDepth = randDepth*0.25f;

      if (random == 0 || random == 1)
         SetupObject(x + n + 0.5f, yVal + .2f, d/2.0f - 0.6f, 0.5f + randScale/2.0f, whiteFlwrMesh);
      else if (random == 2 || random == 3 || random == 4) {
         if (subrand == 0)
            SetupObject(x + n + 0.5f, yVal + .3f, d/2.0f - 0.6f, 0.4f + randScale, blueFlwrMesh);
         else if (subrand == 1)
            SetupObject(x + n + 0.5f, yVal + .3f, d/2.0f - 0.6f, 0.4f + randScale, tealFlwrMesh);
         else if (subrand == 2)
            SetupObject(x + n + 0.5f, yVal + .3f, d/2.0f - 0.6f + .4f, 0.4f + randScale, purpleFlwrMesh);
         else if (subrand == 3)
            SetupObject(x + n + 0.5f, yVal + .3f, d/2.0f - 0.6f, 0.4f + randScale, yellowFlwrMesh);
         else if (subrand == 3)
            SetupObject(x + n + 0.5f, yVal + .3f, d/2.0f - 0.6f, 0.4f + randScale, whiteSunflwrMesh);
      }
      else if (random == 5 && !oldFern) {
         SetupObject(x + n + 0.5f, yVal + 0.2f, d/2.0f - 0.6f, 0.7f, fernMesh);
         oldFern = true;
      }
      else if (random == 5 && oldFern) {
         n--;
      }

      if (random != 5)
         oldFern = false;
   }
}

void CDecorManager::PrepShaders() {
   DeferredFlat = CShaderLoader::loadShader("Deferred/Diffuse");
   DeferredDiffuse = CShaderLoader::loadShader("Deferred/Diffuse");
   DeferredToonTexture = CShaderLoader::loadShader("Deferred/Textured");
   DeferredTexture = CShaderLoader::loadShader("Deferred/Textured");
   DeferredToon = CShaderLoader::loadShader("Deferred/Diffuse");
   DeferredToonBright = CShaderLoader::loadShader("Deferred/Diffuse");

   Flat = CShaderLoader::loadShader("Diffuse");
   Diffuse = CShaderLoader::loadShader("Diffuse");
   ToonTexture = CShaderLoader::loadShader("ToonTexture");
   DiffuseTexture = CShaderLoader::loadShader("DiffuseTextureBright");
   normalColor = CShaderLoader::loadShader("Simple");
   Toon = CShaderLoader::loadShader("Toon");
   ToonBright = CShaderLoader::loadShader("ToonBright");
   BlackShader = CShaderLoader::loadShader("SpecialDiscThing");
}

void CDecorManager::PrepMeshes()
{
   //Forest Meshes
   basicTreeMesh = CMeshLoader::load3dsMesh("Base/tree4.3ds");
   if (basicTreeMesh) {
      basicTreeMesh->resizeMesh(SVector3(0.5));
      basicTreeMesh->centerMeshByExtents(SVector3(0));
      basicTreeMesh->calculateNormalsPerVertex();
   }
   else {
      fprintf(stderr, "Failed to load the basic tree mesh\n");
   }

   christmasTreeMesh = CMeshLoader::load3dsMesh("Base/christmasTree3.3ds");
   if (christmasTreeMesh) {
      christmasTreeMesh->resizeMesh(SVector3(0.5));
      christmasTreeMesh->centerMeshByExtents(SVector3(0));
      christmasTreeMesh->calculateNormalsPerFace();
   }
   else {
      fprintf(stderr, "Failed to load the christmas tree mesh\n");
   }

   blueFlwrMesh = CMeshLoader::load3dsMesh("Base/simpleflower1.3ds");
   if (blueFlwrMesh) {
      blueFlwrMesh->centerMeshByExtents(SVector3(0));
      blueFlwrMesh->calculateNormalsPerFace();
   }
   else {
      fprintf(stderr, "Failed to load blue flower mesh.\n");
   }

   whiteFlwrMesh = CMeshLoader::load3dsMesh("Base/simpleflower2.3ds");
   if (whiteFlwrMesh) {
      whiteFlwrMesh->centerMeshByExtents(SVector3(0));
      whiteFlwrMesh->resizeMesh(SVector3(.8f));
      whiteFlwrMesh->calculateNormalsPerFace();
   }
   else
      fprintf(stderr, "Failed to load white flower mesh.\n");

   whiteSunflwrMesh = CMeshLoader::load3dsMesh("Base/sunflowerwhite.3ds");
   if (whiteSunflwrMesh) {
      whiteSunflwrMesh->centerMeshByExtents(SVector3(0));
      whiteSunflwrMesh->resizeMesh(SVector3(.8f));
      whiteSunflwrMesh->calculateNormalsPerFace();
   }
   else
      fprintf(stderr, "Failed to load white sunflower mesh.\n");

   yellowFlwrMesh = CMeshLoader::load3dsMesh("Base/sunfloweryellow.3ds");
   if (yellowFlwrMesh) {
      yellowFlwrMesh->centerMeshByExtents(SVector3(0));
      yellowFlwrMesh->resizeMesh(SVector3(.8f));
      yellowFlwrMesh->calculateNormalsPerFace();
   }
   else
      fprintf(stderr, "Failed to load yellow sunflower mesh.\n");

   purpleFlwrMesh = CMeshLoader::load3dsMesh("Base/sunflowerpurple.3ds");
   if (purpleFlwrMesh) {
      purpleFlwrMesh->centerMeshByExtents(SVector3(0));
      purpleFlwrMesh->resizeMesh(SVector3(.8f));
      purpleFlwrMesh->calculateNormalsPerFace();
   }
   else
      fprintf(stderr, "Failed to load purple sunflower mesh.\n");

   tealFlwrMesh = CMeshLoader::load3dsMesh("Base/sunflowerteal.3ds");
   if (tealFlwrMesh) {
      tealFlwrMesh->centerMeshByExtents(SVector3(0));
      tealFlwrMesh->resizeMesh(SVector3(.8f));
      tealFlwrMesh->calculateNormalsPerFace();
   }
   else
      fprintf(stderr, "Failed to load teal sunflower mesh.\n");

   fernMesh = CMeshLoader::load3dsMesh("Base/fern.3ds");
   if (fernMesh) {
      fernMesh->centerMeshByExtents(SVector3(0));
      fernMesh->resizeMesh(SVector3(2.f));
      fernMesh->calculateNormalsPerFace();
   }
   else {
      fprintf(stderr, "Failed to load fern mesh.\n");
   }

   //Desert Meshes
   cactus1Mesh = CMeshLoader::load3dsMesh("Base/cactus.3ds");
   if (cactus1Mesh) {
         cactus1Mesh->centerMeshByExtents(SVector3(0));
         cactus1Mesh->resizeMesh(SVector3(.8f));
         cactus1Mesh->calculateNormalsPerFace();
      }
      else {
         fprintf(stderr, "Failed to load cactus mesh.\n");
      }

   cactus2Mesh = CMeshLoader::load3dsMesh("Base/cactus1.3ds");
   if (cactus2Mesh) {
      cactus2Mesh->centerMeshByExtents(SVector3(0));
      cactus2Mesh->resizeMesh(SVector3(.8f));
      cactus2Mesh->calculateNormalsPerFace();
   }
   else {
      fprintf(stderr, "Failed to load cactus mesh.\n");
   }

   cactusBush2Mesh = CMeshLoader::load3dsMesh("Base/cactus2.3ds");
   if (cactusBush2Mesh) {
      cactusBush2Mesh->centerMeshByExtents(SVector3(0));
      cactusBush2Mesh->resizeMesh(SVector3(.8f));
      cactusBush2Mesh->calculateNormalsPerFace();
   }
   else {
      fprintf(stderr, "Failed to load cactus mesh.\n");
   }

   cactusBushMesh = CMeshLoader::load3dsMesh("Base/bushCactus.3ds");
   if (cactusBushMesh) {
      cactusBushMesh->centerMeshByExtents(SVector3(0));
      cactusBushMesh->resizeMesh(SVector3(.8f));
      cactusBushMesh->calculateNormalsPerFace();
   }
   else {
      fprintf(stderr, "Failed to load cactus bush mesh.\n");
   }
}

void CDecorManager::SetupSky() {
   CMeshSceneObject *tempBlock;

   CMesh* quad = CMeshLoader::load3dsMesh("Base/Quad.3ds");
   quad->centerMeshByExtents(SVector3(0.0f));
   quad->linearizeIndices();
   quad->calculateNormalsPerFace();

   cubeMesh = CMeshLoader::createCubeMesh();
   cubeMesh->linearizeIndices();
   cubeMesh->calculateNormalsPerFace();

   tempBlock = new CMeshSceneObject();
   tempBlock->setMesh(quad);
   if(env == 0)
      tempBlock->setTexture(CImageLoader::loadTexture("Base/sky.bmp", true));
   else if(env == 1)
      tempBlock->setTexture(CImageLoader::loadTexture("Base/desert_bg.bmp", true));
   tempBlock->setShader(ERP_DEFAULT, DiffuseTexture);
   tempBlock->setShader(ERP_DEFERRED_OBJECTS, DeferredTexture);
   tempBlock->setTranslation(SVector3(85/*75*/, 13, -5.0));
   tempBlock->setScale(SVector3(250, 1, 50));
   tempBlock->setRotation(SVector3(90.0f, 0.0f, 0.0f));

   CApplication::get().getSceneManager().addSceneObject(tempBlock);
}

void CDecorManager::SetupClouds() {
   CTexture *tex = CImageLoader::loadTexture("Base/Clouds/clouds 3 lowest.tga", true);
   std::vector<CTexture*> cloudTextures;
   cloudTextures.push_back(CImageLoader::loadTexture("Base/Clouds/clouds1.tga", true));
   cloudTextures.push_back(CImageLoader::loadTexture("Base/Clouds/clouds2.tga", true));
   cloudTextures.push_back(CImageLoader::loadTexture("Base/Clouds/clouds3.tga", true));

   CMeshSceneObject *tempBlock;

   /*Initial Sky*/
   tempBlock = new CMeshSceneObject();
   tempBlock->setMesh(cubeMesh);
   tempBlock->setTexture(tex);
   tempBlock->setShader(ERP_DEFAULT, DiffuseTexture);
   tempBlock->setShader(ERP_DEFERRED_OBJECTS, DeferredTexture);
   tempBlock->setTranslation(SVector3(85/*75*/, 13, -4.9f));
   tempBlock->setScale(SVector3(250, -50, 1));
   tempBlock->setVisible(true);

   CApplication::get().getSceneManager().addSceneObject(tempBlock);
   //Clouds.push_back(tempBlock);

   /* Not using the clouds/mist until we have good transparency for DS
   CMesh* quad = CMeshLoader::load3dsMesh("Base/Quad.3ds");
   quad->centerMeshByExtents(SVector3(0.0f));
   quad->calculateNormalsPerFace();

   for(int i = 0; i < 10; i++) {
      tempBlock = new CMeshSceneObject();
      tempBlock->setMesh(quad);
      tempBlock->setTexture(cloudTextures[i % 3]);
      tempBlock->setShader(ERP_DEFAULT, DiffuseTexture);
      tempBlock->setShader(ERP_DEFERRED_OBJECTS, DeferredTexture);
      tempBlock->setTranslation(SVector3(-20 + i*60, 4, 5));
      tempBlock->setScale(SVector3(10, 1, 10));
      tempBlock->setRotation(SVector3(90, 0, 0));
      tempBlock->setVisible(false);

      Application.getSceneManager().addPostOpaqueSceneObject(tempBlock);
      //Application.getSceneManager().addSceneObject(tempBlock);
      Clouds.push_back(tempBlock);
   }
   */
}

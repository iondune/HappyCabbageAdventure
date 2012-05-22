#include "CDecorManager.h"
#include "CGameLevel.h"

#include "CParticleEngine.h"
#include "CabbageScene.h"
#include "CApplication.h"

CGroundBlock::CGroundBlock(float nx, float ny, float nw, float nh, float nd) : x(nx), y(ny), w(nw), h(nh), d(nd) {
}

ISceneObject *CDecorManager::SetupObject(float x, float y, float z, float xScale, CMesh* model, float yScale) {
   if (y == -1)
	   y = xScale / 2.0f;
   if (yScale == -1.0f)
	   yScale = xScale;

   CMeshSceneObject *render;

   render = new CMeshSceneObject();
   render->setMesh(model);
   render->setShader(ERP_DEFAULT, Toon);
   render->setShader(ERP_DEFERRED_OBJECTS, DeferredToon);
   render->setTranslation(SVector3f(x, y, z));
   render->setScale(SVector3f(xScale, xScale, yScale));
   render->setRotation(SVector3f(-90, 0, 0));

   /*Special Case stuff*/
   if (model == fernMesh) {
      render->setRotation(SVector3f(-90, 0, -115));
   }
   else if (model == whiteFlwrMesh) {
      render->setRotation(SVector3f(-90, 0, -80));
   }
   else if (model == cactus1Mesh) {
      render->setShader(ERP_DEFAULT, ToonTexture);
      render->setTexture(CImageLoader::loadTexture("Base/cactus.bmp", true));
      render->setShader(ERP_DEFERRED_OBJECTS, DeferredToonTexture);

      render->setRotation(SVector3f(-90, 0, rand()%179 - 90.f));
   }
   else if (model == cactus2Mesh || model == cactusBush2Mesh) {
      render->setRotation(SVector3f(-90, 0, rand()%179 - 90.f));
   }

   CApplication::get().getSceneManager().addImmobileSceneObject(render, THIS_OBJECT_WILL_NEVER_MOVE_AND_ITS_BOUNDING_BOX_IS_CORRECT);
   return render;
}

ISceneObject *CDecorManager::OceanSetupObject(float x, float y, float z, float xScale, CMesh* model, float yScale) {
   if (y == -1)
	   y = xScale / 2.0f;
   if (yScale == -1.0f)
	   yScale = xScale;

   CMeshSceneObject *render;

   render = new CMeshSceneObject();
   render->setMesh(model);
   render->setShader(ERP_DEFAULT, Toon);
   render->setShader(ERP_DEFERRED_OBJECTS, DeferredToon);
   render->setTranslation(SVector3f(x, y, z));
   render->setScale(SVector3f(xScale, xScale, yScale));
   render->setRotation(SVector3f(-90, 0, 0));

   /*Special Case stuff*/
   if (model == fernMesh) {
      render->setRotation(SVector3f(-90, 0, -115));
   }

   else if (model == seaweed1Mesh || model == seaweed2Mesh || model == seaweed3Mesh || model == seaweed4Mesh ||
		    model == coral1Mesh || model == coral2Mesh || model == coral3Mesh || model == coral4Mesh || model == coral5Mesh ||
		    model == coral6Mesh) {
	   render->setRotation(SVector3f(-90, 0, (float) (rand()%360)));
   }

   CApplication::get().getSceneManager().addSceneObject(render);
   return render;
}

void CDecorManager::update(float f) {
   if(StarEngine)
      StarEngine->step(f);
   for(unsigned int i = 0; i < DecorativeParticleEngines.size(); i++) {
      DecorativeParticleEngines[i]->step(f);
   }
}

CDecorManager::CDecorManager(CGameLevel & level) {
   std::vector<CGroundBlock*> & groundBlocks = level.getGroundBlocks();
   env = level.getEnvironment();
   night = level.isNight();
   PrepShaders();
   PrepMeshes();
   SetupSky();
   StarEngine = NULL;
   if(night && env != Env::WATER) {
      StarEngine = new CParticleEngine(SVector3f(-30.0f, 15.0f, 0.0f), 100, -1, STAR_PARTICLE, level.isNight());
      SetupClouds();
   }
   else if(env == Env::WATER) {
      StarEngine = new CParticleEngine(SVector3f(-30.0f, 15.0f, 0.0f), 100, -1, UNDERWATER_STAR_PARTICLE, level.isNight());
      //SetupClouds();
   }
   oldFern = false;

   CGroundBlock *curBlock;
   int drawTree = rand()%2 + 1;
   int NumTreeTypes = 2;
   int NumFlowerTypes = 2;
   for(unsigned int i = 0; i < groundBlocks.size(); i++) {
      curBlock = groundBlocks[i];
      if (env == Env::FOREST)
         GenerateForestPlants(curBlock);
      else if (env == Env::DESERT) {
    	  drawTree--;
    	  if (drawTree == 0) {
    		  drawTree = rand()%2 + 1;
    		  GenerateDesertPlants(curBlock, true);
    	  }

    	  else
    		  GenerateDesertPlants(curBlock, false);
      }

      else if (env == Env::WATER) {
    	  GenerateWaterPlants(curBlock);
      }

      int n=0;
      if(night) {
         CPointLightSceneObject * point;
         for(float y = 0; y < 15; y += 3) {
            /*
            point = new CPointLightSceneObject(1.5f, n % 2 ? SColor(1.0f, 0.62f, 0.0f) : SColor(0.68f, 1.0f, 0.18f)); //Orange and Yellowgreen
            point->setTranslation(SVector3(curBlock->x, y, 0.f));
            CApplication::get().getSceneManager().addSceneObject(point);
            n++;
            */

            point = new CPointLightSceneObject(1.5f, n % 2 ? SColor(1.0f, 0.62f, 0.0f) : SColor(0.68f, 1.0f, 0.18f)); //Orange and Yellowgreen
            point->setTranslation(SVector3f(curBlock->x + curBlock->w / 2.0f, y, 0.f));
            CApplication::get().getSceneManager().addSceneObject(point);
            n++;
         }

         DecorativeParticleEngines.push_back(new CParticleEngine(SVector3f(curBlock->x + curBlock->w / 3.0f, curBlock->y + curBlock->h, 1), 10, -1, WIGGLE_PARTICLE, level.isNight()));
         DecorativeParticleEngines.push_back(new CParticleEngine(SVector3f(curBlock->x + 2.0f*curBlock->w / 3.0f, curBlock->y + curBlock->h, 1), 10, -1, WIGGLE_PARTICLE, level.isNight()));
         DecorativeParticleEngines.push_back(new CParticleEngine(SVector3f(curBlock->x + curBlock->w / 3.0f, curBlock->y + curBlock->h, -1), 10, -1, WIGGLE_PARTICLE, level.isNight()));
         DecorativeParticleEngines.push_back(new CParticleEngine(SVector3f(curBlock->x + 2.0f*curBlock->w / 3.0f, curBlock->y + curBlock->h, -1), 10, -1, WIGGLE_PARTICLE, level.isNight()));
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

void CDecorManager::GenerateWaterPlants(CGroundBlock* block) {
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
      numForeground = (int) (w / 2);
      numBackground = (int) (w / 1.25);
   }

   div =  w/(float)numBackground;

   //Check how many tree-type objects we should draw in the background
   for (int n = 0; n < numBackground; n++) {
      random = rand()%7;
      randScale = (float) (rand()%60);
      randScale = randScale * .1f;

      if (random == 0)
    	  OceanSetupObject(x + (n)*div + div/2.0f, yVal + 1.0f + randScale/5, -d/2.0f + .4f, 2.0f, seaweed1Mesh, 4.0f + randScale);
      else if (random == 1)
    	  OceanSetupObject(x + (n)*div + div/2.0f, yVal + 1.0f + randScale/5, -d/2.0f + .4f, 2.0f, seaweed2Mesh, 4.0f + randScale);
      else if (random == 2)
    	  OceanSetupObject(x + (n)*div + div/2.0f, yVal + 1.0f + randScale/5, -d/2.0f + .4f, 2.0f, seaweed3Mesh, 4.0f + randScale);
      else if (random == 3)
    	  OceanSetupObject(x + (n)*div + div/2.0f, yVal + 1.0f + randScale/5, -d/2.0f + .4f, 2.0f, seaweed4Mesh, 4.0f + randScale);
      else if (random == 4)
    	  OceanSetupObject(x + (n)*div + div/2.0f, yVal + 1.0f + randScale/5, -d/2.0f + .4f, 2.0f, seaweed2Mesh, 4.0f + randScale);
      else if (random == 5 || random == 6) {
      }
   }

   //Draw flower-type plants in background
   for (int n = 0; n < w; n++) {
      random = rand()%8;
      randScale = (float) (rand()%50);
      randScale = randScale * .025f;
      randDepth = (float) (rand()%2);
      randDepth = (float) randDepth*.25f;

      if (random == 0)
    	  OceanSetupObject(x + n + .5f, yVal + .2f, -d/2.0f + 1.6f + randDepth, .7f + randScale, coral1Mesh);
      else if (random == 1)
    	  OceanSetupObject(x + n + .5f, yVal + .3f, -d/2.0f + 1.5f + randDepth, .7f + randScale, coral2Mesh);
      else if (random == 2)
    	  OceanSetupObject(x + n + .5f, yVal + .3f, -d/2.0f + 1.5f + randDepth, .7f + randScale, coral3Mesh);
      else if (random == 3)
    	  OceanSetupObject(x + n + .5f, yVal + .3f, -d/2.0f + 1.5f + randDepth, .7f + randScale, coral4Mesh);
      else if (random == 4)
    	  OceanSetupObject(x + n + .5f, yVal + .3f, -d/2.0f + 1.5f + randDepth, .7f + randScale, coral5Mesh);
      else if (random == 5)
    	  OceanSetupObject(x + n + .5f, yVal + .3f, -d/2.0f + 1.5f + randDepth, .7f + randScale, coral6Mesh);
      else if (random == 6 || random == 7) {
      }
   }

   //Draw flower-type plants in foreground
   for (int n = 0; n < w; n++) {
      random = rand()%8;
      randScale = (float) (rand()%10);
      randScale = randScale * 0.04f;
      randDepth = (float) (rand()%2);
      randDepth = randDepth*0.25f;

      if (random == 0)
         OceanSetupObject(x + n + 0.5f, yVal + .1f, d/2.0f - 0.6f, 0.5f + randScale/2.0f, coral1Mesh);
      else if (random == 1)
    	  OceanSetupObject(x + n + 0.5f, yVal + .1f, d/2.0f - 0.6f, 0.4f + randScale, coral2Mesh);
      else if (random == 2)
    	  OceanSetupObject(x + n + 0.5f, yVal + .1f, d/2.0f - 0.6f + .4f, 0.4f + randScale, coral3Mesh);
      else if (random == 3)
    	  OceanSetupObject(x + n + 0.5f, yVal + .1f, d/2.0f - 0.6f, 0.4f + randScale, coral4Mesh);
      else if (random == 4)
    	  OceanSetupObject(x + n + 0.5f, yVal + .1f, d/2.0f - 0.6f, 0.4f + randScale, coral5Mesh);
      else if (random == 5)
    	  OceanSetupObject(x + n + 0.5f, yVal + .1f, d/2.0f - 0.6f, 0.4f + randScale, coral5Mesh);
      else if (random == 6 || random == 7) {
      }
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
      basicTreeMesh->resizeMesh(SVector3f(0.5));
      basicTreeMesh->centerMeshByExtents(SVector3f(0));
      basicTreeMesh->calculateNormalsPerVertex();
   }
   else {
      fprintf(stderr, "Failed to load the basic tree mesh\n");
   }

   christmasTreeMesh = CMeshLoader::load3dsMesh("Base/christmasTree3.3ds");
   if (christmasTreeMesh) {
      christmasTreeMesh->resizeMesh(SVector3f(0.5));
      christmasTreeMesh->centerMeshByExtents(SVector3f(0));
      christmasTreeMesh->calculateNormalsPerFace();
   }
   else {
      fprintf(stderr, "Failed to load the christmas tree mesh\n");
   }

   blueFlwrMesh = CMeshLoader::load3dsMesh("Base/simpleflower1.3ds");
   if (blueFlwrMesh) {
      blueFlwrMesh->centerMeshByExtents(SVector3f(0));
      blueFlwrMesh->calculateNormalsPerFace();
   }
   else {
      fprintf(stderr, "Failed to load blue flower mesh.\n");
   }

   whiteFlwrMesh = CMeshLoader::load3dsMesh("Base/simpleflower2.3ds");
   if (whiteFlwrMesh) {
      whiteFlwrMesh->centerMeshByExtents(SVector3f(0));
      whiteFlwrMesh->resizeMesh(SVector3f(.8f));
      whiteFlwrMesh->calculateNormalsPerFace();
   }
   else
      fprintf(stderr, "Failed to load white flower mesh.\n");

   whiteSunflwrMesh = CMeshLoader::load3dsMesh("Base/sunflowerwhite.3ds");
   if (whiteSunflwrMesh) {
      whiteSunflwrMesh->centerMeshByExtents(SVector3f(0));
      whiteSunflwrMesh->resizeMesh(SVector3f(.8f));
      whiteSunflwrMesh->calculateNormalsPerFace();
   }
   else
      fprintf(stderr, "Failed to load white sunflower mesh.\n");

   yellowFlwrMesh = CMeshLoader::load3dsMesh("Base/sunfloweryellow.3ds");
   if (yellowFlwrMesh) {
      yellowFlwrMesh->centerMeshByExtents(SVector3f(0));
      yellowFlwrMesh->resizeMesh(SVector3f(.8f));
      yellowFlwrMesh->calculateNormalsPerFace();
   }
   else
      fprintf(stderr, "Failed to load yellow sunflower mesh.\n");

   purpleFlwrMesh = CMeshLoader::load3dsMesh("Base/sunflowerpurple.3ds");
   if (purpleFlwrMesh) {
      purpleFlwrMesh->centerMeshByExtents(SVector3f(0));
      purpleFlwrMesh->resizeMesh(SVector3f(.8f));
      purpleFlwrMesh->calculateNormalsPerFace();
   }
   else
      fprintf(stderr, "Failed to load purple sunflower mesh.\n");

   tealFlwrMesh = CMeshLoader::load3dsMesh("Base/sunflowerteal.3ds");
   if (tealFlwrMesh) {
      tealFlwrMesh->centerMeshByExtents(SVector3f(0));
      tealFlwrMesh->resizeMesh(SVector3f(.8f));
      tealFlwrMesh->calculateNormalsPerFace();
   }
   else
      fprintf(stderr, "Failed to load teal sunflower mesh.\n");

   fernMesh = CMeshLoader::load3dsMesh("Base/fern.3ds");
   if (fernMesh) {
      fernMesh->centerMeshByExtents(SVector3f(0));
      fernMesh->resizeMesh(SVector3f(2.f));
      fernMesh->calculateNormalsPerFace();
   }
   else {
      fprintf(stderr, "Failed to load fern mesh.\n");
   }

   //Desert Meshes
   cactus1Mesh = CMeshLoader::load3dsMesh("Base/cactus.3ds");
   if (cactus1Mesh) {
         cactus1Mesh->centerMeshByExtents(SVector3f(0));
         cactus1Mesh->resizeMesh(SVector3f(.8f));
         cactus1Mesh->calculateNormalsPerFace();
      }
      else {
         fprintf(stderr, "Failed to load cactus mesh.\n");
      }

   cactus2Mesh = CMeshLoader::load3dsMesh("Base/cactus1.3ds");
   if (cactus2Mesh) {
      cactus2Mesh->centerMeshByExtents(SVector3f(0));
      cactus2Mesh->resizeMesh(SVector3f(.8f));
      cactus2Mesh->calculateNormalsPerFace();
   }
   else {
      fprintf(stderr, "Failed to load cactus mesh.\n");
   }

   cactusBush2Mesh = CMeshLoader::load3dsMesh("Base/cactus2.3ds");
   if (cactusBush2Mesh) {
      cactusBush2Mesh->centerMeshByExtents(SVector3f(0));
      cactusBush2Mesh->resizeMesh(SVector3f(.8f));
      cactusBush2Mesh->calculateNormalsPerFace();
   }
   else {
      fprintf(stderr, "Failed to load cactus mesh.\n");
   }

   cactusBushMesh = CMeshLoader::load3dsMesh("Base/bushCactus.3ds");
   if (cactusBushMesh) {
      cactusBushMesh->centerMeshByExtents(SVector3f(0));
      cactusBushMesh->resizeMesh(SVector3f(.8f));
      cactusBushMesh->calculateNormalsPerFace();
   }
   else {
      fprintf(stderr, "Failed to load cactus bush mesh.\n");
   }


   //Water Meshes
   coral1Mesh = CMeshLoader::load3dsMesh("Base/waterlevel/coral1.3ds");
   if (coral1Mesh) {
      coral1Mesh->centerMeshByExtents(SVector3f(0));
      coral1Mesh->resizeMesh(SVector3f(.8f));
      coral1Mesh->calculateNormalsPerFace();
   }
   else {
      fprintf(stderr, "Failed to load coral1 mesh.\n");
   }

   coral2Mesh = CMeshLoader::load3dsMesh("Base/waterlevel/coral2.3ds");
   if (coral2Mesh) {
      coral2Mesh->centerMeshByExtents(SVector3f(0));
      coral2Mesh->resizeMesh(SVector3f(.8f));
      coral2Mesh->calculateNormalsPerFace();
   }
   else {
      fprintf(stderr, "Failed to load coral2 mesh.\n");
   }

   coral3Mesh = CMeshLoader::load3dsMesh("Base/waterlevel/coral3.3ds");
   if (coral3Mesh) {
      coral3Mesh->centerMeshByExtents(SVector3f(0));
      coral3Mesh->resizeMesh(SVector3f(.8f));
      coral3Mesh->calculateNormalsPerFace();
   }
   else {
      fprintf(stderr, "Failed to load coral3 mesh.\n");
   }

   coral4Mesh = CMeshLoader::load3dsMesh("Base/waterlevel/coral4.3ds");
   if (coral4Mesh) {
      coral4Mesh->centerMeshByExtents(SVector3f(0));
      coral4Mesh->resizeMesh(SVector3f(.8f));
      coral4Mesh->calculateNormalsPerFace();
   }
   else {
      fprintf(stderr, "Failed to load coral4 mesh.\n");
   }

   coral5Mesh = CMeshLoader::load3dsMesh("Base/waterlevel/coral5.3ds");
   if (coral5Mesh) {
      coral5Mesh->centerMeshByExtents(SVector3f(0));
      coral5Mesh->resizeMesh(SVector3f(.8f));
      coral5Mesh->calculateNormalsPerFace();
   }
   else {
      fprintf(stderr, "Failed to load coral5 mesh.\n");
   }

   coral6Mesh = CMeshLoader::load3dsMesh("Base/waterlevel/coral6.3ds");
   if (coral6Mesh) {
      coral6Mesh->centerMeshByExtents(SVector3f(0));
      coral6Mesh->resizeMesh(SVector3f(.8f));
      coral6Mesh->calculateNormalsPerFace();
   }
   else {
      fprintf(stderr, "Failed to load coral6 mesh.\n");
   }

   seaweed1Mesh = CMeshLoader::load3dsMesh("Base/waterlevel/seaweed1.3ds");
   if (seaweed1Mesh) {
	   seaweed1Mesh->centerMeshByExtents(SVector3f(0));
	   seaweed1Mesh->resizeMesh(SVector3f(.8f));
	   seaweed1Mesh->calculateNormalsPerFace();
   }
   else {
	   fprintf(stderr, "Failed to load seaweed1 mesh.\n");
   }

   seaweed2Mesh = CMeshLoader::load3dsMesh("Base/waterlevel/seaweed2.3ds");
   if (seaweed2Mesh) {
	   seaweed2Mesh->centerMeshByExtents(SVector3f(0));
	   seaweed2Mesh->resizeMesh(SVector3f(.8f));
	   seaweed2Mesh->calculateNormalsPerFace();
   }
   else {
	   fprintf(stderr, "Failed to load seaweed2 mesh.\n");
   }

   seaweed3Mesh = CMeshLoader::load3dsMesh("Base/waterlevel/seaweed3.3ds");
   if (seaweed3Mesh) {
	   seaweed3Mesh->centerMeshByExtents(SVector3f(0));
	   seaweed3Mesh->resizeMesh(SVector3f(.8f));
	   seaweed3Mesh->calculateNormalsPerFace();
   }
   else {
	   fprintf(stderr, "Failed to load seaweed3 mesh.\n");
   }

   seaweed4Mesh = CMeshLoader::load3dsMesh("Base/waterlevel/seaweed4.3ds");
   if (seaweed4Mesh) {
	   seaweed4Mesh->centerMeshByExtents(SVector3f(0));
	   seaweed4Mesh->resizeMesh(SVector3f(.8f));
	   seaweed4Mesh->calculateNormalsPerFace();
   }
   else {
	   fprintf(stderr, "Failed to load seaweed4 mesh.\n");
   }

   searock1Mesh = CMeshLoader::load3dsMesh("Base/waterlevel/rock1.3ds");
   if (searock1Mesh) {
	   searock1Mesh->centerMeshByExtents(SVector3f(0));
	   searock1Mesh->resizeMesh(SVector3f(.8f));
	   searock1Mesh->calculateNormalsPerFace();
   }
   else {
	   fprintf(stderr, "Failed to load rock1 mesh.\n");
   }

   searock2Mesh = CMeshLoader::load3dsMesh("Base/waterlevel/rock2.3ds");
   if (searock2Mesh) {
	   searock2Mesh->centerMeshByExtents(SVector3f(0));
	   searock2Mesh->resizeMesh(SVector3f(.8f));
	   searock2Mesh->calculateNormalsPerFace();
   }
   else {
	   fprintf(stderr, "Failed to load rock2 mesh.\n");
   }
}

void CDecorManager::SetupSky() {
   CMeshSceneObject *tempBlock;

   CMesh* quad = CMeshLoader::load3dsMesh("Base/Quad.3ds");
   quad->centerMeshByExtents(SVector3f(0.0f));
   quad->linearizeIndices();
   quad->calculateNormalsPerFace();

   cubeMesh = CMeshLoader::createCubeMesh();
   cubeMesh->linearizeIndices();
   cubeMesh->calculateNormalsPerFace();

   tempBlock = new CMeshSceneObject();
   tempBlock->setMesh(quad);
   if(env == Env::FOREST)
      tempBlock->setTexture(CImageLoader::loadTexture("Base/ForestBackground.bmp", true));
   else if(env == Env::DESERT)
      tempBlock->setTexture(CImageLoader::loadTexture("Base/DesertBackground.bmp", true));
   else if (env == Env::WATER)
      tempBlock->setTexture(CImageLoader::loadTexture("Base/WaterBackground.bmp", true));

   tempBlock->setShader(ERP_DEFAULT, DiffuseTexture);
   tempBlock->setShader(ERP_DEFERRED_OBJECTS, DeferredTexture);
   tempBlock->setTranslation(SVector3f(85/*75*/, 13, -5.0));
   tempBlock->setScale(SVector3f(250, 1, 50));
   tempBlock->setRotation(SVector3f(90.0f, 0.0f, 0.0f));

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
   tempBlock->setTranslation(SVector3f(85/*75*/, 13, -4.9f));
   tempBlock->setScale(SVector3f(250, -50, 1));
   tempBlock->setVisible(true);

   CApplication::get().getSceneManager().addSceneObject(tempBlock);
   //Clouds.push_back(tempBlock);

   /* Not using the clouds/mist until we have good transparency for DS
   CMesh* quad = CMeshLoader::load3dsMesh("Base/Quad.3ds");
   quad->centerMeshByExtents(SVector3f(0.0f));
   quad->calculateNormalsPerFace();

   for(int i = 0; i < 10; i++) {
      tempBlock = new CMeshSceneObject();
      tempBlock->setMesh(quad);
      tempBlock->setTexture(cloudTextures[i % 3]);
      tempBlock->setShader(ERP_DEFAULT, DiffuseTexture);
      tempBlock->setShader(ERP_DEFERRED_OBJECTS, DeferredTexture);
      tempBlock->setTranslation(SVector3f(-20 + i*60, 4, 5));
      tempBlock->setScale(SVector3f(10, 1, 10));
      tempBlock->setRotation(SVector3f(90, 0, 0));
      tempBlock->setVisible(false);

      Application.getSceneManager().addPostOpaqueSceneObject(tempBlock);
      //Application.getSceneManager().addSceneObject(tempBlock);
      Clouds.push_back(tempBlock);
   }
   */
}

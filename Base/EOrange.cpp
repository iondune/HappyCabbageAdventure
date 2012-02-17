/*
 * EOrange.cpp
 *
 *  Created on: Feb 17, 2012
 *      Author: Klonoa
 */

#include "EOrange.h"

#include "EApple.h"

EOrange::EOrange(float x, float y, float w, float h, CGameplayManager* manager) {
   this->x = x;
   this->y = y;
   this->w = w;
   this->h = h;
   Manager = manager;

   loadMesh();
   loadActor();

}

//Loads and moves the mesh
void EOrange::loadMesh() {
   Renderable = new CMeshRenderable();
   CMesh *mesh = CMeshLoader::load3dsMesh("Models/appleEnemy.3ds");
   if(mesh) {
      mesh->resizeMesh(SVector3(1));
      mesh->centerMeshByExtents(SVector3(0));
      mesh->calculateNormalsPerFace();
   }

   else
      printf("ERROR.  MESH DID NOT LOAD PROPERLY.\n");

   Renderable->setMesh(mesh);
   Renderable->getMaterial().Texture = new CTexture(CImageLoader::loadImage("Textures/orange.bmp"));

   Renderable->getMaterial().Shader = CShaderLoader::loadShader("DiffuseTexture");
   Renderable->setTranslation(SVector3((x+(x+1))/2, (y+(y-1))/2, 0));
   Renderable->setScale(SVector3(1, 1, 1));
   Renderable->setRotation(SVector3(-90, 0, 0));

   CApplication::get().getSceneManager().addRenderable(Renderable);
}

//Adds actor to engine and preps engine
void EOrange::loadActor() {
   Actor = Manager->getEngine()->addActor();
   printf("x is %f and y is %f\n", x, y);
   Actor->setArea(SRect2(SVector2(x, y), SVector2(w, h)));

   //Set actor attributes
   Actor->getAttributes().MaxWalk = 1.2f;
}

//Updates AI's decision per frame
void EOrange::update() {
   if (Manager->isPlayerAlive())
   {
       if (Manager->getPlayerLocation().X < Actor->getArea().getCenter().X)
           Actor->setAction(Cabbage::Collider::CActor::EActionType::MoveLeft);
       else
           Actor->setAction(Cabbage::Collider::CActor::EActionType::MoveRight);

       if (Manager->getPlayerLocation().Y - 1 > Actor->getArea().getCenter().Y) {
          Actor->setJumping(true);
       }
   }
   else
   {
       Actor->setAction(Cabbage::Collider::CActor::EActionType::None);
   }
   //if (rand()%1000 == 1)
   //it->Actor->setJumping(true);
}


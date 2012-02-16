#include "EApple.h"

EApple::EApple(float x, float y, float w, float h, CGameplayManager* manager) {
   this->x = x;
   this->y = y;
   this->w = w;
   this->h = h;
   Manager = manager;

   loadMesh();
   loadActor();

}

//Loads and moves the mesh
void EApple::loadMesh() {
   Renderable = new CMeshRenderable();
   CMesh *mesh = CMeshLoader::load3dsMesh("Models/appleEnemy.3ds");
   if(mesh) {
      mesh->resizeMesh(SVector3(1));
      mesh->centerMeshByExtents(SVector3(0));
      mesh->calculateNormalsPerFace();
   }

   ((CMeshRenderable*) Renderable)->setMesh(mesh);
   //tempEnemy->getMaterial().Texture = CImageLoader::loadTexture("Textures/dirt.bmp");
   Renderable->getMaterial().Shader = CBadGuy::Shader; //Why do I need to do this?
   Renderable->setTranslation(SVector3((x+(x+1))/2, (y+(y+1))/2, 0));
   Renderable->setScale(SVector3(1, 1, 1));
   Renderable->setRotation(SVector3(-90, 0, 0));

   CApplication::get().getSceneManager().addRenderable(Renderable);
}

//Adds actor to engine and preps engine
void EApple::loadActor() {
   Actor = Manager->getEngine()->addActor();
   Actor->setArea(SRect2(SVector2(x, y), SVector2(w, h)));

   //Set actor attributes
   Actor->getAttributes().MaxWalk = 1.2f;

   Manager->addEnemy(SVector2(x, y), Renderable);
}

//Updates AI's decision per frame
void EApple::update() {
}

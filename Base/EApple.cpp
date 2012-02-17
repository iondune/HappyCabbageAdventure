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

   else
      printf("ERROR.  MESH DID NOT LOAD PROPERLY.\n");

   Renderable->setMesh(mesh);
   Renderable->getMaterial().Shader = CShaderLoader::loadShader("Diffuse");
   Renderable->setTranslation(SVector3((x+(x+2))/2, (y+(y+2))/2, 0));
   Renderable->setScale(SVector3(1, 1, 1));
   Renderable->setRotation(SVector3(-90, 0, 0));

   CApplication::get().getSceneManager().addRenderable(Renderable);
}

//Adds actor to engine and preps engine
void EApple::loadActor() {
   Actor = Manager->getEngine()->addActor();
   printf("x is %f and y is %f\n", x, y);
   Actor->setArea(SRect2(SVector2(x+2, y+2), SVector2(w, h)));

   //Set actor attributes
   Actor->getAttributes().MaxWalk = 1.2f;
}

//Updates AI's decision per frame
void EApple::update() {
}

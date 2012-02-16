
#include "EApple.h"

EApple::EApple(float x, float y, float w, float h, CGameplayManager* manager) {
   this->x = x;
   this->y = y;
   this->w = w;
   this->h = h;
   this->Manager = manager;

   loadMesh();

}

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
   Renderable->getMaterial().Shader = Shader;
   Renderable->setTranslation(SVector3((x+(x+1))/2, (y+(y+1))/2, 0));
   Renderable->setScale(SVector3(1, 1, 1));
   Renderable->setRotation(SVector3(-90, 0, 0));

   CApplication::get().getSceneManager().addRenderable(Renderable);
}

bool EApple::loadActor() {

}

EApple::~EApple() {
   // TODO Auto-generated destructor stub
}


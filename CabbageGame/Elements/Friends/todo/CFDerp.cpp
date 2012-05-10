#include "CFDerp.h"

CFDerp::CFDerp(float x, float y, float w, float h, CGameplayManager* manager) {
   this->x = x;
   this->y = y;
   this->w = w;
   this->h = h;
   Manager = manager;

   loadMesh();
   loadActor();

   rotate = 0.0f;


}

void CFDerp::loadMesh() {
    Renderable = new CMeshSceneObject();
    CMesh *mesh = CMeshLoader::load3dsMesh("Base/derp.3ds");
    if(mesh) {
        mesh->resizeMesh(SVector3f(1));
        mesh->centerMeshByExtents(SVector3f(0));
        mesh->calculateNormalsPerFace();
    }
    else
        printf("ERROR.  MESH DID NOT LOAD PROPERLY.\n");

    Renderable->setMesh(mesh);
    Renderable->setShader(ERP_DEFAULT, "Toon");
    Renderable->setShader(ERP_DEFERRED_OBJECTS, "Deferred/Toon");
    Renderable->setScale(SVector3f(1.5f));
    Renderable->setRotation(SVector3f(-90, 0, -90));

    CApplication::get().getSceneManager().addSceneObject(Renderable);
}


//Adds actor to engine and preps engine
void CFDerp::loadActor() {
   Actor = Manager->getEngine()->addActor();
   Actor->setArea(SRect2f(x, y, 1, 1));
   Actor->setJumping(true);

   Renderable->setTranslation(SVector3f(Actor->getArea().getCenter().X, Actor->getArea().getCenter().Y + .2f, 0));
   //Set actor attributes
}

//Updates AI's decision per frame
void CFDerp::update(float const TickTime) {
//    if (Manager->isPlayerAlive())
        Actor->setJumping(true);
}

void CFDerp::doRenderable() {
   Renderable->setTranslation(SVector3f(Actor->getArea().getCenter().X, Actor->getArea().getCenter().Y + .2f, 0));
   //printf("%0.2f, %0.2f\n", Actor->getArea().getCenter().X, Actor->getArea().getCenter().Y);
 Actor->setJumping(true);
}

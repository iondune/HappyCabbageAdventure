#include "CElementEnemyBlade.h"
#include "CGameLevel.h"

CElementEnemyBlade::CElementEnemyBlade(SRect2 nArea) :
   CElementEnemy(nArea, Enemies::BLADE), BladeRotate(0.0f) {

}

void CElementEnemyBlade::setupObjects() {
   /* Set up the actor (not actually an actor, since this one doesn't move its position) */
   //TODO: Use singleton class
   //PhysicsEngineObject = CCollisionEngine::get().addActor();
   PhysicsEngineObject = (new CCollisionEngine())->addActor(); //just to compile

   //Makes them immune to gravity
   PhysicsEngineObject->setControlFall(false);
   PhysicsEngineObject->setFallAcceleration(0.0f);
   PhysicsEngineObject->setArea(SRect2(SVector2(Area.Position.X, Area.Position.Y-1.0f), Area.Size));
   PhysicsEngineObject->CollideableType = COLLIDEABLE_TYPE_FLAME;


   /* Set up the renderable */
   SceneObject = new CMeshSceneObject();
   CMesh *mesh = CMeshLoader::load3dsMesh("Base/trap1.3ds");
   if(mesh) {
      mesh->resizeMesh(SVector3(1));
      mesh->centerMeshByExtents(SVector3(0));
      mesh->calculateNormalsPerFace();
   }

   else
      printf("ERROR.  MESH DID NOT LOAD PROPERLY.\n");

   SceneObject->setMesh(mesh);
   SceneObject->setShader(ERP_DEFAULT, "Toon");
   SceneObject->setShader(ERP_DEFERRED_OBJECTS, "Deferred/Toon");
   SceneObject->setTranslation(SVector3(PhysicsEngineObject->getArea().getCenter().X, PhysicsEngineObject->getArea().getCenter().Y, 0));
   SceneObject->setScale(SVector3(2, 2, 2));

   CApplication::get().getSceneManager().addSceneObject(SceneObject);
}

void CElementEnemyBlade::OnCollision(CCollideable *Object, CCollideable *With) {
   //Optional code: setImpulse to other object away from this object, lower their health?
}
                                                            
//CGameplayElement has an attribute called ElapsedTime, which is updated by CGameplayElement's update function.
void CElementEnemyBlade::updatePhysicsEngineObject(float time) {
   return; //Doesn't move in physics engine
}

void CElementEnemyBlade::updateSceneObject(float time) {
   BladeRotate -= 300.0f * time;

   SceneObject->setRotation(SVector3(-90, 0, BladeRotate));
}

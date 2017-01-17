#include "CElementBlockBreakable.h"
#include "Player/CPlayerAbility.h"
#include "CGameLevel.h"
#include "CElementPlayer.h"

CElementBlockBreakable::CElementBlockBreakable(SRect2f nArea)
: CElementBlock(nArea, (int) nArea.Size.X, 0) {
}

void CElementBlockBreakable::OnCollision(const SCollisionEvent& Event) {
   if (Event.Other == Level.getPlayer().getPhysicsEngineObject()) {
   }
   return;
}

void CElementBlockBreakable::writeXML(xmlwriter *l) {
   std::stringstream xValue, yValue, widthValue, heightValue, tagValue, isMovingValue, secretBreakable;
   xValue << Area.Position.X;
   yValue << Area.Position.Y;
   widthValue << Area.Size.X;
   heightValue << Area.Size.Y;

   tagValue << "CBreakable";

   l->AddAtributes("width ", widthValue.str());
   l->AddAtributes("height ", heightValue.str());
   l->AddAtributes("Y ", yValue.str());
   l->AddAtributes("X ", xValue.str());
   l->Createtag(tagValue.str());
   l->CloseLasttag();
}

void CElementBlockBreakable::explode() {
   if(Dead)
      return;
   removeFromPhysicsEngine();
   removeFromSceneManager();
   ParticleEngine = new CParticleEngine(SceneObject->getTranslation(), 35, 4, BURST_PARTICLE, Level.isNight());
   ParticleEngine->UsePhysics(&Level.getPhysicsEngine(), Level.getEnv());
}

void CElementBlockBreakable::setupPhysicsEngineObject() {
   PhysicsEngineObject = Level.getPhysicsEngine().addObject();
   PhysicsEngineObject->setArea(Area);
}

void CElementBlockBreakable::setupSceneObject() {
   CMeshSceneObject *tempBlock = new CMeshSceneObject();
   CMesh *mesh;

   mesh = CMeshLoader::load3dsMesh("Base/breakableblock2.3ds");

   if(mesh) {
         mesh->resizeMesh(SVector3f(1));
         mesh->centerMeshByExtents(SVector3f(0));
         mesh->calculateNormalsPerFace();
         //mesh->calculateNormalsPerVertex();
   }
   else {
      fprintf(stderr, "COULD NOT LOAD BLOCK MESH.\n");
   }

   tempBlock->setMesh(mesh);
   tempBlock->setTexture("ModelImages/breakableblock2_brown.bmp");

   tempBlock->setShader(ERP_DEFAULT, CShaderLoader::loadShader("ToonTexture"));
   tempBlock->setShader(ERP_DEFERRED_OBJECTS, CShaderLoader::loadShader("Deferred/Textured"));

   tempBlock->setTranslation(SVector3f((Area.Position.X+(Area.Position.X+Area.Size.X))/2, (Area.Position.Y+(Area.Position.Y+Area.Size.Y))/2, 0));
   tempBlock->setScale(SVector3f(Area.Size.X, Area.Size.Y, Area.Size.X));

   SVector3f rots((float) (rand()%3*90-90), (float) (rand()%3*90-90), (float) (rand()%3*90-90));
   tempBlock->setRotation(rots);

   SceneObject = tempBlock;
   CApplication::get().getSceneManager().addSceneObject(tempBlock);
}

void CElementBlockBreakable::printInformation() {
   printf("CElementBlockBreakable; Area: [[%0.0f, %0.0f],[%0.0f, %0.0f]]\n", Area.Position.X, Area.Position.Y, Area.Size.X, Area.Size.Y);
}

void CElementBlockBreakable::reactToAbility(Abilities::EAbilityType Ability) {
   switch(Ability) {
      case Abilities::HEADBUTT:
         Level.getPlayer().setShaking(0.45f, 0.7f);
         ((CCollisionActor*)Level.getPlayer().getPhysicsEngineObject())->addImpulse(SVector2f(Level.getPlayer().getDirection() == CElementPlayer::Right ? -9.0f : 9.0f, 0.8f)*6.0f);
      case Abilities::LASER:
         if(!Dead)
            explode();
         break;
      default:
         break;
   }
}

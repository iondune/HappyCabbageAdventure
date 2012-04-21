#include "CElementPlayer.h"
#include "CGameLevel.h"

CElementPlayer::CElementPlayer(SRect2 nArea)
: CGameplayElement(PhysicsEngineObject, SceneObject, nArea) {
}

void CElementPlayer::updatePhysicsEngineObject(float time) {
}

void CElementPlayer::updateSceneObject(float time) {
}

void CElementPlayer::OnCollision(CCollideable *Object, CCollideable *With) {
   return;
}

void CElementPlayer::writeXML(xmlwriter *l) {
   std::stringstream xValue, yValue, widthValue, heightValue, tagValue;
   xValue << Area.Position.X;
   yValue << Area.Position.Y;
   widthValue << Area.Size.X;
   heightValue << Area.Size.Y;

   tagValue << "CCabbage";

   l->AddAtributes("width ", widthValue.str());
   l->AddAtributes("height ", heightValue.str());
   l->AddAtributes("Y ", yValue.str());
   l->AddAtributes("X ", xValue.str());
   l->Createtag(tagValue.str());
   l->CloseLasttag();
}

void CElementPlayer::setupPhysicsEngineObject() {
   //TODO: Fix this once we have a way to refer to the physics engine
   PhysicsEngineObject = (new CCollisionEngine())->addActor();
   PhysicsEngineObject->setArea(Area);
   PhysicsEngineObject->getAttributes().MaxWalk = 3.5f;
   PhysicsEngineObject->CollideableType = COLLIDEABLE_TYPE_PLAYER;
   PhysicsEngineObject->CollideableLevel |= INTERACTOR_SUPERACTORS;
   PhysicsEngineObject->CanCollideWith |= INTERACTOR_SUPERACTORS | INTERACTOR_ITEMS;
}

#include <sstream>
void CElementPlayer::setupSceneObject() {
   SceneObject = new ISceneObject();

   //Normal cabbage meshes and renderables
   ISceneObject *NormalCabbage = new ISceneObject();
   SceneObject->addChild(NormalCabbage);

   for(int i = 1; i <= 5; i++) {
      std::ostringstream fileName;
      fileName << "Base/cabbage/cabbage_" << i << ".3ds";
      CMesh *cabbageMesh = CMeshLoader::load3dsMesh(fileName.str());
      if(cabbageMesh) {
         cabbageMesh->centerMeshByExtents(SVector3(0.0f));
         cabbageMesh->calculateNormalsPerFace();
         cabbageMesh->resizeMesh(SVector3(0.45f));
      }

      CMeshSceneObject *cabbageRenderable = new CMeshSceneObject();
      cabbageRenderable->setMesh(cabbageMesh);
      cabbageRenderable->setShader(ERP_DEFAULT, "Toon");
      cabbageRenderable->setShader(ERP_DEFERRED_OBJECTS, "Deferred/Toon");
      cabbageRenderable->setCullingEnabled(false);
      cabbageRenderable->setVisible(Stats.Health == i);

      NormalCabbage->addChild(cabbageRenderable);
   }

   //Hurt cabbage meshes and renderables
   ISceneObject *HurtCabbage = new ISceneObject();
   SceneObject->addChild(HurtCabbage);

   for(int i = 1; i <= 5; i++) {
      std::ostringstream fileName;
      fileName << "Base/cabbage/cabbage_ouch_" << i << ".3ds";
      CMesh *cabbageMesh = CMeshLoader::load3dsMesh(fileName.str());
      if(cabbageMesh) {
         cabbageMesh->centerMeshByExtents(SVector3(0.0f));
         cabbageMesh->calculateNormalsPerFace();
         cabbageMesh->resizeMesh(SVector3(0.85f));
      }

      CMeshSceneObject *cabbageRenderable = new CMeshSceneObject();
      cabbageRenderable->setMesh(cabbageMesh);
      cabbageRenderable->setShader(ERP_DEFAULT, "Toon");
      cabbageRenderable->setShader(ERP_DEFERRED_OBJECTS, "Deferred/Toon");
      cabbageRenderable->setCullingEnabled(false);
      cabbageRenderable->setVisible(false);
      cabbageRenderable->setTranslation(SVector3(0.0f, 0.0f, 0.2f));

      HurtCabbage->addChild(cabbageRenderable);
   }

   ////
   //PlayerView = new CPlayerView();

   /* SHADOW
   ISceneObject *CabbageShadow = new ISceneObject();
   SceneObject->addChild(CabbageShadow);

   CMeshSceneObject *renderLeftShadow = new CMeshSceneObject();
   renderLeftShadow->setMesh(discMesh);
   renderLeftShadow->setShader(ERP_DEFAULT, BlackShader);
   renderLeftShadow->setShader(ERP_DEFERRED_OBJECTS, BlackShader);
   renderLeftShadow->addUniform("cutoff", PlayerView->LeftShadowCutoffValue);
   renderLeftShadow->addUniform("start", PlayerView->LeftShadowStartValue);

   CMeshSceneObject *renderRightShadow = new CMeshSceneObject();
   renderRightShadow->setMesh(discMesh);
   renderRightShadow->setShader(ERP_DEFAULT, BlackShader);
   renderRightShadow->setShader(ERP_DEFERRED_OBJECTS, BlackShader);
   renderRightShadow->addUniform("cutoff", PlayerView->RightShadowCutoffValue);
   renderRightShadow->addUniform("start", PlayerView->RightShadowStartValue);

   CApplication::get().getSceneManager().addSceneObject(CabbageShadow);
   */
}

#include "CPlayerView.h"
#include "CabbageFramework.h"


#include <sstream>
CPlayerView::CPlayerView(ISceneObject * obj, CElementPlayer::EDirection & dir, CElementPlayer::EAction & act, int CurHealth, SRect2 & nArea, SVector3 & sf, CCollisionActor* peo) :
   SceneObject(obj), CabbageIndex(CurHealth - 1), Direction(dir), Action(act), Hurt(false), Area(nArea), ShakeFactor(sf),
   ySineValue(0.0f), PhysicsEngineObject(peo) {

   SceneObject->setCullingEnabled(false);

   CabbageSceneObject = new ISceneObject();
   SceneObject->addChild(CabbageSceneObject);
   CabbageSceneObject->setCullingEnabled(false);

   //Normal cabbage meshes and renderables
   ISceneObject *NormalCabbage = new ISceneObject();
   CabbageSceneObject->addChild(NormalCabbage);
   NormalCabbage->setCullingEnabled(false);

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
      cabbageRenderable->setVisible(CurHealth == i);

      NormalCabbage->addChild(cabbageRenderable);
      NormalCabbageRenderables[i-1] = cabbageRenderable;
   }

   //Hurt cabbage meshes and renderables
   ISceneObject *HurtCabbage = new ISceneObject();
   CabbageSceneObject->addChild(HurtCabbage);
   HurtCabbage->setCullingEnabled(false);

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
      HurtCabbageRenderables[i-1] = cabbageRenderable;
   }

   //Shadow renderables
   ShadowLeft = new CMeshSceneObject();
   ShadowLeft->setMesh(CMeshLoader::createDiscMesh());
   ShadowLeft->setShader(ERP_DEFAULT, CShaderLoader::loadShader("SpecialDiscThing"));
   ShadowLeft->setShader(ERP_DEFERRED_OBJECTS, CShaderLoader::loadShader("SpecialDiscThing"));
   ShadowLeft->addUniform("cutoff", LeftShadowCutoffValue);
   ShadowLeft->addUniform("start", LeftShadowStartValue);
   SceneObject->addChild(ShadowLeft);

   ShadowRight = new CMeshSceneObject();
   ShadowRight->setMesh(CMeshLoader::createDiscMesh());
   ShadowRight->setShader(ERP_DEFAULT, CShaderLoader::loadShader("SpecialDiscThing"));
   ShadowRight->setShader(ERP_DEFERRED_OBJECTS, CShaderLoader::loadShader("SpecialDiscThing"));
   ShadowRight->addUniform("cutoff", RightShadowCutoffValue);
   ShadowRight->addUniform("start", RightShadowStartValue);
   SceneObject->addChild(ShadowRight);

   CApplication::get().getSceneManager().addSceneObject(SceneObject);
} 

void CPlayerView::setCabbageScale(SVector3 sc) {
   CabbageSceneObject->setScale(sc);
}

void CPlayerView::addLeaf() {
   if(CabbageIndex + 1 == NUM_CABBAGE_MESH_STATES)
      return;
   if(Hurt) {
      HurtCabbageRenderables[CabbageIndex++]->setVisible(false);
      HurtCabbageRenderables[CabbageIndex]->setVisible(true);
   }
   else {
      NormalCabbageRenderables[CabbageIndex++]->setVisible(false);
      NormalCabbageRenderables[CabbageIndex]->setVisible(true);
   }
}

void CPlayerView::removeLeaf() {
   if(CabbageIndex == 0)
      return;
   if(Hurt) {
      HurtCabbageRenderables[CabbageIndex--]->setVisible(false);
      HurtCabbageRenderables[CabbageIndex]->setVisible(true);
   }
   else {
      NormalCabbageRenderables[CabbageIndex--]->setVisible(false);
      NormalCabbageRenderables[CabbageIndex]->setVisible(true);
   }
}


void CPlayerView::setShadowHeights(float l, float r) {
   yLeftShadow = l;
   yRightShadow = r;
}

void CPlayerView::setCutoffPoint(SRect2 left, SRect2 right) {
   ShadowLeft->setVisible(left.Size.X != 0.0f);
   ShadowRight->setVisible(right.Size.X != 0.0f);
   bool equal = false;

   if(left.otherCorner().Y > right.otherCorner().Y) {
      cutOffPoint = left.Position.X + left.Size.X;
   }
   else if(left.otherCorner().Y < right.otherCorner().Y) {
      cutOffPoint = right.Position.X;
   }
   else {
      equal = true;
      cutOffPoint = 0.0f;
   }
   float oldCutOff = cutOffPoint;
   if(!equal) {
      cutOffPoint = (cutOffPoint - (Area.getCenter().X - Area.Size.X/2.f))/Area.Size.X;
   }
}

void CPlayerView::updateView(float time) {
   CApplication::get().getSceneManager().getActiveCamera()->setPosition(SVector3(Area.getCenter().X, Area.getCenter().Y + 1.3f, 10) + ShakeFactor);

   ShadowLeft->setTranslation(SVector3(Area.getCenter().X, yLeftShadow + 0.01, 0));
   LeftShadowStartValue = 0.f;
   LeftShadowCutoffValue = cutOffPoint;

   ShadowRight->setTranslation(SVector3(Area.getCenter().X, yRightShadow + 0.01, 0));
   RightShadowStartValue = cutOffPoint;
   RightShadowCutoffValue = 1.0f;

   float rotateX = 15*sin(ySineValue/2)-90.f;

   CabbageSceneObject->setRotation(SVector3(rotateX, 0, Direction == CElementPlayer::Right ? 80.0f : 0.0f));

   translateCabbage(time);

   float ySineAmount = 0.065f*sin(ySineValue);

   CabbageSceneObject->setTranslation(SVector3(Area.getCenter().X, Area.getCenter().Y + ySineAmount, 0));
}

void CPlayerView::setHurt(bool b) {
   Hurt = b;
   if(Hurt) {
      HurtCabbageRenderables[CabbageIndex]->setVisible(true);
      NormalCabbageRenderables[CabbageIndex]->setVisible(false);
   }
   else {
      HurtCabbageRenderables[CabbageIndex]->setVisible(false);
      NormalCabbageRenderables[CabbageIndex]->setVisible(true);
   }
}

void CPlayerView::setVisible(bool b) {
   SceneObject->setVisible(b);
}


void CPlayerView::translateCabbage(float time) {
   //Verify we aren't moving vertically.
   if (PhysicsEngineObject->getVelocity().Y < .01f && PhysicsEngineObject->getVelocity().Y > -.01f) {
      if (Action == CElementPlayer::Standing)
         ySineValue += 5.f*time;

      else if (Action == CElementPlayer::Walking && Direction == CElementPlayer::Left)
         ySineValue +=10.f*time;

      else if (Action == CElementPlayer::Walking && Direction == CElementPlayer::Right)
         ySineValue -=10.f*time;
   }

   //If we are moving vertically, stop dancing and revert to our normal position.
   else
      ySineValue = 0.f;
}

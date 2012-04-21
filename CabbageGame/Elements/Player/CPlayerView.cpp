#include "CPlayerView.h"
#include "CApplication.h"

#include <sstream>
CPlayerView::CPlayerView(ISceneObject * obj, CElementPlayer::EDirection & dir, CElementPlayer::EAction & act, int CurHealth, SRect2 & nArea) :
   SceneObject(obj), CabbageIndex(CurHealth - 1), Direction(dir), Action(act), Hurt(false), Area(nArea) {
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
      cabbageRenderable->setVisible(CurHealth == i);

      NormalCabbage->addChild(cabbageRenderable);
      NormalCabbageRenderables[i-1] = cabbageRenderable;
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

void CPlayerView::updateView(float time) {
   SceneObject->setTranslation(SVector3(Area.getCenter().X, Area.getCenter().Y, 0));
}

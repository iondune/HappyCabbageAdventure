#include "CElementBlockDeath.h"
#include "CGameLevel.h"
#include "CElementPlayer.h"

CElementBlockDeath::CElementBlockDeath(SRect2f nArea, int D, int T, float R, float S)
: CElementBlock(nArea, D, T), Range(R), Speed(S) {
}

void CElementBlockDeath::OnCollision(const SCollisionEvent& Event) {
   Level.getPlayer().setHealth(0);
   return;
}

void CElementBlockDeath::update(float t) {
   CGameplayElement::update(t);
}

void CElementBlockDeath::writeXML(xmlwriter *l) {
   std::stringstream xValue, yValue, widthValue, heightValue, tagValue, rangeValue, speedValue, depthValue, textureType;
   xValue << Area.Position.X;
   yValue << Area.Position.Y;
   widthValue << Area.Size.X;
   heightValue << Area.Size.Y;
   depthValue << Depth;
   textureType << Texture;
   speedValue << 1;


   tagValue << "DeathBlock";

   l->AddAtributes("isMoving ", speedValue.str());
   l->AddAtributes("texture ", textureType.str());
   l->AddAtributes("depth ", depthValue.str());
   l->AddAtributes("width ", widthValue.str());
   l->AddAtributes("height ", heightValue.str());
   l->AddAtributes("Y ", yValue.str());
   l->AddAtributes("X ", xValue.str());
   l->Createtag(tagValue.str());
   l->CloseLasttag();
}

void CElementBlockDeath::setupSceneObject() {
  CMeshSceneObject *tempBlock = new CMeshSceneObject();
   CMesh *mesh;

   mesh = CMeshLoader::load3dsMesh("Base/cuberock.3ds");

   if(mesh) {
         mesh->resizeMesh(SVector3(1));
         mesh->centerMeshByExtents(SVector3(0));
         mesh->calculateNormalsPerFace();
         //mesh->calculateNormalsPerVertex();
   }

   else {
      fprintf(stderr, "COULD NOT LOAD BLOCK MESH.\n");
   }

   tempBlock->setMesh(mesh);
   tempBlock->setTexture("Base/dirt.bmp");

   tempBlock->setShader(ERP_DEFAULT, CShaderLoader::loadShader("ToonTexture"));
   tempBlock->setShader(ERP_DEFERRED_OBJECTS, CShaderLoader::loadShader("Deferred/Textured"));
   tempBlock->setTranslation(SVector3((Area.Position.X+(Area.Position.X+Area.Size.X))/2, (Area.Position.Y+(Area.Position.Y+Area.Size.Y))/2, 0));
   tempBlock->setScale(SVector3(Area.Size.X, Area.Size.Y, (float) Depth));
   tempBlock->setRotation(SVector3(Texture==-5?-90.f:0, 0, 0));

   SceneObject = tempBlock;
   CApplication::get().getSceneManager().addImmobileSceneObject(tempBlock, THIS_OBJECT_WILL_NEVER_MOVE_AND_ITS_BOUNDING_BOX_IS_CORRECT);

}

void CElementBlockDeath::setupPhysicsEngineObject() {
   PhysicsEngineObject = Level.getPhysicsEngine().addObject();
   PhysicsEngineObject->setArea(Area);
}

void CElementBlockDeath::printInformation() {
   printf("CElementBlockDeath; Area: [[%0.0f, %0.0f],[%0.0f, %0.0f]]\n", Area.Position.X, Area.Position.Y, Area.Size.X, Area.Size.Y);
}

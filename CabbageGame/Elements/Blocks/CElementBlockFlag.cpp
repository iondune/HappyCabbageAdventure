#include "CElementBlockFlag.h"
#include "CGameLevel.h"

CElementBlockFlag::CElementBlockFlag(SRect2 nArea, FlagRole role)
: CElementBlock(nArea, 1, 0), Role(role) {
}

CElementBlockFlag::CElementBlockFlag(SRect2 nArea, int role)
: CElementBlock(nArea, 1, 0), Role((FlagRole)role) {
}

void CElementBlockFlag::OnCollision(CCollideable *Object) {
   return;
}

void CElementBlockFlag::writeXML(xmlwriter *l) {
   std::stringstream xValue, yValue, widthValue, heightValue, tagValue, isMovingValue, secretFlag;
   xValue << Area.Position.X;
   yValue << Area.Position.Y;
   widthValue << Area.Size.X;
   heightValue << Area.Size.Y;
   secretFlag << Role;

   tagValue << "CFlag";

   l->AddAtributes("secret ", secretFlag.str());
   l->AddAtributes("width ", widthValue.str());
   l->AddAtributes("height ", heightValue.str());
   l->AddAtributes("Y ", yValue.str());
   l->AddAtributes("X ", xValue.str());
   l->Createtag(tagValue.str());
   l->CloseLasttag();
}

void CElementBlockFlag::setupPhysicsEngineObject() {
   PhysicsEngineObject = Level.getPhysicsEngine().addObject();
   PhysicsEngineObject->setArea(Area);
}

void CElementBlockFlag::setupSceneObject() {
   CMesh *mesh = CMeshLoader::load3dsMesh("Base/flag2.3ds");
   if(mesh) {
      mesh->centerMeshByExtents(SVector3(0));
      mesh->calculateNormalsPerFace();
   }
   else
      printf("ERROR.  MESH DID NOT LOAD PROPERLY.\n");

   SceneObject = new CMeshSceneObject();
   SceneObject->setMesh(mesh);
   SceneObject->setTranslation(SVector3(Area.Position, 1.0f));
   SceneObject->setRotation(SVector3(-90,0,0));
   SceneObject->setScale(SVector3(.0150f, .00025f,.0016f));
   SceneObject->setTexture(CImageLoader::loadTexture("Base/white.bmp", true));
   SceneObject->setShader(ERP_DEFAULT, "Toon");
   SceneObject->setShader(ERP_DEFERRED_OBJECTS, "Deferred/Toon");

   CApplication::get().getSceneManager().addImmobileSceneObject(SceneObject, THIS_OBJECT_WILL_NEVER_MOVE_AND_ITS_BOUNDING_BOX_IS_CORRECT);
}

void CElementBlockFlag::printInformation() {
   printf("CElementBlockFlag; Area: [[%0.0f, %0.0f],[%0.0f, %0.0f]]; Role: %d\n", Area.Position.X, Area.Position.Y, Area.Size.X, Area.Size.Y, Role);
}

CElementBlockFlag::FlagRole CElementBlockFlag::getRole() {
   return Role;
}

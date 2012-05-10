#include "CElementBlockFlag.h"
#include "CGameLevel.h"
#include "CElementPlayer.h"

CElementBlockFlag::CElementBlockFlag(SRect2f nArea, FlagRole role)
: CElementBlock(nArea, 1, 0), Role(role) {
}

CElementBlockFlag::CElementBlockFlag(SRect2f nArea, int role)
: CElementBlock(nArea, 1, 0), Role((FlagRole)role) {
}

void CElementBlockFlag::OnCollision(CCollideable *Object) {
	if (Object == Level.getPlayer().getPhysicsEngineObject()) {
		Level.getPhysicsEngine().removeObject(PhysicsEngineObject);
		Level.getPlayer().setVictoryFlag(true);
		Level.getPlayer().setAllowMovement(false);
	}
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
      mesh->centerMeshByExtents(SVector3f(0));
      mesh->calculateNormalsPerFace();
   }
   else
      printf("ERROR.  MESH DID NOT LOAD PROPERLY.\n");


   CMesh *eMesh = CMeshLoader::load3dsMesh("Base/cabbage/cabbage_5.3ds");
   if(eMesh) {
      eMesh->centerMeshByExtents(SVector3f(0));
      eMesh->calculateNormalsPerFace();
   }
   else
      printf("ENSIGNIA MESH DID NOT LOAD PROPERLY!\n");


   SceneObject = new CMeshSceneObject();
   SceneObject->setMesh(mesh);
   SceneObject->setRotation(SVector3f(-90,0,0));
   SceneObject->setScale(SVector3f(.0150f, .00025f,.0016f));
   SceneObject->setTexture(CImageLoader::loadTexture("Base/white.bmp", true));
   SceneObject->setShader(ERP_DEFAULT, "Toon");
   SceneObject->setShader(ERP_DEFERRED_OBJECTS, "Deferred/Toon");

   Area.Position.Y += Area.Size.Y/2.f;
   SceneObject->setTranslation(SVector3f(Area.Position, 1.0f));

   /*CMeshSceneObject *flagObject = new CMeshSceneObject();
   flagObject->setMesh(mesh);
   flagObject->setRotation(SVector3f(-90,0,0));
   flagObject->setScale(SVector3f(.0150f, .00025f,.0016f));
   flagObject->setTexture(CImageLoader::loadTexture("Base/white.bmp", true));
   flagObject->setShader(ERP_DEFAULT, "Toon");
   flagObject->setShader(ERP_DEFERRED_OBJECTS, "Deferred/Toon");

   CMeshSceneObject *ensigniaObject = new CMeshSceneObject();
   ensigniaObject->setMesh(eMesh);
   ensigniaObject->setTranslation(SVector3f(0.0f, 3.0f, 0.0f));
   ensigniaObject->setRotation(SVector3f(-90,0,0));
   ensigniaObject->setScale(SVector3f(4.0f, 4.0f,4.0f));
   ensigniaObject->setShader(ERP_DEFAULT, "Toon");
   ensigniaObject->setShader(ERP_DEFERRED_OBJECTS, "Deferred/Toon");

   SceneObject->addChild(ensigniaObject);
   SceneObject->addChild(flagObject);*/


   CApplication::get().getSceneManager().addSceneObject(SceneObject);
}

void CElementBlockFlag::printInformation() {
   printf("CElementBlockFlag; Area: [[%0.0f, %0.0f],[%0.0f, %0.0f]]; Role: %d\n", Area.Position.X, Area.Position.Y, Area.Size.X, Area.Size.Y, Role);
}

CElementBlockFlag::FlagRole CElementBlockFlag::getRole() {
   return Role;
}

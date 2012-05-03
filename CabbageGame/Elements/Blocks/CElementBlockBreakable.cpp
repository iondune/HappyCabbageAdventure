#include "CElementBlockBreakable.h"
#include "Player/CPlayerAbility.h"
#include "CGameLevel.h"
#include "CElementPlayer.h"

CElementBlockBreakable::CElementBlockBreakable(SRect2 nArea)
: CElementBlock(nArea, 1, 0) {
}

void CElementBlockBreakable::OnCollision(CCollideable *Object) {
	if (Object == Level.getPlayer().getPhysicsEngineObject()) {
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

void CElementBlockBreakable::setupPhysicsEngineObject() {
   PhysicsEngineObject = Level.getPhysicsEngine().addObject();
   PhysicsEngineObject->setArea(Area);
}

void CElementBlockBreakable::setupSceneObject() {
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

   tempBlock->setRotation(SVector3(0, 0, 0));

   SceneObject = tempBlock;
   CApplication::get().getSceneManager().addSceneObject(tempBlock);
}

void CElementBlockBreakable::printInformation() {
   printf("CElementBlockBreakable; Area: [[%0.0f, %0.0f],[%0.0f, %0.0f]]\n", Area.Position.X, Area.Position.Y, Area.Size.X, Area.Size.Y);
}

void CElementBlockBreakable::reactToAbility(Abilities::EAbilityType Ability) {
   switch(Ability) {
      case Abilities::LASER:
         if(!Dead)
            this->removeFromGame();
         break;
      default:
         break;
   }
}

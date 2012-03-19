#include "ISceneObject.h"

#include <iostream>

#include "../CabbageCore/glm/gtc/matrix_transform.hpp"

#include "CShaderLoader.h"
#include "CSceneManager.h"


ISceneObject::ISceneObject()
    : DebugDataFlags(0), Visible(true), Parent(0), UseCulling(true), Immobile(false)
{}


void ISceneObject::updateAbsoluteTransformation()
{
   numCulled = 0;
   numObjects = 0;
   if(Immobile)
      return;
	AbsoluteTransformation = Transformation;
	if (Parent)
	{
		AbsoluteTransformation = Parent->AbsoluteTransformation* AbsoluteTransformation;
	}

	for (std::list<ISceneObject *>::iterator it = Children.begin(); it != Children.end(); ++ it)
	{
		(* it)->updateAbsoluteTransformation();
	}
}

glm::mat4 const & ISceneObject::getAbsoluteTransformation() const
{
	return AbsoluteTransformation;
}

void ISceneObject::setTranslation(SVector3 const & translation)
{
   assert(!Immobile);
	Translation = translation;
	Transformation.setTranslation(translation);
}

void ISceneObject::setRotation(SVector3 const & rotation)
{
	Rotation = rotation;
	Transformation.setRotation(rotation);
}

void ISceneObject::setRotation(glm::mat4 const & matrix)
{
	Transformation.setRotation(matrix);
}

void ISceneObject::setScale(SVector3 const & scale)
{
   //assert(!Immobile);
	Scale = scale;
	Transformation.setScale(scale);
}

void ISceneObject::update()
{
	for (std::list<ISceneObject *>::iterator it = Children.begin(); it != Children.end(); ++ it)
		(* it)->update();
}


void ISceneObject::draw(CScene const * const scene, ERenderPass const Pass)
{
	if (! Visible)
		return;

	for (std::list<ISceneObject *>::iterator it = Children.begin(); it != Children.end(); ++ it) {
      numObjects++;
		if (! (* it)->isCulled(scene)) {
			(* it)->draw(scene, Pass);
      }
      else {
         numCulled++;
      }
   }
}

SBoundingBox3 const & ISceneObject::getBoundingBox() const
{
    return BoundingBox;
}

SBoundingBox3 & ISceneObject::getBoundingBox()
{
    return BoundingBox;
}
void ISceneObject::setBoundingBox(SBoundingBox3 const & boundingBox) {
   BoundingBox = boundingBox;
}

bool const ISceneObject::isDebugDataEnabled(EDebugData::Domain const type) const
{
    return (type & DebugDataFlags) != 0;
}

void ISceneObject::enableDebugData(EDebugData::Domain const type)
{
	for (std::list<ISceneObject *>::iterator it = Children.begin(); it != Children.end(); ++ it)
		(* it)->enableDebugData(type);

    if (type == EDebugData::None)
        DebugDataFlags = 0;
    else
        DebugDataFlags |= type;
}

void ISceneObject::disableDebugData(EDebugData::Domain const type)
{
	for (std::list<ISceneObject *>::iterator it = Children.begin(); it != Children.end(); ++ it)
		(* it)->disableDebugData(type);

    if (type == EDebugData::None)
        DebugDataFlags = 0;
    else
        DebugDataFlags ^= type;
}

bool const ISceneObject::intersectsWithLine(SLine3 const & line) const
{
    return BoundingBox.intersectsWithLine(line);
}

bool const ISceneObject::isVisible() const
{
    return Visible;
}

void ISceneObject::setVisible(bool const isVisible)
{
    Visible = isVisible;
}

STransformation3 const & ISceneObject::getTransformation() const
{
	return Transformation;
}

ISceneObject const * const ISceneObject::getParent() const
{
	return Parent;
}

std::list<ISceneObject *> const & ISceneObject::getChildren() const
{
	return Children;
}

void ISceneObject::removeChild(ISceneObject * child)
{
	Children.erase(std::remove(Children.begin(), Children.end(), child), Children.end());
	child->Parent = 0;
}

void ISceneObject::addChild(ISceneObject * child)
{
	Children.push_back(child);
	child->Parent = this;
}

void ISceneObject::setParent(ISceneObject * parent)
{
	if (Parent)
		Parent->removeChild(this);
	if (parent)
		parent->addChild(this);
}

void ISceneObject::removeChildren()
{
	Children.clear();
}

SVector3 const & ISceneObject::getRotation() const
{
	return Rotation;
}

SVector3 const & ISceneObject::getTranslation() const
{
	return Translation;
}

SVector3 const & ISceneObject::getScale() const
{
	return Scale;
}

#include "CCamera.h"
int timesCalled = 0;

bool const ISceneObject::isCulled(CScene const * const Scene) const
{
    timesCalled++;
    if (! UseCulling || ! Scene->isCullingEnabled())
        return false;

    int i = 0;
    for (int i = 0; i < 8; ++ i) {
        SVector3 const Center = getBoundingBox().getCorner(i);
        glm::vec4 Center4(Center.X, Center.Y, Center.Z, 1.f);

        glm::mat4 PVM(Scene->getActiveCamera()->getProjectionMatrix()*Scene->getActiveCamera()->getViewMatrix()*Transformation());
        glm::vec4 prime = PVM * Center4;

        float length = glm::length(glm::vec3(prime.x, prime.y, prime.z));
        //SVector3 v = SVector3(prime.x, prime.y, prime.z) / length;
        float w = prime.w / length;
        if ((-prime.w < prime.x) && (prime.w > prime.x) &&
            (-prime.w < prime.y) && (prime.w > prime.y) && 
            (-prime.w < prime.z) && (prime.w > prime.z))
            return false;
    }

    return true;
}

SVector3 ISceneObject::getWorldBoundingBoxMinPoint() {
   SVector3 p = getBoundingBox().MinCorner; 
   glm::vec4 p4(p.X, p.Y, p.Z, 1.f);
   glm::vec4 temp = Transformation() * p4; 

   return SVector3(temp.x, temp.y, temp.z);
}

SBoundingBox3 ISceneObject::getWorldBoundingBox() {
   SVector3 min = getBoundingBox().MinCorner; 
   glm::vec4 min4(min.X, min.Y, min.Z, 1.f);
   glm::vec4 temp = Transformation() * min4; 

   SVector3 max = getBoundingBox().MinCorner; 
   glm::vec4 max4(max.X, max.Y, max.Z, 1.f);
   glm::vec4 temp2 = Transformation() * max4; 

   return SBoundingBox3(SVector3(min4.x, min4.y, min4.z), SVector3(max4.x, max4.y, max4.z));
}


bool const ISceneObject::isCullingEnabled() const
{
	return UseCulling;
}

void ISceneObject::setCullingEnabled(bool const culling)
{
	UseCulling = culling;
}

void ISceneObject::load(CScene const * const Scene, ERenderPass const Pass)
{
	for (std::list<ISceneObject *>::iterator it = Children.begin(); it != Children.end(); ++ it)
		(* it)->load(Scene, Pass);
}

void ISceneObject::setTreeImmobile(bool value) {
   Immobile = value;
	for (std::list<ISceneObject *>::iterator it = Children.begin(); it != Children.end(); ++ it)
		(* it)->setTreeImmobile(value);
}

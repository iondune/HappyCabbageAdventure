#include "ISceneObject.h"


void ISceneObject::checkAbsoluteTransformation()
{
	// Absolute Transformations are recalculated top-down
	// A dirty transformation triggers a rebuild down all children

	if (TransformationDirty)
		updateAbsoluteTransformation();
	else
		for (std::list<ISceneObject *>::iterator it = Children.begin(); it != Children.end(); ++ it)
			(* it)->checkAbsoluteTransformation();
}

void ISceneObject::updateAbsoluteTransformation()
{
	AbsoluteTransformation = Transformation;

	if (Parent)
		AbsoluteTransformation = Parent->AbsoluteTransformation* AbsoluteTransformation;

	for (std::list<ISceneObject *>::iterator it = Children.begin(); it != Children.end(); ++ it)
		(* it)->updateAbsoluteTransformation();
}

bool ISceneObject::checkAbsoluteBoundingBox()
{
	// Absolute Bounding Boxes are recalculate bottom-up
	// A dirty bounding box triggers a rebuild all the way up

	bool RebuildNecessary = false;
	
	for (std::list<ISceneObject *>::iterator it = Children.begin(); it != Children.end(); ++ it)
		RebuildNecessary |= (* it)->checkAbsoluteBoundingBox();

	if (RebuildNecessary)
		updateAbsoluteBoundingBox();

	return RebuildNecessary;
}

void ISceneObject::updateAbsoluteBoundingBox()
{
	AbsoluteBoundingBox = BoundingBox;

	for (std::list<ISceneObject *>::iterator it = Children.begin(); it != Children.end(); ++ it)
		AbsoluteBoundingBox.addInternalBox((* it)->getAbsoluteBoundingBox());
}

void ISceneObject::update()
{
	for (std::list<ISceneObject *>::iterator it = Children.begin(); it != Children.end(); ++ it)
		(* it)->update();
}

void ISceneObject::load(CScene const * const Scene, ERenderPass const Pass)
{
	for (std::list<ISceneObject *>::iterator it = Children.begin(); it != Children.end(); ++ it)
		(* it)->load(Scene, Pass);
}

bool ISceneObject::draw(CScene const * const scene, ERenderPass const Pass)
{
	if (! Visible)
		return false;
	
	++ TotalObjects;

	if (isCulled(scene, true))
	{
		++ ObjectsCulled;
		return false;
	}
	else
	{
		for (std::list<ISceneObject *>::iterator it = Children.begin(); it != Children.end(); ++ it)
			(* it)->draw(scene, Pass);

		return ! isCulled(scene, false);
	}
}

#include "CSceneObject.h"


void CSceneObject::load(IScene const * const Scene, ERenderPass const Pass)
{
	ISceneObject::load(Scene, Pass);

	for (std::vector<CRenderable *>::iterator it = Renderables.begin(); it != Renderables.end(); ++ it)
	{
		(* it)->load(Scene, Pass);
	}
}

bool CSceneObject::draw(IScene const * const Scene, ERenderPass const Pass, bool const CullingEnabled)
{
	if (! ISceneObject::draw(Scene, Pass, CullingEnabled))
		return false;

	switch (Pass)
	{
	case ERenderPass::Default:
	case ERenderPass::DeferredColors:
	case ERenderPass::ModelSpaceNormals:
		for (std::vector<CRenderable *>::iterator it = Renderables.begin(); it != Renderables.end(); ++ it)
			(* it)->draw(Scene, Pass);
		break;

	case ERenderPass::DeferredLights:
		break;
	}

	return true;
}

#include "CDirectionalLightSceneObject.h"

#include "CShaderLoader.h"
#include "CShaderContext.h"

#include "CSceneEffectManager.h"
#include "CSceneManager.h"
#include "CDeferredShadingManager.h"


CDirectionalLightSceneObject::CDirectionalLightSceneObject(SVector3f const direction, SColor const & color)
	: Color(color), Direction(direction)
{
	Shader = CShaderLoader::loadShader("Deferred/DirectionalLight");

	setCullingEnabled(false);
}

void CDirectionalLightSceneObject::draw(CScene const * const scene, ERenderPass const Pass)
{
	if (! Visible)
		return;

	ISceneObject::draw(scene, Pass);

	switch (Pass)
	{
	case ERP_DEFAULT:
	case ERP_DEFERRED_OBJECTS:
	case ERP_MODEL_NORMALS:
		break;

	case ERP_DEFERRED_LIGHTS:
		{
			if (! Shader)
				break;

			CSceneEffectManager::SPostProcessPass Pass;
			Pass.Textures["uNormal"] = ((CDeferredShadingManager *) ((CSceneManager *)scene)->getEffectManager())->DeferredNormalOutput;
			Pass.SetTarget = false;
			Pass.Shader = Shader;

			Pass.begin();
			Pass.Context->uniform("uColor", Color);
			Pass.Context->uniform("uDirection", Direction);

			Pass.end();
			break;
		}
	}
}

void CDirectionalLightSceneObject::load(CScene const * const Scene)
{
}

void CDirectionalLightSceneObject::setDirection(SVector3f const & direction)
{
	Direction = direction;
}

void CDirectionalLightSceneObject::setColor(SVector3f const & color)
{
	Color = color;
}

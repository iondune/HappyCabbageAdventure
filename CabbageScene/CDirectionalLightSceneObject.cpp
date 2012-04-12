#include "CDirectionalLightSceneObject.h"

#include "CShaderLoader.h"
#include "CShaderContext.h"

#include "CSceneEffectManager.h"
#include "CSceneManager.h"
#include "CDeferredShadingManager.h"


CDirectionalLightSceneObject::CDirectionalLightSceneObject(SVector3 const direction, SColor const & color)
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

			CShaderContext Context(* Shader);
			Context.bindIndexBufferObject(MeshBuffer->IndexBuffer.getHandle());
			Context.bindBufferObject("aPosition", MeshBuffer->PositionBuffer.getHandle(), 3);
			Context.uniform("uModelMatrix", AbsoluteTransformation);
			Context.uniform("uViewMatrix", scene->getActiveCamera()->getViewMatrix());
			Context.uniform("uProjMatrix", scene->getActiveCamera()->getProjectionMatrix());
			Context.uniform("uColor", Color);
			Context.uniform("uRadius", Scale.X);

			Context.bindTexture("uNormal", ((CDeferredShadingManager *) ((CSceneManager *)scene)->getEffectManager())->DeferredNormalOutput);
			Context.bindTexture("uPosition", ((CDeferredShadingManager *) ((CSceneManager *)scene)->getEffectManager())->DeferredPositionOutput);

			glDrawElements(GL_TRIANGLES, MeshBuffer->IndexBuffer.getElements().size(), GL_UNSIGNED_SHORT, 0);
			break;
		}
	}
}

void CDirectionalLightSceneObject::load(CScene const * const Scene)
{
}

void CDirectionalLightSceneObject::setDirection(SVector3 const & direction)
{
	Direction = direction;
}

void CDirectionalLightSceneObject::setColor(SVector3 const & color)
{
	Color = color;
}

#include "CPointLightSceneObject.h"

#include "CShaderLoader.h"
#include "CShaderContext.h"

#include "CSceneEffectManager.h"
#include "CSceneManager.h"
#include "CDeferredShadingManager.h"


CPointLightSceneObject::CPointLightSceneObject()
{
	SphereMesh = CMeshLoader::load3dsMesh("Sphere.3ds");
	SphereMesh->resizeMesh(SVector3(1.f));
	SphereMesh->updateBuffers();

	setScale(SVector3(3.f));

	MeshBuffer = SphereMesh->MeshBuffers[0];

	Shader = CShaderLoader::loadShader("Deferred/PointLight");

	setCullingEnabled(false);
}

void CPointLightSceneObject::draw(CScene const * const scene, ERenderPass const Pass)
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
			if (MeshBuffer->IndexBuffer.isDirty())
				MeshBuffer->IndexBuffer.syncData();

			if (MeshBuffer->PositionBuffer.isDirty())
				MeshBuffer->PositionBuffer.syncData();

			CShaderContext Context(* Shader);
			Context.bindIndexBufferObject(MeshBuffer->IndexBuffer.getHandle());
			Context.bindBufferObject("aPosition", MeshBuffer->PositionBuffer.getHandle(), 3);
			Context.uniform("uModelMatrix", AbsoluteTransformation);
			Context.uniform("uViewMatrix", scene->getActiveCamera()->getViewMatrix());
			Context.uniform("uProjMatrix", scene->getActiveCamera()->getProjectionMatrix());
			Context.uniform("uPosition", Translation);

			Context.bindTexture("uPosition", ((CDeferredShadingManager *) ((CSceneManager *)scene)->getEffectManager())->DeferredPositionOutput);
			Context.bindTexture("uNormal", ((CDeferredShadingManager *) ((CSceneManager *)scene)->getEffectManager())->DeferredNormalOutput);

			glDrawElements(GL_TRIANGLES, MeshBuffer->IndexBuffer.getElements().size(), GL_UNSIGNED_SHORT, 0);
			break;
		}
	}
}

void CPointLightSceneObject::load(CScene const * const Scene)
{
}

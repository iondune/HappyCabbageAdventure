#include "CPointLightSceneObject.h"

#include "CShaderLoader.h"
#include "CShaderContext.h"

#include "CSceneEffectManager.h"
#include "CSceneManager.h"
#include "CDeferredShadingManager.h"


CPointLightSceneObject::CPointLightSceneObject(float const radius, SColor const & color)
	: Color(color)
{
	SphereMesh = CMeshLoader::load3dsMesh("Sphere.3ds");

	if (SphereMesh)
	{
		SphereMesh->resizeMesh(SVector3(2.f));
		SphereMesh->updateBuffers();

		MeshBuffer = SphereMesh->MeshBuffers[0];

		Shader = CShaderLoader::loadShader("Deferred/PointLight");
	}

	setRadius(radius);
	setBoundingBox(SphereMesh->getBoundingBox());
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

			if (! Shader || ! SphereMesh)
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

void CPointLightSceneObject::load(CScene const * const Scene)
{
}

void CPointLightSceneObject::setRadius(float const Radius)
{
	ISceneObject::setScale(SVector3(Radius));
}

void CPointLightSceneObject::setScale(SVector3f const & scale)
{
}

void CPointLightSceneObject::setColor(SVector3f const & color)
{
	Color = color;
}

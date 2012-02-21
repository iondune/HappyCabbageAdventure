#include "CMeshSceneObject.h"

#include "CShaderContext.h"
#include "CShaderLoader.h"

#include "../CabbageCore/glm/gtc/matrix_transform.hpp"


CMeshSceneObject::CMeshSceneObject()
	: LoadedRevision(-1)
{}

void CMeshSceneObject::update()
{
	ISceneObject::update();

	if (Mesh->getRevision() != LoadedRevision)
		setMesh(Mesh);
}

CMesh * CMeshSceneObject::getMesh()
{
    return Mesh;
}

void CMeshSceneObject::setMesh(CMesh * mesh)
{
	CMesh * OldMesh = Mesh;
    Mesh = mesh;

    if (Mesh)
    {
		if (Mesh == OldMesh && Mesh->getRevision() == LoadedRevision)
			return;
		
		Mesh->updateBuffers();

        for (unsigned int i = 0; i < Mesh->MeshBuffers.size(); ++ i)
        {
            CRenderable * Child = 0;
            if (Renderables.size() > i)
                Child = Renderables[i];
            else
            {
                Renderables.push_back(Child = new CRenderable(this));
            }

            // Remove any attributes which might have been set by a previous mesh
            Child->removeAttribute("aPosition");
            Child->removeAttribute("aColor");
            Child->removeAttribute("aNormal");
            Child->removeAttribute("aTexCoord");
            Child->removeUniform("uTexColor");

            // Add mesh attributes
            Child->addAttribute("aPosition", new SAttribute<float>(& Mesh->MeshBuffers[i]->PositionBuffer, 3));
            Child->addAttribute("aColor", new SAttribute<float>(& Mesh->MeshBuffers[i]->ColorBuffer, 3));
            Child->addAttribute("aNormal", new SAttribute<float>(& Mesh->MeshBuffers[i]->NormalBuffer, 3));
            Child->addAttribute("aTexCoord", new SAttribute<float>(& Mesh->MeshBuffers[i]->TexCoordBuffer, 2));
            Child->addUniform("uTexColor", new SUniform<int>(0));

			Child->getMaterial().DiffuseColor = Mesh->MeshBuffers[i]->Material.DiffuseColor;

            // Add mesh index buffer
            Child->setIndexBufferObject(& Mesh->MeshBuffers[i]->IndexBuffer);

            // Set bounding box
            //Child->BoundingBox = Mesh->getBoundingBox();

            // Remove any previous normal debugging object
            if (Child->getDebuggingNormalObject())
            {
                delete Child->getDebuggingNormalObject();
                Child->getDebuggingNormalObject() = 0;
            }

            // Add normal debugging object
            Child->getDebuggingNormalObject() = new CRenderable(this);
            Child->getDebuggingNormalObject()->addAttribute("aPosition", new SAttribute<float>(& Mesh->MeshBuffers[i]->NormalLineBuffer, 3));
            Child->getDebuggingNormalObject()->addAttribute("aColor", new SAttribute<float>(& Mesh->MeshBuffers[i]->NormalColorBuffer, 3));
            Child->getDebuggingNormalObject()->setIndexBufferObject(& Mesh->MeshBuffers[i]->NormalIndexBuffer);
            Child->getDebuggingNormalObject()->setShader(CShaderLoader::loadShader("Simple"));
            Child->getDebuggingNormalObject()->setDrawType(GL_LINES);

            // Reset the shader to load attributes again
            Child->reloadVariablesOnNextDraw();
        }
    }

}

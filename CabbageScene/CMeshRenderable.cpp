#include "CMeshRenderable.h"

#include "CShaderContext.h"
#include "CShaderLoader.h"

#include "../CabbageCore/glm/gtc/matrix_transform.hpp"


CMeshRenderable::CMeshRenderable()
{}

CMesh * CMeshRenderable::getMesh()
{
    return Mesh;
}

void CMeshRenderable::setMesh(CMesh * mesh)
{
    Mesh = mesh;

    if (Mesh)
    {
        std::vector<CBufferObject<float> *> PositionBuffers, ColorBuffers, NormalBuffers, TexCoordBuffers, NormalLineBuffers, NormalColorBuffers;
        std::vector<CBufferObject<unsigned short> *> IndexBuffers, NormalIndexBuffers;

        PositionBuffers = Mesh->makePositionBuffer();
        ColorBuffers = Mesh->makeColorBuffer();
        NormalBuffers = Mesh->makeNormalBuffer();
        TexCoordBuffers = Mesh->makeTexCoordBuffer();
        NormalLineBuffers = Mesh->makeNormalLineBuffer();
        NormalColorBuffers = Mesh->makeNormalColorBuffer();

        IndexBuffers = Mesh->makeIndexBuffer();
        NormalIndexBuffers = Mesh->makeNormalIndexBuffer();

        if (PositionBuffers.size() != ColorBuffers.size() || 
            PositionBuffers.size() != NormalBuffers.size() || 
            PositionBuffers.size() != TexCoordBuffers.size() || 
            PositionBuffers.size() != NormalLineBuffers.size() || 
            PositionBuffers.size() != NormalColorBuffers.size() || 
            PositionBuffers.size() != IndexBuffers.size() || 
            PositionBuffers.size() != NormalIndexBuffers.size())
        {
            std::cerr << "Buffer count mixmatch, object will not draw!" << std::endl;
        }

        for (unsigned int i = 0; i < PositionBuffers.size(); ++ i)
        {
            CMeshRenderable * Child = 0;
            if (SubRenderables.size() > i)
                Child = SubRenderables[i];
            else
            {
                SubRenderables.push_back(Child = new CMeshRenderable());
            }

            // Remove any attributes which might have been set by a previous mesh
            Child->removeAttribute("aPosition");
            Child->removeAttribute("aColor");
            Child->removeAttribute("aNormal");
            Child->removeAttribute("aTexCoord");
            Child->removeUniform("uTexColor");

            // Add mesh attributes
            Child->addAttribute("aPosition", boost::shared_ptr<IAttribute>(new CFloatVecAttribute(PositionBuffers[i], 3)));
            Child->addAttribute("aColor", boost::shared_ptr<IAttribute>(new CFloatVecAttribute(ColorBuffers[i], 3)));
            Child->addAttribute("aNormal", boost::shared_ptr<IAttribute>(new CFloatVecAttribute(NormalBuffers[i], 3)));
            Child->addAttribute("aTexCoord", boost::shared_ptr<IAttribute>(new CFloatVecAttribute(TexCoordBuffers[i], 2)));
            Child->addUniform("uTexColor", boost::shared_ptr<IUniform>(new CIntUniform(0)));

            Child->Material.DiffuseColor->Value = Mesh->MeshBuffers[i]->DiffuseColor;

            // Add mesh index buffer
            Child->setIndexBufferObject(IndexBuffers[i]);

            // Set bounding box
            Child->BoundingBox = Mesh->getBoundingBox();

            // Remove any previous normal debugging object
            if (Child->NormalObject)
            {
                delete Child->NormalObject;
                Child->NormalObject = 0;
            }

            // Add normal debugging object
            Child->NormalObject = new CRenderable();
            Child->NormalObject->addAttribute("aPosition", boost::shared_ptr<IAttribute>(new CFloatVecAttribute(NormalLineBuffers[i], 3)));
            Child->NormalObject->addAttribute("aColor", boost::shared_ptr<IAttribute>(new CFloatVecAttribute(NormalColorBuffers[i], 3)));
            Child->NormalObject->setIndexBufferObject(NormalIndexBuffers[i]);
            Child->NormalObject->getMaterial().Shader = CShaderLoader::loadShader("Simple");
            Child->NormalObject->setDrawType(GL_LINES);

            // Reset the shader to load attributes again
            Child->LastLoadedShader = 0;
        }
    }

}

void CMeshRenderable::draw(CScene const * const scene)
{
    if (! Visible)
        return;

    if (SubRenderables.size())
    {
        for (std::vector<CMeshRenderable *>::iterator it = SubRenderables.begin(); it != SubRenderables.end(); ++ it)
        {
            (* it)->Translation = Translation;
            (* it)->Rotation = Rotation;
            (* it)->RotationMatrix = RotationMatrix;
            (* it)->UsesRotationMatrix = UsesRotationMatrix;
            (* it)->Scale = Scale;

            (* it)->Material.AmbientColor = Material.AmbientColor;
            //(* it)->Material.DiffuseColor = Material.DiffuseColor;
            (* it)->Material.Shader = Material.Shader;
            (* it)->Material.Shininess = Material.Shininess;
            (* it)->Material.Texture = Material.Texture;
            
            (* it)->draw(scene);
        }
    }
    else
        CRenderable::draw(scene);
}

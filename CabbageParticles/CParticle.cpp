#include "CParticle.h"

CParticleRenderable::CParticleRenderable()
{}

CMesh * CParticleRenderable::getMesh()
{
    return Mesh;
}

void CParticleRenderable::draw(CScene const * const scene) {
   if (! Visible)
      return;

   /* Code parallels CMeshRenderable::draw() */
   if (SubRenderables.size())
   {
      for (std::vector<CParticleRenderable *>::iterator it = SubRenderables.begin(); it != SubRenderables.end(); ++ it)
      {
         (* it)->centerPos = centerPos;
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
   /* Code parallels CRenderable::draw() */
   else {
      // If no ibo loaded, we can't draw anything
      // If the ibo loaded hasn't been synced as an index buffer object, 
      if (! IndexBufferObject || ! IndexBufferObject->isIndexBuffer())
      {
         std::cout << "Failed to draw object: IBO address == " << IndexBufferObject << std::endl;
         return;
      }

      CShader * ShaderToUse = Material.Shader;

      if (! ShaderToUse)
         ShaderToUse = CShaderLoader::loadShader("Simple");

      // If normal colors are being shown, switch to the normal color shader
      if (isDebugDataEnabled(EDebugData::NormalColors))
      {
         if (! NormalColorShader)
            NormalColorShader = CShaderLoader::loadShader("NormalColor");
         ShaderToUse = NormalColorShader;
      }

      loadHandlesFromShader(ShaderToUse, scene);

      // Create shader context and link all variables required by the shader
      CShaderContext ShaderContext(* ShaderToUse);

      // Set up transform matrices
      uModelMatrix->Value = glm::translate(glm::mat4(1.0f), centerPos->getGLMVector());
      uModelMatrix->Value = glm::translate(uModelMatrix->Value, Translation.getGLMVector());
      uModelMatrix->Value = glm::rotate(uModelMatrix->Value, Rotation.X, glm::vec3(1, 0, 0));
      uModelMatrix->Value = glm::rotate(uModelMatrix->Value, Rotation.Y, glm::vec3(0, 1, 0));
      uModelMatrix->Value = UsesRotationMatrix ? uModelMatrix->Value * RotationMatrix : glm::rotate(uModelMatrix->Value, Rotation.Z, glm::vec3(0, 0, 1));
      uModelMatrix->Value = glm::scale(uModelMatrix->Value, Scale.getGLMVector());

      // Pass transform matrices to shader
      uNormalMatrix->Value = glm::transpose(glm::inverse(uModelMatrix->Value));

      // Pass values to shader
      for (std::map<std::string, SAttribute>::iterator it = Attributes.begin(); it != Attributes.end(); ++ it)
      {
         if (it->second.Handle >= 0)
            it->second.Value->bindTo(it->second.Handle, ShaderContext);
      }
      for (std::map<std::string, SUniform>::iterator it = Uniforms.begin(); it != Uniforms.end(); ++ it)
      {
         if (it->second.Handle >= 0)
            it->second.Value->bindTo(it->second.Handle, ShaderContext);
      }
      for (std::vector<SUniform>::iterator it = SceneLoadedUniforms.begin(); it != SceneLoadedUniforms.end(); ++ it)
      {
         it->Value->bindTo(it->Handle, ShaderContext);
      }

      // Set up texturing if a texture was supplied
      if (Material.Texture)
      {
         glEnable(GL_TEXTURE_2D);
         glActiveTexture(GL_TEXTURE0);
         glBindTexture(GL_TEXTURE_2D, Material.Texture->getTextureHandle());
      }

      // If the ibo is dirty, sync it!
      if (IndexBufferObject->isDirty())
         IndexBufferObject->syncData();

      // And bind the synced buffer object to shader...
      ShaderContext.bindIndexBufferObject(IndexBufferObject->getHandle());

      // Finally draw!
      glDrawElements(DrawType, IndexBufferObject->getElements().size(), GL_UNSIGNED_SHORT, 0);

      // Draw the normal object if it is enabled
      if (isDebugDataEnabled(EDebugData::Normals) && NormalObject)
      {
         NormalObject->setTranslation(Translation);
         NormalObject->setScale(Scale);
         if (UsesRotationMatrix)
            NormalObject->setRotation(RotationMatrix);
         else
            NormalObject->setRotation(Rotation);
         NormalObject->draw(scene);
      }

      // Cleanup the texture if it was used
      if (Material.Texture)
      {
         glDisable(GL_TEXTURE_2D);
      }
   }
}

void CParticleRenderable::setMesh(CMesh * mesh)
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
            CParticleRenderable * Child = 0;
            if (SubRenderables.size() > i)
                Child = SubRenderables[i];
            else
            {
                SubRenderables.push_back(Child = new CParticleRenderable());
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


void CParticle::setCenterPos(SVector3 *cPos) {
   centerPos = cPos;
}

CRenderable * CParticle::getRenderable() {
   return renderable;
}

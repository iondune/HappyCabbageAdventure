#include "CRenderable.h"

#include "CSceneObject.h"
#include "SUniform.h"
#include "SAttribute.h"
#include "CShaderContext.h"
#include "IScene.h"


void CRenderable::draw(IScene const * const Scene, ERenderPass const Pass, CShaderContext & ShaderContext)
{	
	// If the ibo loaded hasn't been synced as an index buffer object, we can't draw anything
	if (IndexBufferObject && ! IndexBufferObject->isIndexBuffer())
	{
		std::cout << "Failed to draw object, IBO is not index buffer." << std::endl;
		return;
	}

	// Set up transform matrices
	ModelMatrix = Transformation() * ParentObject->getAbsoluteTransformation();
	NormalMatrix = glm::transpose(glm::inverse(ModelMatrix));

	// Pass local values to shader
	for (std::map<std::pair<GLuint, std::string>, boost::shared_ptr<IAttribute const> >::iterator it = LoadedAttributes.begin(); it != LoadedAttributes.end(); ++ it)
		it->second->bind(it->first.first);
	for (std::map<std::pair<GLuint, std::string>, boost::shared_ptr<IUniform const> >::iterator it = LoadedUniforms.begin(); it != LoadedUniforms.end(); ++ it)
		it->second->bind(it->first.first);

	// Set up texturing if a texture was supplied
	if (Material.Textures.size())
	{
		glEnable(GL_TEXTURE_2D);

		for (unsigned int i = 0; i < Material.Textures.size(); ++ i)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, Material.Textures[i]->getTextureHandle());
		}
	}

	if (IndexBufferObject)
	{
		// If the ibo is dirty, sync it!
		if (IndexBufferObject->isDirty())
			IndexBufferObject->syncData();

		// And bind the synced buffer object to shader...
		ShaderContext.bindIndexBufferObject(IndexBufferObject->getHandle());

		glDrawElements(DrawElementType, IndexBufferObject->getElements().size(), GL_UNSIGNED_SHORT, 0);
	}
	else
	{
		glDrawArrays(DrawElementType, 0, ElementCount);
	}

	// Draw the normal object if it is enabled
	if (ParentObject->isDebugDataEnabled(EDebugData::Normals) && NormalObject)
	{
		NormalObject->Transformation = Transformation;
		NormalObject->draw(Scene, Pass, ShaderContext);
	}

	// Cleanup shader variables
	for (std::map<std::pair<GLuint, std::string>, boost::shared_ptr<IAttribute const> >::iterator it = LoadedAttributes.begin(); it != LoadedAttributes.end(); ++ it)
		it->second->unbind(it->first.first);

	// Cleanup the texture if it was used
	if (Material.Textures.size())
	{
		glDisable(GL_TEXTURE_2D);

		for (unsigned int i = 0; i < Material.Textures.size(); ++ i)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
}

void CRenderable::load(IScene const * const Scene, ERenderPass const Pass)
{
	if (IndexBufferObject && IndexBufferObject->isDirty())
		IndexBufferObject->syncData();

	LoadedAttributes.clear();
	LoadedUniforms.clear();

	CShader * Shader = ParentObject->getShader(Pass);
	if (! Shader)
		return;

	for (std::map<std::string, SShaderVariable>::const_iterator it = Shader->getAttributeHandles().begin(); it != Shader->getAttributeHandles().end(); ++ it)
	{
		std::string const & Label = it->first;

		boost::shared_ptr<IAttribute const> Attribute = getAttribute(Label);

		if (! Attribute)
			Attribute = ParentObject->getAttribute(Label);

		if (! Attribute)
			Attribute = Scene->getAttribute(Label);

		if (! Attribute)
			std::cout << "Shader-required attribute '" << Label << "' was not provided for object " << this << "." << std::endl;
	}

	for (std::map<std::string, SShaderVariable>::const_iterator it = Shader->getUniformHandles().begin(); it != Shader->getUniformHandles().end(); ++ it)
	{
		std::string const & Label = it->first;

		boost::shared_ptr<IUniform const> Uniform = getUniform(Label);

		if (! Uniform)
			Uniform = ParentObject->getUniform(Label);

		if (! Uniform)
			Uniform = Scene->getUniform(Label);

		if (! Uniform)
			std::cout << "Shader-required uniform '" << Label << "' was not provided for object " << this << "." << std::endl;
	}
}
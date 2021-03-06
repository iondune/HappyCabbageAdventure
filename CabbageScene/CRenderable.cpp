#include "CRenderable.h"

#include <iostream>

#include "../CabbageCore/glm/gtc/matrix_transform.hpp"

#include "CShaderLoader.h"
#include "CSceneManager.h"


CShader * CRenderable::NormalColorShader = 0;

CRenderable::CRenderable(ISceneObject * parent)
	: DrawType(GL_TRIANGLES), NormalObject(0), IndexBufferObject(0), Parent(parent),
	BindModelMatrix(ModelMatrix), BindNormalMatrix(NormalMatrix),
	DebugCounterThing(0)
{
	for (int i = 0; i < ERP_COUNT; ++ i)
		Shader[i] = 0;
}

CShader * CRenderable::getShader(ERenderPass const Pass)
{
	return Shader[Pass];
}

void CRenderable::setShader(ERenderPass const Pass, CShader * shader)
{
	Shader[Pass] = shader;
}

CMaterial & CRenderable::getMaterial()
{
	return Material;
}

CMaterial const & CRenderable::getMaterial() const
{
	return Material;
}

void CRenderable::loadShaderVariables(CShader const * const shader, CScene const * const scene)
{
	if (! scene->SceneChanged && LastLoadedShader == shader && LastLoadedScene == scene)
		return;
	
#ifdef DEBUG_THING
	DebugCounterThing ++;

	std::cout << "Resetting shader variables : ";

	if (scene->SceneChanged)
		std::cout << "Scene Changed ";
	if (LastLoadedShader != shader)
		std::cout << "New Shader ";
	if (LastLoadedScene != scene)
		std::cout << "New Scene ";
	
	std::cout << "@ " << DebugCounterThing << " [" << this << "])!" << std::endl;
#endif

	// Remove any handles a previous shader might have set
	LoadedAttributes.clear();
	LoadedUniforms.clear();

	LastLoadedShader = shader;
	LastLoadedScene = scene;

	if (! LastLoadedShader)
		return;

	// Check the existence of all required shader attributes
	for (std::map<std::string, SShaderVariable>::const_iterator it = LastLoadedShader->getAttributeHandles().begin(); it != LastLoadedShader->getAttributeHandles().end(); ++ it)
	{
		boost::shared_ptr<IAttribute const> Attribute = getAttribute(it->first);

		if (Attribute)
			LoadedAttributes[it->second.Handle] = Attribute;
		else
			;//std::cout << "Attribute required by shader but not found in renderable: " << it->first << std::endl;
	}

	// Check the existences of all required uniform uniforms - skipping implicit uniforms
	for (std::map<std::string, SShaderVariable>::const_iterator it = LastLoadedShader->getUniformHandles().begin(); it != LastLoadedShader->getUniformHandles().end(); ++ it)
	{
		boost::shared_ptr<IUniform const> Uniform = getUniform(it->first);

		if (! Uniform)
			Uniform = scene->getUniform(it->first);

		if (Uniform)
			LoadedUniforms[it->second.Handle] = Uniform;
		else
			;//std::cout << "Uniform required by shader but not found in renderable or scene: " << it->first << std::endl;
	}
}

GLenum const CRenderable::getDrawType() const
{
	return DrawType;
}

void CRenderable::setDrawType(GLenum const drawType)
{
	DrawType = drawType;
}

CBufferObject<GLushort> * CRenderable::getIndexBufferObject()
{
	return IndexBufferObject;
}

void CRenderable::setIndexBufferObject(CBufferObject<GLushort> * indexBufferObject)
{
	IndexBufferObject = indexBufferObject;
}

CShader * CRenderable::updateShaderVariables(CScene const * const Scene, ERenderPass const Pass)
{
	CShader * ShaderToUse = Shader[Pass];

	if (! ShaderToUse)
		ShaderToUse = Shader[ERP_DEFAULT];

	if (! ShaderToUse)
		ShaderToUse = CShaderLoader::loadShader("Simple");

	// If normal colors are being shown, switch to the normal color shader
	if (Parent->isDebugDataEnabled(EDebugData::NormalColors))
	{
		if (! NormalColorShader)
			NormalColorShader = CShaderLoader::loadShader("NormalColor");
		ShaderToUse = NormalColorShader;
	}

	loadShaderVariables(ShaderToUse, Scene);

	return ShaderToUse;
}

void CRenderable::draw(CScene const * const Scene, ERenderPass const Pass)
{
	// If no ibo loaded, we can't draw anything
	if (! IndexBufferObject)
	{
		std::cout << "Failed to draw object, no IBO." << std::endl;
		return;
	}
	
	// If the ibo loaded hasn't been synced as an index buffer object, we can't draw anything
	if (! IndexBufferObject->isIndexBuffer())
	{
		std::cout << "Failed to draw object, IBO is not index buffer." << std::endl;
		return;
	}

	CShader * ShaderToUse = updateShaderVariables(Scene, Pass);

	// Create shader context and link all variables required by the shader
	CShaderContext ShaderContext(* ShaderToUse);

	// Set up transform matrices
	ModelMatrix = Transformation() * Parent->getAbsoluteTransformation();

	// Pass transform matrices to shader
	NormalMatrix = glm::transpose(glm::inverse(ModelMatrix));

	// Pass values to shader
	for (std::map<GLint, boost::shared_ptr<IAttribute const> >::iterator it = LoadedAttributes.begin(); it != LoadedAttributes.end(); ++ it)
	{
		it->second->bind(it->first, ShaderContext);
	}
	for (std::map<GLint, boost::shared_ptr<IUniform const> >::iterator it = LoadedUniforms.begin(); it != LoadedUniforms.end(); ++ it)
	{
		it->second->bind(it->first, ShaderContext);
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

	for (std::vector<GLenum>::const_iterator it = RenderCapabilities.begin(); it != RenderCapabilities.end(); ++ it)
		glEnable(* it);
	for (std::vector<GLenum>::const_iterator it = RemovedRenderCapabilities.begin(); it != RemovedRenderCapabilities.end(); ++ it)
		glDisable(* it);

	// Finally draw!
	if(DrawType == GL_POINTS) {
		glDrawArrays(DrawType, 0, Size);
	}
	else
	{
		glDrawElements(DrawType, IndexBufferObject->getElements().size(), GL_UNSIGNED_SHORT, 0);
	}

	for (std::vector<GLenum>::const_iterator it = RenderCapabilities.begin(); it != RenderCapabilities.end(); ++ it)
		glDisable(* it);
	for (std::vector<GLenum>::const_iterator it = RemovedRenderCapabilities.begin(); it != RemovedRenderCapabilities.end(); ++ it)
		glEnable(* it);

	// Draw the normal object if it is enabled
	if (Parent->isDebugDataEnabled(EDebugData::Normals) && NormalObject)
	{
		NormalObject->Transformation = Transformation;
		NormalObject->draw(Scene, Pass);
	}

	// Cleanup the texture if it was used
	if (Material.Texture)
	{
		glDisable(GL_TEXTURE_2D);
	}
}

void CRenderable::load(CScene const * const Scene, ERenderPass const Pass)
{
	updateShaderVariables(Scene, Pass);

	if (IndexBufferObject && IndexBufferObject->isDirty())
		IndexBufferObject->syncData();

	for (std::map<GLint, boost::shared_ptr<IAttribute const> >::iterator it = LoadedAttributes.begin(); it != LoadedAttributes.end(); ++ it)
		it->second->load();
}

void CRenderable::drawNormals(CScene const * const scene)
{
	// If no ibo loaded, we can't draw anything
	// If the ibo loaded hasn't been synced as an index buffer object, 
	if (! IndexBufferObject)
	{
		std::cout << "Failed to draw object normals, no IBO." << std::endl;
		return;
	}

	if (! IndexBufferObject->isIndexBuffer())
	{
		std::cout << "Failed to draw object normals, IBO is not index buffer." << std::endl;
		return;
	}

	static CShader * NormalsShader = 0;
	if (! NormalsShader)
		NormalsShader = CShaderLoader::loadShader("NormalColor");

	// Create shader context and link all variables required by the shader
	CShaderContext ShaderContext(* NormalsShader);

	// Set up transform matrices
	ModelMatrix = Transformation() * Parent->getAbsoluteTransformation();

	// Pass values to shader
	std::map<std::string, boost::shared_ptr<IAttribute const> >::iterator aNormalIt = Attributes.find("aNormal");
	std::map<std::string, boost::shared_ptr<IAttribute const> >::iterator aPositionIt = Attributes.find("aPosition");

	if (aNormalIt == Attributes.end())
	{
		std::cerr << "Failed to draw object normals, normal attribute not found." << std::endl;
		return;
	}

	if (aPositionIt == Attributes.end())
	{
		std::cerr << "Failed to draw object normals, position attribute not found." << std::endl;
		return;
	}

	aNormalIt->second->bind(NormalsShader->getAttributeHandles().find("aNormal")->second.Handle, ShaderContext);
	aPositionIt->second->bind(NormalsShader->getAttributeHandles().find("aPosition")->second.Handle, ShaderContext);

	ShaderContext.uniform("uModelMatrix", ModelMatrix);
	ShaderContext.uniform("uViewMatrix", scene->getActiveCamera()->getViewMatrix());
	ShaderContext.uniform("uProjMatrix", scene->getActiveCamera()->getProjectionMatrix());

	// If the ibo is dirty, sync it!
	if (IndexBufferObject->isDirty())
		IndexBufferObject->syncData();

	// And bind the synced buffer object to shader...
	ShaderContext.bindIndexBufferObject(IndexBufferObject->getHandle());

	if(DrawType == GL_POINTS) {
		glDrawArrays(DrawType, 0, Size);
	}
	else {
		// Finally draw!
		glDrawElements(DrawType, IndexBufferObject->getElements().size(), GL_UNSIGNED_SHORT, 0);
	}
}

void CRenderable::addAttribute(std::string const & label, boost::shared_ptr<IAttribute const> const attribute)
{
    Attributes[label] = attribute;
}

void CRenderable::addUniform(std::string const & label, boost::shared_ptr<IUniform const> const uniform)
{
    Uniforms[label] = uniform;
}

void CRenderable::removeAttribute(std::string const & label)
{
	std::map<std::string, boost::shared_ptr<IAttribute const> >::iterator it = Attributes.find(label);

	if (it != Attributes.end())
		Attributes.erase(it);
}

void CRenderable::removeUniform(std::string const & label)
{
	std::map<std::string, boost::shared_ptr<IUniform const> >::iterator it = Uniforms.find(label);

	if (it != Uniforms.end())
		Uniforms.erase(it);
}

boost::shared_ptr<IAttribute const> const CRenderable::getAttribute(std::string const & label)
{
	std::map<std::string, boost::shared_ptr<IAttribute const> >::iterator it = Attributes.find(label);
	if (it != Attributes.end())
		return it->second;

	return boost::shared_ptr<IAttribute const>();
}

boost::shared_ptr<IUniform const> const CRenderable::getUniform(std::string const & label)
{
	if (label == "uModelMatrix")
		return BindUniform(ModelMatrix);

	if (label == "uNormalMatrix")
		return BindUniform(NormalMatrix);

	if (label == "uMaterial.AmbientColor")
		return BindUniform(Material.AmbientColor);

	if (label == "uMaterial.DiffuseColor")
		return BindUniform(Material.DiffuseColor);

	if (label == "uMaterial.Shininess")
		return BindUniform(Material.Shininess);

	std::map<std::string, boost::shared_ptr<IUniform const> >::iterator it = Uniforms.find(label);
	if (it != Uniforms.end())
		return it->second;

	return boost::shared_ptr<IUniform const>();
}

CRenderable * & CRenderable::getDebuggingNormalObject()
{
	return NormalObject;
}

void CRenderable::reloadVariablesOnNextDraw()
{
	LastLoadedShader = 0;
}

void CRenderable::addRenderCapability(GLenum const capability)
{
	RemovedRenderCapabilities.erase(std::remove(RemovedRenderCapabilities.begin(), RemovedRenderCapabilities.end(), capability), RemovedRenderCapabilities.end());
	RenderCapabilities.push_back(capability);
}

void CRenderable::removeRenderCapability(GLenum const capability)
{
	RenderCapabilities.erase(std::remove(RenderCapabilities.begin(), RenderCapabilities.end(), capability), RenderCapabilities.end());
	RemovedRenderCapabilities.push_back(capability);
}

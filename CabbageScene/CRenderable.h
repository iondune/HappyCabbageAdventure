#ifndef _CABBAGE_SCENE_CRENDERABLE_H_INCLUDED_
#define _CABBAGE_SCENE_CRENDERABLE_H_INCLUDED_

#include <map>
#include <set>

#include "../CabbageCore/boost/shared_ptr.hpp"
#include "../CabbageCore/SVector3.h"
#include "../CabbageCore/SBoundingBox3.h"

#include "CBufferObject.h"
#include "CCamera.h"
#include "CShader.h"
#include "CTexture.h"
#include "CShaderContext.h"
#include "ISceneObject.h"

#include "CMaterial.h"
#include "SAttribute.h"
#include "SUniform.h"


/*!
 * A CRenderable is some collection of buffer objects which can be drawn by a single OpenGL draw call.
 */
class CRenderable : public ISceneObject
{

protected:

    // Implicit uniforms
    glm::mat4 uModelMatrix, uNormalMatrix;

    std::map<std::string, IAttribute> Attributes;
    std::map<std::string, IUniform> Uniforms;
    std::vector<SUniform> SceneLoadedUniforms;

    CBufferObject<GLushort> * IndexBufferObject;

    SMaterial Material;

    CShader * NormalColorShader;
    CRenderable * NormalObject;

    GLenum DrawType;

    void loadHandlesFromShader(CShader const * const shader, CScene const * const scene);
    CScene const * LastLoadedScene;
    CShader const * LastLoadedShader;

public:

    CRenderable();

    SMaterial & getMaterial();
    SMaterial const & getMaterial() const;

    CBufferObject<GLushort> * getIndexBufferObject();
    void setIndexBufferObject(CBufferObject<GLushort> * indexBufferObject);

    GLenum const getDrawType() const;
    void setDrawType(GLenum const drawType);

    virtual void draw(CScene const * const scene);

    void addAttribute(std::string const & label, boost::shared_ptr<IAttribute> attribute);
    void addUniform(std::string const & label, boost::shared_ptr<IUniform> uniform);
    void removeAttribute(std::string const & label);
    void removeUniform(std::string const & label);

};

#endif

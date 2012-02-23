#ifndef _CABBAGECOLLIDER_CSCENEMANAGER_H_INCLUDED_
#define _CABBAGECOLLIDER_CSCENEMANAGER_H_INCLUDED_

#include <list>

#include "CSceneObject.h"
#include "CMeshSceneObject.h"

#include "../CabbageCore/SLine3.h"
#include "../CabbageCore/SColor.h"


class CLight
{
	// CLight should implement ISceneObject
	// that way light billboards will be drawable by using a scene->set debug flag
	// and positioning,etc will be inheritted
	
public:

    SColor Color;
    SVector3 Position;

	SUniform<SColor> BindColor;
	SUniform<SVector3> BindPosition;

    // Todo: change values only through get/set, set scene changed when so

    CLight()
		: BindColor(Color), BindPosition(Position)
    {}
};

class CScene
{

    static CLight const NullLight;

protected:

    CCamera DefaultCamera;
    CCamera * ActiveCamera;

    glm::mat4 ViewMatrix, ProjMatrix;
    int LightCount;

	SUniform<glm::mat4> BindViewMatrix, BindProjMatrix;
	SUniform<int> BindLightCount;

    std::map<std::string, IUniform const *> Uniforms;

	ISceneObject RootObject;

	bool UseCulling;

public:

    CScene();

    CCamera * const getActiveCamera();
	CCamera const * const getActiveCamera() const;
    void setActiveCamera(CCamera * const activeCamera);

    void addUniform(std::string const & label, IUniform const * uniform);
    void removeUniform(std::string const & label);

    IUniform const * getUniform(std::string const & label) const;

    void update();

    std::vector<CLight *> Lights;
    bool SceneChanged;

	
	bool const isCullingEnabled() const;
	void setCullingEnabled(bool const culling);

};

class CSceneManager : public CScene
{

    CScene * CurrentScene;

public:

    CSceneManager();

    void addSceneObject(ISceneObject * sceneObject);
    void removeSceneObject(ISceneObject * sceneObject);
    void removeAllSceneObjects();

	CMeshSceneObject * addMeshSceneObject(CMesh * Mesh);
	CMeshSceneObject * addMeshSceneObject(CMesh * Mesh, CShader * Shader);
	CMeshSceneObject * addMeshSceneObject(CMesh * Mesh, CShader * Shader, CMaterial const & Material);
	CMeshSceneObject * addMeshSceneObject(std::string const & Mesh, std::string const & Shader, CMaterial const & Material);

    void drawAll();

};

#endif

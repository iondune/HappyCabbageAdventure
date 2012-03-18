#ifndef _CABBAGECOLLIDER_CSCENEMANAGER_H_INCLUDED_
#define _CABBAGECOLLIDER_CSCENEMANAGER_H_INCLUDED_

#include <list>

#include "CSceneObject.h"
#include "CMeshSceneObject.h"
#include "CFrameBufferObject.h"
#include "CSceneEffectManager.h"

#include <SLine3.h>
#include <SColor.h>
#include <SPosition2.h>
#define THIS_OBJECT_WILL_NEVER_MOVE_AND_ITS_BOUNDING_BOX_IS_CORRECT 3945210


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

    CPerspectiveCamera DefaultCamera;
    ICamera * ActiveCamera;

    glm::mat4 ViewMatrix, ProjMatrix;
    int LightCount;

	SUniform<glm::mat4> BindViewMatrix, BindProjMatrix;
	SUniform<int> BindLightCount;

    std::map<std::string, IUniform const *> Uniforms;

	ISceneObject RootObject;

	bool UseCulling;

public:

    CScene();

    ICamera * const getActiveCamera();
	ICamera const * const getActiveCamera() const;
    void setActiveCamera(ICamera * const activeCamera);

    void addUniform(std::string const & label, IUniform const * uniform);
    void removeUniform(std::string const & label);

    IUniform const * getUniform(std::string const & label) const;

    void update();

    std::vector<CLight *> Lights;
    bool SceneChanged;
	
	bool const isCullingEnabled() const;
	void setCullingEnabled(bool const culling);
	
    void enableDebugData(EDebugData::Domain const type);
    void disableDebugData(EDebugData::Domain const type);

};

class CApplication;

class CSceneManager : public CScene
{

    CScene * CurrentScene;

	static GLuint QuadHandle;

	CFrameBufferObject * SceneFrameBuffer;
	CTexture * SceneFrameTexture;
	CRenderBufferObject * SceneDepthBuffer;

	CSceneEffectManager * EffectManager, * DeferredManager, * DefaultManager;
	CShader * BlurHorizontal;

	SSize2 ScreenSize;

   // Immobile scene objects: to be combined in a hierarchy and culled based on X location 
	std::vector<ISceneObject *> ImmobileSceneObjects;
private:
   ISceneObject* runImmobileObjectsThroughHierarchyAlgorithm();

public:

    CSceneManager(SPosition2 const & screenSize);

    void addSceneObject(ISceneObject * sceneObject);
    void addImmobileSceneObject(ISceneObject * sceneObject, unsigned int agreement);
    void removeSceneObject(ISceneObject * sceneObject);

    void removeAllSceneObjects();

	CMeshSceneObject * addMeshSceneObject(CMesh * Mesh);
	CMeshSceneObject * addMeshSceneObject(CMesh * Mesh, CShader * Shader, CShader * DeferredShader);
	CMeshSceneObject * addMeshSceneObject(CMesh * Mesh, CShader * Shader, CShader * DeferredShader, CMaterial const & Material);
	CMeshSceneObject * addMeshSceneObject(std::string const & Mesh, std::string const & Shader, std::string const & DeferredShader, CMaterial const & Material);

	void drawAll();
	void endDraw();
	void blurSceneIn(float seconds, float const RunTime);

	void load();

	CFrameBufferObject * getSceneFrameBuffer();
	CTexture * getSceneFrameTexture();
	CRenderBufferObject * getSceneDepthBuffer();

	CSceneEffectManager * getEffectManager();
	void setEffectManager(CSceneEffectManager * effectManager);

	SSize2 const & getScreenSize() const;

	float FinalBlurSize, Dim;
	float BlurInTime, BlurOutTime, CurTime;

	static GLuint const getQuadHandle();

	void setDeferred(bool const isDeferred);

};

#endif

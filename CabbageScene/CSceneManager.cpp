#include "CSceneManager.h"

#include <algorithm>
#include <sstream>

#include "CShaderLoader.h"
#include "CMeshLoader.h"
#include "CTextureLoader.h"

#include "CDeferredShadingManager.h"


CLight const CScene::NullLight;


CScene::CScene()
	: BindProjMatrix(ProjMatrix), BindViewMatrix(ViewMatrix), BindLightCount(LightCount), UseCulling(true), HierarchyObject(0), NonHierarchyObject(0), UseHierarchy(true)
{
	ActiveCamera = & DefaultCamera;

	addUniform("uProjMatrix", ProjMatrix);
	addUniform("uViewMatrix", ViewMatrix);
	addUniform("uLightCount", LightCount);
}

void CScene::addUniform(std::string const & label, boost::shared_ptr<IUniform const> const uniform)
{
	Uniforms[label] = uniform;
}

void CScene::removeUniform(std::string const & label)
{
	std::map<std::string, boost::shared_ptr<IUniform const> >::iterator it = Uniforms.find(label);

	if (it != Uniforms.end())
		Uniforms.erase(it);
}

ICamera * const CScene::getActiveCamera()
{
	return ActiveCamera;
}

ICamera const * const CScene::getActiveCamera() const
{
	return ActiveCamera;
}

void CScene::setActiveCamera(ICamera * const activeCamera)
{
	ActiveCamera = activeCamera;
}

bool const CScene::isCullingEnabled() const
{
	return UseCulling;
}

void CScene::setCullingEnabled(bool const culling)
{
	UseCulling = culling;
}

void CScene::toggleUseHierarchy() {
	setUseHierarchy(!UseHierarchy);
}

bool CScene::getUseHierarchy() {
	return UseHierarchy;
}

void CScene::setUseHierarchy(bool h) {
	if(h != UseHierarchy) {
		UseHierarchy = h;
		if(HierarchyObject && NonHierarchyObject) {
			if(h) {
				RootObject.removeChild(NonHierarchyObject);
				RootObject.addChild(HierarchyObject);
			}
			else {
				RootObject.removeChild(HierarchyObject);
				RootObject.addChild(NonHierarchyObject);
			}
			//printf("RootObject #: %d\n", RootObject.getChildren().size());
		}
	}
}

unsigned int const digitCount(int n)
{
	int count = 1;
	if (n < 0)
	{
		n *= -1;
		++ count;
	}

	while (n > 10)
	{
		++ count;
		n /= 10;
	}

	return count;
}

boost::shared_ptr<IUniform const> const CScene::getUniform(std::string const & label) const
{
	if (label.substr(0, 8) == "uLights[")
	{
		std::stringstream ss(label.substr(8));
		unsigned int index;
		ss >> index;
		std::string remaining = ss.str();
		remaining = remaining.substr(2 + digitCount(index));

		if (remaining == "Color")
		{
			if (index >= Lights.size())
				return BindUniform(NullLight.Color);
			else
				return BindUniform(Lights[index]->Color);
		}
		else if (remaining == "Position")
		{
			if (index >= Lights.size())
				return BindUniform(NullLight.Position);
			else
				return BindUniform(Lights[index]->Position);
		}
	}

	std::map<std::string, boost::shared_ptr<IUniform const> >::const_iterator it = Uniforms.find(label);

	if (it != Uniforms.end())
		return it->second;

	return boost::shared_ptr<IUniform const>();
}


extern int timesCalled, numObjects, numCulled;
int CSceneManager::getTimesCalled() {
	return timesCalled;
}

int CSceneManager::getNumObjects() {
	return numObjects;
}

int CSceneManager::getNumCulled() {
	return numCulled;
}


void CScene::update()
{
	ActiveCamera->recalculateViewMatrix();
	ViewMatrix = ActiveCamera->getViewMatrix();
	ProjMatrix = ActiveCamera->getProjectionMatrix();

	if (LightCount != Lights.size())
	{
		SceneChanged = true;
		LightCount = Lights.size();
	}

	RootObject.updateAbsoluteTransformation();
	PostOpaqueRootObject.updateAbsoluteTransformation();
	RootObject.update();
	PostOpaqueRootObject.update();
}

GLuint CSceneManager::QuadHandle = 0;

CSceneManager::CSceneManager(SPosition2 const & screenSize)
	: FinalBlurSize(0.0f), SceneFrameBuffer(0),
	EffectManager(0), ScreenSize(screenSize)
{
	CurrentScene = this;

	// Create a simple quad VBO to use for draw operations!
	if (! QuadHandle)
	{
		GLfloat QuadVertices[] = 
		{
			-1.0, -1.0,
			1.0, -1.0,
			1.0,  1.0,
			-1.0,  1.0
		};

		glGenBuffers(1, & QuadHandle);
		glBindBuffer(GL_ARRAY_BUFFER, QuadHandle);
		glBufferData(GL_ARRAY_BUFFER, sizeof(QuadVertices), QuadVertices, GL_STATIC_DRAW);
	}


	STextureCreationFlags Flags;
	Flags.MipMaps = false;
	SceneFrameTexture = new CTexture(ScreenSize, true, Flags);
	SceneDepthBuffer = new CRenderBufferObject(GL_DEPTH_COMPONENT, ScreenSize);

	SceneFrameBuffer = new CFrameBufferObject();
	SceneFrameBuffer->attach(SceneDepthBuffer, GL_DEPTH_ATTACHMENT);
	SceneFrameBuffer->attach(SceneFrameTexture, GL_COLOR_ATTACHMENT0);

	if (! SceneFrameBuffer->isValid())
		std::cerr << "Failed to make FBO for scene drawing!!!!!!" << std::endl  << std::endl  << std::endl;

	EffectManager = DefaultManager = new CSceneEffectManager(this);
	EffectManager->setEffectEnabled(ESE_BLOOM, true);

	DeferredManager = new CDeferredShadingManager(this);

	BlurHorizontal = CShaderLoader::loadShader("FBO/QuadCopyUV.glsl", "BlurH.frag");
}

void CSceneManager::addSceneObject(ISceneObject * sceneObject)
{
	RootObject.addChild(sceneObject);
}

void CSceneManager::addPostOpaqueSceneObject(ISceneObject *sceneObject) {
	PostOpaqueRootObject.addChild(sceneObject);
}

void CSceneManager::addImmobileSceneObject(ISceneObject * sceneObject, unsigned int agreement)
{
	if(agreement != THIS_OBJECT_WILL_NEVER_MOVE_AND_ITS_BOUNDING_BOX_IS_CORRECT) {
		fprintf(stderr, "addImmobileSceneObject failure! The agreement argument was not accepted.\nEntered: %d. Expected: THIS_OBJECT_WILL_NEVER_MOVE_AND_ITS_BOUNDING_BOX_IS_CORRECT\n", agreement);
		exit(1);
	}
	ImmobileSceneObjects.push_back(sceneObject);   
}

void CSceneManager::removeSceneObject(ISceneObject * sceneObject)
{
	RootObject.removeChild(sceneObject);
	PostOpaqueRootObject.removeChild(sceneObject);
	//Because you're always gonna want it removed, even if you don't specify the correct location
}

void CSceneManager::removePostOpaqueSceneObject(ISceneObject * sceneObject) {
	PostOpaqueRootObject.removeChild(sceneObject);
}

void CSceneManager::removeAllSceneObjects()
{
	RootObject.removeChildren();
	PostOpaqueRootObject.removeChildren();
}

bool sortISOXY (ISceneObject* a, ISceneObject* b) {
	SVector3 av = a->getWorldBoundingBoxMinPoint();
	SVector3 bv = b->getWorldBoundingBoxMinPoint();
	if(av.X == bv.X) {
		return av.Y < bv.Y;
	}
	else return av.X < bv.X;
}

ISceneObject* CSceneManager::runImmobileObjectsThroughHierarchyAlgorithm() {
	float ARBITRARILY_INCREASING_VALUE = (9.5f + 8.0f);
	float const INF = std::numeric_limits<float>::infinity();
	std::vector<ISceneObject *> dontUseMyName;
	// aList is a list of children to be assigned to a parent
	// bList is a list of parents that were just created
	std::vector<ISceneObject *> *aList = &ImmobileSceneObjects, *bList = &dontUseMyName;

	NonHierarchyObject = new ISceneObject();
	NonHierarchyObject->setCullingEnabled(false);
	for (unsigned int i = 0; i < ImmobileSceneObjects.size(); i++) {
		NonHierarchyObject->addChild(ImmobileSceneObjects[i]);
	}

	// Sort ImmobileSceneObjects by bounding box min X point
	sort(aList->begin(), aList->end(), sortISOXY);

	int iterations = 0, tNumObjects = 0;
	// TODO: Fix the argument for this while loop
	while(aList->size() != 1) {
		iterations++;
		//printf("~~~~~~~~~~~~ NEW ITERATION (%d) ~~~~~~~~~~~~\n", iterations);
		unsigned int j = 0;
		// From the smallest X to the largest X in aList, do:
		for (float i = (*aList)[0]->getWorldBoundingBoxMinPoint().X; j < aList->size() && i <= (*aList)[aList->size()-1]->getWorldBoundingBoxMinPoint().X + ARBITRARILY_INCREASING_VALUE;) {
			ISceneObject * parentNode = new ISceneObject();
			parentNode->setImmobile(true);
			parentNode->setBoundingBox(SBoundingBox3(SVector3(i, -INF, -INF), SVector3(i + ARBITRARILY_INCREASING_VALUE, INF, INF)));
			// TODO: See if I can get away with not setting any transforms for the nodes in the algorithm. Reasoning: if each object has its own transforms, they'll draw correctly, and if the parent has the correct bounding box, it will cull correctly (since its transforms will be 0).

			int oldJ = j;
			while (j < aList->size()) {
				if ((*aList)[j]->getWorldBoundingBox().intersects(parentNode->getBoundingBox())) {
					if (iterations == 1) {
						tNumObjects += (*aList)[j]->getNumLeaves();
					}
					//(*aList)[j]->setImmobile(true);
					parentNode->addChild((*aList)[j]);
					parentNode->getBoundingBox().addInternalPoint((*aList)[j]->getWorldBoundingBox().MinCorner);
					parentNode->getBoundingBox().addInternalPoint((*aList)[j]->getWorldBoundingBox().MaxCorner);
					(*aList)[j] = NULL;
					j++;
				}
				else
					break;
			}
			if(j == oldJ) {
				/*printf("Didn't add. Bounding box: (%0.2f,%0.2f,%0.2f),(%0.2f,%0.2f,%0.2f)\n",
				parentNode->getBoundingBox().MinCorner.X,
				parentNode->getBoundingBox().MinCorner.Y,
				parentNode->getBoundingBox().MinCorner.Z,
				parentNode->getBoundingBox().MaxCorner.X,
				parentNode->getBoundingBox().MaxCorner.Y,
				parentNode->getBoundingBox().MaxCorner.Z
				);
				*/
			}
			else if(j == oldJ + 1) {
				//printf("Size of children list (should be 1): %d\n", parentNode->getChildren().size());
				bList->push_back(parentNode->getChildren().front());
				//bList->push_back(parentNode);
			}
			else {
				//printf("Added a node that contains %d children\n", parentNode->getChildren().size());
				parentNode->getBoundingBox().shrink();
				bList->push_back(parentNode);
			}
			i += ARBITRARILY_INCREASING_VALUE;
		}

		for (unsigned int k = 0; k < aList->size(); k++) {
			assert((*aList)[k] == NULL);
		}
		int numChildren = 0;
		for (unsigned int k = 0; k < bList->size(); k++) {
			numChildren += (*bList)[k]->getChildren().size();
		}
		//printf("Num children in bList: %d\n", numChildren);

		ARBITRARILY_INCREASING_VALUE *= ARBITRARILY_INCREASING_VALUE*ARBITRARILY_INCREASING_VALUE;
		std::vector<ISceneObject *> *cList = aList; 
		aList = bList;
		bList = cList;
		bList->clear();
	}
	ISceneObject *toReturn = (*aList)[0];
	ImmobileSceneObjects.clear();
	//printf("There are a total of %d leaves.\n", tNumObjects);
	return toReturn;
}

void CSceneManager::drawAll()
{
	if (ImmobileSceneObjects.size() > 0) {
		for (unsigned int i = 0; i < ImmobileSceneObjects.size(); ++ i) {
			ImmobileSceneObjects[i]->updateAbsoluteTransformation();
			ImmobileSceneObjects[i]->update();
		}

		HierarchyObject = runImmobileObjectsThroughHierarchyAlgorithm();
		//std::list<ISceneObject *> & toAdd = toAddObj->getChildren();
		RootObject.addChild(HierarchyObject);
		//for (std::list<ISceneObject *>::const_iterator it = toAdd->getChildren().begin(); it != toAdd->getChildren().end(); ++ it)
		//   RootObject.addChild((*it));
		//printf("There are a total of %d leaves.\n", RootObject.getNumLeaves());
	}
	timesCalled = numObjects = numCulled = 0;
	CurrentScene->update();

   PostOpaqueRootObject.sortChildrenByZTranslation();

	if (EffectManager)
	{
		for (std::vector<CSceneEffectManager::SRenderPass>::iterator it = EffectManager->RenderPasses.begin(); it != EffectManager->RenderPasses.end(); ++ it)
		{
			if (it->Target)
				it->Target->bind();
			else
				glBindFramebuffer(GL_FRAMEBUFFER, 0);

			if (it->Pass == ERP_DEFERRED_LIGHTS)
			{
				glClearColor(0.f,0.f,0.f,0.f);
				glDisable(GL_DEPTH_TEST);
				glEnable(GL_BLEND);
				glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
			}

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			RootObject.draw(CurrentScene, it->Pass);

			if (it->Pass != ERP_DEFERRED_LIGHTS) {
				glEnable(GL_ALPHA);
				glEnable(GL_BLEND);
				glEnable(GL_DEPTH_TEST);
            //glDepthMask(GL_FALSE);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			}
			PostOpaqueRootObject.draw(CurrentScene, it->Pass);
			if (it->Pass != ERP_DEFERRED_LIGHTS) {
				glBlendFunc(GL_ONE, GL_MAX);
            //glDepthMask(GL_TRUE);
				glDisable(GL_BLEND);
				glDisable(GL_ALPHA);
			}

			if (it->Pass == ERP_DEFERRED_LIGHTS)
			{
				glDisable(GL_BLEND);
				glEnable(GL_DEPTH_TEST);
			}
		}

		EffectManager->apply();
	}
	else
	{
		SceneFrameBuffer->bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_ALPHA);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		RootObject.draw(CurrentScene, ERP_DEFAULT);
		PostOpaqueRootObject.draw(CurrentScene, ERP_DEFAULT);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glDisable(GL_BLEND);
		glDisable(GL_ALPHA);
	}

	SceneChanged = false;

	SceneFrameBuffer->bind();
}

void CSceneManager::blurSceneIn(float seconds, float const RunTime)
{
	std::cout << "Blurring in scene..." << std::endl;
	BlurOutTime = 0;
	BlurInTime = seconds;
	CurTime = RunTime;
}

#include <CApplication.h>
void CSceneManager::endDraw()
{
	// Do fade-in
	if (CurTime == -1.0f)
		Dim = 1.0f;

	if (BlurInTime > 0.0f)
	{
		FinalBlurSize = 0.0f;
		Dim = std::max(1.0f - (BlurInTime - (CApplication::get().getRunTime() - CurTime))/BlurInTime, 0.0f);
		if ((CApplication::get().getRunTime() - CurTime) > BlurInTime)
		{
			BlurInTime = 0.0f;
			CurTime = -1.0f;
		}
	}


	// Draw to screen
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);

	// THE FINAL RENDER
	{
		CShaderContext Context(* BlurHorizontal);

		Context.bindTexture("uTexColor", SceneFrameTexture);
		Context.uniform("BlurSize", FinalBlurSize);
		Context.uniform("DimAmount", Dim);
		Context.bindBufferObject("aPosition", QuadHandle, 2);

		glDrawArrays(GL_QUADS, 0, 4);
	}

	glEnable(GL_DEPTH_TEST);
}

CMeshSceneObject * CSceneManager::addMeshSceneObject(CMesh * Mesh)
{
	CMeshSceneObject * Object = new CMeshSceneObject();
	Object->setMesh(Mesh);
	addSceneObject(Object);
	return Object;
}

CMeshSceneObject * CSceneManager::addMeshSceneObject(CMesh * Mesh, CShader * Shader, CShader * DeferredShader)
{
	CMeshSceneObject * Object = new CMeshSceneObject();
	Object->setMesh(Mesh);
	Object->setShader(ERP_DEFAULT, Shader);
	Object->setShader(ERP_DEFERRED_OBJECTS, DeferredShader);
	addSceneObject(Object);
	return Object;
}

CMeshSceneObject * CSceneManager::addMeshSceneObject(CMesh * Mesh, CShader * Shader, CShader * DeferredShader, CMaterial const & Material)
{
	CMeshSceneObject * Object = new CMeshSceneObject();
	Object->setMesh(Mesh);
	Object->setShader(ERP_DEFAULT, Shader);
	Object->setShader(ERP_DEFERRED_OBJECTS, DeferredShader);
	Object->setMaterial(Material);
	addSceneObject(Object);
	return Object;
}

CMeshSceneObject * CSceneManager::addMeshSceneObject(std::string const & Mesh, std::string const & Shader, std::string const & DeferredShader, CMaterial const & Material)
{
	CMeshSceneObject * Object = new CMeshSceneObject();
	Object->setMesh(CMeshLoader::load3dsMesh(Mesh));
	Object->setShader(ERP_DEFAULT, CShaderLoader::loadShader(Shader));
	Object->setShader(ERP_DEFERRED_OBJECTS, CShaderLoader::loadShader(DeferredShader));
	Object->setMaterial(Material);
	addSceneObject(Object);
	return Object;
}



void CScene::enableDebugData(EDebugData::Domain const type)
{
	RootObject.enableDebugData(type);
	PostOpaqueRootObject.enableDebugData(type);
}

void CScene::disableDebugData(EDebugData::Domain const type)
{
	RootObject.disableDebugData(type);
	PostOpaqueRootObject.disableDebugData(type);
}


void CSceneManager::load()
{
	RootObject.load(this, ERP_DEFAULT);
	PostOpaqueRootObject.load(this, ERP_DEFAULT);
}

CFrameBufferObject * CSceneManager::getSceneFrameBuffer()
{
	return SceneFrameBuffer;
}

CTexture * CSceneManager::getSceneFrameTexture()
{
	return SceneFrameTexture;
}

CRenderBufferObject * CSceneManager::getSceneDepthBuffer()
{
	return SceneDepthBuffer;
}

CSceneEffectManager * CSceneManager::getEffectManager()
{
	return EffectManager;
}

void CSceneManager::setEffectManager(CSceneEffectManager * effectManager)
{
	EffectManager = effectManager;
}

GLuint const CSceneManager::getQuadHandle()
{
	return QuadHandle;
}

SSize2 const & CSceneManager::getScreenSize() const
{
	return ScreenSize;
}

void CSceneManager::setDeferred(bool const isDeferred)
{
	if (isDeferred)
		EffectManager = DeferredManager;
	else
		EffectManager = DefaultManager;
}

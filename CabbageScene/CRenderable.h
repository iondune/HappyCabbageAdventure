#ifndef _CABBAGE_SCENE_IRENDERABLE_H_INCLUDED_
#define _CABBAGE_SCENE_IRENDERABLE_H_INCLUDED_

#include "../CabbageCore/SVector3.h"
#include "CShader.h"
#include "CMesh.h"


class CRenderable
{

protected:

	GLuint PositionBufferHandle;
    GLuint ColorBufferHandle;
	GLuint IndexBufferHandle;

	GLsizei IndexCount;

	SVector3 Translation, Rotation, Scale;

public:

	CRenderable(CMesh const & Mesh);

	SVector3 const & getTranslation() const;
	SVector3 const & getRotation() const;
	SVector3 const & getScale() const;

	void setTranslation(SVector3 const & translation);
	void setRotation(SVector3 const & rotation);
	void setScale(SVector3 const & scale);

	virtual void draw(CShader const & shader);

};

#endif

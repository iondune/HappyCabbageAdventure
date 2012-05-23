#ifndef _CABBAGE_SCENE_CMATERIAL_H_INCLUDED_
#define _CABBAGE_SCENE_CMATERIAL_H_INCLUDED_

#include "SUniform.h"
#include "SAttribute.h"
#include "CTexture.h"

class CMaterial
{

public:

    CTexture * Texture;

	SColor AmbientColor, DiffuseColor;
	float Shininess;

	SUniformReference<SColor> BindAmbientColor, BindDiffuseColor;
	SUniformReference<float> BindShininess;

    CMaterial();
	CMaterial(CMaterial const & other);
	CMaterial & operator = (CMaterial const & other);

	void loadDefaults();

	IUniform const * const getUniform(std::string const & label);

};

#endif

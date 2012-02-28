#ifndef _CABBAGEGUI_CFONT_H_INCLUDED_
#define _CABBAGEGUI_CFONT_H_INCLUDED_

namespace OGLFT
{
	class Face;
}

class CFont
{

	
	OGLFT::Face * Renderer;

public:

	CFont(std::string const & fileName);

};

#endif

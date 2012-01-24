#include "CObject.h"

#include <algorithm>

#include "../CabbageCore/Utils.h"

#ifdef __unix__
#include <GL/gl.h>
#endif

#ifdef _WIN32
#include <GL/glew.h>
#endif

namespace Cabbage
{
namespace Collider
{

	void CObject::draw()
	{
		glPushMatrix();
		glTranslatef(Area.Position.X, Area.Position.Y, 0);

		glBegin(GL_QUADS);
			glVertex3f(0.f, 0.f, 0.f);
			glVertex3f(Area.Size.X, 0.f, 0.f);
			glVertex3f(Area.Size.X, Area.Size.Y, 0.f);
			glVertex3f(0.f, Area.Size.Y, 0.f);
		glEnd();

		glPopMatrix();
	}

}
}

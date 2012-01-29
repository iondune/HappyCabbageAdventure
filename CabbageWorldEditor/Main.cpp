#include <Gwen/Renderers/OpenGL.h>
#include <Gwen/Controls/Canvas.h>
#include <Gwen/Skins/Simple.h>

#pragma comment (lib,"GWEN-Renderer-OpenGL.lib")
#pragma comment (lib,"OpenGL32.lib")
#pragma comment (lib,"FreeImage.lib")

int main() 
{
	Gwen::Renderer::OpenGL * pRenderer = new Gwen::Renderer::OpenGL();

	Gwen::Skin::Simple skin;
	skin.SetRender (pRenderer);

	Gwen::Controls::Canvas * pCanvas = new Gwen::Controls::Canvas(&skin);
	pCanvas->SetSize(1024, 512);


	pCanvas->RenderCanvas();
}

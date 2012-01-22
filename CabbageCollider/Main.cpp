/*!  
 *  All the godforsaken lib files necessary to make this hideous monster compile.
 *  :-(
 */
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "sfml-system.lib")
#pragma comment(lib, "sfml-graphics.lib")
#pragma comment(lib, "sfml-window.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "freetype.lib")
#pragma comment(lib, "winmm.lib")

/*!  
 *  Includes for OpenGL and SFML, our windowing system.
 *  :-D
 */
#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

//! STL and other StdLib files
#include <algorithm>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>

#include "CCabbageColliderEngine.h"
using namespace CabbageCollider;

void Initialize()
{
	glClearColor (0.9f, 0.6f, 0.3f, 1.0f);
	glClearDepth (1.0f);
	glEnable (GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, 1.0, 0.01, 100.0);
}

CActor * Player;

void Draw()
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(
		Player->getArea().Position.X, Player->getArea().Position.Y, 8, 
		Player->getArea().Position.X, Player->getArea().Position.Y, 0, 
		0, 1, 0);

	glPointSize(10.f);
	glBegin(GL_POINTS);
		glVertex3f(0.f, 0.f, 0.f);
		glVertex3f(0.f, 5.f, 0.f);
		glVertex3f(0.f, 10.f, 0.f);
		glVertex3f(0.f, 0.f, 0.f);
		glVertex3f(5.f, 0.f, 0.f);
		glVertex3f(10.f, 0.f, 0.f);
	glEnd();
}

int main()
{
	// Create SFML Window Context
	sf::WindowSettings Settings;
	Settings.DepthBits         = 24; // Request a 24 bits depth buffer
	Settings.StencilBits       = 8;  // Request a 8 bits stencil buffer
	Settings.AntialiasingLevel = 2;  // Request 2 levels of antialiasing
	sf::RenderWindow App(sf::VideoMode(800, 600, 32), "SFML OpenGL", sf::Style::Close, Settings);
	App.PreserveOpenGLStates(true);

	// Load OpenGL Extensions
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		fprintf(stderr, "Error initializing glew! (%s)\n", glewGetErrorString(err));
		system("PAUSE");
		return 1;
	}

   	Initialize();

	// Load font
	sf::Font MyFont;
	if (! MyFont.LoadFromFile("Fonts/DejaVuSansMono.ttf"))
	{
		fprintf(stderr, "Error installing font!\n");
		system("PAUSE");
		return 1;
	}

	App.UseVerticalSync(false);

	bool IsKeyDown[sf::Key::Count];
	for (unsigned int i = 0; i < sf::Key::Count; ++ i)
		IsKeyDown[i] = false;

	CEngine * Engine = new CEngine();
	Player = Engine->addActor();
	Player->setArea(SRect2(0, 3, 1, 1));

	CObject * Block = Engine->addObject();
	Block->setArea(SRect2(-1, -1, 15, 0.9f));

  	while (App.IsOpened())
	{
		sf::Event Event;
		while (App.GetEvent(Event))
		{
			// Close window : exit
			if (Event.Type == sf::Event::Closed)
				App.Close();

			if (Event.Type == sf::Event::KeyPressed)
				IsKeyDown[Event.Key.Code] = true;

			if (Event.Type == sf::Event::KeyReleased)
				IsKeyDown[Event.Key.Code] = false;

			// Escape key : exit
			if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Escape))
				App.Close();

			// Resize event : adjust viewport
			if (Event.Type == sf::Event::Resized)
				glViewport(0, 0, Event.Size.Width, Event.Size.Height);
		}

		SVector2 Accel = Player->getAcceleration();
		Accel.X = 0;
		static float const MoveAccel = 5.8f;
		static float const JumpSpeed = 5.f;
		static float const AirMod = 0.25f;

		if (IsKeyDown[sf::Key::D])
		{
			Accel.X += MoveAccel * (Player->isStanding() ? 1 : AirMod);
		}

		if (IsKeyDown[sf::Key::A])
		{
			Accel.X -= MoveAccel * (Player->isStanding() ? 1 : AirMod);
		}

		if (IsKeyDown[sf::Key::Space] && Player->isStanding())
		{
			Player->setVelocity(SVector2(Player->getVelocity().X, JumpSpeed));
		}

		Player->setAcceleration(Accel);

		Engine->updateAll(App.GetFrameTime());

		App.SetActive();

		Draw();

		glPushMatrix();
		glTranslatef(Player->getArea().Position.X, Player->getArea().Position.Y, 0);

		if (Player->isStanding())
			glColor3f(1.0, 0, 0);
		else
			glColor3f(1, 1, 1);

		glBegin(GL_QUADS);
			glVertex3f(0.f, 0.f, 0.f);
			glVertex3f(1.f, 0.f, 0.f);
			glVertex3f(1.f, 1.f, 0.f);
			glVertex3f(0.f, 1.f, 0.f);
		glEnd();
		glPopMatrix();

		glColor3f(1, 1, 1);

		glPushMatrix();
		glTranslatef(Block->getArea().Position.X, Block->getArea().Position.Y, 0);

		glBegin(GL_QUADS);
			glVertex3f(0.f, 0.f, 0.f);
			glVertex3f(15.f, 0.f, 0.f);
			glVertex3f(15.f, 0.9f, 0.f);
			glVertex3f(0.f, 0.9f, 0.f);
		glEnd();
		glPopMatrix();

		sf::String Text;
		std::stringstream str;
		str << "FPS: ";
		str << (1.f / App.GetFrameTime());
		Text.SetText(str.str().c_str());
		Text.SetFont(MyFont);
		Text.SetSize(50);
		App.Draw(Text);

		App.Display();
	}

   	return 0;
}


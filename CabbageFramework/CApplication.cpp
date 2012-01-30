#include "CApplication.h"

#ifdef _WIN32
#include <GL/glew.h>
#endif

#include <iostream>

CApplication::CApplication()
{}

void CApplication::setupRenderContext()
{
	SDL_VideoInfo const * video;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
        fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
        waitForUser();
		exit(1);
    }
        
    atexit(SDL_Quit);

    video = SDL_GetVideoInfo();
    if(video == NULL)
	{
        fprintf(stderr, "Couldn't get video information: %s\n", SDL_GetError());
        waitForUser();
		exit(2);
    }

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    if(! SDL_SetVideoMode(WindowSize.X, WindowSize.Y, video->vfmt->BitsPerPixel, SDL_OPENGL))
	{
        fprintf(stderr, "Couldn't set video mode: %s\n", SDL_GetError());
        waitForUser();
		exit(1);
    }

#ifdef _WIN32
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::cerr << "Error initializing glew! " << glewGetErrorString(err) << std::endl;
		waitForUser();
		exit(3);
	}
#endif
}


void CApplication::init(SPosition2 const & windowSize)
{
	WindowSize = windowSize;

	EventManager = new CEventManager();
	StateManager = new CStateManager();
    SceneManager = new CSceneManager();

	EventManager->OnGameTickStart.connect(StateManager, & CStateManager::OnGameTickStart);
	EventManager->OnGameTickEnd.connect(StateManager, & CStateManager::OnGameTickEnd);
	EventManager->OnRenderStart.connect(StateManager, & CStateManager::OnRenderStart);
	EventManager->OnRenderEnd.connect(StateManager, & CStateManager::OnRenderEnd);

	setupRenderContext();
}

CApplication & CApplication::get()
{
	static CApplication SingletonInstance;

	return SingletonInstance;
}

CEventManager & CApplication::getEventManager()
{
	return * EventManager;
}

CStateManager & CApplication::getStateManager()
{
	return * StateManager;
}

CSceneManager & CApplication::getSceneManager()
{
	return * SceneManager;
}

void CApplication::run()
{
	bool Running = true;

	unsigned int Time0, Time1;
	Time0 = SDL_GetTicks();

	while (Running)
	{
		SDL_Event Event;
        while (SDL_PollEvent(& Event))
		{
            switch (Event.type)
			{

            case SDL_QUIT:
                Running = false;
                break;

			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
				{

					SMouseEvent MouseEvent;
					MouseEvent.Type = SMouseEvent::EType::Click;
					MouseEvent.Location = EventManager->MouseLocation;
					MouseEvent.Pressed = Event.button.state == SDL_PRESSED;

					switch (Event.button.button)
					{

					case SDL_BUTTON_LEFT:
						MouseEvent.Button = SMouseEvent::EButton::Left;
						EventManager->OnMouseEvent(MouseEvent);
						break;
						
					case SDL_BUTTON_RIGHT:
						MouseEvent.Button = SMouseEvent::EButton::Right;
						EventManager->OnMouseEvent(MouseEvent);
						break;

					case SDL_BUTTON_MIDDLE:
						MouseEvent.Button = SMouseEvent::EButton::Middle;
						EventManager->OnMouseEvent(MouseEvent);
						break;

					default:
						break;

					}

					break;

				}

			case SDL_MOUSEMOTION:
				{

					SMouseEvent MouseEvent;
					MouseEvent.Type = SMouseEvent::EType::Move;
					MouseEvent.Location = EventManager->MousePositionState = SPosition2(Event.motion.x, Event.motion.y);
					MouseEvent.Movement = SPosition2(Event.motion.xrel, Event.motion.yrel);
					EventManager->OnMouseEvent(MouseEvent);

					break;

				}

			case SDL_KEYDOWN:
			case SDL_KEYUP:
				{

					SKeyboardEvent KeyEvent;
					KeyEvent.Pressed = Event.key.state == SDL_PRESSED;
					KeyEvent.Key = Event.key.keysym.sym;
					EventManager->OnKeyboardEvent(KeyEvent);

					break;

				}
            } // switch (Event.type)
        } // while (SDL_PollEvent(& Event))

		Time1 = SDL_GetTicks();
		float Delta = (float) (Time1 - Time0) / 1000.f;
		Time0 = Time1;

		EventManager->OnGameTickStart(Delta);
		EventManager->OnGameTickEnd(Delta);

		EventManager->OnRenderStart(Delta);
		EventManager->OnRenderEnd(Delta);

    } // while (Running)
}

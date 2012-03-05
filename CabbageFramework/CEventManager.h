#ifndef _CABBAGECOLLIDER_CEVENTMANAGER_H_INCLUDED_
#define _CABBAGECOLLIDER_CEVENTMANAGER_H_INCLUDED_

#include "../CabbageCore/SPosition2.h"

#include "sigslot/sigslot.h"

#include <SDL/SDL.h>

struct SKeyboardEvent
{
    bool Pressed;
    SDLKey Key;
};

struct SMouseEvent
{
    class EButton
    {

    public:

        enum Domain
        {
            Left,
            Right,
            Middle,
            Count
        };

        Domain Value;

        EButton();

        EButton(Domain const value);

    };

    class EType
    {

    public:

        enum Domain
        {
            Move,
            Click
        };

        Domain Value;

        EType();

        EType(Domain const value);

    };

    SPosition2 Location, Movement;
    EButton Button;
    EType Type;
    bool Pressed;

};

class CApplication;

class CApplicationEventReceiver : public sigslot::has_slots<>
{

	CApplication * const Application;

public:

	CApplicationEventReceiver();

	void OnKeyboardEvent(SKeyboardEvent const & Event);
	void OnMouseEvent(SMouseEvent const & Event);

	void OnGameTickStart(float const Elapsed);
	void OnGameTickEnd(float const Elapsed);

	void OnRenderStart(float const Elapsed);
	void OnRenderEnd(float const Elapsed);

	void OnApplicationExit();

};

class CEventManager
{

    friend class CApplication;

    CEventManager();

    bool KeyStates[SDLK_LAST];
    bool MouseStates[SMouseEvent::EButton::Count];
    SPosition2 MousePositionState;

public:	

    sigslot::signal1<SKeyboardEvent const &> OnKeyboardEvent;
    sigslot::signal1<SMouseEvent const &> OnMouseEvent;

    sigslot::signal1<float const> OnGameTickStart;
    sigslot::signal1<float const> OnGameTickEnd;

    sigslot::signal1<float const> OnRenderStart;
    sigslot::signal1<float const> OnRenderEnd;

    sigslot::signal0<> OnApplicationExit;

    bool const (& IsKeyDown)[SDLK_LAST];
    bool const (& IsMouseDown)[SMouseEvent::EButton::Count];

    SPosition2 const & MouseLocation;

};

#endif

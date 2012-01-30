#ifndef _CABBAGECOLLIDER_CSTATEMANAGER_H_INCLUDED_
#define _CABBAGECOLLIDER_CSTATEMANAGER_H_INCLUDED_

#include "sigslot/sigslot.h"

class IState
{

public:

    virtual void begin() =0;
    virtual void end() =0;

    virtual void OnGameTickStart(float const Elapsed) =0;
    virtual void OnGameTickEnd(float const Elapsed) =0;

    virtual void OnRenderStart(float const Elapsed) =0;
    virtual void OnRenderEnd(float const Elapsed) =0;

};

template <class TImplementation>
class CState : public IState
{

public:

    static TImplementation & get()
    {
        static TImplementation Instance;
        return Instance;
    }

};

class CStateManager : public sigslot::has_slots<>
{

    friend class CApplication;

    CStateManager();

    IState * CurrentState;

public:

    void OnGameTickStart(float const Elapsed);
    void OnGameTickEnd(float const Elapsed);

    void OnRenderStart(float const Elapsed);
    void OnRenderEnd(float const Elapsed);

    template <class StateClass>
    void setState()
    {
        if (CurrentState)
            CurrentState->end();

        CurrentState = & StateClass::get();

        CurrentState->begin();
    }

};

#endif

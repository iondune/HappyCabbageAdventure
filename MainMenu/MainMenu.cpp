
#include "../CabbageCore/CabbageCore.h"
#include "../CabbageFramework/CabbageFramework.h"
#include "../CabbageScene/CabbageScene.h"


class CMainMenuState : public CState<CMainMenuState>
{

public:

    virtual void begin()
    {}
    virtual void end()
    {}


    virtual void OnRenderStart(float const Elapsed)
    {}
    virtual void OnRenderEnd(float const Elapsed)
    {}


    virtual void OnMouseEvent(SMouseEvent const & Event)
    {}
    virtual void OnKeyboardEvent(SKeyboardEvent const & Event)
    {}

};

int main(int argc, char * argv[])
{
    CApplication & Application = CApplication::get();
    Application.init(SPosition2(200, 200));

    CStateManager & StateManager = Application.getStateManager();
    StateManager.setState<CMainMenuState>();

    Application.run();

    return 0;
}

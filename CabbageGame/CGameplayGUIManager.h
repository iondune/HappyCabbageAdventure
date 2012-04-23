#ifndef CGAMEPLAYGUIMANAGER__H
#define CGAMEPLAYGUIMANAGER__H
#include "CabbageGUI.h"
#include "CElementPlayer.h"
#include "CApplication.h"

class CGameplayGUIManager {
   Cabbage::PlayerInformation & Stats;
   CGUIFontWidget *LivesText, *seedText;
   CGUIImageWidget *Health[5];

   public:
      void update();
      CGameplayGUIManager(Cabbage::PlayerInformation & s);
};

#endif

#ifndef CGAMEPLAYGUIMANAGER__H
#define CGAMEPLAYGUIMANAGER__H
#include "CabbageGUI.h"
#include "CElementPlayer.h"
#include "CApplication.h"

class CGameplayGUIManager {
   Cabbage::PlayerInformation & Stats;
   CGUIFontWidget *livesText, *seedText, *fpsText;
   CGUIImageWidget *Health[5], *CabbageFace, *CabbageMeter, *CabbageHurtFace;

   private:
   float MaxMeterSize;
   float currentEnergyPercentage, oldEnergyPercentage, tempEnergyPercentage;
   int numFrames;
   float timeTotal;

   public:
      void update(float);
      CGameplayGUIManager(Cabbage::PlayerInformation & s);
};

#endif

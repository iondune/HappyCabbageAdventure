#include "CGameplayGUIManager.h"
#include "CElementPlayer.h"

void CGameplayGUIManager::update(float time) {
   char buf[30];
   sprintf(buf, "%d", Stats.Lives);
   LivesText->setText(buf);

   char buff[30];
   sprintf(buff, "x%d", Stats.Seeds);
   seedText->setText(buff);

   for(int i = 0; i < 5; i++) {
      if(i < Stats.Health)
         Health[i]->setVisible(true);
      else
         Health[i]->setVisible(false);
   }

   float currentEnergyPercentage = (float)Stats.Energy / (float)Stats.MaxEnergy;

   if (currentEnergyPercentage - .01f > tempEnergyPercentage)
      tempEnergyPercentage += .4f * time;
   else if (currentEnergyPercentage + .01f < tempEnergyPercentage)
      tempEnergyPercentage -= .4f * time;
   else if (tempEnergyPercentage < 0.00f)
      tempEnergyPercentage = 0.00f;

   if (tempEnergyPercentage + .01f > currentEnergyPercentage || tempEnergyPercentage - .01f < currentEnergyPercentage)
      CabbageMeter->setSize(SVector2(MaxMeterSize * tempEnergyPercentage, .1f));

   oldEnergyPercentage = currentEnergyPercentage;

   printf("tempEnergyPercentage: %f\n", tempEnergyPercentage);
}

CGameplayGUIManager::CGameplayGUIManager(Cabbage::PlayerInformation & s) : Stats(s) {
   CGUIImageWidget *CabbageEnergyBar, *Seed;

   oldEnergyPercentage = 0.0f;
   tempEnergyPercentage = 0.0f;
   MaxMeterSize = .47f;

   CabbageFace =  new CGUIImageWidget(CImageLoader::loadTGAImage("Base/cabbage.tga"), SVector2(.3f, .15f));
   CabbageFace->setPosition(SVector2(-.07f, .85f));

   CabbageHurtFace =  new CGUIImageWidget(CImageLoader::loadTGAImage("Base/cabbageouch.tga"), SVector2(.3f, .15f));
   CabbageHurtFace->setPosition(SVector2(-.081f, .861f));
   CabbageHurtFace->setVisible(false);

   for(int i = 1; i <= 5; i++) {
      char buff[30];
      sprintf(buff, "Base/HealthCabbage%d.tga", i);
      Health[i-1] = new CGUIImageWidget(CImageLoader::loadTGAImage(buff), SVector2(.1f, .1f));
      Health[i-1]->setPosition(SVector2(.11f + i*0.05, .86f));
      CApplication::get().getGUIEngine().addWidget(Health[i-1]);
   }

   CabbageEnergyBar = new CGUIImageWidget(CImageLoader::loadTGAImage("Base/EnergyBarTop.tga"), SVector2(MaxMeterSize, .1f));
   CabbageEnergyBar->setPosition(SVector2(.02f, .78f));

   CabbageMeter = new CGUIImageWidget(CImageLoader::loadTGAImage("Base/EnergyBarBottom.tga"), SVector2(MaxMeterSize, .1f));
   CabbageMeter->setPosition(SVector2(.02f, .78f));

   Seed = new CGUIImageWidget(CImageLoader::loadTGAImage("Base/seed.tga"), SVector2(.075f, .075f));
   Seed->setPosition(SVector2(CApplication::getAspectRatio() - 0.15f, 0.90f));

   CApplication::get().getGUIEngine().addWidget(CabbageMeter);
   CApplication::get().getGUIEngine().addWidget(CabbageEnergyBar);
   CApplication::get().getGUIEngine().addWidget(CabbageFace);
   CApplication::get().getGUIEngine().addWidget(CabbageHurtFace);
   CApplication::get().getGUIEngine().addWidget(Seed);

   LivesText = new CGUIFontWidget("WIFFLES_.TTF", 30.f);
   LivesText->setText("Lives: ");
   LivesText->setVisible(true);
   LivesText->setPosition(SVector2(0.13f, 0.87f));
   LivesText->setColor(SColor(0.0f, 0.80f, 0.0f));

   seedText = new CGUIFontWidget("WIFFLES_.TTF", 30.f);
   seedText->setText("x ");
   seedText->setVisible(true);
   seedText->setPosition(SVector2(CApplication::getAspectRatio() - 0.08f, 0.92f));
   seedText->setColor(SColor(1.0f));

   LivesText->addDropShadow();
   seedText->addDropShadow();

   CApplication::get().getGUIEngine().addWidget(LivesText);
   CApplication::get().getGUIEngine().addWidget(seedText);
}

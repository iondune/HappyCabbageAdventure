#define  _CRT_SECURE_NO_WARNINGS

#include "CGameplayGUIManager.h"
#include "CElementPlayer.h"

void CGameplayGUIManager::update(float time) {
   char buf[30];
   sprintf(buf, "%d", Stats.Lives);
   livesText->setText(buf);

   char buff[30];
   sprintf(buff, "x%d", Stats.Seeds);
   seedText->setText(buff);

   numFrames++;
   timeTotal += time;
   if(timeTotal >= 0.1f) {
      float fps = numFrames / timeTotal;
      sprintf(buf, "FPS: %0.0f", fps);
      fpsText->setText(buf);
      timeTotal = (float) (numFrames = 0);

   }




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
      CabbageMeter->setSize(SVector2f(MaxMeterSize * tempEnergyPercentage, .1f));

   oldEnergyPercentage = currentEnergyPercentage;
}

CGameplayGUIManager::CGameplayGUIManager(Cabbage::PlayerInformation & s) : Stats(s), numFrames(0), timeTotal(0.0f) {
   CGUIImageWidget *CabbageEnergyBar, *Seed;

   oldEnergyPercentage = (float)Stats.Energy / (float)Stats.MaxEnergy;
   tempEnergyPercentage = (float)Stats.Energy / (float)Stats.MaxEnergy;
   MaxMeterSize = .47f;

   CabbageFace =  new CGUIImageWidget(CImageLoader::loadTGAImage("Base/cabbage.tga"), SVector2f(.3f, .15f));
   CabbageFace->setPosition(SVector2f(-.07f, .85f));

   CabbageHurtFace =  new CGUIImageWidget(CImageLoader::loadTGAImage("Base/cabbageouch.tga"), SVector2f(.3f, .15f));
   CabbageHurtFace->setPosition(SVector2f(-.081f, .861f));
   CabbageHurtFace->setVisible(false);

   for(int i = 1; i <= 5; i++) {
      char buff[30];
      sprintf(buff, "Base/HealthCabbage%d.tga", i);
      Health[i-1] = new CGUIImageWidget(CImageLoader::loadTGAImage(buff), SVector2f(.1f, .1f));
      Health[i-1]->setPosition(SVector2f(0.11f + i*0.05f, 0.86f));
      CApplication::get().getGUIEngine().addWidget(Health[i-1]);
   }

   CabbageEnergyBar = new CGUIImageWidget(CImageLoader::loadTGAImage("Base/EnergyBarTop.tga"), SVector2f(MaxMeterSize, .1f));
   CabbageEnergyBar->setPosition(SVector2f(.02f, .78f));

   CabbageMeter = new CGUIImageWidget(CImageLoader::loadTGAImage("Base/EnergyBarBottom.tga"), SVector2f(MaxMeterSize, .1f));
   CabbageMeter->setPosition(SVector2f(.02f, .78f));

   Seed = new CGUIImageWidget(CImageLoader::loadTGAImage("Base/seed.tga"), SVector2f(.075f, .075f));
   Seed->setPosition(SVector2f(CApplication::getAspectRatio() - 0.15f, 0.90f));

   CApplication::get().getGUIEngine().addWidget(CabbageMeter);
   CApplication::get().getGUIEngine().addWidget(CabbageEnergyBar);
   CApplication::get().getGUIEngine().addWidget(CabbageFace);
   CApplication::get().getGUIEngine().addWidget(CabbageHurtFace);
   CApplication::get().getGUIEngine().addWidget(Seed);

   livesText = new CGUIFontWidget("WIFFLES_.TTF", 30.f);
   livesText->setText("Lives: ");
   livesText->setVisible(true);
   livesText->setPosition(SVector2f(0.13f, 0.87f));
   livesText->setColor(SColor(0.0f, 0.80f, 0.0f));


   seedText = new CGUIFontWidget("WIFFLES_.TTF", 30.f);
   seedText->setText("x ");
   seedText->setVisible(true);
   seedText->setPosition(SVector2f(CApplication::getAspectRatio() - 0.08f, 0.92f));
   seedText->setColor(SColor(1.0f));

   fpsText = new CGUIFontWidget("WIFFLES_.TTF", 15.f);
   fpsText->setText("FPS: ");
   fpsText->setVisible(true);
   fpsText->setPosition(SVector2f(CApplication::getAspectRatio() - 0.1f, 0.98f));
   fpsText->setColor(SColor(1.0f));


   livesText->addDropShadow();
   seedText->addDropShadow();
   fpsText->addDropShadow();

   CApplication::get().getGUIEngine().addWidget(livesText);
   CApplication::get().getGUIEngine().addWidget(seedText);
   CApplication::get().getGUIEngine().addWidget(fpsText);
}

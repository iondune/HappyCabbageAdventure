#include "CMainMenuState.h"

CMainMenuState::CMainMenuState() 
	: Application (CApplication::get())
{
  WoodTexture = BackgroundTexture = 0;

	size = Application.getWindowSize();
	maxX = (float) size.X;
	maxY = (float) size.Y;
	sineValue = 0.0f;
	buttonNum = 0;
printf("Before warp\n");
   SDL_WarpMouse(Uint16(0.5f*maxX), Uint16(0.4f*maxY));
   printf("After warp\n");
}

void CMainMenuState::setupTextures()
{
   WoodTexture = new CTexture(CImageLoader::loadTGAImage("MainMenu/wood.tga"));
   BackgroundTexture = new CTexture(CImageLoader::loadImage("../background.bmp"));
}

void CMainMenuState::setupButtons() {
	float ratio = (float)size.X/(float) size.Y;

	StartGame = new CGUIImageWidget(WoodTexture, SVector2f(.4f, .12f));
	StartGame->setPosition(SVector2f(ratio/2.f - 0.2f, 0.5f));

	StartEditor = new CGUIImageWidget(WoodTexture, SVector2f(.4f, .12f));
	StartEditor->setPosition(SVector2f(ratio/2.f - 0.2f, 0.32f));

	ExitGame = new CGUIImageWidget(WoodTexture, SVector2f(.4f, .12f));
	ExitGame->setPosition(SVector2f(ratio/2.f - 0.2f, 0.14f));

   SColor FontColor(1.0f, 1.0f, 1.0f);

	StartFont = new CGUIFontWidget("JustAnotherFont.TTF", 32.f);
	StartFont->setText("Start Game");
	StartFont->setColor(FontColor);
	StartFont->setPosition(SVector2f(ratio/2.f - 0.175f, 0.5f + 0.05f));

	EditorFont = new CGUIFontWidget("JustAnotherFont.TTF", 32.f);
	EditorFont->setText("Level Editor");
	EditorFont->setColor(FontColor);
	EditorFont->setPosition(SVector2f(ratio/2.f - 0.179f, 0.32f + 0.05f));

	ExitFont = new CGUIFontWidget("JustAnotherFont.TTF", 32.f);
	ExitFont->setText("Exit Game");
	ExitFont->setColor(FontColor);
	ExitFont->setPosition(SVector2f(ratio/2.f - .162f, .14f + .05f));

	//StartEditor = new CGUIImageWidget(WoodTexture, SVector2f(0.65f, .25f));
	Application.getGUIEngine().addWidget(StartGame);
	Application.getGUIEngine().addWidget(StartEditor);
	Application.getGUIEngine().addWidget(ExitGame);

	Application.getGUIEngine().addWidget(StartFont);
	Application.getGUIEngine().addWidget(EditorFont);
	Application.getGUIEngine().addWidget(ExitFont);
}

void CMainMenuState::setupMeshes()
{
  CShader *Toon, *DeferredToon, *Texture, * DeferredTexture;
  Toon = CShaderLoader::loadShader("Toon");
  Texture = CShaderLoader::loadShader("ToonTexture");
  DeferredToon = CShaderLoader::loadShader("Deferred/Diffuse");
  DeferredTexture = CShaderLoader::loadShader("Deferred/Textured");

  LogoMesh = CMeshLoader::load3dsMesh("Base/HappyLogo3.3ds");
  if (LogoMesh)
  {
    LogoMesh->resizeMesh(SVector3f(0.4f));
    LogoMesh->centerMeshByExtents(SVector3f(0));
    LogoMesh->calculateNormalsPerFace();
  }
  else 
  {
    fprintf(stderr, "Failed to load the logo mesh\n");
  }

  CubeMesh = CMeshLoader::createCubeMesh();
  if (CubeMesh) {
	  CubeMesh->resizeMesh(SVector3f(0.4f));
	  CubeMesh->centerMeshByExtents(SVector3f(0));
	  CubeMesh->calculateNormalsPerFace();
  }
  else
	  fprintf(stderr, "Failed to load the cube mesh\n");


  RenderLogo = CApplication::get().getSceneManager().addMeshSceneObject(LogoMesh, Toon, DeferredToon);
  RenderLogo->setTranslation(SVector3f(0.00, 0.07f, .2f));
  RenderLogo->setScale(SVector3f(.3f));
  RenderLogo->setRotation(SVector3f(75, 180, 0));



  RenderBackground = CApplication::get().getSceneManager().addMeshSceneObject(CubeMesh, Texture, DeferredTexture);
  RenderBackground->setTranslation(SVector3f(.0f, .0f, .3f));
  RenderBackground->setRotation(SVector3f(0.f, 0.f, 180.f));
  RenderBackground->setScale(SVector3f(1.23f, .92f, .1f));
  RenderBackground->setTexture(BackgroundTexture);
}

void CMainMenuState::setupSoundtrack() {
#ifdef _ENABLED_CABBAGE_SOUND_
   std::string temp;

   if(Mix_OpenAudio(22050, AUDIO_S16, 2, 4096))
      printf("Could not open audio!\n");

   std::string MusicDirectory = "../Media/Music/";


   temp = MusicDirectory + "Soundtracks/MainMenu.mp3";

   Soundtrack = Mix_LoadMUS(temp.c_str());

   if (!Soundtrack)
         fprintf(stderr, "Soundtrack, Mix_LoadMUS: %s\n", Mix_GetError());

   Mix_PlayMusic(Soundtrack, -1);
#endif
}

#include "CSceneEffectManager.h"
void CMainMenuState::begin()
{
   SPosition2 size = Application.getWindowSize();
   float WindowWidth = (float) size.X;
   float WindowHeight = (float) size.Y;

   glEnable(GL_DEPTH_TEST);

   Camera = new CPerspectiveCamera(WindowWidth/WindowHeight, 0.01f, 100.f, 60.f);
   Application.getSceneManager().setActiveCamera(Camera);

   Application.getSceneManager().Lights.push_back(new CLight());
   Application.getSceneManager().Lights.back()->Color = SVector3f(1.f);
   Application.getSceneManager().Lights.back()->Position = SVector3f(0.f, 0.f, -1.f);

   setupSoundtrack();
   setupTextures();
   setupMeshes();
   setupButtons();
   glClearColor(0.f,0.f,0.f,0.f);

   curAngle = 0.f;
   curDirection = 'l';

   RenderLogo->setCullingEnabled(false);
   RenderBackground->setCullingEnabled(false);
   CApplication::get().getSceneManager().getEffectManager()->setEffectEnabled(ESE_BLOOM, true);
}

void CMainMenuState::end()
{
    Application.getSceneManager().Lights.clear();
    Application.getSceneManager().removeAllSceneObjects();
    Application.getGUIEngine().removeAllWidgets();
}


void CMainMenuState::OnRenderStart(float const Elapsed)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Application.get().getSceneManager().drawAll();
	Application.getGUIEngine().drawAll();

	SDL_GetMouseState(&mouse_x, &mouse_y);

	if (mouse_x < .675f*maxX && mouse_x > .35f*maxX) {
		if(mouse_y >.375f*maxY && mouse_y < .5f*maxY)
		{
    	   if (curDirection == 'l' && curAngle <= 15) {
    		   curAngle += 15.f*Elapsed*(16.f - fabs(curAngle));

    		   StartGame->setRotation(curAngle);
    		   StartFont->setRotation(curAngle);
    	   }

    	   if (curAngle > 15) {
			   curAngle = 15;
    		   curDirection = 'r';
    	   }

    	   if (curDirection == 'r' && curAngle >= -15) {
    		   curAngle -= 15.f*Elapsed*(16.f - fabs(curAngle));

    		   StartGame->setRotation(curAngle);
    		   StartFont->setRotation(curAngle);
    	   }

    	   if (curAngle < -15) {
			   curAngle = -15;
    		   curDirection = 'l';
    	   }
       }

		else {
			StartGame->setRotation(0.f);
			StartFont->setRotation(0.f);
		}

       if(mouse_y >.55f*maxY && mouse_y < .667f*maxY)
       {
    	   if (curDirection == 'l' && curAngle <= 15) {
    		   curAngle += 15.f*Elapsed*(16.f - fabs(curAngle));
    		   StartEditor->setRotation(curAngle);
    		   EditorFont->setRotation(curAngle);
    	   }

    	   if (curAngle > 15) {
			   curAngle = 15;
    		   curDirection = 'r';
    	   }

    	   if (curDirection == 'r' && curAngle >= -15) {
    		   curAngle -= 15.f*Elapsed*(16.f - fabs(curAngle));
    		   StartEditor->setRotation(curAngle);
    		   EditorFont->setRotation(curAngle);
    	   }

    	   if (curAngle < -15) {
			   curAngle = -15;
    		   curDirection = 'l';
    	   }
       }

       else {
    	   StartEditor->setRotation(0.f);
    	   EditorFont->setRotation(0.f);
       }

       if(mouse_y >.735f*maxY  && mouse_y < .86f*maxY)
       {
    	   if (curDirection == 'l' && curAngle <= 15) {
    		   curAngle += 15.f*Elapsed*(16.f - fabs(curAngle));
    		   ExitGame->setRotation(curAngle);
    		   ExitFont->setRotation(curAngle);
    	   }

    	   if (curAngle > 15) {
			   curAngle = 15;
    		   curDirection = 'r';
    	   }

    	   if (curDirection == 'r' && curAngle >= -15) {
    		   curAngle -= 15.f*Elapsed*(16.f - fabs(curAngle));
    		   ExitGame->setRotation(curAngle);
    		   ExitFont->setRotation(curAngle);
    	   }

    	   if (curAngle < -15) {
			   curAngle = -15;
    		   curDirection = 'l';
    	   }
       }

       else {
    	   ExitGame->setRotation(0.f);
    	   ExitFont->setRotation(0.f);
       }
   }
	else {
		curAngle = 0.f;
		curDirection = 'l';

		StartGame->setRotation(0.f);
		StartFont->setRotation(0.f);
		StartEditor->setRotation(0.f);
		EditorFont->setRotation(0.f);
		ExitGame->setRotation(0.f);
		ExitFont->setRotation(0.f);
	}

	Application.get().getSceneManager().endDraw();
   SDL_GL_SwapBuffers();
}
void CMainMenuState::OnRenderEnd(float const Elapsed)
{}

void CMainMenuState::OnKeyboardEvent(SKeyboardEvent const & Event)
{
   if (Event.Pressed) {
      if (Event.Key == SDLK_UP || Event.Key == SDLK_w) {
         if (buttonNum > 0)
            buttonNum--;
         else
            buttonNum = 2;
      }

      else if (Event.Key == SDLK_DOWN || Event.Key == SDLK_s) {
         if (buttonNum < 2)
            buttonNum++;
         else
            buttonNum = 0;
      }

      else if (Event.Key == SDLK_SPACE) {
         if (buttonNum == 0) {
            OnWidgetClick(StartGame);
         }

         else if (buttonNum == 1) {
            OnWidgetClick(StartEditor);
         }

         else if (buttonNum == 2) {
            OnWidgetClick(ExitGame);
         }
      }

      else if (Event.Key == SDLK_ESCAPE) {
         OnWidgetClick(ExitGame);
      }

      if (buttonNum == 0)
         SDL_WarpMouse(Uint16(0.5f*maxX), Uint16(0.4f*maxY));
      else if (buttonNum == 1)
         SDL_WarpMouse(Uint16(0.5f*maxX), Uint16(0.6f*maxY));
      else if (buttonNum == 2)
         SDL_WarpMouse(Uint16(0.5f*maxX), Uint16(0.8f*maxY));
   }
}

void CMainMenuState::OnWidgetHover(CGUIWidget * Widget)
{}

void CMainMenuState::OnWidgetUnHover(CGUIWidget * Widget)
{}

#include "../CabbageGame/CabbageGameInformation.h"
void CMainMenuState::OnWidgetClick(CGUIWidget * Widget)
{
   if(Widget == StartGame) {
      COverworldState::get().newGame = true;
      COverworldState::get().Stats = Cabbage::PlayerInformation();
#ifdef _ENABLED_CABBAGE_SOUND_
      Mix_FadeOutMusic(2000);
#endif
      CApplication::get().getStateManager().setState(new CFadeOutState(& COverworldState::get(), 0.3f));
      //CGameplayManager::setLives(3);
      //printf("NumLives: %d\n", CGameplayManager::getNumLives());
   }
   else if(Widget == StartEditor) {
      //stopSoundtrack();
      CApplication::get().getStateManager().setState(new CFadeOutState(& CLWIBState::get(), 0.3f));
#ifdef _ENABLED_CABBAGE_SOUND_
      Mix_FadeOutMusic(2000);
#endif
   }
   else if(Widget == ExitGame) {
      CApplication::get().close();
   }
}

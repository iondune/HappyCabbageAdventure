#include "CMainMenuState.h"

CMainMenuState::CMainMenuState() 
: Application (CApplication::get())
{
  WoodTexture = BackgroundTexture = 0;

	size = Application.getWindowSize();
	maxX = (float) size.X;
	maxY = (float) size.Y;
	sineValue = 0.0f;
}

void CMainMenuState::setupTextures()
{
   WoodTexture = new CTexture(CImageLoader::loadTGAImage("wood.tga"));
   BackgroundTexture = new CTexture(CImageLoader::loadImage("../Media/background.bmp"));
}

void CMainMenuState::setupButtons() {
	float ratio = (float)size.X/(float) size.Y;

	StartGame = new CGUIImageWidget(WoodTexture, SVector2(.4f, .12f));
	StartGame->setPosition(SVector2(ratio/2.f - .2f, .5f));

	StartEditor = new CGUIImageWidget(WoodTexture, SVector2(.4f, .12f));
	StartEditor->setPosition(SVector2(ratio/2.f - .2f, .32f));

	ExitGame = new CGUIImageWidget(WoodTexture, SVector2(.4f, .12f));
	ExitGame->setPosition(SVector2(ratio/2.f - .2f, .14f));

   SColor FontColor(1.0f, 1.0f, 1.0f);

	StartFont = new CGUIFontWidget("JustAnotherFont.TTF", 32.f);
	StartFont->setText("Start Game");
	StartFont->setColor(FontColor);
	StartFont->setPosition(SVector2(ratio/2.f - .175f, .5 + .05f));

	EditorFont = new CGUIFontWidget("JustAnotherFont.TTF", 32.f);
	EditorFont->setText("Level Editor");
	EditorFont->setColor(FontColor);
	EditorFont->setPosition(SVector2(ratio/2.f - .179f, .32 + .05f));

	ExitFont = new CGUIFontWidget("JustAnotherFont.TTF", 32.f);
	ExitFont->setText("Exit Game");
	ExitFont->setColor(FontColor);
	ExitFont->setPosition(SVector2(ratio/2.f - .162f, .14f + .05f));

	//StartEditor = new CGUIImageWidget(WoodTexture, SVector2(0.65f, .25f));
	Application.getGUIEngine().addWidget(StartGame);
	Application.getGUIEngine().addWidget(StartEditor);
	Application.getGUIEngine().addWidget(ExitGame);

	Application.getGUIEngine().addWidget(StartFont);
	Application.getGUIEngine().addWidget(EditorFont);
	Application.getGUIEngine().addWidget(ExitFont);
}

void CMainMenuState::setupMeshes()
{
  CShader *Toon, *Texture;
  Toon = CShaderLoader::loadShader("Toon");
  Texture = CShaderLoader::loadShader("ToonTexture");

  LogoMesh = CMeshLoader::load3dsMesh("../Base/Models/HappyLogo3.3ds");
  if (LogoMesh)
  {
    LogoMesh->resizeMesh(SVector3(0.4f));
    LogoMesh->centerMeshByExtents(SVector3(0));
    LogoMesh->calculateNormalsPerFace();
  }
  else 
  {
    fprintf(stderr, "Failed to load the logo mesh\n");
  }

  CubeMesh = CMeshLoader::createCubeMesh();
  if (CubeMesh) {
	  CubeMesh->resizeMesh(SVector3(0.4f));
	  CubeMesh->centerMeshByExtents(SVector3(0));
	  CubeMesh->calculateNormalsPerFace();
  }
  else
	  fprintf(stderr, "Failed to load the cube mesh\n");


  RenderLogo = CApplication::get().getSceneManager().addMeshSceneObject(LogoMesh, Toon);
  RenderLogo->setTranslation(SVector3(0.00, 0.07f, .2f));
  RenderLogo->setScale(SVector3(.3f));
  RenderLogo->setRotation(SVector3(75, 180, 0));



  RenderBackground = CApplication::get().getSceneManager().addMeshSceneObject(CubeMesh, Texture);
  RenderBackground->setTranslation(SVector3(.0f, .0f, .3f));
  RenderBackground->setRotation(SVector3(0.f, 0.f, 180.f));
  RenderBackground->setScale(SVector3(1.23f, .92f, .1f));
  RenderBackground->setTexture(BackgroundTexture);
}

void CMainMenuState::begin()
{
   SPosition2 size = Application.getWindowSize();
   float WindowWidth = (float) size.X;
   float WindowHeight = (float) size.Y;

   glEnable(GL_DEPTH_TEST);

   Camera = new CPerspectiveCamera(WindowWidth/WindowHeight, 0.01f, 100.f, 60.f);
   Application.getSceneManager().setActiveCamera(Camera);

   Application.getSceneManager().Lights.push_back(new CLight());
   Application.getSceneManager().Lights.back()->Color = SVector3(1.f);
   Application.getSceneManager().Lights.back()->Position = SVector3(0.f, 0.f, -1.f);

   //Sound Setup
   soundInit();
   setupSoundtrack("BeginningAnew.mp3");
   startSoundtrack();

   setupTextures();
   setupMeshes();
   setupButtons();
   our_font.init("WIFFLES_.TTF", 30);
   glClearColor(0.f,0.f,0.f,0.f);

   curAngle = 0.f;
   curDirection = 'l';

   RenderLogo->setCullingEnabled(false);
   RenderBackground->setCullingEnabled(false);
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

   /*
   SDL_GetMouseState(&mouse_x, &mouse_y);

   glTranslatef(.04f, .11f, 0);

   if(mouse_x < 540 && mouse_x > 280 && mouse_y >225 && mouse_y < 300)
   {
      if (curDirection == 'l' && curAngle <= 15) {
         glPushMatrix();
         glRotatef(curAngle+=.1f, 0, 0, 1);
         drawButton();
         glPopMatrix();
      }

      else {
         curDirection = 'r';
      }

      if (curDirection == 'r' && curAngle >= -15) {
         glPushMatrix();
         glRotatef(curAngle-=.1f, 0, 0, 1);
         drawButton();
         glPopMatrix();
      }

      else {
         curDirection = 'l';
      }

      glPushMatrix();
      glLoadIdentity();
      glTranslatef(92, 125, 0);
      glRotatef(curAngle,0,0,1);
      glScalef(1,.8f+.3f*cos(curAngle/5),1);
      glTranslatef(-80,0,0);
     freetype::print(our_font, 315, 150, "New Game", curAngle);
     glPopMatrix();
   }

   else {
      drawButton();
      freetype::print(our_font, 332, 325, "New Game");
   }

   glTranslatef(0, -0.35f, 0);

   if(mouse_x < 540 && mouse_x > 280 && mouse_y > 330 && mouse_y < 400)
   {
      if (curDirection == 'l' && curAngle <= 15) {
         glPushMatrix();
         glRotatef(curAngle+=.1f, 0, 0, 1);
         drawButton();
         glPopMatrix();
      }

      else {
         curDirection = 'r';
      }

      if (curDirection == 'r' && curAngle >= -15) {
         glPushMatrix();
         glRotatef(curAngle-=.1f, 0, 0, 1);
         //drawButton();
         glPopMatrix();
      }

      else {
         curDirection = 'l';
      }

     glPushMatrix();
      glLoadIdentity();
      glTranslatef(102, 125, 0);
      glRotatef(curAngle,0,0,1);
      glScalef(1,.8f+.3f*cos(curAngle/5),1);
      glTranslatef(-90,0,0);
     freetype::print(our_font, 300, 95, "Stage Editor", curAngle);
     glPopMatrix();
   }

   else {
      //drawButton();
      //freetype::print(our_font, 310, 215.f, "Stage Editor");
   }

   glTranslatef(0, -0.35f, 0);


   if(mouse_x < 540 && mouse_x > 280 && mouse_y > 435 && mouse_y < 510)
   {
      if (curDirection == 'l' && curAngle <= 15) {
         glPushMatrix();
         glRotatef(curAngle+=.1f, 0, 0, 1);
         drawButton();
         glPopMatrix();
      }

      else {
         curDirection = 'r';
      }

      if (curDirection == 'r' && curAngle >= -15) {
         glPushMatrix();
         glRotatef(curAngle-=.1f, 0, 0, 1);
         drawButton();
         glPopMatrix();
      }

      else {
         curDirection = 'l';
      }

     glPushMatrix();
      glLoadIdentity();
      glTranslatef(90, 125, 0);
      glRotatef(curAngle,0,0,1);
      glScalef(1,.8f+.3f*cos(curAngle/5),1);
      glTranslatef(-90,0,0);
     freetype::print(our_font, 325, -10, "Exit Game", curAngle);
     glPopMatrix();

   }

   else {
      drawButton();
      freetype::print(our_font, 325, 115.f, "Exit Game");
   }

   //Check if should reset curAngle
   if (mouse_x >= 540 || mouse_x <= 280 || mouse_y >= 510 || mouse_y <= 225
      || (mouse_y <= 435 && mouse_y >= 400) || (mouse_y <= 330 && mouse_y >= 300))
   {
      curAngle = 0;
      curDirection = 'l';
   }




   glPopMatrix();

   glEnable(GL_LIGHTING);*/

	Application.get().getSceneManager().endDraw();
   SDL_GL_SwapBuffers();
}
void CMainMenuState::OnRenderEnd(float const Elapsed)
{
}


void CMainMenuState::OnMouseEvent(SMouseEvent const & Event)
{
	if(Event.Type.Value == SMouseEvent::EType::Click){

      switch (Event.Button.Value){

      case SMouseEvent::EButton::Left:

         if(!Event.Pressed && Event.Location.X < .675f*maxX/*540*/ && Event.Location.X > .35f*maxX/*280*/)
         {
            if(Event.Location.Y >.375f*maxY/*225*/ && Event.Location.Y < .5f*maxY/*300*/)
            {
               CApplication::get().getStateManager().setState(& COverworldState::get());
            } 
            else if(Event.Location.Y >.55f*maxY/*330*/ && Event.Location.Y < .667f*maxY/*400*/)
            {
               stopSoundtrack();
               CApplication::get().getStateManager().setState(& CLWIBState::get());
            } 
            else if(Event.Location.Y >.735f*maxY/*441*/  && Event.Location.Y < .86f*maxY/*516*/)
            {
               printf("Exiting program.\n");
               exit(1);
            }
         }
         break;
      case SMouseEvent::EButton::Right:
         //printf(" Right\n", Event.Pressed);
         break;
      case SMouseEvent::EButton::Middle:
         //printf(" Middle\n", Event.Pressed);
         break;
      }
   }
}
void CMainMenuState::OnKeyboardEvent(SKeyboardEvent const & Event)
{
   if(Event.Pressed && Event.Key == SDLK_a) {
      if(RenderBackground->getShader() == CShaderLoader::loadShader("BlurV")) {
         RenderBackground->setShader("NotBlur");
         printf("Set shader to NotBlur\n");
      }
      else if(RenderBackground->getShader() == CShaderLoader::loadShader("BlurH")) {
         RenderBackground->setShader("BlurV");
         printf("Set shader to BlurV\n");
      }
      else {
         RenderBackground->setShader("BlurH");
         printf("Set shader to BlurH\n");
      }
   }
   if(Event.Pressed && Event.Key == SDLK_s) {
      if(RenderBackground->getShader() == CShaderLoader::loadShader("BlurH")) {
         RenderBackground->setShader("NotBlur");
         printf("Set shader to NotBlur\n");
      }
      else if(RenderBackground->getShader() == CShaderLoader::loadShader("BlurV")) {
         RenderBackground->setShader("BlurH");
         printf("Set shader to BlurH\n");
      }
      else {
         RenderBackground->setShader("BlurV");
         printf("Set shader to BlurV\n");
      }
   }
}

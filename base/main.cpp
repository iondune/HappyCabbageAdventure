#include <iostream>

#ifdef __unix__
#include <GL/gl.h>
#include <SDL/SDL.h>
#endif

#include "header.h"

#include "../CabbageScene/CabbageScene.h"
#include "../CabbageFramework/CabbageFramework.h"

//Variables need to create VBOs of meshes, textures, and shaders
CShader *Shader, *Diffuse;  //Use Diffuse for trees (doesn't need texture)
CImage *grassImg, *skyImg, *dirtImg;
CTexture grassTxt, skyTxt, dirtTxt;
CMesh *basicTreeMesh, *christmasTreeMesh;
CMeshRenderable renderChristmasTree, renderBasicTree;


class CGameState : public CState<CGameState>
{

    CApplication & Application;

   public:
      CGameState()
         : Application(CApplication::get())
      {}

      //Initalizer fxn
      void begin()
      {
         glClearColor(0.6f, 0.3f, 0.9f, 1.0f);

         glEnable(GL_DEPTH_TEST);
         glDepthFunc(GL_LEQUAL);
         //glEnable(GL_CULL_FACE);
         //glCullFace(GL_BACK);
         
         SDL_WM_SetCaption("Happy Cabbage Adventure", NULL);

         //Initialize Font
         our_font.init("WIFFLES_.TTF", 30);

         //Initialize Fxns
         //EngineInit();
         //ViewInit();
         //DemoLight();
         setupSoundtrack();
         startSoundtrack();

         //Set up camera  I DON'T KNOW WHAT TO DO HERE!
         //float AspectRatio = (float)WindowWidth/(float)Windowheight;
         //Camera = new CCamera(AspectRatio, ...


//This code should all be working.  Commented out so can make sure have the basics working first
         //Load shader and attributes
         //Diffuse = CShaderLoader::loadShader(Shaders/Diffuse);

         //Load3DS();
         //LoadTextures();

         //Load the meshes into VBOs
         //PrepMeshes();
      }

      //Runs at very start of display
      void OnRenderStart(float const Elapsed)
      {
      }

      //Sends event every time key pressed (also when held)
      void OnKeyboardEvent(SKeyboardEvent const & Event)
      {
         if(Event.Pressed){
            if(Event.Key == SDLK_w){
               wDown = 1;
            }
            if(Event.Key == SDLK_s){
               sDown = 1;
            }
            if(Event.Key == SDLK_a){
               aDown = 1;
            }
            if(Event.Key == SDLK_d){
               dDown = 1;
            }
            if(Event.Key == SDLK_k){
               backwardsView = !backwardsView;
            }
            if(Event.Key == SDLK_j){
               overView = NEXT(overView);
            }
            if(Event.Key == SDLK_m){
               if(musicOn) {
                  musicOn = false;
                  Mix_HaltMusic();
               }
               else {
                  musicOn = true;
                  Mix_PlayMusic(music, -1);
               }
            }
            if(Event.Key == SDLK_SPACE) {
               spaceDown = 1;

            }
            if(Event.Key == SDLK_ESCAPE) {
               finished = true;
            }
         }
         //Check if key let go, Not sure if this will work in here.
         else  {
            if(Event.Key == SDLK_w){
               wDown = 0;
            }
            if(Event.Key == SDLK_s){
               sDown = 0;
            }
            if(Event.Key == SDLK_a){
               aDown = 0;
            }
            if(Event.Key == SDLK_d){
               dDown = 0;
            }
            if(Event.Key == SDLK_k){
            }
            if(Event.Key == SDLK_j){
            }
            if(Event.Key == SDLK_SPACE){
               spaceDown = 0;
            }
         }
      }

      //Runs at program close (currently not implemented)
      void end()
      {
         stopSoundtrack();
         Mix_CloseAudio();
         our_font.clean();
      }
}

int main (int argc, char *argv[])
{
   CApplication & Application = Application.get();
   Application.init(SPosition2(1024, 768));
   
   Application.getStateManager().setState<CMainState>();

   Application.run();

   return 0;
}


void Load3DS()
{
   *basicTreeMesh = load3dsMesh("Models/tree.3ds");
   //Mesh->resizeMesh();
   basicTreeMesh->centerMeshByExtents(SVector3(0));
   basicTreeMesh->calculateNormalsPerFace();

   *christmasTreeMesh = load3dsMesh("Models/christmasTree.3ds");
   christmasTreeMesh->centerMeshByExtents(SVector3(0));
   christmasTreeMesh->calculateNormalsPerFace();
}

void LoadTextures()
{
   CImage *grassImg = CImageLoader::loadImage("Textures/grass.bmp");
   CImage *skyImg = CImageLoader::loadImage("Textures/sky.bmp");
   CImage *dirtImg = CImageLoader::loadImage("Textures/dirt.bmp");

   grassTxt = new CTexture(grassImg);
   skyTxt = new CTexture(skyImg);
   dirtTxt = new CTexture(dirtImg);
}

void PrepMeshes(std::vector<CMesh*> meshes, std::vector<CTexture*> textures, std::vector<CMeshRenderable> renderables)
{

   renderBasicTree = new CMeshRenderable();
   renderBasicTree->setMesh(*basicTree);
   renderBasicTree->setShader(Diffuse);

//My attempt at a for loop.  Not sure if it's actually worth doing
/*   for (std::vector<CMeshRenderable>::iterator render =  renderables.begin(),
        std::vector<CMesh*>::iterator mesh,
        std::vector<CTexture*>::iterator texture;
         itr != renderables.end(); itr++, mesh++, texture++)
   {
      *itr = new CMeshRenderable();
      itr->setMesh();
      itr->setMesh(*mesh);
      itr->setTexture(*texture);
      itr->setShader(Shader);
   }*/

}

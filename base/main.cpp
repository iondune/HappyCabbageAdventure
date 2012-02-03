#include <iostream>

#ifdef __unix__
#include <GL/gl.h>
#include <SDL/SDL.h>
#endif

#include "header.h"

//#include "../CabbageScene/CabbageScene.h"
//#include "../CabbageFramework/CabbageFramework.h"


class CGameState : public CState<CGameState>
{

    CApplication & Application;

   public:
      CGameState()
         : WindowWidth(1024), WindowHeight(768), Scale(1), Application(CApplication::get())
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
         float AspectRatio = (float)WindowWidth/(float)Windowheight;
         //Camera = new CCamera(AspectRatio, ...

         //Load shader and attributes
         //Shader = CShaderLoader::loadShader(Shaders/Diffuse);

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
         if(event.type == SDL_KEYDOWN){
            if(event.key.keysym.sym == SDLK_w){
               wDown = 1;
            }
            if(event.key.keysym.sym == SDLK_s){
               sDown = 1;
            }
            if(event.key.keysym.sym == SDLK_a){
               aDown = 1;
            }
            if(event.key.keysym.sym == SDLK_d){
               dDown = 1;
            }
            if(event.key.keysym.sym == SDLK_k){
               backwardsView = !backwardsView;
            }
            if(event.key.keysym.sym == SDLK_j){
               overView = NEXT(overView);
            }
            if(event.key.keysym.sym == SDLK_m){
               if(musicOn) {
                  musicOn = false;
                  Mix_HaltMusic();
               }
               else {
                  musicOn = true;
                  Mix_PlayMusic(music, -1);
               }
            }
            if(event.key.keysym.sym == SDLK_SPACE) {
               spaceDown = 1;

            }
            if(event.key.keysym.sym == SDLK_ESCAPE) {
               finished = true;
            }
         }


         //Check if key let go, Not sure if this will work in here.
         if(event.type == SDL_KEYUP){
            if(event.key.keysym.sym == SDLK_w){
               wDown = 0;
            }
            if(event.key.keysym.sym == SDLK_s){
               sDown = 0;
            }
            if(event.key.keysym.sym == SDLK_a){
               aDown = 0;
            }
            if(event.key.keysym.sym == SDLK_d){
               dDown = 0;
            }
            if(event.key.keysym.sym == SDLK_k){
            }
            if(event.key.keysym.sym == SDLK_j){
            }
            if(event.key.keysym.sym == SDLK_SPACE){
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
   CApplication & Application = CApplication::get();
   Application.init(SPosition2(1024, 768));
   
   Application.getStateManager().setState<CMainState>();

   Application.run();

   return 0;
}


void Load3DS()
{
   CMesh *basicTree = load3dsMesh("Models/tree.3ds");
   //Mesh->resizeMesh();
   Mesh->centerMeshByExtents(SVector3(0));
   Mesh->calculateNormalsPerFace();

   CMesh *christmasTree = load3dsMesh("Models/christmasTree.3ds");
   Mesh->centerMeshByExtents(SVector3(0));
   Mesh->calculateNormalsPerFace();
}

void LoadTextures()
{
   CImage *grassImg = CImageLoader::loadImage("Textures/grass.bmp");
   CImage *skyImg = CImageLoader::loadImage("Textures/sky.bmp");
   CImage *dirtImg = CImageLoader::loadImage("Textures/dirt.bmp");

   grassTexture = new CTexture(grassImg);
   skyTexture = new CTexture(skyImg);
   dirtTexture = new CTexture(dirtImg);
}

void PrepMeshes(std::vector<CMesh*> meshes, std::vector<CTexture*> textures, std::vector<CMeshRenderable> renderables)
{
   for (std::vector<CMeshRenderable>::iterator render =  renderables.begin(),
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

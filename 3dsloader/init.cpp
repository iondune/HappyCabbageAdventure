#include "main.h"   
                                    
using namespace std;

int VideoFlags = 0;                                     
SDL_Surface * MainWindow = NULL;                       

//---------CREATE WINDOW---------
void CreateMyWindow(const char * strWindowName, int width, int height, int VideoFlags)
{
    MainWindow = SDL_SetVideoMode(width, height, SCREEN_DEPTH, VideoFlags);     

    if( MainWindow == NULL )                            
    {
        cerr << "Failed to Create Window : " << SDL_GetError() << endl;         
        Quit(0);
    }
    SDL_WM_SetCaption(strWindowName, "www.spacesimulator.net - 3d engine tutorials: Tutorial 4");  
}

//---------SETUP PIXEL FORMAT---------
void SetupPixelFormat(void)
{

    VideoFlags    = SDL_OPENGL;                                         
    VideoFlags   |= SDL_HWPALETTE;                      
    VideoFlags   |= SDL_RESIZABLE;                      

    const SDL_VideoInfo * VideoInfo = SDL_GetVideoInfo();    

    if(VideoInfo == NULL)                                     
    {
        cerr << "Failed getting Video Info : " << SDL_GetError() << endl;          
        Quit(0);
    }

    if(VideoInfo -> hw_available)                      
        VideoFlags |= SDL_HWSURFACE;
    else
        VideoFlags |= SDL_SWSURFACE;

    
    if(VideoInfo -> blit_hw)                 
        VideoFlags |= SDL_HWACCEL;

    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );     
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE,   SCREEN_DEPTH);        
    SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, 0);       
    SDL_GL_SetAttribute( SDL_GL_ACCUM_RED_SIZE, 0);     
    SDL_GL_SetAttribute( SDL_GL_ACCUM_GREEN_SIZE, 0);   
    SDL_GL_SetAttribute( SDL_GL_ACCUM_BLUE_SIZE, 0);
    SDL_GL_SetAttribute( SDL_GL_ACCUM_ALPHA_SIZE, 0);
}

//---------RESIZE OPENGL SCREEN---------
void SizeOpenGLScreen(int width, int height)            
{
    if (height==0)                                        
    {
        height=1;                                        
    }

    glViewport(0,0,width,height);                       
                                                                                                                                                                                                                          
    glMatrixMode(GL_PROJECTION);                        
    glLoadIdentity();                                   
                                                                                                                                                                                                                         
                  // FOV        // Ratio                //  The farthest distance before it stops drawing
    gluPerspective(45.0f,(GLfloat)width/(GLfloat)height, .5f ,5000.0f);

    glMatrixMode(GL_MODELVIEW);                           
    glLoadIdentity();                                    
}

//---------INITIALIZE OPENGL---------
void InitializeOpenGL(int width, int height)
{
    	glEnable( GL_DEPTH_TEST );                             
    	glClearColor(0.0, 0.0, 0.2, 0.0); // This clear the background color to dark blue
    	glShadeModel(GL_SMOOTH); // Type of shading for the polygons
    	glPolygonMode (GL_FRONT_AND_BACK, GL_FILL); // Polygon rasterization mode (polygon filled)
	glEnable(GL_TEXTURE_2D); // This Enable the Texture mapping

   SizeOpenGLScreen(width, height);                       
}


//---------MAIN---------
int main(void)
{

    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )                      
    {
        cerr << "Failed initializing SDL Video : " << SDL_GetError() << endl;      
        return 1;                                             
    }

    SetupPixelFormat();    
    CreateMyWindow("www.spacesimulator.net - 3d engine tutorials: Tutorial 4", SCREEN_WIDTH, SCREEN_HEIGHT, VideoFlags);
     
    Init();
    MainLoop();
    
    return 0;
}

void Quit(int ret_val)
{
    SDL_Quit();                                            
    exit(ret_val);                                        
}

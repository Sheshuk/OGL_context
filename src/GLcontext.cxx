#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#include "GLcontext.h"
#include "GLError.h"
//--------------------------------------------------------
void VirtualContext::StartLoop(){
	printf("Starting loop\n");
	running=true;
	timr=SDL_GetTicks();
}
//--------------------------------------------------------
void VirtualContext::StopLoop(){
	printf("Stopping loop\n");
	running=false;
}
//--------------------------------------------------------
void VirtualContext::DoLoop(){
	StartLoop();
	while(running) Loop();
}
//--------------------------------------------------------
void VirtualContext::Loop(){
	//main loop cycle
	
	//update time information
	int t=SDL_GetTicks();
	dt=(t-timr)/1000.0; 
	timr=t;
	Update(dt);	//update scene (physics etc)
	//drawing:
	// printf("drawing\n");
	Clear(); 	
	Render();	
	glFlush();
	SDL_GL_SwapBuffers();

	//handle events
	SDL_Event event; 
	while ( SDL_PollEvent(&event) ){
		switch (event.type){
			case SDL_QUIT: 
				StopLoop();
				break;
			default:
				HandleEvent(event);
				break;
		};
	}
}
//--------------------------------------------------------
VirtualContext::VirtualContext(){
	printf("Creating OpenGL context\n");
}
//--------------------------------------------------------
VirtualContext::~VirtualContext(){
	printf("Closing OpenGL context\n");
	Close();
}
//--------------------------------------------------------
void VirtualContext::Clear(){
	Clear_OGL_buf();
}
//--------------------------------------------------------
void VirtualContext::Clear_OGL_buf(){
    glClear(GL_COLOR_BUFFER_BIT);
}
//--------------------------------------------------------
void VirtualContext::Close(){
	if(screen)SDL_FreeSurface(screen);
	screen=0;
	SDL_Quit();
}
//-------------------------------------------------------
void VirtualContext::PrintGL(){
	printf("Running OpenGL on:  %s\n",glGetString(GL_VENDOR));
	printf(" renderer:  %s\n",glGetString(GL_RENDERER));
	printf(" version:   %s\n",glGetString(GL_VERSION));
	printf(" Shader version: %s\n",glGetString(GL_SHADING_LANGUAGE_VERSION));
}
//-------------------------------------------------------
void VirtualContext::InitScreen(int width, int height, Uint32 flags){
	screen=SDL_SetVideoMode( width, height, 32,  SDL_HWSURFACE|SDL_OPENGL|flags);
	if(screen==0)throw SDL_GetError( );
	printf("screen=0x%x\n",screen);
	glViewport(0,0,width,height);
}
//--------------------------------------------------------
void VirtualContext::SetViewOrto(float x0,float x1,float y0,float y1,float z0,float z1){
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(x0,x1,y0,y1,z0,z1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

//--------------------------------------------------------
void VirtualContext::SetViewPers(float fovy,float aspect,float near,float far){
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fovy,aspect,near,far);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
//--------------------------------------------------------
void VirtualContext::Init_SDL()
{
	if(SDL_Init(SDL_INIT_EVERYTHING)!=0)throw -1;
	SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 32 );
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
}
void VirtualContext::Init_OGL(){
	  // glShadeModel(GL_SMOOTH);                // Enable Smooth Shading
	  // glClearColor(0.0f, 0.0f, 0.0f, 1.f);           // Black Background
	//glClearDepth(1.0f);                 // Depth Buffer Setup
	glClearColor (0.0, 0.0, 0.0, 1.0);
   	glShadeModel (GL_FLAT);
    glEnable(GL_LINE_SMOOTH);
}



	  // glEnable(GL_DEPTH_TEST);                // Enables Depth Testing
	  // glEnable(GL_TEXTURE_2D);
	  // glDepthFunc(GL_LEQUAL);                 // The Type Of Depth Testing To Do

	  // glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Really Nice Perspective Calculation

	  // glEnable(GL_POLYGON_SMOOTH);
	  // glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);              // Выбирается самый качественный
	  // GetGLError("Before init GL");

	  
	// }
//--------------------------------------------------------
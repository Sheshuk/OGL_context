#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#include "NiceLog.h"
#define _NiceVlev VLev
#include "GLcontext.h"
// #include "GLError.h"
//--------------------------------------------------------
void GL_Context::StartLoop(){
	_LogThis(1);
	//printf("Starting loop\n");
	running=true;
	timr=SDL_GetTicks();
}
//--------------------------------------------------------
void GL_Context::StopLoop(){
	_LogThis(1);
	running=false;
}
//--------------------------------------------------------
void GL_Context::DoLoop(){
	StartLoop();
	while(running) Loop();
}
//--------------------------------------------------------
void GL_Context::Loop(){
	//main loop cycle
	_LogThis(3);
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
	while ( SDL_PollEvent(&event) )
		HandleEvent(event);
}
//--------------------------------------------------------
void GL_Context::HandleEvent(SDL_Event event){
	switch (event.type){
		case SDL_QUIT: 
			StopLoop();
			break;
		case SDL_KEYDOWN:
		case SDL_KEYUP:
			keysK[event.key.keysym.sym]=(event.key.state==SDL_PRESSED);
			HandleEvent_Kbd(event);
			break;
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			keysM[event.button.button]=(event.button.state==SDL_PRESSED);
		case SDL_MOUSEMOTION:
			HandleEvent_Mouse(event);
			break;
		default: ;
		//HandleEvent(event);
		break;
	};
}
//--------------------------------------------------------
GL_Context::GL_Context(){
	_Log(1,"Creating OpenGL context\n");
	memset(keysK,0,sizeof(bool)*SDLK_LAST); //set all keys to false
	memset(keysM,0,sizeof(bool)*4); //set all keys to false
}
//--------------------------------------------------------
GL_Context::~GL_Context(){
	_Log(1,"Closing OpenGL context\n");
	Close();
}
//--------------------------------------------------------
void GL_Context::Clear(){
	_Log(4,"Clearing buffer\n");
	Clear_OGL_buf();
}
//--------------------------------------------------------
void GL_Context::Clear_OGL_buf(){
	_LogThis(5);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}
//--------------------------------------------------------
void GL_Context::Close(){
	_Log(2,"Delete screen ang close SDL\n");
	if(screen)SDL_FreeSurface(screen);
	screen=0;
	SDL_Quit();
}
//-------------------------------------------------------
void GL_Context::PrintGL(){
	_Info(0,"Running OpenGL on:  %s\n",glGetString(GL_VENDOR));
	_Info(0,"renderer:  %s\n",glGetString(GL_RENDERER));
	_Info(0,"version:   %s\n",glGetString(GL_VERSION));
	_Info(0,"Shader version: %s\n",glGetString(GL_SHADING_LANGUAGE_VERSION));
}
//-------------------------------------------------------
void GL_Context::InitScreen(int width, int height, Uint32 flags){
	_Log(2,"size=[%dx%d], flags=0x%x\n",width,height,flags);
	screen=SDL_SetVideoMode( width, height, 32,  SDL_HWSURFACE|SDL_OPENGL|flags);
	if(screen==0)throw SDL_GetError( );
	glViewport(0,0,width,height);
}
//--------------------------------------------------------
void GL_Context::SetViewOrto(float x0,float x1,float y0,float y1,float z0,float z1){
	_Log(2,"limits={[%4.2f,%4.2f]x[%4.2f,%4.2f]x[%4.2f,%4.2f]}\n",x0,x1,y0,y1,z0,z1);
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(x0,x1,y0,y1,z0,z1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

//--------------------------------------------------------
void GL_Context::SetViewPers(float fovy,float aspect,float near,float far){
	_Log(2,"fov=[%4.2f,%4.2f], aspect=%4.2f, clip=[%4.2f - %4.2f]\n",fovy*aspect,fovy,aspect,near,far);
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fovy,aspect,near,far);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
//--------------------------------------------------------
void GL_Context::Init_SDL(){	
	_LogThis(2);
	if(SDL_Init(SDL_INIT_EVERYTHING)!=0)throw -1;
	SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 32 );
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
}
void GL_Context::Init_OGL(){
		_LogThis(2);
	// glShadeModel(GL_SMOOTH);                // Enable Smooth Shading
	// glClearColor(0.0f, 0.0f, 0.0f, 1.f);           // Black Background
	glClearDepth(1.0f);                 // Depth Buffer Setup
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
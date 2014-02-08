#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

//#include "GLError.h"
#include "GLcontext.h"



class GL_test: public VirtualContext{
public:
	virtual ~GL_test(){};
	virtual void Init(){ 
		InitScreen(1000,1000/*,SDL_FULLSCREEN*/);
		Init_SDL();
		Init_OGL();
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
		PrintGL();
		// SetViewOrto(-50.,50.,-50.,50.,-50.,50.);
		SetViewPers(45.,1.,10.,200);
		glTranslatef(0,0,-100);
	}
	virtual void Render();
	virtual void Update(float dt);
	virtual void HandleEvent(SDL_Event event);	
private:
	float  ang[3] ={0,0,0};
	float  av[3]  ={0,0,0};
public:
	unsigned long Nframes = 0;
};

//--------------------------------------------------------
void GL_test::Update(float dt){
	for(unsigned i = 0; i < 3; ++i) {
		ang[i]+=av[i]*dt;
		if(ang[i]>360)ang[i]-=360;
		else if(ang[i]<0)ang[i]+=360;
	}
	
};
//--------------------------------------------------------
void GL_test::HandleEvent(SDL_Event event){
	if ( event.type == SDL_KEYDOWN )
		switch (event.key.keysym.sym){
		case SDLK_LEFT:  av[2]+=50; break;
		case SDLK_RIGHT: av[2]-=50; break;
		case SDLK_UP:    av[0]+=50; break;
		case SDLK_DOWN:  av[0]-=50; break;
		case SDLK_ESCAPE: StopLoop(); break;
		default: printf("Key = %d\n",event.key.keysym.sym);
	}
}
//--------------------------------------------------------
void DrawCube(float s=1.){
	glBegin(GL_QUADS);
  ///top
  glVertex3i( s,-s, s);
  glVertex3i( s, s, s);
  glVertex3i(-s, s, s);
  glVertex3i(-s,-s, s);
  
  glVertex3i( s, s,-s);
  glVertex3i( s,-s,-s);
  glVertex3i( s,-s, s);
  glVertex3i( s, s, s);
//   
  glVertex3i( s, s,-s);
  glVertex3i( s,-s,-s);
  glVertex3i(-s,-s,-s);
  glVertex3i(-s, s,-s);
//   
  glVertex3i(-s,-s,-s);
  glVertex3i(-s, s,-s);
  glVertex3i(-s, s, s);
  glVertex3i(-s,-s, s);
//   
  ///front
  glVertex3i(-s,-s, s);
  glVertex3i(-s,-s,-s);
  glVertex3i( s,-s,-s);
  glVertex3i( s,-s, s);
//   
  glVertex3i(-s, s, s);
  glVertex3i(-s, s,-s);
  glVertex3i( s, s,-s);
  glVertex3i( s, s, s);
  glEnd();
}
//--------------------------------------------------------
void GL_test::Render(){
	float s=20;
	glPushMatrix();
	//glColor3f(1.-ang[0]/180, ang[1]/180, ang[2]/180);
	//glRectf(-s, -s, s, s);
	for(unsigned i = s-1; i > 0; --i) {
		glRotatef(ang[0], 1.0, 0.0, 0.0);
		glRotatef(ang[1], 0.0, 1.0, 0.0);
		glRotatef(ang[2], 0.0, 0.0, 1.0);
		glColor3f(1.-i*1./s,0,i*1./s);
		DrawCube(i+1);
	}
	//DrawCube(s);
	glPopMatrix();
	Nframes++;
}
//==============================================================

	int main(int argc, char const *argv[])
	{
		printf("Hello!\n");
		// int res=SDL_Init(SDL_INIT_EVERYTHING);
		// printf("(res=%d)\n", res);
		GL_test GL;
		try {
			/* code */
			GL.Init();
			int t=SDL_GetTicks();
			GL.DoLoop();
			t=SDL_GetTicks()-t;
			std::cout<<" Rendered "<<GL.Nframes<<" frames during "<<t/1000.<<" seconds\n";
			std::cout<<"FPS = "<<GL.Nframes*1000./t<<std::endl;
		}
		catch(char* msg) {
			std::cerr << msg << '\n';
		}
		return 0;
	}
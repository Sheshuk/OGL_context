#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#define NCOLOR 1

#include "GLerror.h"
#include "GLcontext.h"
#include "GLshader.h"
#include "taylor.h"



class GL_test: public GL_Context{
public:
	virtual ~GL_test(){};
	virtual void Init(){ 
		InitScreen(1000,1000/*,SDL_FULLSCREEN*/);
		Init_SDL();
		Init_OGL();
		// glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
		glEnable(GL_DEPTH_TEST);                // Enables Depth Testing
	    glDepthFunc(GL_LEQUAL);                 // The Type Of Depth Testing To Do
	  // glEnable(GL_TEXTURE_2D);
		PrintGL();
		// SetViewOrto(-50.,50.,-50.,50.,-50.,50.);
		SetViewPers(45.,1.,50.,200);
		glTranslatef(0,0,-100);
	}
	virtual void Render();
	virtual void Update(float dt);
	virtual void HandleEvent_Kbd(SDL_Event event);
	virtual void HandleEvent_Mouse(SDL_Event event);
private:
	taylor  ang[3] ={3,3,3};
public:
	unsigned long Nframes = 0;
};

//--------------------------------------------------------
void GL_test::Update(float dt){
	ang[2][2]=50.0*(1.0*keysK[SDLK_LEFT]-1.0*keysK[SDLK_RIGHT]);
	ang[0][2]=50.0*(1.0*keysK[SDLK_UP]-1.0*keysK[SDLK_DOWN]);
	for(unsigned i = 0; i < 3; ++i) {
		ang[i].upd(dt);
		// if(ang[i]>360)ang[i]-=360;
		// else if(ang[i]<0)ang[i]+=360;
	}
};
//--------------------------------------------------------
void GL_test::HandleEvent_Kbd(SDL_Event event){
	if(event.type==SDL_KEYDOWN)
		if(event.key.keysym.sym==SDLK_ESCAPE)
			StopLoop();
}
//------------------------------------------------------
void GL_test::HandleEvent_Mouse(SDL_Event event){
	if(event.type==SDL_MOUSEBUTTONDOWN){
		StopLoop();
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
	float s=4;
	glPushMatrix();
	//glColor3f(1.-ang[0]/180, ang[1]/180, ang[2]/180);
	//glRectf(-s, -s, s, s);
	for(unsigned i = s-1; i > 0; --i) {
		glRotatef(ang[0], 1.0, 0.0, 0.0);
		glRotatef(ang[1], 0.0, 1.0, 0.0);
		glRotatef(ang[2], 0.0, 0.0, 1.0);
		glColor3f(1.-i*1./s,0,i*1./s);
		DrawCube(20-i);
	}
	//DrawCube(s);
	glPopMatrix();
	Nframes++;
}
//==============================================================

	int main(int argc, char const *argv[])
	{
		printf("Hello!\n");
		try {

			GL_test GL;
			if(argc>1)GL.SetVerbose(atoi(argv[1]));
			GL.Init();

			//test shader:
			gl_shader::Vlev=5;
			GL_ShaderProgram prog;
			gl_shader* sv=new gl_shader(GL_VERTEX_SHADER,  "shader/test.vert");
			prog+=sv;
			prog+=new gl_shader(GL_FRAGMENT_SHADER,"shader/test.frag");
			prog.Link();
			prog.Print();
			prog.Use();
			// init main loop
			int t=SDL_GetTicks();
			GL.DoLoop();
			t=SDL_GetTicks()-t;
			std::cout<<" Rendered "<<GL.Nframes<<" frames during "<<t/1000.<<" seconds\n";
			std::cout<<"FPS = "<<GL.Nframes*1000./t<<std::endl;
		}
		catch(char* msg) {
			std::cerr << msg << '\n';
		}
		catch(gl_error err){
			err.Print();
		}
		return 0;
	}
#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>


#include <glm/glm.hpp> 

#include "GLerror.h"
#include "GLcontext.h"
#include "GLshader.h"
#include "GLcamera.h"
#include "taylor.h"

extern unsigned gl_shader::Vlev;
extern unsigned GL_camera::Vlev;
extern unsigned GL_Context::Vlev;

#include <vector>
//------------------------------------------------------
// some printing functions

void Print(const glm::vec4 V){
   printf("[ %4.2f %4.2f %4.2f %4.2f ]\n",V[0],V[1],V[2],V[3]);
};

void Print(const glm::mat4 M){
int row=0;
   printf("| %4.2f %4.2f %4.2f %4.2f |\n",M[row][0],M[row][1],M[row][2],M[row][3]); row++;
   printf("| %4.2f %4.2f %4.2f %4.2f |\n",M[row][0],M[row][1],M[row][2],M[row][3]); row++;
   printf("| %4.2f %4.2f %4.2f %4.2f |\n",M[row][0],M[row][1],M[row][2],M[row][3]); row++;
   printf("| %4.2f %4.2f %4.2f %4.2f |\n",M[row][0],M[row][1],M[row][2],M[row][3]); row++;
};
//------------------------------------------------------

class GL_test: public GL_Context{
public:
	virtual ~GL_test(){};
	virtual void Init(){ 
		printf("VLEV=%d\n",GL_Context::Vlev);
		InitScreen(1000,1000/*,SDL_FULLSCREEN*/);
		Init_SDL();
		Init_OGL();
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
		// glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
		InitShaders();
		glEnable(GL_DEPTH_TEST);                // Enables Depth Testing
	    glDepthFunc(GL_LEQUAL);                 // The Type Of Depth Testing To Do
	  // glEnable(GL_TEXTURE_2D);
		PrintGL();
		//float s=10;
		//cam.SetOrtho(-s,s,-s,s,-s,s);
		cam.SetPersp(50.,1.,0.1,20);
		glUniformMatrix4fv(shaders[0]->loc("Mproj"),1,GL_FALSE,cam.Mproj_ptr());

		ang[0][0]=-72.40;
		ang[1][0]=0.00;
		ang[2][0]=153.80;
		pos[2][0]=-10;
		mixt[0]=0.5;
	}
	void InitShaders();
	virtual void Render();
	virtual void Update(float dt);
	virtual void HandleEvent_Kbd(SDL_Event event);
	virtual void HandleEvent_Mouse(SDL_Event event);
private:
	GL_camera cam="Main view";
	taylor ang[3] ={3,3,3};
	taylor pos[3] ={3,3,3};
	taylor mixt=3;
	std::vector<GL_ShaderProgram*> shaders;
public:
	unsigned long Nframes = 0;
};

//--------------------------------------------------------
void GL_test::InitShaders(){
	//test shader:
	GL_ShaderProgram* prog=new GL_ShaderProgram;
	shaders.push_back(prog);
	(*prog)+=new gl_shader(GL_VERTEX_SHADER,  "shader/test.vert");
	(*prog)+=new gl_shader(GL_FRAGMENT_SHADER,"shader/test.frag");
	prog->Link();
	prog->Print();
	prog->Use();
	glUniform4f(prog->loc("inputColour"),0,0,1,1);
}
//--------------------------------------------------------
void GL_test::Update(float dt){
	ang[2][2]=50.0*(1.0*keysK[SDLK_LEFT]-1.0*keysK[SDLK_RIGHT]);
	ang[0][2]=50.0*(1.0*keysK[SDLK_UP]  -1.0*keysK[SDLK_DOWN]);
	
	pos[0][1]=1.0*(1.0*keysK[SDLK_KP6]  -1.0*keysK[SDLK_KP4]);
	pos[1][1]=1.0*(1.0*keysK[SDLK_KP8]  -1.0*keysK[SDLK_KP2]);
	pos[2][1]=1.0*(1.0*keysK[SDLK_KP9]  -1.0*keysK[SDLK_KP1]);
	// if(keysK[SDLK_KP6])printf("kp6\n");pos[0].print();
	mixt[1]=0.5*keysK[SDLK_KP_MINUS]-0.5*keysK[SDLK_KP_PLUS];
	mixt.upd(dt);
	if(mixt[0]>1){mixt[0]=1; mixt.stop();}
	if(mixt[0]<0){mixt[0]=0; mixt.stop();}
	// mixt[0] = frac(mixt[0]);
	glUniform1f(shaders[0]->loc("mixt"),mixt);
	for(unsigned i = 0; i < 3; ++i) {
		ang[i].upd(dt);
		pos[i].upd(dt);
		// if(ang[i]>360)ang[i]-=360;
		// else if(ang[i]<0)ang[i]+=360;
	}
};

//--------------------------------------------------------
void GL_test::HandleEvent_Kbd(SDL_Event event){
	if(event.type!=SDL_KEYDOWN)return;
	switch(event.key.keysym.sym){
		case SDLK_ESCAPE:
			StopLoop();
			break;
		case SDLK_c:
			printf("pos=%4.2f %4.2f %4.2f\n",pos[0][0],pos[1][0],pos[2][0]);
			printf("ang=%4.2f %4.2f %4.2f\n",ang[0][0],ang[1][0],ang[2][0]);
			printf("mixt=%f\n",mixt[0]);
		default:
			break;
	}
}
//------------------------------------------------------
void GL_test::HandleEvent_Mouse(SDL_Event event){
	if(event.type==SDL_MOUSEBUTTONDOWN){
		StopLoop();
	}
}
//--------------------------------------------------------
void DrawCubeP(float s=1.){
	glBegin(GL_POINTS);
	for(float x=-1;x<=1;x+=0.1)
	for(float y=-1;y<=1;y+=0.1)
	for(float z=-1;z<=1;z+=0.1)
		glVertex3f( s*x,s*y,s*z);
	glEnd();
}
//--------------------------------------------------------
void DrawCube(float s=1.){
	glBegin(GL_QUADS);
  ///top
  glVertex3f( s,-s, s);
  glVertex3f( s, s, s);
  glVertex3f(-s, s, s);
  glVertex3f(-s,-s, s);
  
  glVertex3f( s, s,-s);
  glVertex3f( s,-s,-s);
  glVertex3f( s,-s, s);
  glVertex3f( s, s, s);
//   
  glVertex3f( s, s,-s);
  glVertex3f( s,-s,-s);
  glVertex3f(-s,-s,-s);
  glVertex3f(-s, s,-s);
//   
  glVertex3f(-s,-s,-s);
  glVertex3f(-s, s,-s);
  glVertex3f(-s, s, s);
  glVertex3f(-s,-s, s);
//   
  ///front
  glVertex3f(-s,-s, s);
  glVertex3f(-s,-s,-s);
  glVertex3f( s,-s,-s);
  glVertex3f( s,-s, s);
//   
  glVertex3f(-s, s, s);
  glVertex3f(-s, s,-s);
  glVertex3f( s, s,-s);
  glVertex3f( s, s, s);
  glEnd();
}
//--------------------------------------------------------
void GL_test::Render(){
	float s=mixt[0]*10;

	cam.ResetView();
	cam.Move(pos[0],pos[1],pos[2]);
	cam.Rotate(ang[0], 1.0, 0.0, 0.0);
	cam.Rotate(ang[1], 0.0, 1.0, 0.0);
	cam.Rotate(ang[2], 0.0, 0.0, 1.0);
	// Print(cam.Mproj());
	// Print(cam.Mview());
	glUniformMatrix4fv(shaders[0]->loc("Mview"),1,GL_FALSE,cam.Mview_ptr());
	glUniform1f (shaders[0]->loc("time"),timr*0.001);
	//glColor3f(1.-ang[0]/180, ang[1]/180, ang[2]/180);

	glUniform4f (shaders[0]->loc("inputColour"),0,1,0,1);
	DrawCubeP(.5*s);
	glUniform4f (shaders[0]->loc("inputColour"),0,0,1,1);
	glRectf(-s, -s, s, s);
/*	for(unsigned i = s-1; i > 0; --i) {
		glRotatef(ang[0], 1.0, 0.0, 0.0);
		glRotatef(ang[1], 0.0, 1.0, 0.0);
		glRotatef(ang[2], 0.0, 0.0, 1.0);
		glColor3f(1.-i*1./s,0,i*1./s);
		DrawCube(20-i);
	}
	*/
	// DrawCube(20);
	// glPopMatrix();
	Nframes++;
}
//==============================================================

	int main(int argc, char const *argv[])
	{
		printf("Hello!\n");
		try {

			GL_test GL;
			if(argc>1){
				GL_Context::Vlev=(atoi(argv[1]));
				gl_shader::Vlev=GL_Context::Vlev;
				GL_camera::Vlev=GL_Context::Vlev;
			}
			GL.Init();

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
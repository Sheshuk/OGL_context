#ifndef GL_CONTEXT__H
#define GL_CONTEXT__H

/*class GL_VAO
//vertex arrays object
{
public:
	GL_VAO(){glGenVertexArrays(1,h_vao);}
	~GL_VAO();
	unsigned AddBuffer(int idx,int size,GLenum type,void * data,int stride=0,bool normalized=false){
		glVertexAttribPointer(idx,size,type,norm,stride,data);
	}
	unsigned AddBuffer1(int idx,float* data,int stride=0){AddBuffer(idx,1,GL_FLOAT,data,stride,false);}
	unsigned AddBuffer1(int idx,char * data,int stride=0){AddBuffer(idx,1,GL_BYTE,data,stride,false);}
	unsigned AddBuffer1(int idx,int* data,int stride=0)  {AddBuffer(idx,1,GL_FLOAT,data,stride,false);}
 	GLsizei stride,
 	const GLvoid * pointer);
;
private:
	GLuint h_vao;
};*/
	
class GL_Context{
	//main context: SDL+OGL
public:
	GL_Context();
	~GL_Context();
	virtual void Init() = 0;
	virtual void Close();
	virtual void Clear();
	virtual void Render() = 0;
	virtual void Update(float dt) = 0;
	virtual void HandleEvent(SDL_Event event);
	virtual void HandleEvent_Kbd  (SDL_Event event){};
	virtual void HandleEvent_Mouse(SDL_Event event){};

	void StartLoop();
	void Loop();
	void StopLoop();
	void DoLoop();
	void PrintGL();

	//useful methods
	void InitScreen (int w, int h, Uint32 flags=0);
	void SetViewOrto(float x0,float x1,float y0,float y1,float z0,float z1);
	void SetViewPers(float fovy,float aspect,float near,float far);
	void Init_SDL();
	void Init_OGL();
	void Clear_OGL_buf();
	//verbosity output
	void SetVerbose(unsigned lev){VLev=lev;}
	//check if keys are pressed
	bool IsPressedK(unsigned key){return keysK[key];};
	bool IsPressedM(unsigned but){return keysM[but];}
protected:
	unsigned VLev=0; //verbose output level
	SDL_Surface* screen = 0;
	int timr;
	float dt;
	bool running = false;
	bool keysK[SDLK_LAST]; //pressed keys status
	bool keysM[4]; //mouse keys status
};


#endif

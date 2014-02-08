#ifndef GL_CONTEXT__H
#define GL_CONTEXT__H

class VirtualContext{
public:
	VirtualContext();
	~VirtualContext();
	virtual void Init() = 0;
	virtual void Close();
	virtual void Clear();
	virtual void Render() = 0;
	virtual void Update(float dt) = 0;
	virtual void HandleEvent(SDL_Event event) = 0;

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
protected:
	SDL_Surface* screen = 0;
	int timr;
	float dt;
	bool running = false;
};


#endif

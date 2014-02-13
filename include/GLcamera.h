#ifndef GL_CAMERA__H
#define GL_CAMERA__H value
#include <glm/glm.hpp> 
class GL_camera{
public:
	GL_camera(const char* name="UNTITLED");
	void SetName(const char* name){Name=name;}
	//modify view matrix
	void ResetView();
	void Move(float x, float y, float z);
	void Rotate(float ang,float ax, float ay, float az){Rotate(ang,glm::vec3(ax,ay,az));};
	void Rotate(float ang, float *axis){Rotate(ang,glm::vec3(axis[0],axis[1],axis[2]));};
	void Rotate(float ang,const glm::vec3 &axis);
	//modify proj matrix
	void ResetProj();
	void SetOrtho(float x0,float x1,float y0,float y1,float z0,float z1);
	void SetPersp(float fovy,float aspect,float near,float far);
	glm::mat4 UpdMatrix(){M_tot=M_view*M_proj; return M_tot;}
	//getting matrices data pointers
	float* Mview_ptr();
	glm::mat4 &Mview(){return M_view;}
	float* Mproj_ptr();
	glm::mat4 &Mproj(){return M_proj;}
public:
	static unsigned Vlev;
private:
	const char* Name; //camera name
	glm::mat4 M_view; //world to view transform
	glm::mat4 M_proj; //projection matrix
	glm::mat4 M_tot; //total matrix (M_view*M_proj)

};
#endif
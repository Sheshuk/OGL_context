#include "GLcamera.h"
#include "NiceLog.h"
#define _NiceVlev GL_camera::Vlev
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp> 

GL_camera::GL_camera(const char* name):Name(name){
	_Log(1,"Created camera \"%s\"\n",name);
}
void GL_camera::ResetView(){
	M_view=glm::mat4(1);
}
//--------------------------------------------------------
void GL_camera::Move(float x, float y, float z){
	M_view=glm::translate(M_view,glm::vec3(x,y,z));
};
//------------------------------------------------------
void GL_camera::Rotate(float ang,const glm::vec3 &axis){
	M_view=glm::rotate(M_view,ang,axis);
}
//------------------------------------------------------
float* GL_camera::Mview_ptr(){
	return glm::value_ptr(M_view);
}
//------------------------------------------------------
float* GL_camera::Mproj_ptr(){
	return glm::value_ptr(M_proj);
}
//------------------------------------------------------
void GL_camera::ResetProj(){
	M_proj=glm::mat4(1);
}
//--------------------------------------------------------
void GL_camera::SetOrtho(float x0,float x1,float y0,float y1,float z0,float z1){
	_Log(2,"limits={[%4.2f,%4.2f]x[%4.2f,%4.2f]x[%4.2f,%4.2f]}\n",x0,x1,y0,y1,z0,z1);
    M_proj=glm::ortho(x0,x1,y0,y1,z0,z1);
}

//--------------------------------------------------------
void GL_camera::SetPersp(float fovy,float aspect,float near,float far){
	_Log(2,"fov=[%4.2f,%4.2f], aspect=%4.2f, clip=[%4.2f - %4.2f]\n",fovy*aspect,fovy,aspect,near,far);
	M_proj=glm::perspective(fovy,aspect,near,far);
}
//--------------------------------------------------------
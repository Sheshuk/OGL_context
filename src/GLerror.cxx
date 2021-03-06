#include <cstdio>
#include <GL/gl.h>

#include "GLerror.h"
int gl_error::get(const char* nme){
  //get standard GL error. If there is error - throw exception.
  int err=glGetError();
  if(err==GL_NO_ERROR)return err;
  const char* desc=GetGLErrorDesc(err);
  throw(gl_error(err,nme,desc));
  // fprintf(stderr,"\033[1;31mGL_ERROR <%s>\033[0m\t",nme);fprintf(stderr,desc);
  return err;
}
//------------------------------------------------------
void gl_error::Print(){
    if(name)fprintf(stderr,"\033[1;31mGL_ERROR <%s>\033[0m\t",name);
    if(what)fprintf(stderr,"%s\n",what);
};
//------------------------------------------------------
const char* gl_error::GetGLErrorDesc(int err){
  switch(err){
    case GL_NO_ERROR: return "no error";
    case GL_INVALID_ENUM:
      return "GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument. The offending command is ignored and has no other side effect than to set the error flag.\n";
    case GL_INVALID_VALUE:
      return "GL_INVALID_VALUE: A numeric argument is out of range. The offending command is ignored and has no other side effect than to set the error flag.\n";
    case GL_INVALID_OPERATION:
      return "GL_INVALID_OPERATION: The specified operation is not allowed in the current state. The offending command is ignored and has no other side effect than to set the error flag.\n";
    case GL_INVALID_FRAMEBUFFER_OPERATION:
      return "GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete. The offending command is ignored and has no other side effect than to set the error flag.\n";
    case GL_OUT_OF_MEMORY:
      return "GL_OUT_OF_MEMORY: There is not enough memory left to execute the command. The state of the GL is undefined, except for the state of the error flags, after this error is recorded.\n";
    case GL_STACK_UNDERFLOW:
      return "GL_STACK_UNDERFLOW: An attempt has been made to perform an operation that would cause an internal stack to underflow.\n";
    case GL_STACK_OVERFLOW:
      return "GL_STACK_OVERFLOW: An attempt has been made to perform an operation that would cause an internal stack to overflow.\n";
    default: printf("Unknown error#%d\n",err); return "???";
  }
}
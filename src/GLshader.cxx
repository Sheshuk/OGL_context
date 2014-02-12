
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include "GLshader.h"

#include "GLerror.h"

#include "NiceLog.h"
#define _NiceVlev gl_shader::Vlev

extern unsigned gl_shader::Vlev;
//----------------------------------------------------
char* gl_shader::file_to_buf(const char *file)
/*       Function will read a text file into allocated char buffer       */
{
    FILE *fptr;
    long length;
    char *buf;
 
    fptr = fopen(file, "rb"); /* Open file for reading */
    if (!fptr) /* Return NULL on failure */
        throw(gl_error(1,"shader source: could not open",file));
    fseek(fptr, 0, SEEK_END); /* Seek to the end of the file */
    length = ftell(fptr); /* Find out how many bytes into the file we are */
    buf = (char*)malloc(length+1); /* Allocate a buffer for the entire length of the file and a null terminator */
    fseek(fptr, 0, SEEK_SET); /* Go back to the beginning of the file */
    fread(buf, length, 1, fptr); /* Read the contents of the file in to the buffer */
    fclose(fptr); /* Close the file */
    buf[length] = 0; /* Null terminator */
 
    return buf; /* Return the buffer */
}
//----------------------------------------------------
gl_shader::gl_shader(GLuint shader_Type):type(shader_Type){
    _LogThis(3);
    init();
    _Log(2,"Created shader #%d of type%d:=\"%s\"\n",id,type,typeString());
}
//----------------------------------------------------
gl_shader::gl_shader(GLuint shader_Type,const char* fname):type(shader_Type){
    _LogThis(3);
    init();
    _Log(2,"Created shader #%d of type%d:=\"%s\"\n",id,type,typeString());
    source(fname);
    compile();
    if(is_compiled()==false){
        throw(gl_error(1,"shader compile",get_compile_log()));
    }
}
//----------------------------------------------------
void gl_shader::init(){
    id=glCreateShader(type);
    gl_error::get("shader init");
    if(id==0)throw(gl_error(0,"glCreateShader","returned 0"));
}
//----------------------------------------------------
gl_shader::~gl_shader(){
    _LogThis(3);
    glDeleteShader(id);
    gl_error::get("shader delete");
    _Log(2,"Deleted shader #%d of type%d:=\"%s\"\n",id,type,typeString());
}
//----------------------------------------------------
const char* gl_shader::typeString() const{
    switch (type){
        case GL_VERTEX_SHADER:  return "GL_VERTEX_SHADER";
        case GL_FRAGMENT_SHADER: return "GL_FRAGMENT_SHADER";
    #ifdef GL_GEOMETRY_SHADER
        case GL_GEOMETRY_SHADER: return "GL_GEOMETRY_SHADER";
    #endif 
    #ifdef GL_COMPUTE_SHADER
        case GL_COMPUTE_SHADER: return "GL_COMPUTE_SHADER";
    #endif
    #ifdef GL_TESS_CONTROL_SHADER
        case GL_TESS_CONTROL_SHADER: return "GL_TESS_CONTROL_SHADER";
    #endif
        default: return "!!Unknown shader type!!";
    }
}
//------------------------------------------------------
void gl_shader::Print() const{
    printf("shader #%d of type%d:=\"%s\"\t",id,type,typeString());
    if(is_compiled()){_GRN("compiled\n");}
    else _Red("not compiled\n");
}
//------------------------------------------------------
void gl_shader::source(const char* fname){
    _Log(1,"Reading source from file %s\n",fname);
    char* src=file_to_buf(fname);
    glShaderSource(id, 1, (const GLchar**)&src, 0);
    gl_error::get("shader source");
    free(src);
}
//------------------------------------------------------
void gl_shader::compile(){
    _LogThis(2);
    glCompileShader(id);
    gl_error::get("shader compile");
    _Log(1,"Compilation of shader#%d started\n",id);
}
//------------------------------------------------------
bool gl_shader::is_compiled() const{
    GLint compiled = 0;
    glGetShaderiv( id, GL_COMPILE_STATUS, &compiled );
     gl_error::get("shader compile_status");
    return compiled!=GL_FALSE;
}
//------------------------------------------------------
char* gl_shader::get_compile_log(){
    GLint logSize = 0;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logSize);
    GLchar* infoLog= (GLchar*) malloc(logSize);
    glGetShaderInfoLog(id, logSize, &logSize,infoLog);
    gl_error::get("shader compile log");
    return infoLog;
}
//------------------------------------------------------
//------------------------------------------------------
GL_ShaderProgram::GL_ShaderProgram(){
    id = glCreateProgram();
    _Log(1,"Created shader program#%d\n",id);
};
//------------------------------------------------------
GL_ShaderProgram::~GL_ShaderProgram(){
    glDeleteProgram(id);
    _Log(1,"Deleted shader program#%d\n",id);
};
//------------------------------------------------------
void GL_ShaderProgram::Attach(const gl_shader &s){
    shaders.insert(s);
    glAttachShader(id,s.Id());
     gl_error::get("Attach shader to program");
    _Log(1,"Deleted shader program#%d\n",id);  
}
//------------------------------------------------------
void GL_ShaderProgram::Detach(const gl_shader &s){
    shaders.erase(s);
    glDetachShader(id,s.Id());
    gl_error::get("Detach shader from program");
}
//------------------------------------------------------
void GL_ShaderProgram::Link(){
    _Log(1,"Linking shader program #%d with %d shaders\n",id,Nshaders());
    glLinkProgram(id);
    gl_error::get("Link shader program");
}
//------------------------------------------------------
bool GL_ShaderProgram::is_linked() const{
    GLint isLinked = 0;
    glGetProgramiv(id, GL_LINK_STATUS, &isLinked);
    return isLinked!=GL_FALSE;
}
//------------------------------------------------------
bool GL_ShaderProgram::is_valid () const{
  glValidateProgram (id);
  int valid = -1;
  glGetProgramiv (id, GL_VALIDATE_STATUS, &valid);
  return valid!=GL_FALSE;
}
//------------------------------------------------------
char* GL_ShaderProgram::get_link_log(){
    GLint logSize = 0;
    glGetProgramiv(id, GL_INFO_LOG_LENGTH, &logSize);
    GLchar* infoLog= (GLchar*) malloc(logSize);
    glGetProgramInfoLog(id, logSize, &logSize,infoLog);
    return infoLog;
}
//------------------------------------------------------
void GL_ShaderProgram::ListShaders() const{
    for(auto&& s : shaders) s.Print();
}
//------------------------------------------------------
void GL_ShaderProgram::Print () const{
    printf("shader program #%d with %d shaders is ",id,Nshaders());
    if(is_linked()){_GRN("    linked\t");}
    else _Red("not linked\t");
    if(is_valid()){_GRN("    valid\n");}
    else _Red("not valid\n");
}
//------------------------------------------------------
void GL_ShaderProgram::Use(){
    _Log(1,"Using program #%d\n",id);
    if(is_linked()==false)throw gl_error(1,"shader program use","Program not linked!");
    if(is_valid() ==false)throw gl_error(1,"shader program use","Program not valid !");
    glUseProgram(id);
    gl_error::get("glUseProgram");
}
//------------------------------------------------------
void GL_ShaderProgram::None(){
    _Log(1,"Shaders disabled\n");
    glUseProgram(0);
    gl_error::get("glUseProgram(0)");
}
//------------------------------------------------------
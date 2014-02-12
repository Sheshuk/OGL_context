#ifndef GL_SHADER__H
#define GL_SHADER__H value

#include <set>
class gl_shader
{
public:
	gl_shader(GLuint shader_Type);
	gl_shader(GLuint shader_Type, const char* fname);
	~gl_shader();
	void init();
	void source(const char* fname);
	void compile();
	//getters
	GLuint Id() const {return id;}
	GLuint Type() const {return type;}
	const char* typeString() const;
	//check status
	bool is_compiled() const;
	char* get_compile_log();
	void Print() const;

	bool operator <(const gl_shader &s) const {return id<s.Id();}
private:
	static char* file_to_buf(const char* fname);
//data
protected:
	GLuint id;   //shader id
	GLuint type; //shader type
	
public:
	static unsigned Vlev; //verbose level
};


class GL_ShaderProgram
{
public:
	GL_ShaderProgram();
	~GL_ShaderProgram();
	void Attach(const gl_shader &s);
	void Detach(const gl_shader &s);
	void Link();
	void Use(); //use this program
static void None(); //use no program (default behaviour)
	//check status
	bool is_linked() const;
	bool is_valid() const;
	char* get_link_log();
	void Print() const;
	//manipulate shader list
	void ListShaders() const;
	unsigned Nshaders() const{return shaders.size();}
	void operator +=(const gl_shader *s){Attach(*s);}
	void operator -=(const gl_shader *s){Detach(*s);}
private:
	GLuint id; //program handle
	std::set<gl_shader> shaders;
};
#endif
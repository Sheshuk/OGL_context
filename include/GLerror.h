#ifndef GLERROR__H
#define GLERROR__H value
class gl_error{
public:
  gl_error(int Type, const char* Name, const char* What):type(Type),name(Name),what(What){};
  //~gl_error(){if(name)delete[] name; if(what)delete[] what;}
  void Print();
  
  static const char* GetGLErrorDesc(int err);
  static int get(const char* nme=0); //get standard GL error

  int type=0;
  const char* name = 0;
  const char* what = 0;
};

#endif

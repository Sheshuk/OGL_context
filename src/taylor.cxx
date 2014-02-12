#include "taylor.h"
#include <cassert>
#include <cstring>
#include <iostream>
///--------------------------------------------------------
taylor::taylor(unsigned n){
  N=n;
  c=new double[N];
  memset(c,0,sizeof(double)*N);
}
///--------------------------------------------------------
taylor::taylor(const taylor &t){
  N=t.N;
  c=new double[N];
  memcpy(c,t.c,sizeof(double)*N);
}
///--------------------------------------------------------
taylor::~taylor(){delete[] c;}
///--------------------------------------------------------
void taylor::print(){
  for(unsigned i=0;i<N;++i){
    printf("% 6.2f, ",c[i]);
  }
  printf(".\n");
}
///--------------------------------------------------------
void taylor::upd(double dt){
  if(dt==0||N==1)return;
  double x=1;
  taylor res(N);
  for(unsigned i=0;i<N;++i){
    if(i)x*=dt/i;
    for(unsigned n=0;n<N-i;++n)res[n]+=c[n+i]*x;
  }
  memcpy(c,res.c,sizeof(double)*N);
}
///--------------------------------------------------------
double & taylor::operator[](unsigned n){
  assert(n>=0 && n<N);
  return c[n];
}
///--------------------------------------------------------
bool taylor::is_moving(){
  assert(N>0);
  for(unsigned n=1;n<N;++n){
    if(c[n]!=0)return true; 
  }
  return false;
}
///--------------------------------------------------------
void taylor::stop(){
  assert(N>0);
  for(unsigned i=1; i<N; ++i){
    c[i]=0;
  }
}
///--------------------------------------------------------
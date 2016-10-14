#include "Color.h"

#ifdef __APPLE__      // if compiling on Mac OS
  #include <GLUT/glut.h>
  #include <OpenGL/gl.h>
  #include <OpenGL/glu.h>
#else         // else compiling on Linux OS
  #include <GL/glut.h>
  #include <GL/gl.h>
  #include <GL/glu.h>
#endif

#ifndef LIGHT_H
#define LIGHT_H

class Light {
  private:
  GLenum number;
  Color amb, dif, spec;
  float x, y, z;

  public:
    Light(GLenum lightNum);
    Light(GLenum lightNum, Color ambIn, Color difIn, Color specIn);
    void setPosition(float xIn, float yIn, float zIn);
    void enable();
    void setColors(Color ambIn, Color difIn, Color specIn);
    void setCutoffAngle(float angle);
  
};

#endif
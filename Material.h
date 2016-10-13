#include "Color.h"

#ifdef __APPLE__			// if compiling on Mac OS
	#include <GLUT/glut.h>
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
#else					    // else compiling on Linux OS
	#include <GL/glut.h>
	#include <GL/gl.h>
	#include <GL/glu.h>
#endif


#ifndef MATERIAL_H
#define MATERIAL_H

class Material {
  private:
  Color amb, dif, spec;
  float shine; 
  float alpha;

public:
  Material();
  Material(Color amb, Color dif, Color specIn, float shineIn);
  void setMaterial();
  

};

#endif
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

  void emerald(){amb=Color(0.0215,0.1745,0.0215);dif=Color(0.07568,0.61424,0.07568);spec=Color(0.633,0.727811,0.633);shine=0.6;}
  void jade(){amb=Color(0.135,0.2225,0.1575);dif=Color(0.54,0.89,0.63);spec=Color(0.316228,0.316228,0.316228);shine=0.1;}
  void obsidian(){amb=Color(0.05375,0.05,0.06625);dif=Color(0.18275,0.17,0.22525);spec=Color(0.332741,0.328634,0.346435);shine=0.3;}
  void pearl(){amb=Color(0.25,0.20725,0.20725);dif=Color(1,0.829,0.829);spec=Color(0.296648,0.296648,0.296648);shine=0.088;}
  void ruby(){amb=Color(0.1745,0.01175,0.01175);dif=Color(0.61424,0.04136,0.04136);spec=Color(0.727811,0.626959,0.626959);shine=0.6;}
  void turquoise(){amb=Color(0.1,0.18725,0.1745);dif=Color(0.396,0.74151,0.69102);spec=Color(0.297254,0.30829,0.306678);shine=0.1;}
  void brass(){amb=Color(0.329412,0.223529,0.027451);dif=Color(0.780392,0.568627,0.113725);spec=Color(0.992157,0.941176,0.807843);shine=0.21794872;}
  void bronze(){amb=Color(0.2125,0.1275,0.054);dif=Color(0.714,0.4284,0.18144);spec=Color(0.393548,0.271906,0.166721);shine=0.2;}
  void chrome(){amb=Color(0.25,0.25,0.25);dif=Color(0.4,0.4,0.4);spec=Color(0.774597,0.774597,0.774597);shine=0.6;}
  void copper(){amb=Color(0.19125,0.0735,0.0225);dif=Color(0.7038,0.27048,0.0828);spec=Color(0.256777,0.137622,0.086014);shine=0.1;}
  void gold(){amb=Color(0.24725,0.1995,0.0745);dif=Color(0.75164,0.60648,0.22648);spec=Color(0.628281,0.555802,0.366065);shine=0.4;}
  void silver(){amb=Color(0.19225,0.19225,0.19225);dif=Color(0.50754,0.50754,0.50754);spec=Color(0.508273,0.508273,0.508273);shine=0.4;}
  void blackplastic(){amb=Color(0.0,0.0,0.0);dif=Color(0.01,0.01,0.01);spec=Color(0.50,0.50,0.50);shine=.25;}
  void cyanplastic(){amb=Color(0.0,0.1,0.06);dif=Color(0.0,0.50980392,0.50980392);spec=Color(0.50196078,0.50196078,0.50196078);shine=.25;}
  void greenplastic(){amb=Color(0.0,0.0,0.0);dif=Color(0.1,0.35,0.1);spec=Color(0.45,0.55,0.45);shine=.25;}
  void redplastic(){amb=Color(0.0,0.0,0.0);dif=Color(0.5,0.0,0.0);spec=Color(0.7,0.6,0.6);shine=.25;}
  void whiteplastic(){amb=Color(0.0,0.0,0.0);dif=Color(0.55,0.55,0.55);spec=Color(0.70,0.70,0.70);shine=.25;}
  void yellowplastic(){amb=Color(0.0,0.0,0.0);dif=Color(0.5,0.5,0.0);spec=Color(0.60,0.60,0.50);shine=.25;}
  void blackrubber(){amb=Color(0.02,0.02,0.02);dif=Color(0.01,0.01,0.01);spec=Color(0.4,0.4,0.4);shine=.078125;}
  void cyanrubber(){amb=Color(0.0,0.05,0.05);dif=Color(0.4,0.5,0.5);spec=Color(0.04,0.7,0.7);shine=.078125;}
  void greenrubber(){amb=Color(0.0,0.05,0.0);dif=Color(0.4,0.5,0.4);spec=Color(0.04,0.7,0.04);shine=.078125;}
  void redrubber(){amb=Color(0.05,0.0,0.0);dif=Color(0.5,0.4,0.4);spec=Color(0.7,0.04,0.04);shine=.078125;}
  void whiterubber(){amb=Color(0.05,0.05,0.05);dif=Color(0.5,0.5,0.5);spec=Color(0.7,0.7,0.7);shine=.078125;}
  void yellowrubber(){amb=Color(0.05,0.05,0.0);dif=Color(0.5,0.5,0.4);spec=Color(0.7,0.7,0.04);shine=.078125;}
  

};

#endif
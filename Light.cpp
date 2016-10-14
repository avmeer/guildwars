#include "Light.h"

Light::Light(GLenum lightNum) {
  number = lightNum;
  float dif4[4] = { .1, .1, .1, 1}; 
  float spec4[4] = { 1, 1, 1, 1 };
  float amb4[4] = { 0.5, 0.5, 0.5, 1.0 }; 
  glLightfv(number, GL_AMBIENT, amb4);
  glLightfv(number, GL_DIFFUSE, dif4);
  glLightfv(number, GL_SPECULAR, spec4);
  x = y = z = 0;
  float pos[4] = { 0, 0, 0, 1 };
  glLightfv(number, GL_POSITION, pos);
}

Light::Light(GLenum lightNum, Color ambIn, Color difIn, Color specIn) {
  number = lightNum;
  amb = ambIn;
  dif = difIn;
  spec = specIn;
  float amb4[4] = { amb.get_red(), amb.get_green(), amb.get_blue(), 1 };
  float dif4[4] = { dif.get_red(), dif.get_green(), dif.get_blue(), 1 };
  float spec4[4] = { spec.get_red(), spec.get_green(), spec.get_blue(), 1 };
  glLightfv(number, GL_AMBIENT, amb4);
  glLightfv(number, GL_DIFFUSE, dif4);
  glLightfv(number, GL_SPECULAR, spec4);
  x = y = z = 0;
  float pos[4] = { 0, 0, 0, 1 };
  glLightfv(number, GL_POSITION, pos);
}

void Light::setPosition(float xIn, float yIn, float zIn) {
  x = xIn;
  y = yIn;
  z = zIn;
  float pos[4] = { x, y, z, 1 };
  glLightfv(number, GL_POSITION, pos);
}

void Light::enable() {
  glEnable(number);
}

void Light::setColors(Color ambIn, Color difIn, Color specIn) {
  amb = ambIn;
  dif = difIn;
  spec = specIn;
  float amb4[4] = { amb.get_red(), amb.get_green(), amb.get_blue(), 1 };
  float dif4[4] = { dif.get_red(), dif.get_green(), dif.get_blue(), 1 };
  float spec4[4] = { spec.get_red(), spec.get_green(), spec.get_blue(), 1 };
  glLightfv(number, GL_AMBIENT, amb4);
  glLightfv(number, GL_DIFFUSE, dif4);
  glLightfv(number, GL_SPECULAR, spec4);
}
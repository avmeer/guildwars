#include "Material.h"

Material::Material() {
  amb = Color();
  dif = Color();
  spec = Color();
  shine = 0;
  alpha = 1;
}
Material::Material(Color ambIn, Color difIn, Color specIn, float shineIn) {
  amb = ambIn;
  dif = difIn;
  spec = specIn;
  shine = shineIn;
  alpha = 1;
}

void Material::setMaterial() {
  float amb4[4] = { amb.get_red(), amb.get_green(), amb.get_blue(), alpha };
  float dif4[4] = { dif.get_red(), dif.get_green(), dif.get_blue(), alpha };
  float spec4[4] = { spec.get_red(), spec.get_green(), spec.get_blue(), alpha };
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb4);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, dif4);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec4);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shine * 128);
}
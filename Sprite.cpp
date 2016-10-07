#include "Sprite.h"

Sprite::Sprite(){
	//these values work out nicely
	spriteRadius = 0.4f;
	spriteRadiusStepSize = 0.005f;
	torusRot = 0;
}

//function to draw entire sprite using helper functions
void Sprite::drawSprite(){
	drawSpriteCenter();
	drawInnerTorus();
	drawOuterTorus();
}

void Sprite::drawSpriteCenter(){
	glPushMatrix();
	//nice light blue color
	glColor3ub(0,191,255);
	//draw the center of our sprite (sphere with varying radius that grows and shrinks)
	glutSolidSphere(spriteRadius,20,20);
	glPopMatrix();
}
void Sprite::drawInnerTorus(){
	glPushMatrix();
	//nice chartreuse shade
	glColor3ub(127,255,0);
	//draw the inner torus, rotating around its y axis
	glRotatef(torusRot,0,1,0);
	glutSolidTorus(0.25,0.75,50,50);
	glPopMatrix();
}
void Sprite::drawOuterTorus(){
	glPushMatrix();
	//nice blue color
	glColor3f(0,.5,.7);
	//draw the outermost torus, rotating around its x axis
	glRotatef(torusRot,1,0,0);
	glutSolidTorus(0.25,1.25,50,50);
	glPopMatrix();
}

//function called in timer function, used for anims
void Sprite::updateSprite(){
	
	//increase/decrease radius of sprite sphere
	spriteRadius += spriteRadiusStepSize;
	if (spriteRadius > 0.5f){spriteRadiusStepSize = -spriteRadiusStepSize;}
	if (spriteRadius < 0.1f){spriteRadiusStepSize = -spriteRadiusStepSize;}
	
	//rotate torus a little bit each time up to 360 and repeat
	torusRot += 2;
	if (torusRot >= 360){torusRot = 0;}
}
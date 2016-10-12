#include "Hero3.h"

void Hero3(){
	//position
	float carX=0;
	float carY=0;
	float carZ=0;
	
	bool wingFlap=false;
	int wingAngle=0;
}

void Hero3::drawTriangle(){
	glPushMatrix(); {
		// tell OpenGL we want to draw triangles
		glScalef( 10.0f, 10.0f, 1.0f );
		glRotatef( wingAngle, 0, 0, 1 );
		
		
		glBegin( GL_TRIANGLES ); {
			glVertex2f( 0, 5 );		// lower left corner
			glVertex2f(  5, 5 );		// lower right corner
			glVertex2f(  5,  10 );	// top corner
		}; glEnd();	// tell OpenGL we are done drawing triangles
	}; glPopMatrix(); // we pushed the matrix earlier, pop it to prevent overflows
}

void Hero3::drawWings(){
	glColor3f( 0, 1, 1 );
	
	glPushMatrix();
	glRotatef(90,0,1, 0);
	glTranslatef(25,0,50);
	
	drawTriangle();
	glPopMatrix();
	
	glPushMatrix();
	glRotatef(90,0,1, 0);
	glTranslatef(0,0,50);
	glScalef(-1,1,1);
	drawTriangle();
	glPopMatrix();

}

void Hero3::drawEye(){
	//eye white
	glPushMatrix();{
		glColor3f( 1, 1, 1 );
		glutSolidSphere(14,20,20);
	}glPopMatrix();
	
	//iris
	
	//drawCircle(7);
	glPushMatrix();{
		glColor3f( 0, .25, .5 );
		glTranslatef(-9,0,0);
		glutSolidSphere(7,20,20);
	}glPopMatrix();
	
	//pupil
	glPushMatrix();{
		glTranslatef(-15,0,0);
		glColor3f( 0, 0, 0 );
		glutSolidSphere(3,20,20);
	}glPopMatrix();
}


void Hero3::wingTimer( int value ){
	if(wingFlap){
		wingAngle=25;
	}
	else{
		wingAngle=0;
	}
	wingFlap=!wingFlap;
	
	//glutTimerFunc( 100, wingTimer, 0 );
}

void Hero3::drawHead(){
	glColor3f(0, 1, 0);
	glutSolidSphere(50,200,200);
	
	//left eye
	glTranslatef(-50,0,-30);
	drawEye();
	//right eye
	glTranslatef(0,0,45);
	drawEye();
	glPushMatrix();
	glTranslatef(-10,0,0);
	drawWings();
	glPopMatrix();
}

void Hero3::draw(){
	drawHead();
}
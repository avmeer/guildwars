#include "Transport.h"

Transport::Transport() {
	sphereRadius = 5;
	transportOffset = 0.1;
	scaleFactor = 0.25;
	engineRotation = 0;
	dodecDisplacement = 0;
	hoverUp = true;
}

void Transport::drawEngine() {
    glPushMatrix(); {
        glColor3ub(21, 21, 21);
        glutSolidTorus(1, 1.5, 20, 10);
    } glPopMatrix();
}

void Transport::drawCockpit() {
    glPushMatrix(); {
        glColor3ub(12, 20, 25);
        glutSolidSphere(sphereRadius, 20, 20);
        glTranslatef(sphereRadius * 0.65, 0, 0);
        glRotatef(90, 0, 1, 0);
        glutSolidCone(sphereRadius * 0.7, 1.5 * sphereRadius, 10, 10);
        glTranslatef(0, 0.01, 0);
        glColor3ub(5, 5, 5);
        glutSolidCone(sphereRadius * 0.7, 1.5 * sphereRadius, 10, 10);
    } glPopMatrix();
}

void Transport::drawDisk() {
    glPushMatrix(); {
        GLUquadricObj *myDisk;
        myDisk = gluNewQuadric();
        gluQuadricDrawStyle(myDisk, GLU_FILL);
        gluDisk(myDisk, 0, 1, 10, 10);
    } glPopMatrix();
}

void Transport::draw() {
	glPushMatrix();
	glTranslatef(0, transportOffset, 0);
    glRotatef(90,0,0,1);

	drawHeroName();

	glScalef(scaleFactor, scaleFactor, scaleFactor);

    //draw the main body of the transport
    drawCockpit();

    //draw the disks on the side of the main body
    glPushMatrix(); {
        glColor3f(.45, .45, .45);
        glTranslatef(0, 0, sphereRadius);
        glScalef(1.2, 1.2, 1.2);
        drawDisk();
    } glPopMatrix();

    glPushMatrix(); {
        glColor3f(.45, .45, .45);
        glTranslatef(0, 0, -sphereRadius);
        glRotatef(180, 0, 1, 0); //rotation is necessary to get the colors to show up properly
        glScalef(1.2, 1.2, 1.2);
        drawDisk();
    } glPopMatrix();

    glPushMatrix(); {
        glColor3f(0.1, 0.1, 0.1);
        glTranslatef(0, 0, sphereRadius + 0.01);
        glScalef(1.1, 1.1, 1.1);
        drawDisk();
    } glPopMatrix();

    glPushMatrix(); {
        glColor3f(0.1, 0.1, 0.1);
        glTranslatef(0, 0, -(sphereRadius + 0.01));
        glRotatef(180, 0, 1, 0);
        glScalef(1.1, 1.1, 1.1);
        drawDisk();
    } glPopMatrix();

    //draw the rear engine/dodecahedron thing because dodecahedrons are cool
    glPushMatrix(); {
        glRotatef(-90, 0, 1, 0);
        glTranslatef(0, 0, sphereRadius);
        glRotatef(engineRotation, 0, 0, 1);
        drawEngine();
        glColor3ub(13, 92, 125);
        glTranslatef(0, 0, dodecDisplacement);
        glScalef(0.5, 0.5, 0.5);
        glutSolidDodecahedron();
    } glPopMatrix();

    glPopMatrix();
}

void Transport::updateAnimation() {
	if (hoverUp) {
        transportOffset += 0.01;
        if (transportOffset > 0.5)
            hoverUp = false;
    }
    else {
        transportOffset -= 0.01;
        if (transportOffset <= 0)
            hoverUp = true;
    }

    engineRotation += 5;
}

void Transport::drawHeroName(){
	// the text
	char name[64]= "Dannister";
	glDisable(GL_LIGHTING);
	// choose a colour
	glColor3f(.7, .7, .3);
	glPushMatrix();
	// where we want it written
	glTranslatef(0, sphereRadius/2, 2.5);
	// how big we want it
	glScalef(.01, .01, .01);
	glRotatef(90, 0, 1, 0);
	for (int c=0; name[c] != 0; ++c)
		glutStrokeCharacter(GLUT_STROKE_ROMAN, name[c]);
	glPopMatrix();
	glEnable(GL_LIGHTING);
}
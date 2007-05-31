
#include "Camera.h"

Camera::Camera()
{
	setEye(0,0,0);
	setAt(0,-20,0);
	setUp(0,1,0);
}

Camera::~Camera()
{
}

void Camera::setEye(GLfloat x, GLfloat y, GLfloat z)
{
	eyeX = x;
	eyeY = y;
	eyeZ = z;
}

void Camera::setAt(GLfloat x, GLfloat y, GLfloat z)
{
	atX = x;
	atY = y;
	atZ = z;
}

void Camera::setUp(GLfloat x, GLfloat y, GLfloat z)
{
	upX = x;
	upY = y;
	upZ = z;
}

void Camera::updateLookAt()
{
	printf("Camera - Pos (%f, %f, %f)\n", eyeX, eyeY, eyeZ);
	printf("Camera - At  (%f, %f, %f)\n", atX, atY, atZ);
	printf("Camera - Up  (%f, %f, %f)\n\n", upX, upY, upZ);
	gluLookAt(	eyeX,	eyeY,	eyeZ,
				atX,	atY,	atZ,
				upX,	upY,	upZ
			 );
}

void Camera::render()
{
	glPushMatrix();
	glTranslatef(eyeX, eyeY, eyeZ);
	glTranslatef(-0.5f, -0.5f, -0.5f);
	glColor3f(0.0f, 0.0f, 0.3f);
	draw_unit_cube();
	glPopMatrix();

	glPushMatrix();
	glBegin(GL_LINES);
		glColor3f(0.3f, 0.0f, 0.0f);
		glVertex3f(eyeX, eyeY, eyeZ);
		glVertex3f(atX, atY, atZ);

		glColor3f(0.0f, 0.3f, 0.0f);
		glVertex3f(eyeX, eyeY, eyeZ);
		glVertex3f(eyeX+(upX*5), eyeY+(upY*5), eyeZ+(upZ*5));
	glEnd();
	glPopMatrix();
}



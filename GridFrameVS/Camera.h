
#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "headers.h"


class Camera
{
protected:
	GLfloat eyeX, eyeY, eyeZ;
	GLfloat atX, atY, atZ;
	GLfloat upX, upY, upZ;

public:
	Camera();
	~Camera();

	void setEye(GLfloat x, GLfloat y, GLfloat z);
	void addEye(GLfloat x, GLfloat y, GLfloat z);
	void setAt(GLfloat x, GLfloat y, GLfloat z);
	void addAt(GLfloat x, GLfloat y, GLfloat z);
	void setUp(GLfloat x, GLfloat y, GLfloat z);
	void print();
	void updateLookAt();
	void render();

	GLfloat getEyeX();
	GLfloat getEyeY();
	GLfloat getEyeZ();
};

#endif // __BALL_H__


#ifndef __BALL_H__
#define __BALL_H__

#include "headers.h"
#include "Camera.h"

class Ball
{
protected:
	GLfloat x, y, z;
	GLfloat scale;
	GLfloat radius;

	Orientation orientation;
	Orientation facing;

	libmd3_file *model;
public:
	Ball(GLfloat blockSize, GLfloat offsetX, GLfloat offsetY, GLfloat offsetZ);
	~Ball();
	void render();
	void lookAtMe(Camera *c);

	void turnLeft();
	void turnRight();
};

#endif // __BALL_H__

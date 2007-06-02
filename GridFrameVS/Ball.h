
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

	void move(GLfloat byF, GLfloat byS);

	GLfloat getX();
	GLfloat getY();
	GLfloat getZ();
	
	GLfloat getFacingX();
	GLfloat getFacingY();
	GLfloat getFacingZ();
	GLfloat getSideX();
	GLfloat getSideY();
	GLfloat getSideZ();
};

#endif // __BALL_H__

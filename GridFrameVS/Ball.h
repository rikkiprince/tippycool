
#ifndef __BALL_H__
#define __BALL_H__

#include "headers.h"


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
	Ball();
	~Ball();
	void render();
	void lookAtMe();

	void turnLeft();
	void turnRight();
};

#endif // __BALL_H__


#ifndef __LEVEL_H__
#define __LEVEL_H__

#include "headers.h"

#include "AbstractBlock.h"
#include "NormalBlock.h"
#include "Ball.h"
#include "Camera.h"


class Level
{
protected:
	GLfloat blockSize;

	int grid_width;
	int grid_height;
	int grid_depth;
	AbstractBlock **block;

	Ball *ball;

	GLfloat offsetX;
	GLfloat offsetY;
	GLfloat offsetZ;

	void draw_grid();
	int getOffset(int i, int j, int k);
public:
	Level(int x, int y, int z);
	~Level();
	void render();
	void updateCameraPosition(Camera *c);

	void turnLeft();
	void turnRight();
};

#endif // __LEVEL_H__

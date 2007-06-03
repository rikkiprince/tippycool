
#ifndef __LEVEL_H__
#define __LEVEL_H__

#include "headers.h"

#include "AbstractBlock.h"
#include "NormalBlock.h"
#include "Ball.h"
#include "Camera.h"
#include "IniFile.h"


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

	GLfloat velF;
	GLfloat velS;
	GLfloat accF;
	GLfloat accS;

	GLdouble rotF;
	GLdouble rotS;

	Camera *camera;

	void draw_grid();
	int getOffset(int i, int j, int k);
public:
	Level(int x, int y, int z);
	~Level();
	void render();
	void updateCameraPosition(bool lookat);
	void renderCamera();

	void turnLeft();
	void turnRight();

	void update();
	void update1();

	void up();
	void down();
	void left();
	void right();
	void stop();

	bool validBlock(int x, int y, int z);
};

#endif // __LEVEL_H__

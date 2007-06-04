
#ifndef __LEVEL_H__
#define __LEVEL_H__

#include "headers.h"

#include "AbstractBlock.h"
#include "NormalBlock.h"
#include "EndBlock.h"
#include "SpikeBlock.h"
#include "StarBlock.h"
#include "ButtonBlock.h"
#include "ShowerBlock.h"

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

	IniFile *ini;

	int remainingCollectables, totalCollectables;
	bool completed;

	void load();
	void destroy();
	void draw_grid();
	int getOffset(int i, int j, int k);
public:
	Level(char *fn);
	~Level();
	void render();
	void updateCameraPosition(bool lookat);
	void renderCamera();

	void turnLeft();
	void turnRight();

	void update();
	void update1();

	void handleCollisionWith(AbstractBlock *block);
	void moveMoveablesToward(Orientation f);
	bool isCompleted();

	void up();
	void down();
	void left();
	void right();
	void stop();
	void setAcceleration(GLfloat f, GLfloat s);

	void reset();

	bool validBlock(int x, int y, int z);
};

#endif // __LEVEL_H__

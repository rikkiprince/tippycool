
#ifndef __LEVEL_H__
#define __LEVEL_H__

#include "headers.h"

#include <stdio.h>
class Level
{
protected:
	int grid_width;
	int grid_height;
	int grid_depth;
	AbstractBlock **block;

	void draw_grid();
	int getOffset(int i, int j, int k);
public:
	Level(int x, int y, int z);
	~Level();
	void render();
};

#endif // __LEVEL_H__

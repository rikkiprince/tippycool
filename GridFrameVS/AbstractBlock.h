#ifndef __ABSTRACT_BLOCK_H__
#define __ABSTRACT_BLOCK_H__

#include "types.h"

enum CollisionResult { NONE, DIE, COLLECT_COLLECTABLE, COMPLETE_LEVEL, CHANGE_COLOUR, MOVE_MOVEABLE };

class AbstractBlock
{
protected:
	bool moveable;
	bool moved;
	bool solid;
	Orientation facing;
	int r,g,b;

public:
	virtual void render() = 0;
	virtual void print(int i) = 0;
	virtual CollisionResult collision() = 0;

	bool isMoveable()
	{
		return this->moveable;
	}
	bool hasMoved()
	{
		return this->moved;
	}
	bool isSolid()
	{
		return this->solid;
	}
	Orientation getFacing()
	{
		return this->facing;
	}
	int getR()
	{
		return this->r;
	}
	int getG()
	{
		return this->g;
	}
	int getB()
	{
		return this->b;
	}

	void setMoved(bool m)
	{
		this->moved = m;
	}
};

#endif // __ABSTRACT_BLOCK_H__


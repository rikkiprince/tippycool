#ifndef __ABSTRACT_BLOCK_H__
#define __ABSTRACT_BLOCK_H__

enum CollisionResult { NONE, DIE, COLLECT_COLLECTABLE, COMPLETE_LEVEL, CHANGE_COLOUR, MOVE_MOVEABLE };

class AbstractBlock
{
protected:
	bool moveable;
	bool solid;

public:
	virtual void render() = 0;
	virtual void print(int i) = 0;
	virtual CollisionResult collision() = 0;

	bool isMoveable()
	{
		return this->moveable;
	}
	bool isSolid()
	{
		return this->solid;
	}
};

#endif // __ABSTRACT_BLOCK_H__


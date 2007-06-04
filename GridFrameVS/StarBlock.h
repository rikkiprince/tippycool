
#ifndef __STAR_BLOCK_H__
#define __STAR_BLOCK_H__

#include "SpecialBlock.h"
#include "primitives.h"

class StarBlock : public SpecialBlock
{
protected:
	bool collected;

public:
	StarBlock(Orientation o);
	~StarBlock();
	void render();
	void print(int i);
	CollisionResult collision();
};

#endif // __STAR_BLOCK_H__

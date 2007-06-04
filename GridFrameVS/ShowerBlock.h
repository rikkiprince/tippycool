
#ifndef __SHOWER_BLOCK_H__
#define __SHOWER_BLOCK_H__

#include "SpecialBlock.h"
#include "primitives.h"

class ShowerBlock : public SpecialBlock
{
protected:

public:
	ShowerBlock(Orientation o);
	ShowerBlock(Orientation o, bool m);
	~ShowerBlock();
	void print(int i);
	CollisionResult collision();
};

#endif // __SHOWER_BLOCK_H__

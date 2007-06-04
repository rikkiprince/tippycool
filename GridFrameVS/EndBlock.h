
#ifndef __END_BLOCK_H__
#define __END_BLOCK_H__

#include "SpecialBlock.h"
#include "primitives.h"

class EndBlock : public SpecialBlock
{
protected:

public:
	EndBlock(Orientation o);
	EndBlock(Orientation o, bool m);
	~EndBlock();
	void print(int i);
	CollisionResult collision();
};

#endif // __END_BLOCK_H__

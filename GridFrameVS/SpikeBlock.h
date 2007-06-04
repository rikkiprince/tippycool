
#ifndef __SPIKE_BLOCK_H__
#define __SPIKE_BLOCK_H__

#include "SpecialBlock.h"
#include "primitives.h"

class SpikeBlock : public SpecialBlock
{
protected:

public:
	SpikeBlock(Orientation o);
	SpikeBlock(Orientation o, bool m);
	~SpikeBlock();
	void print(int i);
	CollisionResult collision();
};

#endif // __SPIKE_BLOCK_H__


#ifndef __SPIKE_BLOCK_H__
#define __SPIKE_BLOCK_H__

#include "SpecialBlock.h"
#include "primitives.h"

class SpikeBlock : public SpecialBlock
{
protected:

public:
	SpikeBlock(Orientation o);
	~SpikeBlock();
	void print(int i);
};

#endif // __SPIKE_BLOCK_H__

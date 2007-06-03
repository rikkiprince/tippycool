
#ifndef __SPIKE_BLOCK_H__
#define __SPIKE_BLOCK_H__

#include "AbstractBlock.h"
#include "primitives.h"

class SpikeBlock : public AbstractBlock
{
protected:
	Orientation orientation;
	libmd3_file *model;

public:
	SpikeBlock(Orientation o);
	~SpikeBlock();
	void render();
	void print(int i);
};

#endif // __SPIKE_BLOCK_H__

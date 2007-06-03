
#ifndef __STAR_BLOCK_H__
#define __STAR_BLOCK_H__

#include "AbstractBlock.h"
#include "primitives.h"

class StarBlock : public AbstractBlock
{
protected:
	Orientation orientation;
	libmd3_file *model;

public:
	StarBlock(Orientation o);
	~StarBlock();
	void render();
	void print(int i);
};

#endif // __STAR_BLOCK_H__

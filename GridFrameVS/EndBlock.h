
#ifndef __END_BLOCK_H__
#define __END_BLOCK_H__

#include "AbstractBlock.h"
#include "primitives.h"

class EndBlock : public AbstractBlock
{
protected:
	Orientation orientation;
	libmd3_file *model;

public:
	EndBlock(Orientation o);
	~EndBlock();
	void render();
	void print(int i);
};

#endif // __END_BLOCK_H__

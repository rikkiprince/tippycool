
#ifndef __SPECIAL_BLOCK_H__
#define __SPECIAL_BLOCK_H__

#include "AbstractBlock.h"
#include "primitives.h"

class SpecialBlock : public AbstractBlock
{
protected:
	Orientation orientation;
	libmd3_file *model;

public:
	SpecialBlock();
	~SpecialBlock();
	void render();
	void print(int i);
};

#endif // __SPECIAL_BLOCK_H__

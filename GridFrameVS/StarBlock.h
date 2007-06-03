
#ifndef __STAR_BLOCK_H__
#define __STAR_BLOCK_H__

#include "SpecialBlock.h"
#include "primitives.h"

class StarBlock : public SpecialBlock
{
protected:
	Orientation orientation;
	libmd3_file *model;

public:
	StarBlock(Orientation o);
	~StarBlock();
	void print(int i);
};

#endif // __STAR_BLOCK_H__

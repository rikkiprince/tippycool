
#ifndef __SPECIAL_BLOCK_H__
#define __SPECIAL_BLOCK_H__

#include "AbstractBlock.h"

class SpecialBlock : public AbstractBlock
{
protected:

public:
	SpecialBlock();
	~SpecialBlock();
	void render();
};

#endif // __SPECIAL_BLOCK_H__


#ifndef __EMPTY_BLOCK_H__
#define __EMPTY_BLOCK_H__

#include "AbstractBlock.h"

class EmptyBlock : public AbstractBlock
{
protected:

public:
	EmptyBlock();
	~EmptyBlock();
	void render();
};

#endif // __EMPTY_BLOCK_H__

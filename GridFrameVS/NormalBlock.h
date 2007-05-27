
#ifndef __NORMAL_BLOCK_H__
#define __NORMAL_BLOCK_H__

#include "AbstractBlock.h"

class NormalBlock : public AbstractBlock
{
protected:
	int texture[6];

public:
	NormalBlock();
	~NormalBlock();
	void render();
};

#endif // __NORMAL_BLOCK_H__

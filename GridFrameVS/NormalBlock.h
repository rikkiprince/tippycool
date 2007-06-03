
#ifndef __NORMAL_BLOCK_H__
#define __NORMAL_BLOCK_H__

#include "AbstractBlock.h"
#include "primitives.h"

#include <stdio.h>

class NormalBlock : public AbstractBlock
{
protected:
	bool textured;
	int texture[6];

public:
	NormalBlock();
	NormalBlock(GLuint texture);
	NormalBlock(GLuint texture, bool m);
	~NormalBlock();
	void render();
	void print(int i);
};

#endif // __NORMAL_BLOCK_H__


#ifndef __SPECIAL_BLOCK_H__
#define __SPECIAL_BLOCK_H__

#include "AbstractBlock.h"
#include "primitives.h"

class SpecialBlock : public AbstractBlock
{
protected:
	Orientation orientation;
	libmd3_file *model;
	GLfloat currentColour[4];

	void preRender();
	void postRender();

public:
	SpecialBlock();
	~SpecialBlock();
	void render();
	void print(int i);
};

#endif // __SPECIAL_BLOCK_H__

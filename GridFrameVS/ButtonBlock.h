
#ifndef __BUTTON_BLOCK_H__
#define __BUTTON_BLOCK_H__

#include "AbstractBlock.h"
#include "primitives.h"

class ButtonBlock : public AbstractBlock
{
protected:
	Orientation orientation;
	libmd3_file *model;

public:
	ButtonBlock(Orientation o);
	~ButtonBlock();
	void render();
	void print(int i);
};

#endif // __BUTTON_BLOCK_H__

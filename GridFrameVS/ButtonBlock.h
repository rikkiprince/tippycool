
#ifndef __BUTTON_BLOCK_H__
#define __BUTTON_BLOCK_H__

#include "SpecialBlock.h"
#include "primitives.h"

class ButtonBlock : public SpecialBlock
{
protected:
	Orientation facing;

public:
	ButtonBlock(Orientation o, Orientation f);
	~ButtonBlock();
	void render();
	void print(int i);
};

#endif // __BUTTON_BLOCK_H__

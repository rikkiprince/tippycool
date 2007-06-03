
#ifndef __BUTTON_BLOCK_H__
#define __BUTTON_BLOCK_H__

#include "SpecialBlock.h"
#include "primitives.h"

class ButtonBlock : public SpecialBlock
{
protected:

public:
	ButtonBlock(Orientation o);
	~ButtonBlock();
	void print(int i);
};

#endif // __BUTTON_BLOCK_H__

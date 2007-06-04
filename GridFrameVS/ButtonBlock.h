
#ifndef __BUTTON_BLOCK_H__
#define __BUTTON_BLOCK_H__

#include "SpecialBlock.h"
#include "primitives.h"

class ButtonBlock : public SpecialBlock
{
protected:

public:
	ButtonBlock(Orientation o, Orientation f);
	ButtonBlock(Orientation o, Orientation f, bool m);
	~ButtonBlock();
	void render();
	void print(int i);
	CollisionResult collision();
	Orientation getFacing();
};

#endif // __BUTTON_BLOCK_H__

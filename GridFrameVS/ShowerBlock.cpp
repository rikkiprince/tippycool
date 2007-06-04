#include "ShowerBlock.h"

#include <stdio.h>

ShowerBlock::ShowerBlock(Orientation o)
{
	printf("Constructing ShowerBlock!\n");

	this->model = NULL;
	this->orientation = o;

	printf("Finished constructing\n");
}

ShowerBlock::~ShowerBlock()
{
	printf("Destructing ShowerBlock!\n");
}

void ShowerBlock::print(int i)
{
	printf("Hello\n");
}

CollisionResult ShowerBlock::collision()
{
	return CHANGE_COLOUR;
}

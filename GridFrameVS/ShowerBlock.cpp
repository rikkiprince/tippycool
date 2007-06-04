#include "ShowerBlock.h"

#include <stdio.h>

ShowerBlock::ShowerBlock(Orientation o)
{
	printf("Constructing ShowerBlock!\n");

	this->model = NULL;
	this->orientation = o;
	this->facing = none;

	this->moveable = false;

	this->moved = false;

	printf("Finished constructing\n");
}

ShowerBlock::ShowerBlock(Orientation o, bool m)
{
	printf("Constructing ShowerBlock!\n");

	this->model = NULL;
	this->orientation = o;
	this->facing = none;

	this->moveable = m;

	this->moved = false;

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

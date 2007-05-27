#include "EmptyBlock.h"

#include <stdio.h>

EmptyBlock::EmptyBlock()
{
	printf("Constructing EmptyBlock!\n");
}

EmptyBlock::~EmptyBlock()
{
	printf("Destructing EmptyBlock!\n");
}

void EmptyBlock::render()
{
	//printf("Hello\n");
}

#include "SpecialBlock.h"

#include <stdio.h>

SpecialBlock::SpecialBlock()
{
	printf("Constructing SpecialBlock!\n");
}

SpecialBlock::~SpecialBlock()
{
	printf("Destructing SpecialBlock!\n");
}

void SpecialBlock::render()
{
	printf("Hello\n");
}

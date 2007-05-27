#include "NormalBlock.h"
#include "primitives.h"

#include <stdio.h>

NormalBlock::NormalBlock()
{
	printf("Constructing NormalBlock!\n");
}

NormalBlock::~NormalBlock()
{
	printf("Destructing NormalBlock!\n");
}

void NormalBlock::render()
{
	//printf("Hello\n");
	draw_unit_cube();
}

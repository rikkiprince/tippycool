#include "StarBlock.h"

#include <stdio.h>

StarBlock::StarBlock(Orientation o)
{
	printf("Constructing StarBlock!\n");

	this->model = NULL;

	if((this->model = load_model("models/star.md3")) == NULL)
	{
		printf("Could not load model!\n");
	}

	this->orientation = o;

	printf("Finished constructing\n");
}

StarBlock::~StarBlock()
{
	printf("Destructing StarBlock!\n");
}

void StarBlock::print(int i)
{
	printf("Hello\n");
}

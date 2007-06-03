#include "ButtonBlock.h"

#include <stdio.h>

ButtonBlock::ButtonBlock(Orientation o)
{
	printf("Constructing ButtonBlock!\n");

	this->model = NULL;

	if((this->model = load_model("models/button.md3")) == NULL)
	{
		printf("Could not load model!\n");
	}

	this->orientation = o;

	printf("Finished constructing\n");
}

ButtonBlock::~ButtonBlock()
{
	printf("Destructing ButtonBlock!\n");
}

void ButtonBlock::print(int i)
{
	printf("Hello\n");
}

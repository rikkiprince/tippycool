#include "SpikeBlock.h"

#include <stdio.h>

SpikeBlock::SpikeBlock(Orientation o)
{
	printf("Constructing SpikeBlock!\n");

	this->model = NULL;

	if((this->model = load_model("models/spikes.md3")) == NULL)
	{
		printf("Could not load model!\n");
	}

	this->orientation = o;
	this->facing = none;

	this->moveable = false;

	this->moved = false;

	printf("Finished constructing\n");
}

SpikeBlock::SpikeBlock(Orientation o, bool m)
{
	printf("Constructing SpikeBlock!\n");

	this->model = NULL;

	if((this->model = load_model("models/spikes.md3")) == NULL)
	{
		printf("Could not load model!\n");
	}

	this->orientation = o;
	this->facing = none;

	this->moveable = m;

	this->moved = false;

	printf("Finished constructing\n");
}

SpikeBlock::~SpikeBlock()
{
	printf("Destructing SpikeBlock!\n");
}

void SpikeBlock::print(int i)
{
	printf("Hello\n");
}

CollisionResult SpikeBlock::collision()
{
	return DIE;
}

#include "NormalBlock.h"

NormalBlock::NormalBlock()
{
	printf("Constructing NormalBlock!\n");

	this->textured = false;
	this->moveable = false;
	this->solid = true;
}

NormalBlock::NormalBlock(GLuint texture)
{
	printf("Constructing NormalBlock with texture %d!\n", texture);
	this->texture[0] = this->texture[1] = this->texture[2] = this->texture[3] = this->texture[4] = this->texture[5] = texture;

	this->textured = true;
	this->moveable = false;
	this->solid = true;
}

NormalBlock::NormalBlock(GLuint texture, bool m)
{
	printf("Constructing NormalBlock with texture %d!\n", texture);
	this->texture[0] = this->texture[1] = this->texture[2] = this->texture[3] = this->texture[4] = this->texture[5] = texture;

	this->textured = true;
	this->moveable = m;
	this->solid = true;
}

NormalBlock::~NormalBlock()
{
	printf("Destructing NormalBlock!\n");
}

void NormalBlock::render()
{
	//printf("Hello\n");
	if(this->textured)
		draw_textured_unit_cube(this->texture[0]);
	else
		draw_unit_cube();
}
void NormalBlock::print(int i)
{
	printf("i = %d\n", i);
}

CollisionResult NormalBlock::collision()
{
	return NONE;
}

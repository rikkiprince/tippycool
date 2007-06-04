#include "SpecialBlock.h"

#include <stdio.h>

SpecialBlock::SpecialBlock()
{
	printf("Constructing SpecialBlock!\n");

	this->solid = false;
}

SpecialBlock::~SpecialBlock()
{
	printf("Destructing SpecialBlock!\n");
}

void SpecialBlock::preRender()
{
	glPushMatrix();
	//printf("Hello\n");

	glGetFloatv(GL_CURRENT_COLOR, this->currentColour);
}

void SpecialBlock::postRender()
{
	glColor4f(currentColour[0], currentColour[1], currentColour[2], currentColour[3]);

	glPopMatrix();
}

void SpecialBlock::render()
{
	preRender();

	glColor3f(0.435294f, 0.192f, 0.596f);

	glTranslatef(0.5, 0.5, 0.5);
	if(this->orientation == TOP)
		glRotatef(-90, 1.0, 0.0, 0.0);
	else if(this->orientation == BOTTOM)
		glRotatef(90, 1.0, 0.0, 0.0);
	else if(this->orientation == LEFT)
		glRotatef(-90, 0.0, 1.0, 0.0);
	else if(this->orientation == RIGHT)
		glRotatef(90, 0.0, 1.0, 0.0);
	else if(this->orientation == FRONT)
		glRotatef(0, 1.0, 0.0, 0.0);
	else if(this->orientation == BACK)
		glRotatef(-180, 1.0, 0.0, 0.0);

	glRotatef(90, 0.0, 0.0, 1.0);
	glTranslatef(-0.5, -0.5, -0.5);
	
	if(this->model == NULL)
		draw_unit_cube();
	else
		draw_md3_file(this->model);

	postRender();
}

void SpecialBlock::print(int i)
{
	printf("Hello\n");
}

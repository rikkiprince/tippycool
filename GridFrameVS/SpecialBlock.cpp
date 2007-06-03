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
	glPushMatrix();
	//printf("Hello\n");

	GLfloat col[4];
	glGetFloatv(GL_CURRENT_COLOR, col);
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
	glTranslatef(-0.5, -0.5, -0.5);
	
	if(this->model == NULL)
		draw_unit_cube();
	else
		draw_md3_file(this->model);

	glColor4f(col[0], col[1], col[2], col[3]);

	glPopMatrix();
}

void SpecialBlock::print(int i)
{
	printf("Hello\n");
}

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

void StarBlock::render()
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

StarBlock::~StarBlock()
{
	printf("Destructing StarBlock!\n");
}

void StarBlock::print(int i)
{
	printf("Hello\n");
}

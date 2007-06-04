#include "ButtonBlock.h"

#include <stdio.h>

ButtonBlock::ButtonBlock(Orientation o, Orientation f)
{
	printf("Constructing ButtonBlock!\n");

	this->model = NULL;

	if((this->model = load_model("models/button.md3")) == NULL)
	{
		printf("Could not load model!\n");
	}

	this->orientation = o;
	this->facing = f;

	printf("Finished constructing\n");
}

intXYZ rotation_vector[MAX_DIRS] = 
{
	{ 0, 0, 0},	// None
	{ 0, 0, 1},	// Top
	{ 0, 0,-1},	// Bottom
	{ 0, 0,-1},	// Left
	{ 0, 0, 1},	// Right
	{ 0, 0, 1},	// Front
	{ 0, 0,-1},	// Back
};

GLfloat facing_angle[MAX_DIRS][MAX_DIRS] = {
//facing =	none	TOP,	BOTTOM,	LEFT,	RIGHT,	FRONT,	BACK 
		{	0,		0,		0,		0,		0,		0,		0},		// orientation = none
		{	0,		0,		0,		90,		-90,	180,	0},		// orientation = TOP
		{	0,		0,		0,		-90,	90,		0,		180},	// orientation = BOTTOM
		{	0,		0,		180,	0,		0,		90,		-90},	// orientation = LEFT
		{	0,		180,	0,		0,		0,		90,		-90},	// orientation = RIGHT
		{	0,		0,		180,	90,		-90,	0,		0},		// orientation = FRONT
		{	0,		180,	0,		-90,	90,		0,		0},		// orientation = BACK
};

void ButtonBlock::render()
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

	glRotatef(facing_angle[this->orientation][this->facing], rotation_vector[this->orientation].x, rotation_vector[this->orientation].y, rotation_vector[this->orientation].z);
	glTranslatef(-0.5, -0.5, -0.5);
	
	if(this->model == NULL)
		draw_unit_cube();
	else
		draw_md3_file(this->model);

	glColor4f(col[0], col[1], col[2], col[3]);

	glPopMatrix();
}

ButtonBlock::~ButtonBlock()
{
	printf("Destructing ButtonBlock!\n");
}

void ButtonBlock::print(int i)
{
	printf("Hello\n");
}

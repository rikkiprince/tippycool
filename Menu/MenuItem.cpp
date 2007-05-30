#include "MenuItem.h"

MenuItem::MenuItem(char *t)
{
	this->width = 10;
}

MenuItem::~MenuItem()
{
}

void MenuItem::setXY(int x, int y)
{
	this->x = x;
	this->y = y;
}

void MenuItem::render(bool highlighted)
{
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_QUADS); 
		glVertex2f(x, y); 
		glVertex2f(x + width, y); 
		glVertex2f(x + width, y + height); 
		glVertex2f(x, y + height); 
    glEnd();
	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_LINE_LOOP); 
		glVertex2f(x, y); 
		glVertex2f(x + width, y); 
		glVertex2f(x + width, y + height);
		glVertex2f(x, y + height);  
    glEnd();
	glColor3f(1.0, 1.0, 1.0);
}

bool MenuItem::click(int x, int y)
{
	if((x >= this->x && x <= (this->x + width)) && (y >= this->y && y <= (this->y + height)))
	{
		return true;
	}
}

int MenuItem::getWidth()
{
	return width;
}

void MenuItem::setHeight(int h)
{
	this->height = h;
}

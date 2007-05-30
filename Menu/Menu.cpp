#include "Menu.h"

Menu::Menu(char *t, int w, int h)
{
	screenWidth = w;
	screenHeight = h;
	selected = 0;
}

Menu::~Menu()
{
}

bool Menu::add(MenuItem *item)
{
	for(int i = 0; i < max; i++) 
	{
		if(items[i] == NULL)
		{
			items[i] = item;
			return true;
		}
	}
	return false;
}

void Menu::remove(int index)
{
	items[index] = NULL;
}

void Menu::layout()
{
	int count = 0;
	for(int i = 0; i < max; i++)
	{
		if(items[i] != NULL)
		{
			count++;
		}
	}

	int rowHeight = screenHeight / (count + count + 1);
	int midWidth = screenWidth / 2;

	int x = 0;
	int y = 0 + rowHeight;

	for(int i = 0; i < max; i++)
	{
		if(items[i] != NULL)
		{
			x = midWidth - (items[i]->getWidth() / 2);
			items[i]->setXY(x, y);
			items[i]->setHeight(rowHeight);
			y = y + rowHeight + rowHeight;
		}
	}
}

void Menu::render()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, screenWidth, 0, screenHeight);

	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity();
	//glTranslatef(0.375f, 0.375f, 0.f);
	//glScalef(0.5f, 0.5f, 0.f);
	glScalef(1.0f, -1.0f, 0.0f);
	glTranslatef(0.0f, -screenHeight, 0.0f);

	for(int i = 0; i < max; i++)
	{
		if(items[i] != NULL)
		{
			items[i]->render((i == selected));
		}
	}
}

bool Menu::click(int x, int y)
{
	for(int i = 0; i < this->max; i++)
	{
		if(this->items[i] != NULL)
		{
			if(this->items[i]->click(x, y))
			{
				//item i was clicked
			}
		}
	}

	return false;
}
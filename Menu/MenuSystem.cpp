#include "MenuSystem.h"

MenuSystem::MenuSystem()
{
	selected = 0;
	for(int i = 0; i < max; i++) 
	{
		menus[i] = NULL;
	}
}

MenuSystem::~MenuSystem()
{
}

bool MenuSystem::add(Menu *menu)
{
	for(int i = 0; i < max; i++) 
	{
		if(menus[i] == NULL)
		{
			menus[i] = menu;
			return true;
		}
	}
	return false;
}

void MenuSystem::remove(int index)
{
	menus[index] = NULL;
}

void MenuSystem::render()
{
	for(int i = 0; i < max; i++)
	{
		if(menus[i] != NULL && i == selected)
		{
			menus[i]->render();
		}
	}
}

bool MenuSystem::click(int x, int y)
{
	for(int i = 0; i < this->max; i++)
	{
		if(this->menus[i] != NULL)
		{
			if(this->menus[i]->click(x, y))
			{
				//item i was clicked
			}
		}
	}

	return false;
}
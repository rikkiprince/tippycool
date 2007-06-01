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

void MenuSystem::mouseMotion(int x, int y)
{
	menus[selected]->mouseMotion(x, y);
}

void MenuSystem::changeSelected(int change)
{
	menus[selected]->changeSelected(change);
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

int MenuSystem::enter()
{
	int newMenu = menus[selected]->enter();
	if(newMenu > -1 && newMenu < 3)
	{
		selected = newMenu;
	}
	return newMenu;
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

void MenuSystem::mouseDown(int x, int y)
{
	menus[selected]->mouseDown(x, y);
	/*int newMenu = this->menus[selected]->mouseDown(x, y);
	if(newMenu > -1 && newMenu < 3)
	{
		selected = newMenu;
	}*/
	//return -1;
}

int MenuSystem::mouseUp(int x, int y)
{
	int newMenu = this->menus[selected]->mouseUp(x, y);
	if(newMenu > -1 && newMenu < 3)
	{
		selected = newMenu;
		menus[selected]->mouseUp(x, y);
	}
	return newMenu;
}
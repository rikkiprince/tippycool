#ifndef __MENUSYSTEM_H__
#define __MENUSYSTEM_H__

#include "MenuItem.h";
#include "Menu.h";

class MenuSystem
{
protected:
	static const int max = 10;
	Menu *menus[10];
	int selected;
public:
	MenuSystem();
	~MenuSystem();
	bool add(Menu *menu);
	void changeSelected(int change);
	int enter();
	void mouseMotion(int x, int y);
	void remove(int index);
	void render();
	int mouseUp(int x, int y);
	bool mouseDown(int x, int y);
};

#endif

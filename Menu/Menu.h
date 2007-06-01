#ifndef __MENU_H__
#define __MENU_H__

#include "MenuItem.h";

class Menu
{
protected:
	static const int max = 15;
	MenuItem *items[15];
	int selected;
	TTF_Font *font;
	int screenWidth, screenHeight;
	char title[256];
public:
	Menu(char *t, int w, int h, TTF_Font *font);
	~Menu();
	bool add(MenuItem *item);
	void remove(int index);
	void layout();
	void render();
	int click(int x, int y);
};

#endif

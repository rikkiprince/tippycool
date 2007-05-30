#ifndef __MENUITEM_H__
#define __MENUITEM_H__

#include "headers.h"

class MenuItem
{
protected:
	int x, y;	// screen co-ordinates
	int width, height;
	char text[256];
public:
	MenuItem(char *t);
	~MenuItem();
	void setXY(int x, int y);
	void render(bool highlighted);
	bool click(int x, int y);
	//void setWidth(int w);
	int getWidth();
	void setHeight(int h);
	//int getHeight();
};

#endif // __MENUITEM_H__

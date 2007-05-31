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
	void render(TTF_Font *font, bool highlighted);
	bool click(int x, int y);
	void SDL_GL_RenderText(char *text, TTF_Font *font, SDL_Color color, SDL_Rect *location);
	//void setWidth(int w);
	int getWidth();
	void setHeight(int h);
	//int getHeight();
};

#endif // __MENUITEM_H__

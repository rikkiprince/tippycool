#ifndef __MENUITEM_H__
#define __MENUITEM_H__

#include "headers.h"

class MenuItem
{
protected:
	int x, y;	// screen co-ordinates
	int width, height;
	char text[256];
	int gotoMenu;
	bool selectable, outlined;
	bool pressed;
	TTF_Font *font;
	SDL_Color textColor;
	SDL_Color buttonColor;
	SDL_Rect position;
	SDL_Surface *initial;
	SDL_Surface *intermediary;
	GLuint texture;
public:
	MenuItem(TTF_Font *font, int x, int y, int width, int height, int gotoMenu);
	MenuItem(char *t, TTF_Font *font, int gotoMenu, bool selectable, bool outlined);
	~MenuItem();
	void setXY(int x, int y);
	int getMenu();
	void render(bool highlighted);
	bool mouseDown(int x, int y);
	bool mouseUp(int x, int y);
	void SDL_GL_RenderText();
	int round(double x);
	int nextpoweroftwo(int x);
	void deleteTexture();
	bool isSelectable();
	void setPressed(bool state);
	bool mouseMotion(int x, int y);
	void preRenderFont();
	//void setWidth(int w);
	int getWidth();
	void setHeight(int h);
	//int getHeight();
};

#endif // __MENUITEM_H__

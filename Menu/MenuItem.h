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
	SDL_Color textColor;
	SDL_Color buttonColor;
	SDL_Rect position;
	SDL_Surface *initial;
	SDL_Surface *intermediary;
	GLuint texture;
public:
	MenuItem(char *t, TTF_Font *font, int gotoMenu);
	~MenuItem();
	void setXY(int x, int y);
	int getMenu();
	void render(TTF_Font *font, bool highlighted);
	bool click(int x, int y);
	void SDL_GL_RenderText(char *text, TTF_Font *font, SDL_Color color, SDL_Rect *location);
	void preRenderFont(char *text, TTF_Font *font, SDL_Color color, SDL_Rect *location);
	//void setWidth(int w);
	int getWidth();
	void setHeight(int h);
	//int getHeight();
};

#endif // __MENUITEM_H__

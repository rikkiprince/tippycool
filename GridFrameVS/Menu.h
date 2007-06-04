#ifndef __MENU_H__
#define __MENU_H__

#include "MenuItem.h";

class Menu
{
protected:
	static const int max = 15;
	MenuItem *items[15];
	int selected;
	int x, y;
	TTF_Font *font;
	int screenWidth, screenHeight, width, height;
	char title[256];
	bool instructions;
	SDL_Color textColor;
	SDL_Rect position;
	SDL_Color buttonColor;
	SDL_Surface *initial;
	SDL_Surface *intermediary;
	GLuint texture;
public:
	Menu(char *t, int w, int h, TTF_Font *font);
	~Menu();
	bool add(MenuItem *item);
	void remove(int index);
	void layout();
	void render();
	int getLevel();
	void setInstructions(bool state);
	void SDL_GL_RenderText();
	bool mouseDown(int x, int y);
	int enter();
	void preRenderFont();
	void mouseMotion(int x, int y);
	void changeSelected(int change);
	int mouseUp(int x, int y);
};

#endif

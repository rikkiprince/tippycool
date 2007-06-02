#ifndef __STATUSBAR_H__
#define __STATUSBAR_H__

#include "headers.h"

class StatusBar
{
protected:
	bool top;
	char text[256];
	int x, y;	// screen co-ordinates
	int screenWidth, screenHeight, width, height;
	TTF_Font *font;
	SDL_Color textColor;
	SDL_Color buttonColor;
	SDL_Rect position;
	SDL_Surface *initial;
	SDL_Surface *intermediary;
	GLuint texture;
public:
	StatusBar(TTF_Font *font, bool top, int screenWidth, int screenHeight);
	~StatusBar();
	void setText(TTF_Font *font, char *t);
	void render();
	void SDL_GL_RenderText();
	int round(double x);
	int nextpoweroftwo(int x);
	void deleteTexture();
	void preRenderFont();
};

#endif // __STATUSBAR_H__


#include "MenuItem.h"

	SDL_Color color;
	SDL_Rect position;
	SDL_Event event;


MenuItem::MenuItem(char *t)
{
	this->width = 10;
}

MenuItem::~MenuItem()
{
}

void MenuItem::setXY(int x, int y)
{
	this->x = x;
	this->y = y;
}

void MenuItem::render(TTF_Font *font, bool highlighted)
{
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_QUADS); 
		glVertex2f(x, y); 
		glVertex2f(x + width, y); 
		glVertex2f(x + width, y + height); 
		glVertex2f(x, y + height); 
    glEnd();
	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_LINE_LOOP); 
		glVertex2f(x, y); 
		glVertex2f(x + width, y); 
		glVertex2f(x + width, y + height);
		glVertex2f(x, y + height);  
    glEnd();
	glColor3f(1.0, 1.0, 1.0);

		color.r = 255;
		color.g = 0;
		color.b = 0;
			
		position.x = this->x;
		position.y = this->y;

		//SDL_Rect location={x, y, width, height};
	SDL_GL_RenderText("Hello", font, color, &position);
}

int round(double x)
{
	return (int)(x + 0.5);
}

int nextpoweroftwo(int x)
{
	double logbase2 = log((double)x) / log((double)2);
	return round(pow((double)2,(double)ceil(logbase2)));
}

#define GL_BGRA	0x80e1

// http://www.gamedev.net/community/forums/topic.asp?topic_id=284259
void MenuItem::SDL_GL_RenderText(char *text, 
                      TTF_Font *font,
                      SDL_Color color,
                      SDL_Rect *location)
{
	SDL_Surface *initial;
	SDL_Surface *intermediary;
	SDL_Rect rect;
	int w,h;
	GLuint texture;
	
	/* Use SDL_TTF to render our text */
	initial = TTF_RenderText_Blended(font, text, color);
	
	/* Convert the rendered text to a known format */
	w = nextpoweroftwo(initial->w);
	h = nextpoweroftwo(initial->h);
	
	intermediary = SDL_CreateRGBSurface(0, w, h, 32, 
			0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);

	SDL_BlitSurface(initial, 0, intermediary, 0);
	
	/* Tell GL about our new texture */
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, w, h, 0, GL_BGRA, 
			GL_UNSIGNED_BYTE, intermediary->pixels );
	
	/* GL_NEAREST looks horrible, if scaled... */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	

	/* prepare to render our texture */
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glColor3f(1.0f, 1.0f, 1.0f);
	
	/* Draw a quad at location */
	glBegin(GL_QUADS);
		/* Recall that the origin is in the lower-left corner
		   That is why the TexCoords specify different corners
		   than the Vertex coors seem to. */
		glTexCoord2f(0.0f, -1.0f); 
			glVertex2f(location->x    , location->y);
		glTexCoord2f(1.0f, -1.0f); 
			glVertex2f(location->x + w, location->y);
		glTexCoord2f(1.0f, 0.0f); 
			glVertex2f(location->x + w, location->y + h);
		glTexCoord2f(0.0f, 0.0f); 
			glVertex2f(location->x    , location->y + h);
	glEnd();
	
	/* Bad things happen if we delete the texture before it finishes */
	glFinish();
	
	/* return the deltas in the unused w,h part of the rect */
	location->w = initial->w;
	location->h = initial->h;
	
	/* Clean up */
	SDL_FreeSurface(initial);
	SDL_FreeSurface(intermediary);
	glDeleteTextures(1, &texture);
}

bool MenuItem::click(int x, int y)
{
	if((x >= this->x && x <= (this->x + width)) && (y >= this->y && y <= (this->y + height)))
	{
		return true;
	}
}

int MenuItem::getWidth()
{
	return width;
}

void MenuItem::setHeight(int h)
{
	this->height = h;
}

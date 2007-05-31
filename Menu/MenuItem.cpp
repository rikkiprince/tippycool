#include "MenuItem.h"



#define GL_BGRA	0x80e1

MenuItem::MenuItem(char *t, TTF_Font *font)
{
	strncpy(text,t,255);
	text[255]='\0';

	int minx,maxx,miny,maxy,advance;
	TTF_GlyphMetrics(font,'M',&minx,&maxx,&miny,&maxy,&advance);

	int length = strlen(text);	
	this->width = (maxx-minx) * length;

	textColor.r = 0;
	textColor.g = 0;
	textColor.b = 0;
		
	position.x = this->x;
	position.y = this->y;

	preRenderFont(t, font, textColor, &position);

	this->width = position.w;
	this->height = position.h;
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

// http://www.gamedev.net/community/forums/topic.asp?topic_id=284259
void MenuItem::preRenderFont(char *text, 
                      TTF_Font *font,
                      SDL_Color color,
                      SDL_Rect *location)
{
	SDL_Rect rect;
	int w,h;
	int oldw,oldh;
	SDL_Rect centre;
	
	/* Use SDL_TTF to render our text */
	initial = TTF_RenderText_Blended(font, text, color);

	oldw = initial->w;
	oldh = initial->h;
	
	/* Convert the rendered text to a known format */
	w = nextpoweroftwo(initial->w);
	h = nextpoweroftwo(initial->h);
	
	intermediary = SDL_CreateRGBSurface(0, w, h, 32, 
			0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);

	centre.x = (w - oldw)/2;
	centre.y = (h - oldh)/2;

	SDL_FillRect(intermediary,NULL, SDL_MapRGB(intermediary->format, buttonColor.r * 255, buttonColor.b * 255, buttonColor.g * 255));

	SDL_BlitSurface(initial, 0, intermediary, &centre);
	
	/* Tell GL about our new texture */
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, w, h, 0, GL_BGRA, 
			GL_UNSIGNED_BYTE, intermediary->pixels );
	
	/* GL_NEAREST looks horrible, if scaled... */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	
	
	/* return the deltas in the unused w,h part of the rect */
	location->w = w;//initial->w;
	location->h = h;//initial->h;
}

MenuItem::~MenuItem()
{
	/* Clean up */
	SDL_FreeSurface(initial);
	SDL_FreeSurface(intermediary);
	glDeleteTextures(1, &texture);
}

void MenuItem::setXY(int x, int y)
{
	this->x = x;
	this->y = y;
}

void MenuItem::render(TTF_Font *font, bool highlighted)
{
	buttonColor.r = 1;
	buttonColor.g = 0;
	buttonColor.b = 0;
	glColor3f(buttonColor.r, buttonColor.g, buttonColor.b);
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

	textColor.r = 0;
	textColor.g = 0;
	textColor.b = 0;
		
	position.x = this->x;
	position.y = this->y;

	//SDL_Rect location={x, y, width, height};
	SDL_GL_RenderText(text, font, textColor, &position);
}

void MenuItem::SDL_GL_RenderText(char *text, 
                      TTF_Font *font,
                      SDL_Color color,
                      SDL_Rect *location)
{

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
			glVertex2f(this->x    , this->y);
		glTexCoord2f(1.0f, -1.0f); 
			glVertex2f(this->x + this->width, this->y);
		glTexCoord2f(1.0f, 0.0f); 
			glVertex2f(this->x + this->width, this->y + this->height);
		glTexCoord2f(0.0f, 0.0f); 
			glVertex2f(this->x    , this->y + this->height);
	glEnd();
	
	/* Bad things happen if we delete the texture before it finishes */
	glFinish();
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

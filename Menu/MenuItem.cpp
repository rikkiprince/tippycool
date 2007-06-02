#include "MenuItem.h"
//#define GL_BGRA	0x80e1

MenuItem::MenuItem(TTF_Font *font, int x, int y, int width, int height, int gotoMenu)
{
	this->gotoMenu = gotoMenu;
	this->selectable = false;
	this->outlined = false;
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->font = font;

	textColor.r = 0;
	textColor.g = 0;
	textColor.b = 0;
		
	buttonColor.r = 61;
	buttonColor.g = 184;
	buttonColor.b = 184;
		
	strncpy(text, " ", 255);
	text[255]='\0';
	preRenderFont();
}

MenuItem::MenuItem(char *t, TTF_Font *font, int gotoMenu, bool selectable, bool outlined)
{
	strncpy(text,t,255);
	text[255]='\0';

	this->font = font;
	this->pressed = false;
	this->outlined = outlined;
	this->gotoMenu = gotoMenu;
	this->selectable = selectable;

	int minx,maxx,miny,maxy,advance;
	TTF_GlyphMetrics(font,'M',&minx,&maxx,&miny,&maxy,&advance);

	int length = strlen(text);	
	this->width = (maxx-minx) * length;

	textColor.r = 0;
	textColor.g = 0;
	textColor.b = 0;
		
	buttonColor.r = 61;
	buttonColor.g = 184;
	buttonColor.b = 184;
		
	position.x = this->x;
	position.y = this->y;

	preRenderFont();

	this->width = position.w;
	this->height = position.h;
}

bool MenuItem::isSelectable()
{
	return selectable;
}

int MenuItem::getMenu()
{
	return gotoMenu;
}

int MenuItem::round(double x)
{
	return (int)(x + 0.5);
}

int MenuItem::nextpoweroftwo(int x)
{
	double logbase2 = log((double)x) / log((double)2);
	return round(pow((double)2,(double)ceil(logbase2)));
}

// http://www.gamedev.net/community/forums/topic.asp?topic_id=284259
void MenuItem::preRenderFont()
{
	SDL_Rect rect;
	int w,h;
	int oldw,oldh;
	SDL_Rect centre;
	
	/* Use SDL_TTF to render our text */
	initial = TTF_RenderText_Blended(font, text, textColor);

	oldw = initial->w;
	oldh = initial->h;
	
	/* Convert the rendered text to a known format */
	w = nextpoweroftwo(initial->w);
	h = nextpoweroftwo(initial->h);
	
	intermediary = SDL_CreateRGBSurface(0, w, h, 32, 
			0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);

	centre.x = (w - oldw)/2;
	centre.y = (h - oldh)/2;

	SDL_FillRect(intermediary, NULL, SDL_MapRGB(intermediary->format, buttonColor.r /* 255*/, buttonColor.g /* 255*/, buttonColor.b /* 255*/));

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
	position.w = w;//initial->w;
	position.h = h;//initial->h;
}

MenuItem::~MenuItem()
{
	/* Clean up */
	deleteTexture();
}

void MenuItem::setXY(int x, int y)
{
	this->x = x;
	this->y = y;
}

void MenuItem::deleteTexture()
{
	SDL_FreeSurface(initial);
	SDL_FreeSurface(intermediary);
	glDeleteTextures(1, &texture);
}

void MenuItem::render(bool highlighted)
{
	glColor3f(buttonColor.r, buttonColor.g, buttonColor.b);
	glBegin(GL_QUADS); 
		glVertex2f(x, y); 
		glVertex2f(x + width, y); 
		glVertex2f(x + width, y + height); 
		glVertex2f(x, y + height); 
    glEnd();

	textColor.r = 0;
	textColor.g = 0;
	textColor.b = 0;
		
	position.x = this->x;
	position.y = this->y;

	//SDL_Rect location={x, y, width, height};
	SDL_GL_RenderText();

	if(outlined)
	{
		glColor3f(0.0, 0.0, 0.0);
		glBegin(GL_LINE_LOOP); 
			glVertex2f(x, y-1); 
			glVertex2f(x + width, y); 
			glVertex2f(x + width, y + height);
			glVertex2f(x, y + height);  
		glEnd();
	}

	if(highlighted && selectable && !pressed)
	{
		glColor3ub(245, 122, 0);
		glBegin(GL_LINE_LOOP); 
			glVertex2f(x-1, y-2); 
			glVertex2f(x-1 + width + 2, y-1); 
			glVertex2f(x-1 + width+2, y-1 + height+2);
			glVertex2f(x-1, y-1 + height+2);  
		glEnd();
		glColor3f(0.0, 0.0, 0.0);
		glBegin(GL_LINE_LOOP); 
			glVertex2f(x-2, y-3); 
			glVertex2f(x-2 + width+4, y-2); 
			glVertex2f(x-2 + width+4, y-2 + height+4);
			glVertex2f(x-2, y-2 + height+4);
		glEnd();
	}
	glColor3f(1.0, 1.0, 1.0);
}

void MenuItem::setPressed(bool state)
{
	this->pressed = state;

	if(state && selectable)
	{
		buttonColor.r = 0;
		buttonColor.g = 184;
		buttonColor.b = 64;
		deleteTexture();
		preRenderFont();
	}
	else
	{
		buttonColor.r = 61;
		buttonColor.g = 184;
		buttonColor.b = 184;
		deleteTexture();
		preRenderFont();
	}
}

void MenuItem::SDL_GL_RenderText()
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

bool MenuItem::mouseUp(int x, int y)
{
	if((x >= this->x && x <= (this->x + width)) && (y >= this->y && y <= (this->y + height)))
	{
		return true;
	}
	else 
	{
		return false;
	}
}

bool MenuItem::mouseDown(int x, int y)
{
	if((x >= this->x && x <= (this->x + width)) && (y >= this->y && y <= (this->y + height)))
	{
		setPressed(true);
		return true;
	}
	else 
	{
		setPressed(false);
		return false;
	}
}

bool MenuItem::mouseMotion(int x, int y)
{
	if((x >= this->x && x <= (this->x + width)) && (y >= this->y && y <= (this->y + height)))
	{
		setPressed(true);
		return true;
	}
	else
	{
		setPressed(false);
		return false;
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

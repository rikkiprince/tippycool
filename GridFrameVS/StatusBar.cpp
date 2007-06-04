#include "StatusBar.h"

StatusBar::StatusBar(TTF_Font *font, bool top, int screenWidth, int screenHeight)
{
	this->font = font;
	this->top = top;
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;
	setText(font, " ");
}

StatusBar::~StatusBar()
{
	/* Clean up */
	deleteTexture();
}

void StatusBar::setText(TTF_Font *font, char *t)
{
	printf("settext: %s\n", t);
	strncpy(text,t,255);
	text[255]='\0';
	this->font = font;

	int minx,maxx,miny,maxy,advance;
	TTF_GlyphMetrics(font,'M',&minx,&maxx,&miny,&maxy,&advance);

	this->width = screenWidth;
	this->height = maxy-miny;
	this->x = 0;

	if(top)
	{
		this->y = 0;
	}
	else
	{
		this->y = screenHeight - height;
	}

	textColor.r = 0;
	textColor.g = 0;
	textColor.b = 0;
	
	buttonColor.r = 255;
	buttonColor.g = 153;
	buttonColor.b = 0;

	position.x = x;
	position.y = y;

	preRenderFont();

	this->width = position.w;
	this->height = position.h;
	if(top)
	{
		this->y = 0;
	}
	else
	{
		this->y = screenHeight - height;
	}
}

void StatusBar::render()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, screenWidth, 0, screenHeight);

	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity();
	//glTranslatef(0.375f, 0.375f, 0.f);
	//glScalef(0.5f, 0.5f, 0.f);
	glScalef(1.0f, -1.0f, 0.0f);
	glTranslatef(0.0f, -screenHeight, 0.0f);

	SDL_GL_RenderText();
}

void StatusBar::SDL_GL_RenderText()
{
	/* prepare to render our texture */
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	
	int startx = 0;

	/* Draw a quad at location */
	glBegin(GL_QUADS);
		/* Recall that the origin is in the lower-left corner
		   That is why the TexCoords specify different corners
		   than the Vertex coors seem to. */
		glTexCoord2f(0.0f, -1.0f); 
			glVertex2f(startx    , this->y);
		glTexCoord2f(1.0f, -1.0f); 
			glVertex2f(startx + this->width, this->y);
		glTexCoord2f(1.0f, 0.0f); 
			glVertex2f(startx + this->width, this->y + this->height);
		glTexCoord2f(0.0f, 0.0f); 
			glVertex2f(startx    , this->y + this->height);
	glEnd();
	
	/* Bad things happen if we delete the texture before it finishes */
	glFinish();
}

int StatusBar::round(double x)
{
	return (int)(x + 0.5);
}

int StatusBar::nextpoweroftwo(int x)
{
	double logbase2 = log((double)x) / log((double)2);
	return round(pow((double)2,(double)ceil(logbase2)));
}

void StatusBar::deleteTexture()
{
	SDL_FreeSurface(initial);
	SDL_FreeSurface(intermediary);
	glDeleteTextures(1, &texture);
}

void StatusBar::preRenderFont()
{
	//SDL_Rect rect;
	int w,h;
	int oldw,oldh;
	SDL_Rect centre;
	
	/* Use SDL_TTF to render our text */
	initial = TTF_RenderText_Blended(font, text, textColor);

	oldw = initial->w;
	oldh = initial->h;
	
	/* Convert the rendered text to a known format */
	w = nextpoweroftwo(screenWidth);//initial->w);
	h = nextpoweroftwo(initial->h);
	
	intermediary = SDL_CreateRGBSurface(0, w, h, 32, 
			0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);

	centre.x = (screenWidth / 2) - (oldw / 2);//(w - oldw)/2;
	centre.y = (h - oldh)/2;

	SDL_FillRect(intermediary,NULL, SDL_MapRGB(intermediary->format, buttonColor.r /* 255*/, buttonColor.g /* 255*/, buttonColor.b /* 255*/));

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
	this->position.w = w;//initial->w;
	this->position.h = h;//initial->h;
}

#include "Menu.h"
//#define GL_BGRA	0x80e1

Menu::Menu(char *t, int w, int h, TTF_Font *font)
{
	strncpy(title,t,255);
	title[255]='\0';
	screenWidth = w;
	screenHeight = h;
	selected = 0;
	this->font = font;
	this->instructions = false;

	int minx,maxx,miny,maxy,advance;
	TTF_GlyphMetrics(font,'M',&minx,&maxx,&miny,&maxy,&advance);
	int length = strlen(title);	
	this->width = screenWidth;
	this->height = maxy-miny;
	this->x = 0;
	this->y = 0;

	for(int i = 0; i < max; i++) 
	{
		items[i] = NULL;
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
}

void Menu::setInstructions(bool state)
{
	this->instructions = state;
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
void Menu::preRenderFont()
{
	SDL_Rect rect;
	int w,h;
	int oldw,oldh;
	SDL_Rect centre;
	
	/*buttonColor.r = 61;
	buttonColor.g = 184;
	buttonColor.b = 184;*/
	/* Use SDL_TTF to render our text */
	initial = TTF_RenderText_Blended(font, title, textColor);

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

Menu::~Menu()
{
}

bool Menu::add(MenuItem *item)
{
	for(int i = 0; i < max; i++) 
	{
		if(items[i] == NULL)
		{
			items[i] = item;
			return true;
		}
	}
	return false;
}

void Menu::remove(int index)
{
	items[index] = NULL;
}

void Menu::SDL_GL_RenderText()
{
	/* prepare to render our texture */
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
//	glColor3f(1.0f, 1.0f, 1.0f);
	
	//int midwidth = screenWidth / 2;
	int startx = 0;//midwidth - (this->width / 2);

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

void Menu::layout()
{
	int count = 0;
	for(int i = 0; i < max; i++)
	{
		if(items[i] != NULL)
		{
			count++;
		}
	}

	if(count > 10)
	{
		int numButtons = 5;//screenWidth / (items[0]->getWidth() * 2);
		int gapWidth = (screenWidth - (items[0]->getWidth() * numButtons)) / (numButtons + 2);
		int rowHeight = 20;//screenHeight / (((count / numButtons) * 2) + 1);
		int midWidth = screenWidth / 2;

		int tempx = 0 + gapWidth;
		int tempy = 50;//0 + rowHeight;

		for(int i = 0; i < max; i++)
		{
			if(items[i] != NULL)
			{
				items[i]->setXY(tempx, tempy+height+30);
				//items[i]->setHeight(rowHeight);
				if(i % numButtons == numButtons-1)
				{
					if (i == 9)
					{
						tempy = tempy + (rowHeight * 5);
						tempx = midWidth - (items[i+1]->getWidth() / 2);
					}
					else
					{
						tempy = tempy + rowHeight + rowHeight;
						tempx = 0 + gapWidth;
					}
				}
				else
				{
					tempx = tempx + items[i]->getWidth() + gapWidth;
				}
			}
		}
	}
	else
	{
		int rowHeight = (screenHeight-height-30) / (count + count + 1);
		int midWidth = screenWidth / 2;

		int tempx = 0;
		int tempy = 0;

		for(int i = 0; i < max; i++)
		{
			if(items[i] != NULL)
			{
				tempx = midWidth - (items[i]->getWidth() / 2);
				items[i]->setXY(tempx, tempy+height+30);
				//items[i]->setHeight(rowHeight);
				tempy = tempy + rowHeight + rowHeight;
			}
		}
	}
}

void Menu::render()
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

	if(instructions)
	{
		int tempx = 20;
		int tempy = height+10;
		this->add(new MenuItem(font, tempx, tempy, screenWidth-(tempx*1.5), screenHeight-tempy-85, -1));
	}
	/*glColor3f(buttonColor.r/255.0f, buttonColor.g/255.0f, buttonColor.b/255.0f);
		glBegin(GL_QUADS); 
			glVertex2f(tempx, tempy); 
			glVertex2f(tempx + (screenWidth-(tempx * 2)), tempy); 
			glVertex2f(tempx + (screenWidth-(tempx * 2)), tempy + screenHeight-tempy-55); 
			glVertex2f(tempx, tempy + screenHeight-tempy-55); 
		glEnd();
	}*/

	for(int i = max-1; i >=0 ; i--)
	{
		if(items[i] != NULL)
		{
			items[i]->render((i == selected));
		}
	}
}

void Menu::changeSelected(int change)
{
	int newSelected = selected + change;
	if(newSelected >= 0 && newSelected < max)
	{
		if(items[newSelected] != NULL && items[newSelected]->isSelectable())
		{
			selected = newSelected;
		}
	}
}

int Menu::enter()
{
	return items[selected]->getMenu();
}

int Menu::getLevel()
{
	return items[selected]->getLevel();
}

void Menu::mouseMotion(int x, int y)
{
	for(int i = 0; i < this->max; i++)
	{
		if(this->items[i] != NULL)
		{
			if(this->items[i]->mouseMotion(x, y))
			{
				if(items[i]->isSelectable())
				{
					selected = i;
				}
			}
		}
	}
}

bool Menu::mouseDown(int x, int y)
{
	for(int i = 0; i < this->max; i++)
	{
		if(this->items[i] != NULL)
		{
			//if(this->items[i]->mouseDown(x, y))
			//{
				if(items[i]->mouseDown(x, y))
				{
					return true;
				}
				//setPressed(state);
				//return items[i]->getMenu();
			//}
		}
	}
	return false;
	//return -1;
}

int Menu::mouseUp(int x, int y)
{
	for(int i = 0; i < this->max; i++)
	{
		if(this->items[i] != NULL)
		{
			if(this->items[i]->mouseUp(x, y))
			{
				return items[i]->getMenu();
			}
		}
	}
	return -1;
}
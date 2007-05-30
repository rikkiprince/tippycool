#include "MenuItem.h";

class Menu
{
protected:
	static const int max = 10;
	MenuItem *items[10];
	int selected;
	int screenWidth, screenHeight;
	char title[256];
public:
	Menu(char *t, int w, int h);
	~Menu();
	bool add(MenuItem *item);
	void remove(int index);
	void layout();
	void render();
	bool click(int x, int y);
};

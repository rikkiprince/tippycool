// This file may be redistributed and modified only under the terms of
// the GNU General Public License (See COPYING for details).
// Copyright (C) 2000,2004 Alistair Riddoch

#include "main.h"
#include "headers.h"
#include "StatusBar.h"
#include "MenuSystem.h";
#include "MenuItem.h";
#include "Menu.h";
#include "fmod.h"

bool testing = false;

int rotate_up=0, rotate_side=0;

#define GAME			(0)
#define SELECTLEVEL		(1)
#define INSTRUCTIONS	(2)
#define PAUSE			(3)
#define LEVELCOMPLETED	(4)
#define GAMECOMPLETED	(5)
#define RESET			(6)
#define CONTINUE		(7)
#define BLANK			(8)
#define STARTMENU		(9)
#define EXITGAME		(10)

// Constants

static const int screen_width = 600;
static const int screen_height = 400;

StatusBar *topsb, *bottomsb;
MenuSystem *ms;
Menu *game, *selectLevel, *instructions, *pause, *completeLevel, *completeGame, *blank;
TTF_Font *font24, *font20, *font16, *font14;

// Number of squares in the grid. The number of points is this number +1.
static const int grid_width = 4;
static const int grid_height = 4;
static const int grid_depth = 4;

// Number of milliseconds between steps in the game model.
static const int step_time = 100;

// Variables that store the game state

static int block_x = 2;
static int block_y = 2;
static int block_z = 0;

int score = -1;
int currentLevel = -1;
bool paused = false;

// Structure to hold the properties of a single square on the grid.
// If you want to add more information to the grid, add new members here.
typedef struct block_properties {
    bool block;
} BlockProperties;


// Current blocks on the grid, stored as true if there is a block at the
// given localtion.
BlockProperties properties[grid_width][grid_height][grid_depth] = 
{
	// first floor									// second floor						// third floor						// fourth floor
	// back   mid    mid    front					// back   mid     mid     front		// back   mid    mid    front		// back   mid    mid    front
	{{{true},{true},{true},{true}},		/*left*/	{{false},{false},{false},{true}},	{{false},{false},{false},{true}},	{{false},{false},{false},{true}}},
	{{{false},{false},{false},{false}},	/*centre*/	{{false},{false},{false},{false}},	{{false},{false},{false},{false}},	{{false},{false},{false},{true}}},
	{{{false},{false},{false},{false}},	/*centre*/	{{false},{false},{false},{false}},	{{false},{false},{false},{false}},	{{false},{false},{false},{true}}},
	{{{true},{false},{false},{false}},	/*right*/	{{true},{false},{false},{false}},	{{true},{false},{false},{false}},	{{true},{true},{true},{true}}},
};

Level *level;
//Camera *camera;

AbstractBlock* blocks[grid_width][grid_height][grid_depth];

GLuint texture;

FSOUND_SAMPLE *menuclick = NULL;
FSOUND_SAMPLE *buttonblockclick = NULL;
FSOUND_SAMPLE *getstar = NULL;
FSOUND_SAMPLE *goal = NULL;
FSOUND_SAMPLE *blockmove = NULL;
FSOUND_SAMPLE *shower = NULL;
FSOUND_SAMPLE *hitspikes = NULL;

bool init_audio() {	
	return FSOUND_Init(44100, 32, 0);
}

void uninit_audio() {
	FSOUND_Sample_Free(menuclick);
	FSOUND_Sample_Free(buttonblockclick);
	FSOUND_Sample_Free(getstar);
	FSOUND_Sample_Free(goal);
	FSOUND_Sample_Free(blockmove);
	FSOUND_Sample_Free(shower);
	FSOUND_Sample_Free(hitspikes);
	FSOUND_Close();
}

void load_audio() {
	menuclick = FSOUND_Sample_Load (0, "audio//menuclick.wav", 0, 0, 0); //http://www.partnersinrhyme.com/pirsounds/WEB_DESIGN_SOUNDS_WAV/BUTTONS.shtml
	buttonblockclick = FSOUND_Sample_Load (0, "audio//buttonblockclick.wav", 0, 0, 0); //http://www.wavsource.com/sfx/sfx.htm
	getstar = FSOUND_Sample_Load (0, "audio//getstar.wav", 0, 0, 0); //http://www.wavsource.com/sfx/sfx.htm
	goal = FSOUND_Sample_Load (0, "audio//goal.wav", 0, 0, 0); //http://www.wavsource.com/sfx/sfx.htm
	blockmove = FSOUND_Sample_Load (0, "audio//blockmove.wav", 0, 0, 0); //http://www.a1freesoundeffects.com/button.html
	shower = FSOUND_Sample_Load (0, "audio//shower.wav", 0, 0, 0); //http://www.a1freesoundeffects.com/button.html
	hitspikes = FSOUND_Sample_Load (0, "audio//hitspikes.wav", 0, 0, 0); //http://www.wavsource.com/sfx/sfx2.htm
	FSOUND_SetSFXMasterVolume(50);
}

void play_audio(int channel, FSOUND_SAMPLE *sound) {
	FSOUND_PlaySound (channel, sound);
}

void init_grid()
{
	char *filename = ".\\levels\\test\\level2.ini";
	//char *filename = ".\\levels\\level8.ini";
	level = new Level(filename);
//	camera = new Camera();
	texture = LoadGLTexture("texture.bmp");
/*
	//Level level = new Level();

	for(int k=0; k<grid_depth; k++)
	{
		for(int j=0; j<grid_height; j++)
		{
			for(int i=0; i<grid_width; i++)
			{
				if(properties[i][j][k].block)
					blocks[i][j][k] = new NormalBlock(texture);
				else
					blocks[i][j][k] = NULL;
			}
		}
	}*/

/*blocks[0][0][0] = new NormalBlock(texture);
	blocks[3][0][0] = new NormalBlock(texture);
	blocks[3][1][0] = new NormalBlock(texture);
	blocks[3][2][0] = new NormalBlock(texture);
	blocks[3][3][0] = new NormalBlock(texture);
	blocks[0][0][1] = new NormalBlock(texture);
	blocks[3][3][1] = new NormalBlock(texture);
	blocks[0][0][2] = new NormalBlock(texture);
	blocks[3][3][2] = new NormalBlock(texture);
	blocks[0][0][3] = new NormalBlock(texture);
	blocks[0][1][3] = new NormalBlock(texture);
	blocks[0][2][3] = new NormalBlock(texture);
	blocks[0][3][3] = new NormalBlock(texture);
	blocks[1][3][3] = new NormalBlock(texture);
	blocks[2][3][3] = new NormalBlock(texture);
	blocks[3][3][3] = new NormalBlock(texture);*/

	
	/*for(int k=0; k<grid_depth; k++)
	{
		for(int j=0; j<grid_height; j++)
		{
			for(int i=0; i<grid_width; i++)
			{
				if(properties[i][j][k].block)
				{
					//printf("blocks[%d][%d][%d] = new NormalBlock(texture);\n", i, j, k);
					printf("[%d,%d,%d]\ntype=Normal\n\n", i, j, k);
				}
			}
		}
	}*/
}

// Flag used to inform the main loop if the program should now terminate.
// Set this to true if its done.
static bool program_finished = false;

// Calculated frames per second to display. Very useful feedback when
// debugging graphics performance problems.
int average_frames_per_second;

// Texture handles for the texture used to handle printing text on the
// screen.
GLuint textTexture;
GLuint textBase;


bool mouse_down = false;
int mouse_down_x = 0;
int mouse_down_y = 0;

bool mouse_left = false;
bool mouse_right = false;



/*typedef struct
{
	GLfloat r;
	GLfloat g;
	GLfloat b;
	GLfloat a;
} RGBA;
typedef struct
{
	GLfloat x;
	GLfloat y;
	GLfloat z;
	GLfloat w;
} XYZW;*/

typedef struct
{
	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat position[4];
} LIGHT;

#define NUM_LIGHTS	(2)
LIGHT light[NUM_LIGHTS] =	{
								{{0.5f, 0.5f, 0.5f, 1.0f}, {1.0f, 0.98f, 0.8f, 1.0f}, {-5.0f, -5.0f, -5.0f, 1.0f}},
								{{0.5f, 0.5f, 0.5f, 1.0f}, {1.0f, 0.98f, 0.8f, 1.0f}, {+5.0f, +5.0f, +5.0f, 1.0f}},
							};






void init_lighting()
{
	glEnable(GL_LIGHTING);

	glEnable(GL_COLOR_MATERIAL);

	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	for(int i=0; i<NUM_LIGHTS; i++)
	{
		glLightfv(GL_LIGHT1+i, GL_AMBIENT, light[i].ambient);
		glLightfv(GL_LIGHT1+i, GL_DIFFUSE, light[i].diffuse);
		glLightfv(GL_LIGHT1+i, GL_POSITION, light[i].position);
		glEnable(GL_LIGHT1+i);
	}

	//glEnableClientState(GL_NORMAL_ARRAY);
}

void render_lights()
{
	for(int i=0; i<NUM_LIGHTS; i++)
	{
		glColor3fv(light[i].diffuse);
		glBegin(GL_LINES);
			// X
			glVertex3f(light[i].position[0]-1, light[i].position[1], light[i].position[2]);
			glVertex3f(light[i].position[0]+1, light[i].position[1], light[i].position[2]);
			
			// Y
			glVertex3f(light[i].position[0], light[i].position[1]-1, light[i].position[2]);
			glVertex3f(light[i].position[0], light[i].position[1]+1, light[i].position[2]);
			
			// Z
			glVertex3f(light[i].position[0], light[i].position[1], light[i].position[2]-1);
			glVertex3f(light[i].position[0], light[i].position[1], light[i].position[2]+1);
		glEnd();
	}
}

void move_lights()
{
	glEnable(GL_LIGHTING);

	for(int i=0; i<NUM_LIGHTS; i++)
	{
		glLightfv(GL_LIGHT1+i, GL_POSITION, light[i].position);
	}
}








libmd3_file *model = 0;

bool init_models()
{
	if((model = load_model("models/testbox4.md3")) == NULL) return false;

	return true;
}











// Initialise the graphics subsystem. This is pretty much boiler plate
// code with very little to worry about.
bool init_graphics()
{
    // Initialise SDL
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0) {
        // std::cerr << "Failed to initialise video" << std::endl << std::flush;
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    SDL_Surface * screen;

	SDL_WM_SetCaption("Title-Bar", "Icon name");
	SDL_WM_SetIcon(SDL_LoadBMP("icon.bmp"), NULL);

    // Create the window
    screen = SDL_SetVideoMode(screen_width, screen_height, 0, SDL_OPENGL);
    if (screen == NULL) {
        // std::cerr << "Failed to set video mode" << std::endl << std::flush;
        SDL_Quit();
        return false;
    }

    // Setup the viewport transform
    glViewport(0, 0, screen_width, screen_height);

    // Enable vertex arrays
    glEnableClientState(GL_VERTEX_ARRAY);
    // Texture coordinate arrays well need to be enabled _ONLY_ when using
    // texture coordinates from an array, and disabled afterwards.
    // glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    // Set the colour the screen will be when cleared - black
    //glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearColor(0.6f, 0.85f, 0.917f, 0.0);

    // Initialise the texture used for rendering text
    glGenTextures(1, &textTexture);
    glBindTexture(GL_TEXTURE_2D, textTexture);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexImage2D(GL_TEXTURE_2D, 0, texture_font_internalFormat,
                 texture_font_width, texture_font_height, 0,
                 texture_font_format, GL_UNSIGNED_BYTE, texture_font_pixels);
    if (glGetError() != 0) {
        return false;
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    textBase = glGenLists(256);
    float vertices[] = { 0, 0, 16, 0, 16, 16, 0, 16 };
    glVertexPointer(2, GL_FLOAT, 0, vertices);
    for(int loop=0; loop<256; loop++) {
        float cx=(float)(loop%16)/16.0f;      // X Position Of Current Character
        float cy=(float)(loop/16)/16.0f;      // Y Position Of Current Character

        float texcoords[] = { cx, 1-cy-0.0625f,
                              cx+0.0625f, 1-cy-0.0625f,
                              cx+0.0625f, 1-cy,
                              cx, 1-cy };

        glNewList(textBase+loop,GL_COMPILE);   // Start Building A List

        glTexCoordPointer(2, GL_FLOAT, 0, texcoords);
        glDrawArrays(GL_QUADS, 0, 4);

        glTranslated(10,0,0);                  // Move To The Right Of The Character
        glEndList();                           // Done Building The Display List
    }
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	init_lighting();

    return true;
}

// Clear the grid state.
void clear()
{
    /*for(int i = 0; i < grid_width; ++i) {
        for(int j = 0; j < grid_height; ++j) {
			for(int k = 0; k < grid_depth; ++k) {
	            properties[i][j][k].block = false;
			}
        }
    }*/
}

// Print a text string on the screen at the current position.
void gl_print(const char * str)
{
    glPushMatrix();
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glBindTexture(GL_TEXTURE_2D, textTexture);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glListBase(textBase-32);
    glCallLists(strlen(str),GL_BYTE,str);
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glPopMatrix();
}

void setup()
{
	init_grid();
    // Clear the block store
    clear();

}

//load font for menus
char fontpath[] = "font//arial.ttf";

void init_fonts()
{
	if(TTF_Init()==-1) {
		printf("TTF_Init: %s\n", TTF_GetError());
		exit(2);
	}

	if(!(font24 = TTF_OpenFont(fontpath, 24))) {
		printf("Error loading font: %s", TTF_GetError());
		//return 1;
	}

	if(!(font20 = TTF_OpenFont(fontpath, 20))) {
		printf("Error loading font: %s", TTF_GetError());
		//return 1;
	}

	if(!(font16 = TTF_OpenFont(fontpath, 16))) {
		printf("Error loading font: %s", TTF_GetError());
		//return 1;
	}

	if(!(font14 = TTF_OpenFont(fontpath, 14))) {
		printf("Error loading font: %s", TTF_GetError());
		//return 1;
	}
}

void uninit_fonts()
{
	// free the font
	TTF_CloseFont(font14);
	TTF_CloseFont(font16);
	TTF_CloseFont(font20);
	TTF_CloseFont(font24);
	font14=NULL;
	font16=NULL;
	font20=NULL;
	font24=NULL;
	TTF_Quit();
}

void createTopStatusBar()
{
	init_fonts();
	topsb = new StatusBar(font20, true, screen_width, screen_height);
}

void createBottomStatusBar()
{
	init_fonts();
	bottomsb = new StatusBar(font20, false, screen_width, screen_height);
}

void setStatusBarText(StatusBar *sb, char *text)
{
	sb->setText(font20, text);
}

void createGameMenuSystem()
{
	init_fonts();

	//-1 - No Menu
	//0 - Game Menu
	//1 - Level Menu
	//2 - Instructions Menu
	//3 - Pause Menu
	//4 - Completed Level Menu
	//5 - Completed Game
	//6 - Reset
	//7 - Unpause
	//8 - Blank Menu
	//9 - Start Game
	//10 - Exit Game

	game = new Menu("Main Menu", screen_width, screen_height, font20);
	game->add(new MenuItem("Start", font20, STARTMENU, true, true, 1));
	game->add(new MenuItem("Select Level", font20, SELECTLEVEL, true, true));
	game->add(new MenuItem("Instructions", font20, INSTRUCTIONS, true, true));
	game->add(new MenuItem("Exit", font20, EXITGAME, true, true));
	game->layout();

	selectLevel = new Menu("Level Menu", screen_width, screen_height, font20);
	selectLevel->add(new MenuItem("Level 1", font16, STARTMENU, true, true, 1));
	selectLevel->add(new MenuItem("Level 2", font16, STARTMENU, true, true, 2));
	selectLevel->add(new MenuItem("Level 3", font16, STARTMENU, true, true, 3));
	selectLevel->add(new MenuItem("Level 4", font16, STARTMENU, true, true, 4));
	selectLevel->add(new MenuItem("Level 5", font16, STARTMENU, true, true, 5));
	selectLevel->add(new MenuItem("Level 6", font16, STARTMENU, true, true, 6));
	selectLevel->add(new MenuItem("Level 7", font16, STARTMENU, true, true, 7));
	selectLevel->add(new MenuItem("Level 8", font16, STARTMENU, true, true, 8));
	selectLevel->add(new MenuItem("Level 9", font16, STARTMENU, true, true, 9));
	selectLevel->add(new MenuItem("Level 10", font16, STARTMENU, true, true, 10));
	selectLevel->add(new MenuItem("Back to Main Menu", font20, GAME, true, true));
	selectLevel->layout();

	instructions = new Menu("Instructions Menu", screen_width, screen_height, font20);
	instructions->add(new MenuItem(" ", font16, -1, false, false));
	instructions->add(new MenuItem("TextTextText", font16, -1, false, false));
	instructions->add(new MenuItem("TextTextText", font16, -1, false, false));
	instructions->add(new MenuItem("TextTextText", font16, -1, false, false));
	instructions->add(new MenuItem("TextTextText", font16, -1, false, false));
	instructions->add(new MenuItem(" ", font16, -1, false, false));
	instructions->add(new MenuItem(" ", font16, -1, false, false));
	instructions->add(new MenuItem("Back to Main Menu", font20, GAME, true, true));
	instructions->setInstructions(true);
	instructions->layout();

	completeLevel = new Menu("Level Complete", screen_width, screen_height, font20);
	completeLevel->add(new MenuItem("Continue", font20, CONTINUE, true, true, currentLevel+1));
	completeLevel->add(new MenuItem("Retry Level", font20, RESET, true, true, currentLevel));
	completeLevel->add(new MenuItem("Select Level", font20, SELECTLEVEL, true, true));
	completeLevel->add(new MenuItem("Back to Main Menu", font20, GAME, true, true));
	completeLevel->layout();

	pause = new Menu("Game Paused", screen_width, screen_height, font20);
	pause->add(new MenuItem("Resume Game", font20, CONTINUE, true, true));
	pause->add(new MenuItem("Restart Level", font20, RESET, true, true, currentLevel));
	pause->add(new MenuItem("Instructions", font20, INSTRUCTIONS, true, true));
	pause->add(new MenuItem("Back to Main Menu", font20, GAME, true, true));
	pause->layout();

	completeGame = new Menu("Game Completed!", screen_width, screen_height, font20);
	completeGame->add(new MenuItem("Retry Level", font20, RESET, true, true, currentLevel));
	completeGame->add(new MenuItem("Select Level", font20, SELECTLEVEL, true, true));
	completeGame->add(new MenuItem("Back to Main Menu", font20, GAME, true, true));
	completeGame->add(new MenuItem("Exit", font20, EXITGAME, true, true));
	completeGame->layout();

	blank = new Menu(" ", screen_width, screen_height, font20);
	blank->layout();

	ms = new MenuSystem();
	ms->add(game);
	ms->add(selectLevel);
	ms->add(instructions);
	ms->add(completeLevel);
	ms->add(pause);
	ms->add(completeGame);
	ms->add(blank);
}

void start(int l)
{
	printf("Level: %d\n", l);
//	setup();
	currentLevel = l;
	char filename[256];
	printf("Level Loaded");
	sprintf(filename, ".\\levels\\level%d.ini", l);
	level = new Level(filename);

	if(ms->getSelected() != BLANK)
	{
		ms->enter(BLANK);
	}

	createTopStatusBar();
	char message[256];
	sprintf(message, "Level %d", currentLevel);
	setStatusBarText(topsb, message);


	createBottomStatusBar();
	setStatusBarText(bottomsb, "Game Hints");
}



float camera_rotation = 0.0f;
float camera_rotation_x = 90.0f;
float camera_rotation_y = 0.0f;
float camera_dist = -10.0f;

void camera_pos()
{
    // Set up the modelview
    glMatrixMode(GL_MODELVIEW);
    // Reset the camera
    glLoadIdentity();

	if(testing)
	{
		// Move the camera 20 units from the objects
		glTranslatef(0.0f, 0.0f, camera_dist);
		// Add a little camera movement
		//glRotatef(10, sin(camera_rotation), cos(camera_rotation), 0.0f);
		glRotatef(camera_rotation_x, 0.0, 1.0, 0.0);
		glRotatef(camera_rotation_y, 1.0, 0.0, 0.0);
		//glRotatef(-75, 1,0,0);
		//glRotatef(-45, 0,1,0);
		level->updateCameraPosition(false);
	}
	else
	{
		/*gluLookAt(	grid_width * 1.0, 10.0, grid_height * 1.0,
					0,0,0,
					0,0,1
				 );*/
		//level->updateCameraPosition(camera);
		level->updateCameraPosition(true);
		//camera->updateLookAt();
	}
}

void render_axes()
{
	glBegin(GL_LINES);
		glColor3f(1.0f, 0.0f, 0.0f);	// R
		glVertex3f(0.0f, 0.0f, 0.0f);	// X
		glVertex3f(1.0f, 0.0f, 0.0f);

		glColor3f(0.0f, 1.0f, 0.0f);	// G
		glVertex3f(0.0f, 0.0f, 0.0f);	// Y
		glVertex3f(0.0f, 1.0f, 0.0f);

		glColor3f(0.0f, 0.0f, 1.0f);	// B
		glVertex3f(0.0f, 0.0f, 0.0f);	// Z
		glVertex3f(0.0f, 0.0f, 1.0f);
	glEnd();
}

void render_scene()
{
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Enable the depth test
    glEnable(GL_DEPTH_TEST);

    // Set the projection transform
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (float)screen_width/screen_height, 1.f, 100.f);

    // Set the camera position
    camera_pos();

    // Draw the scene
    //draw_grid();
	level->render();

	move_lights();

//	if(testing)
	{
		render_axes();
		render_lights();
		level->renderCamera();
	}
/*	glRotatef(rotate_up, 1.0, 0.0, 0.0);
	glRotatef(rotate_side, 0.0, 1.0, 0.0);
	glTranslatef(-0.5, -0.5, -0.5);
	draw_textured_unit_cube(texture);*/
}

// Draw any text output and other screen oriented user interface
// If you want any kind of text or other information overlayed on top
// of the 3d view, put it here.
void render_interface()
{
    char buf[256];

    // Set the projection to a transform that allows us to use pixel
    // coordinates.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, screen_width, 0, screen_height, -800.0f, 800.0f);

    // Set up the modelview
    glMatrixMode(GL_MODELVIEW);
    // Reset the camera
    glLoadIdentity();

    // Disable the depth test, as its not useful when rendering text
    glDisable(GL_DEPTH_TEST);

    // Print the number of frames per second. This is essential performance
    // information when developing 3D graphics.

    // Use glTranslatef to go to the screen coordinates where we want the
    // text. The origin is the bottom left by default in OpenGL.
    glTranslatef(5.f, 5.f, 0);
    glColor3f(1.f, 1.f, 1.f);
    sprintf(buf, "FPS: %d", average_frames_per_second);
    gl_print(buf);

	if(ms != NULL)
	{
		ms->render();
	}
	if(topsb != NULL)
	{
		topsb->render();
	}
	if(bottomsb != NULL)
	{
		bottomsb->render();
	}
}

// Handle a mouse click. Call this function with the screen coordinates where
// the mouse was clicked, in OpenGL format with the origin in the bottom left.
// This function exactly mimics rendering a scene, and then detects which
// grid location of the scene were under the mouse pointer when the click
// occured.
void mouse_click(unsigned int x, unsigned int y)
{
/*    GLuint selectBuf[512];
    GLfloat square_vertices[] = { 0.f, 0.f, 0.f, 1.f, 0.f, 0.f,
                                  1.f, 1.f, 0.f, 0.f, 1.f, 0.f };

    // Put OpenGL into Selection mode rather than Render mode. This disables
    // rendering to the window.
    glSelectBuffer(512,selectBuf);
    glRenderMode(GL_SELECT);

    
    // This is just like setting up the projection for normal rendering
    // except we use gluPickMatrix to specify the portion of the screen
    // we are interested in - the 1 pixel square under the mouse pointer.
    {
        // Set the projection transform
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT,viewport);
        gluPickMatrix(x, y, 1, 1, viewport);
        gluPerspective(45, (float)screen_width/screen_height, 1.f, 100.f);

        // Set the camera position
        camera_pos();
    }

    // Each thing we render will have a numerical name that we must later use
    // to identify which thing we rendered was under the mouse pointer.
    // The names are pushed onto a stack, so a thing can have more than
    // one. We will just have one name per thing, so we start by pushing a
    // token name onto the stack.
    glInitNames();
    glPushName(0);

    // Here we will draw the grid exactly as it was drawn when rendering to
    // the screen, except we draw a quad on each square rather than just
    // drawing lines. If you want to detect clicking on other things, you
    // will need to modify the code here.
    glTranslatef(-(float)grid_width/2.0f, -(float)grid_height/2.0f, 0.0f);

    glVertexPointer(3, GL_FLOAT, 0, square_vertices);

    for(int i = 0; i < grid_width; ++i) {
        for(int j = 0; j < grid_height; ++j) {
            // Before we render each grid square, we specify the name by
            // loading it into the top of the name stack. The name
            // is derived from the coordinates of the square, so that later
            // when we get a name back, we can tell which square it corresponds
            // to.
            glLoadName(i + j * grid_width);
            glDrawArrays(GL_QUADS, 0, 4);
            glTranslatef(0.0f, 1.0f, 0.0f);
        }
        glTranslatef(1.0f, -grid_height, 0.0f);
    }

    // We have now finished with the name stack, so we empty it.
    glPopName();

    // Put OpenGL back into rendering mode. This has the side effect of
    // returning how many hits we got in the selection code above.
    int hits = glRenderMode(GL_RENDER);

    // std::cout << "Got " << hits << " hits" << std::endl << std::flush;

    // If we got no hits, the user clicked on empty space.
    if (hits == 0) {
        return;
    }

    // This code searches through the results buffer to find what hits we
    // got.

    // ptr points to the beginning of the results
    GLuint * ptr = &selectBuf[0];
    GLuint minDepth = UINT_MAX, noNames = 0;
    GLuint * namePtr = 0;
    for (int i = 0; i < hits; i++) {
        int names = *(ptr++);
        // std::cout << "{" << *ptr << "}";
        // Check if this hit is closer to the viewer than the last one we
        // processed. If this test passes, then this is the closest hit
        // so far, so we record the necessary details.
        if (*ptr < minDepth) {
            noNames = names;
            minDepth = *ptr;
            namePtr = ptr + 2;
        }
        ptr += (names + 2);
    }
    // Once this loop is complete, we have the details of the nearest hit
    // stored, which is the thing we rendered that was closest to the viewer
    // under the mouse pointer. This should be the thing the player was aiming
    // to click on when they clicked.

    // noNames = Number of names our hit has
    // namePtr = Pointer to list of names

    // Extract the X and Y coords of the clicked square from the hit name
    GLuint hitName = *namePtr;
    int hit_x = hitName % grid_width;
    int hit_y = hitName / grid_width;

    // Place or remove a block on the square the user clicked.
    if (hit_x < grid_width && hit_y < grid_height) {
        properties[hit_x][hit_y].block = !properties[hit_x][hit_y].block;
    }*/
}

// This function is called every step_time milliseconds. In many games you
// will want to use this function to update the game state. For example in
// a Tetris game, this would be the place where the falling blocks were
// moved down the grid.
void step()
{
	level->update();
}

void unpause()
{
	paused = false;
	ms->enter(BLANK);
	setStatusBarText(bottomsb, " ");
}

void handleEvents()
{
    SDL_Event event;

	while (SDL_PollEvent(&event))
	{
        switch (event.type)
		{
            case SDL_QUIT:
                // The user closed the window
                program_finished = true;
                break;
            case SDL_KEYDOWN:
                // We have a keypress
                if ( event.key.keysym.sym == SDLK_p ) {
					if(ms->getSelected() == BLANK)
					{
						paused = !paused;
					}
					if(paused)
					{
						setStatusBarText(bottomsb, "Game Paused");
					}
					else
					{
						unpause();
					}
                }
                if ( event.key.keysym.sym == SDLK_ESCAPE )
				{
                    // quit
                    program_finished = true;
                }
                if ( event.key.keysym.sym == SDLK_UP )
				{
                    if ((block_y < (grid_height-1)) && !properties[block_x][block_y + 1][block_z].block) {
                        ++block_y;
                    }
					//rotate_up+=10;
					if(ms->getSelected() == BLANK)
					{
						level->up();
					}
					else
					{
						ms->changeSelected(-1);
					}
				}
                if ( event.key.keysym.sym == SDLK_DOWN )
				{
                    // Move block down
                    if ((block_y > 0) && !properties[block_x][block_y - 1][block_z].block) {
                        --block_y;
                    }   
					//rotate_up-=10;
					if(ms->getSelected() == BLANK)
					{
						level->down();
					}
					else
					{
						ms->changeSelected(1);
					}
                }
                if ( event.key.keysym.sym == SDLK_LEFT )
				{
                    // Move block left
                    if ((block_x > 0) && !properties[block_x - 1][block_y][block_z].block) {
                        --block_x;
                    }   
					//rotate_side-=10;
					if(ms->getSelected() == BLANK)
					{
						level->left();
					}
					else
					{
						ms->changeSelected(-1);
					}
                }
                if ( event.key.keysym.sym == SDLK_RIGHT )
				{
                    // Move block right
                    if ((block_x < (grid_width-1)) && !properties[block_x + 1][block_y][block_z].block) {
                        ++block_x;
                    }   
					//rotate_side+=10;
					if(ms->getSelected() == BLANK)
					{
						level->right();
					}
					else
					{
						ms->changeSelected(1);
					}
                }
                if ( event.key.keysym.sym == SDLK_RETURN ) {
					if(ms->getSelected() != BLANK)
					{
						int num = ms->mouseUp(event.button.x, event.button.y);
						printf("num is: %d", num);
						if(num > -1)
						{
							play_audio(0,  menuclick);
						}
						if(num == STARTMENU)
						{
							paused = false;
							int newLevel = ms->getLevel();
							if(newLevel > 0)
							{
								start(newLevel);
							}
							else
							{
								start(1);
							}
						}
						else if(num == RESET)
						{
							paused = false;
							ms->enter(BLANK);
							level->reset();
						}
						else if(num == CONTINUE)
						{
							paused = false;
							ms->enter(BLANK);
						}
						else if(num == EXITGAME)
						{
							Sleep(100);
							program_finished = true;
						}
						else
						{
							ms->enter(num);
						}
					}
                }
				if(event.key.keysym.sym == SDLK_SPACE)
				{
					if(ms->getSelected() == BLANK)
					{
						level->stop();
					}
				}
				if(event.key.keysym.sym == SDLK_t)
				{
					if(ms->getSelected() == BLANK)
					{
						testing = !testing;
					}
				}
				if(event.key.keysym.sym == SDLK_q)
				{
					if(ms->getSelected() == BLANK)
					{
						level->turnLeft();
					}
				}
				if(event.key.keysym.sym == SDLK_w)
				{
					if(ms->getSelected() == BLANK)
					{
						level->turnRight();
					}
				}
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
				{
					if(ms->getSelected() == BLANK)
					{
						mouse_click(event.button.x, screen_height - event.button.y);
						mouse_down = true;
						mouse_down_x = event.button.x;
						mouse_down_y = event.button.y;

						mouse_left = true;
					}
					else
					{
						ms->mouseDown(event.button.x, event.button.y);
					}
                }
                if (event.button.button == SDL_BUTTON_RIGHT)
				{
					if(ms->getSelected() == BLANK)
					{
						mouse_right = true;
					}
                }
				else if(event.button.button == 4)
				{
					if(ms->getSelected() == BLANK)
					{
						if(testing)
							camera_dist++;
					}
				}
				else if(event.button.button == 5)
				{
					if(ms->getSelected() == BLANK)
					{
						if(testing)
							camera_dist--;
					}
				}
                break;
			case SDL_MOUSEBUTTONUP:
				if(event.button.button == SDL_BUTTON_LEFT)
				{
					if(ms->getSelected() == BLANK)
					{
						mouse_down = false;
						printf("x=%d; y=%d;\n", event.button.x, event.button.y);
							level->setAcceleration(0, 0);

						mouse_left = false;
						if(mouse_right)
							level->turnRight();
					}
					else
					{
						int num = ms->mouseUp(event.button.x, event.button.y);
						printf("num is: %d", num);
						if(num > -1)
						{
							play_audio(0,  menuclick);
							if(num == STARTMENU)
							{
								paused = false;
								printf("selected is: %d\n", ms->getSelected());
								int newLevel = ms->getLevel();
								printf("newlevel is: %d\n", newLevel);
								if(newLevel > 0)
								{
									start(newLevel);
								}
								else
								{
									start(1);
								}
							}
							else if(num == RESET)
							{
								paused = false;
								ms->enter(BLANK);
								level->reset();
							}
							else if(num == CONTINUE)
							{
								paused = false;
								ms->enter(BLANK);
							}
							else if(num == EXITGAME)
							{
								Sleep(100);
								program_finished = true;
							}
							else
							{
								ms->enter(num);
							}
						}
					}
				}
                if (event.button.button == SDL_BUTTON_RIGHT)
				{
					if(ms->getSelected() == BLANK)
					{
						mouse_right = false;
						if(mouse_left)
							level->turnLeft();
					}
                }
				break;
			case SDL_MOUSEMOTION:
				//printf("%d %d %d %d\n", event.motion.x, event.motion.xrel, event.motion.y, event.motion.yrel);
				if(ms->getSelected() == BLANK)
				{
					if(mouse_down)
					{
						if(testing)
						{
							//camera_rotation += (event.motion.x - mouse_down_x) * M_PI/180;
							camera_rotation_x += 15 * (event.motion.xrel) * M_PI/180;
							camera_rotation_y += 15 * (event.motion.yrel) * M_PI/180;
						}
						else
						{
							GLfloat m = 0.05;
							level->setAcceleration((event.button.y - mouse_down_y)*m, (event.button.x - mouse_down_x)*m);
						}
					}
				}
				else
				{
					ms->mouseMotion(event.motion.x, event.motion.y);
				}
            default:
                break;
        }
    }
}

// The main program loop function. This does not return until the program
// has finished.
void loop()
{
    int elapsed_time = SDL_GetTicks();
    int last_step = elapsed_time;
    int frame_count = 0;

    // This is the main program loop. It will run until something sets
    // the flag to indicate we are done.
    while (!program_finished) {
        // Check for events
        handleEvents();

        ++frame_count;

        // Get the time and check if a complete time step has passed.
        // For step based games like Tetris, this is used to update the
        // the game state
        const int ticks = SDL_GetTicks();
        if ((ticks - last_step) > step_time) {
            last_step = ticks;
            average_frames_per_second = frame_count;
            frame_count = 0;
            //step();
			if(!paused) {
				step();
				if(level->isCompleted())
				{
					if(currentLevel == 10)
					{
						ms->enter(GAMECOMPLETED);
					}
					else
					{
						ms->enter(LEVELCOMPLETED);
					}
				}
			}
			else
			{
				if(ms->getSelected() == BLANK)
				{
					ms->enter(PAUSE);
				}
			}
        }

        // Calculate the time in seconds since the last frame
        // For a real time program this would be used to update the game state
        float delta = (ticks - elapsed_time) / 1000.0f;
        elapsed_time = ticks;

        // Update the rotation on the camera
        /*camera_rotation += delta;
        if (camera_rotation > (2 * M_PI)) {
            camera_rotation -= (2 * M_PI);
        }*/

        // Render the screen
        render_scene();
        render_interface();

        SDL_GL_SwapBuffers();
    }
}

int gotStar()
{
	score = score + 100;
	return score;
}

int main( int argc, char **argv )
{
    // Initialise the graphics
    if (!init_graphics()) {
        return 1;
    }
	
	if(!init_models()) {
		return 1;
	}

	//Initialise the audio
    if (!init_audio()) {
        return 1;
    }
	load_audio();

	createGameMenuSystem();

	// Intialise the game state
    setup();

    // Run the game
    loop();
    return 0;
}

/*
#ifdef WIN32

int _stdcall WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow
)
{
    return main();
}

#endif
*/

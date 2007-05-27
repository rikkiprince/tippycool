// This file may be redistributed and modified only under the terms of
// the GNU General Public License (See COPYING for details).
// Copyright (C) 2000,2004 Alistair Riddoch

// The game Stop, Look & Listen has been written by Rikki Prince & Emma Lee-Choon,
// based on the arcade game Frogger.

#ifdef WIN32
#include <Windows.h>
#define M_PI 3.14159265f
#endif

extern "C" {
#include <libmd3/structure.h>
#include <libmd3/loader.h>
#include <libmd3/convert.h>
#include <libmd3/mesh.h>
}

#include <SDL.h>
#include <SDL_mixer.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "font.h"

#include <iostream>

#include <cmath>
#include <cstring>

#include "textures.h"

#include <time.h>

// Constants
static const int screen_width = 600;
static const int screen_height = 400;

// Number of squares in the grid. The number of points is this number +1.
static const int grid_width = 27;
static const int grid_height = 8;
static const int width = 15;
static const int left_no_entry = (grid_width - width)/2;
static const int right_no_entry = grid_width - left_no_entry;

// Number of milliseconds between steps in the game model.
static const int step_time = 200;

// Variables that store the game state

static int block_x = grid_width/2;
static int block_y = 0;

// Structure to hold the properties of a single square on the grid.
// If you want to add more information to the grid, add new members here.
typedef struct block_properties {
    //bool block;
	int type;
	int channel;
} BlockProperties;

// Current blocks on the grid, stored as true if there is a block at the
// given localtion.
BlockProperties properties[grid_width][grid_height];

#define BLOCK_TYPE_1	(0)
#define BLOCK_TYPE_2	(1)
#define EMPTY			(3)
#define CAR				(5)
#define VAN				(10)
#define VAN_FRONT		(VAN)
#define VAN_BACK		(VAN+1)
#define TRUCK			(20)
#define TRUCK_FRONT		(TRUCK)
#define TRUCK_MIDDLE	(TRUCK+1)
#define TRUCK_BACK		(TRUCK+2)

#define MAX_VEHICLES	(TRUCK_BACK+1)

typedef struct lane_properties {
	int capacity;
	int vehicle_type;
	int timesteps;
	int counter;
	bool going_right;
} LaneProperties;

LaneProperties lane[grid_height] = {			{0, EMPTY, 0, 0, false},
												{6, CAR, 1, 0, false},
												{4, VAN, 2, 0, true},
												{3, TRUCK, 1, 0, false},
												{3, TRUCK, 1, 0, true},
												{4, VAN, 2, 0, false},
												{6, CAR, 1, 0, true},
												{0, EMPTY, 0, 0, false}
											};

typedef struct game_state
{
	bool you_lose;
	bool over;
	bool paused;
	int curr_start_block;
	int curr_end_block;
	int score;
	int level;
	int lives;

	double car_frequency;
	int car_gap;
	int time_step;
} GameState;

GameState game = {	false,			// you_lose start value
					false,			// game over start value
					false,			// paused?
					0,				// start lane value
					grid_height-1,	// end lane value
					0,				// score
					1,				// level
					3,				// lives
					0.2,			// car frequency
					6,				// car gap
					200,			// time step
				  };

typedef struct level_change {
	int change;
	int type;
	float amount;
} LevelChange;

static const int LEVEL_LOOP = 3;

#define DELTA		(1)
#define SETTING		(2)

#define CAR_FREQ	(1)
#define TIME_STEP	(2)
#define LANE_COUNT	(4)
#define CAR_GAP		(8)


LevelChange level[LEVEL_LOOP] = {
									{CAR_FREQ, DELTA, +0.2f},
									{TIME_STEP, DELTA, -15},
									{CAR_GAP, DELTA, -1},
								};

#define LEVEL_UP	(5)
#define ADDED_LIFE	(5)

enum messages {EMPTY_STRING=1, INSTRUCTIONS, SPACE_TO_CONTINUE, GAME_OVER};
messages messageString = EMPTY_STRING;

int countdown = 100;

enum direction {FORWARD=1, BACKWARD, RIGHT, LEFT};
direction current_dir = FORWARD;

// Flag used to inform the main loop if the program should now terminate.
// Set this to true if its done.
static bool program_finished = false;

// Texture handles for the texture used to handle printing text on the
// screen.
GLuint textTexture;
GLuint textBase;

GLuint splatTexture;
GLuint blockTexture[MAX_VEHICLES];
GLuint pathTexture;
GLuint roadTexture;
GLuint roadbottomTexture;
GLuint roadtopTexture;
GLuint grassTexture;

Mix_Chunk *splatChunk;
Mix_Chunk *applauseChunk;
Mix_Chunk *one_upChunk;
Mix_Chunk *ribbitChunk;
Mix_Chunk *beepChunk;
Mix_Music *gameover;
Mix_Music *music;

int ribbitChannel = -1;

enum channels { ANY_CHANNEL=-1, SPLAT_CHANNEL, APPLAUSE_CHANNEL, ONEUP_CHANNEL, RIBBIT_CHANNEL, BEEP_CHANNEL, RESERVE_CHANNELS};

static libmd3_file * modelFile = 0;
libmd3_file *models[MAX_VEHICLES];
libmd3_file *frogmodel = 0;
libmd3_file *no_entry = 0;
static void draw_md3_file(libmd3_file *modelFile);

#define NUM_LIGHTS	(2)
GLfloat LightAmbient[2][4]= {{ 0.5f, 0.5f, 0.5f, 1.0f }, { 1.0f, 0.1f, 0.1f, 1.0f }}; 
GLfloat LightDiffuse[2][4]= {{ 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 0.1f, 0.1f, 1.0f }};
GLfloat LightPosition[2][4]= {{ 0.0f, 0.0f, 10.0f, 1.0f }, { -10.0f, -10.0f, -10.0f, 1.0f }};

void step();
int get_normals(libmd3_mesh * mesh);
void libmd3_unpack_normals(libmd3_mesh * mesh);

bool init_audio() {
	int audio_rate = 22050; //playback frequency
	Uint16 audio_format = AUDIO_S16SYS; //signed 16-bit samples in system byte order
	int audio_channels = 2; //2 - stereo sound, 1 - monaural
	int audio_buffers = 8192; //size of memory chunk for storage & playback of samples
	 
	if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0) {
		char stuff[512];
		sprintf(stuff, "Unable to initialize audio: %s\n", Mix_GetError());
		return false;
	}

	Mix_AllocateChannels(16+RESERVE_CHANNELS);
	Mix_ReserveChannels(RESERVE_CHANNELS);

	return true;
}

void uninit_audio() {
	Mix_FreeChunk(applauseChunk);
	Mix_FreeChunk(one_upChunk);
	Mix_FreeChunk(splatChunk);
	Mix_FreeChunk(ribbitChunk);
	Mix_FreeChunk(beepChunk);
	Mix_FreeMusic(gameover);
	Mix_FreeMusic(music);
	Mix_CloseAudio();
}

Mix_Chunk* load_audio(char *filename) {
	//load wav file to memory
	Mix_Chunk *sound = NULL;
	 
	sound = Mix_LoadWAV(filename);
	if(sound == NULL) {
		fprintf(stderr, "Unable to load WAV file: %s\n", Mix_GetError());
		return NULL;
	}
	return sound;
}

int play_audio(channels chan, Mix_Chunk *sound, int repeat) {
	//begin playback
	int channel;
	 
	//Mix_PlayChannel (int channel, Mix_Chunk *chunk, int loops)
	//	channel: -1 plays sample on first available sound channel, otherwise specify certain channel
	//  *chunk: pointer to chunk containing sample for playback
	//  loops: number of times sample should be looped.  -1 = infinite loop, 0 = once, 1 = twice, etc.
	channel = Mix_PlayChannel(chan, sound, repeat);
	if(channel == -1) {
		fprintf(stderr, "Unable to play WAV file: %s\n", Mix_GetError());
	}

	return channel;
}

// Initialise the graphics subsystem. This is pretty much boiler plate
// code with very little to worry about.
bool init_graphics()
{
    // Initialise SDL
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_AUDIO) != 0) {
        // std::cerr << "Failed to initialise video" << std::endl << std::flush;
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    SDL_Surface * screen;

    // Create the window
    screen = SDL_SetVideoMode(screen_width, screen_height, 0, SDL_OPENGL);
    if (screen == NULL) {
        // std::cerr << "Failed to set video mode" << std::endl << std::flush;
        SDL_Quit();
        return false;
    }
	SDL_WM_SetCaption("Stop, Look & Listen", NULL);

    // Setup the viewport transform
    glViewport(0, 0, screen_width, screen_height);

    // Enable vertex arrays
    glEnableClientState(GL_VERTEX_ARRAY);

    // Set the colour the screen will be when cleared
	glClearColor(0.20f, 0.20f, 0.20f, 0.0f);
	glClearStencil(0);

	glShadeModel(GL_SMOOTH);

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

	
	glClearStencil(0);
	glEnable(GL_STENCIL_TEST);

	glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	messageString = INSTRUCTIONS;

	return true;
}

// Clear the grid state.
void clear()
{
    for(int i = 0; i < grid_width; ++i) {
        for(int j = 0; j < grid_height; ++j) {
            properties[i][j].type = EMPTY;
			if(properties[i][j].channel > -1)
			{
				Mix_FadeOutChannel(properties[i][j].channel,1000);
				properties[i][j].channel = -1;
			}
        }
    }
}

// Print a text string on the screen at the current position.
void gl_print(const char * str)
{
	glDisable(GL_LIGHTING);
    glPushMatrix();
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glBindTexture(GL_TEXTURE_2D, textTexture);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glListBase(textBase-32);
    glCallLists((GLsizei)strlen(str),GL_BYTE,str);
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glPopMatrix();
	
	glEnable(GL_LIGHTING);
}

libmd3_file * load_model(const char * filename)
{
	libmd3_file *temp;

	temp = libmd3_file_load(filename);
	if (temp == NULL) {
        return NULL;
    }

    for(int i = 0; i < temp->header->mesh_count; ++i) {
		libmd3_unpack_normals(&temp->meshes[i]);
    }

	return temp;
}

bool load_files()
{
	if((models[TRUCK] = load_model("models/truck.md3")) == NULL) return false;
	if((models[VAN] = load_model("models/van.md3")) == NULL) return false;
	if((models[CAR] = load_model("models/car2.md3")) == NULL) return false;

	if((frogmodel = load_model("models/frog.md3")) == NULL) return false;

	if((no_entry = load_model("models/no_entry.md3")) == NULL) return false;

	applauseChunk = load_audio("./audio/applause2.wav");  //https://secure.ecs.soton.ac.uk/notes/comp6005/wavs.zip
	one_upChunk = load_audio("./audio/one_up.wav");  //http://www.basementarcade.com/arcade/sounds/DK/complete.wav
	splatChunk = load_audio("./audio/splat2.wav");  //http://irc.sanguinarius.org/audio/splat.wav
	ribbitChunk = load_audio("./audio/frog3.wav");  //http://www.ilovewavs.com/Effects/Animals/Frog01.wav
	beepChunk = load_audio("./audio/beep.wav");  //http://www.mediacollege.com/downloads/sound-effects/vehicle/horns/beepbeep-01.wav

	music = Mix_LoadMUS("./audio/music.wav");  //http://www.80smusiclyrics.com/games.shtml#null
	gameover = Mix_LoadMUS("./audio/dead3.wav");  //http://www.i-free.co.in/ringtones/category/cid0/iid7398

	return true;
}

void setup()
{
    // Clear the block store
    clear();

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_NORMALIZE);

	splatTexture = LoadGLTexture("./textures/frog_splat2.bmp");  
	pathTexture = LoadGLTexture("./textures/pavement2.bmp");  //http://www.3dxtra.cygad.net/list/668/1.htm
	roadTexture = LoadGLTexture("./textures/road.bmp");
	roadbottomTexture = LoadGLTexture("./textures/road_bottom.bmp");
	roadtopTexture = LoadGLTexture("./textures/road_top.bmp");
	grassTexture = LoadGLTexture("./textures/grass4.bmp");  //http://www.vectordepot.com/downloads/KATextureLib/grass4.JPG

	for(int i=0; i<NUM_LIGHTS; i++)
	{
		glLightfv(GL_LIGHT1+i, GL_AMBIENT, LightAmbient[i]);
		glLightfv(GL_LIGHT1+i, GL_DIFFUSE, LightDiffuse[i]);
		glLightfv(GL_LIGHT1+i, GL_POSITION,LightPosition[i]);
		glEnable(GL_LIGHT1+i);
	}

	glEnable(GL_LIGHTING);
	glDisable(GL_LIGHT2);

	srand((int)time(0));  // Initialize random number generator.

	for(int i=0; i<100; i++)
		step();

	
	Mix_PlayMusic(music, -1);
}


void libmd3_unpack_normals(libmd3_mesh * mesh)
{
    int i;
    uint8_t lat, lng;
    float flat, flng;

    if (mesh->mesh_header->vertex_count < 2) {
        return;
    }

    mesh->normals = (float *)calloc(mesh->mesh_header->vertex_count * 3, sizeof(float));

    for(i = 0; i < mesh->mesh_header->vertex_count; ++i) {
        lat = (mesh->vertices[i * 4 + 3] >> 8) & 0xff;
        lng = (mesh->vertices[i * 4 + 3]) & 0xff;

        flat = lat * (3.14159265f / 128.f);
        flng = lng * (3.14159265f / 128.f);

        mesh->normals[i * 3 + 0] = cos(flat) * sin(flng);
        mesh->normals[i * 3 + 1] = sin(flat) * sin(flng);
        mesh->normals[i * 3 + 2] =             cos(flng);

        if (i == 0) { continue; }

        memmove(&mesh->vertices[i * 3],
                &mesh->vertices[i * 4],
                3 * sizeof(int16_t));
    }
}

static void fixPath(char * filename)
{
    unsigned int i;
    for(i = 0; i < strlen(filename); ++i) {
        if (filename[i] == '\\') {
            filename[i] = '/';
        }
    }
}

static void draw_one_mesh(libmd3_mesh * mesh)
{
    if (mesh->mesh_header->skin_count != 0) {
        if (mesh->user.u == 0) {
            fixPath((char*)mesh->skins[0].name);
            mesh->user.u = LoadGLTexture((char*)mesh->skins[0].name);
        }
    }

    if (mesh->user.u != 0) {
        glEnable(GL_TEXTURE_2D);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glBindTexture(GL_TEXTURE_2D, mesh->user.u);
    }
	
	glEnableClientState(GL_NORMAL_ARRAY);

    glVertexPointer(3, GL_SHORT, 0, mesh->vertices);
	glNormalPointer(GL_FLOAT, 0, mesh->normals);
    glTexCoordPointer(2, GL_FLOAT, 0, mesh->texcoords);
    glDrawElements(GL_TRIANGLES, mesh->mesh_header->triangle_count * 3,
                   GL_UNSIGNED_INT, mesh->triangles);

    if (mesh->user.u != 0) {
        glDisable(GL_TEXTURE_2D);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }
}

void draw_square()
{
    static const float front_vertices[] = {
        0.f, 0.f, 0.f,
        1.f, 0.f, 0.f,
        1.f, 1.f, 0.f,
        0.f, 1.f, 0.f,
    };
	static const float front_normals[] = {
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
	};
	static const float front_coords[] = {
		1.f, 0.f,
		0.f, 0.f,
		0.f, 1.f,
		1.f, 1.f,
	};

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glTexCoordPointer(2, GL_FLOAT, 0, front_coords);
    glVertexPointer(3, GL_FLOAT, 0, front_vertices);
	glNormalPointer(GL_FLOAT, 0, front_normals);
    glDrawArrays(GL_QUADS, 0, 4);
}

void draw_unit_cube(const float front_coords[])
{
    static const float front_vertices[] = {
        0.f, 0.f, 1.f,
        1.f, 0.f, 1.f,
        1.f, 1.f, 1.f,
        0.f, 1.f, 1.f,
    };
	static const float front_normals[] = {
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
	};

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_FLAT);

	glTexEnvi(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glTexCoordPointer(2, GL_FLOAT, 0, front_coords);
    glVertexPointer(3, GL_FLOAT, 0, front_vertices);
	glNormalPointer(GL_FLOAT, 0, front_normals);
    glDrawArrays(GL_QUADS, 0, 4);

    static const float left_vertices[] = {
        0.f, 0.f, 0.f,
        0.f, 0.f, 1.f,
        0.f, 1.f, 1.f,
        0.f, 1.f, 0.f,
    };
	static const float left_normals[] = {
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
	};
    glVertexPointer(3, GL_FLOAT, 0, left_vertices);
	glTexCoordPointer(2, GL_FLOAT, 0, front_coords);
	glNormalPointer(GL_FLOAT, 0, left_normals);
    glDrawArrays(GL_QUADS, 0, 4);

    static const float right_vertices[] = {
        1.f, 0.f, 1.f,
        1.f, 0.f, 0.f,
        1.f, 1.f, 0.f,
        1.f, 1.f, 1.f,
    };
	static const float right_normals[] = {
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
	};
    glVertexPointer(3, GL_FLOAT, 0, right_vertices);
	glTexCoordPointer(2, GL_FLOAT, 0, front_coords);
	glNormalPointer(GL_FLOAT, 0, right_normals);
    glDrawArrays(GL_QUADS, 0, 4);

    static const float top_vertices[] = {
        0.f, 1.f, 1.f,
        1.f, 1.f, 1.f,
        1.f, 1.f, 0.f,
        0.f, 1.f, 0.f,
    };
	static const float top_normals[] = {
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
	};
    glVertexPointer(3, GL_FLOAT, 0, top_vertices);
	glTexCoordPointer(2, GL_FLOAT, 0, front_coords);
	glNormalPointer(GL_FLOAT, 0, top_normals);
    glDrawArrays(GL_QUADS, 0, 4);

    static const float bottom_vertices[] = {
        0.f, 0.f, 0.f,
        1.f, 0.f, 0.f,
        1.f, 0.f, 1.f,
        0.f, 0.f, 1.f,
    };
	static const float bottom_normals[] = {
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
	};
    glVertexPointer(3, GL_FLOAT, 0, bottom_vertices);
	glTexCoordPointer(2, GL_FLOAT, 0, front_coords);
	glNormalPointer(GL_FLOAT, 0, bottom_normals);
    glDrawArrays(GL_QUADS, 0, 4);
}


void draw_ground() {
    static const float front_vertices[] = {
        -50.f, -50.f, -0.01f,
        50.f, -50.f, -0.01f,
        50.f, 50.f, -0.01f,
        -50.f, 50.f, -0.01f,
    };
	static const float front_normals[] = {
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
	};
	static const float front_coords[] = {
		0.f, 0.f,
		25.f, 0.f,
		25.f, 25.f,
		0.f, 25.f,
	};

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	
	glTexCoordPointer(2, GL_FLOAT, 0, front_coords);
    glVertexPointer(3, GL_FLOAT, 0, front_vertices);
	glNormalPointer(GL_FLOAT, 0, front_normals);
    glDrawArrays(GL_QUADS, 0, 4);
}

void draw_grid()
{
    float horizontal_line_vertices[] = {
        0.f, 0.f, 0.f,
        grid_width, 0.f, 0.f,
    };
    float vertical_line_vertices[] = {
        0.f, 0.f, 0.f,
        0.f, grid_height, 0.f,
    };

    float left_no_entry_line_vertices[] = {
        0.5f, 0.5f, 0.f,
        0.5f, grid_height-0.5, 0.f,
    };

    float right_no_entry_line_vertices[] = {
        0.5f, 0.5f, 0.f,
        0.5f, grid_height-0.5, 0.f,
    };

	glColor3f(0.3f, 0.3f, 0.3f);

	// draw ground
	glBindTexture(GL_TEXTURE_2D, grassTexture);
	draw_ground();

	// Move to the origin of the grid
    glTranslatef(-(float)grid_width/2.0f, -(float)grid_height/2.0f, 0.0f);

	glColor3f(1.0, 1.0, 1.0);

	//draw pavement at bottom of grid
	glPushMatrix();
		for(int i = 0; i < grid_width; ++i) {
			glBindTexture(GL_TEXTURE_2D, pathTexture);
			draw_square();
			glTranslatef(1.0f, 0.0f, 0.f);
		}
	glPopMatrix();

	//draw bottom of road
	glPushMatrix();
	    glTranslatef(0.0f, 1.0f, 0.0f);
		for(int i = 0; i < grid_width; ++i) {
			glBindTexture(GL_TEXTURE_2D, roadbottomTexture);
			draw_square();
			glTranslatef(1.0f, 0.0f, 0.f);
		}
	glPopMatrix();

	//draw road
	glPushMatrix();
		glTranslatef(0.0f, 2.0f, 0.0f);
		for (int j = 2; j < grid_height-2; ++j) {
			glPushMatrix();
				for(int i = 0; i < grid_width; ++i) {
					glBindTexture(GL_TEXTURE_2D, roadTexture);
					draw_square();
					glTranslatef(1.0f, 0.0f, 0.f);
				}
			glPopMatrix();
			glTranslatef(0.0f, 1.0f, 0.0f);
		}
	glPopMatrix();
	
	//draw top of road
	glPushMatrix();
	    glTranslatef(0.0f, (GLfloat)grid_height-2.0f, 0.0f);
		for(int i = 0; i < grid_width; ++i) {
			glBindTexture(GL_TEXTURE_2D, roadtopTexture);
			draw_square();
			glTranslatef(1.0f, 0.0f, 0.f);
		}
	glPopMatrix();

	//draw pavement at top of grid
	glPushMatrix();
	    glTranslatef(0.0f, (GLfloat)grid_height-1.0f, 0.0f);
		for(int i = 0; i < grid_width; ++i) {
			glBindTexture(GL_TEXTURE_2D, pathTexture);
			draw_square();
			glTranslatef(1.0f, 0.0f, 0.f);
		}
	glPopMatrix();

	glColor3f(0.5f, 0.5f, 0.5f);

	glPushMatrix();
		//draw "no entry" sign - near left
		glTranslatef((GLfloat)left_no_entry-0.25f, 0.5f, 0.f);
		glScalef(0.25f, 0.25f, 0.25f);
		draw_md3_file(no_entry);
	glPopMatrix();

	glPushMatrix();
		//draw "no entry" sign - near right
		glTranslatef((GLfloat)right_no_entry+1.75f, 0.5f, 0.f);
		glScalef(0.25f, 0.25f, 0.25f);
		draw_md3_file(no_entry);
	glPopMatrix();

	glPushMatrix();
		//draw "no entry" sign - far left
		glTranslatef((GLfloat)left_no_entry-0.25f, grid_height-0.5, 0.f);
		glScalef(0.25f, 0.25f, 0.25f);
		draw_md3_file(no_entry);
	glPopMatrix();

	glPushMatrix();
		//draw "no entry" sign - far right
		glTranslatef((GLfloat)right_no_entry+1.75f, grid_height-0.5, 0.f);
		glScalef(0.25f, 0.25f, 0.25f);
		draw_md3_file(no_entry);
	glPopMatrix();

	glPushMatrix();
		// DRAW FROG
		glTranslatef((GLfloat)block_x, (GLfloat)block_y, 0.f);
		
		glPushMatrix();
		glTranslatef(0.5f, 1.5f, 0.0f);
		glRotatef(63.5, 1.0, 0.0, 0.0);

		// All drawing commands fail the stencil test, and are not
		// drawn, but increment the value in the stencil buffer.
		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_NEVER, 0x0, 0x0);
		glStencilOp(GL_INCR, GL_INCR, GL_INCR);

		GLUquadric *quadric = gluNewQuadric();
		gluCylinder(quadric, 0.75, 0, 0, 20, 20);
		gluDeleteQuadric(quadric);
		glPopMatrix();
		
		// Now, allow drawing, except where the stencil pattern is 0x1
		// and do not make any further changes to the stencil buffer
		glStencilFunc(GL_NOTEQUAL, 0x1, 0x1);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

	glPopMatrix();

    glPushMatrix();
		// Draw blocks whereever one should be placed on the grid.
		for(int i = 0; i < grid_width; ++i) {
			for(int j = 0; j < grid_height; ++j) {
				if(j>=block_y)
					glDisable(GL_STENCIL_TEST);
				else
					glEnable(GL_STENCIL_TEST);
				switch(properties[i][j].type)
				{
					case CAR:			glPushMatrix();
											glScalef(0.13f, 0.25f, 0.25f);
											if (lane[j].going_right) {
												glTranslatef(3.9f, 1.75f, 0.0f);
											}
											else {
												glRotatef(180, 0, 0, 1);
												glTranslatef(-4.0f, -2.0f, 0.0f);
											}
											draw_md3_file(models[CAR]);
										glPopMatrix();
								break;
					case VAN_FRONT:	
										glPushMatrix();
											if (lane[j].going_right) {
												glScalef(0.47f, 0.4f, 0.5f);
												glTranslatef(0.0f, 1.25f, 0.5f);
											}
											else {
												glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
												glScalef(0.47f, 0.4f, 0.5f);
												glTranslatef(-2.1f, -1.25f, 0.5f);
											}
											draw_md3_file(models[VAN]);
										glPopMatrix();
								break;
					case TRUCK_FRONT:	
										glPushMatrix();
											glScalef(0.15f, 0.2f, 0.3f);
											if (lane[j].going_right) {
												glTranslatef(-3.8f, 2.5f, 0.0f);
											}
											else {
												glRotatef(180, 0, 0, 1);
												glTranslatef(-11.0f, -2.5f, 0.0f);
											}
											draw_md3_file(models[TRUCK]);
										glPopMatrix();
										break;
					default:	
								break;
				}
				glEnable(GL_STENCIL_TEST);
				glTranslatef(0.0f, 1.0f, 0.0f);
			}
			glTranslatef(1.0f, -grid_height, 0.0f);
		}
		glColor3f(1.0f, 1.0f, 1.0f);
    glPopMatrix();



	glPushMatrix();
		// Draw the user controlled block - The FROG!
		glTranslatef((GLfloat)block_x, (GLfloat)block_y, 0.01f);
		if(game.you_lose)
		{
			glDisable(GL_STENCIL_TEST);
			glBindTexture(GL_TEXTURE_2D, splatTexture);
			glScalef(0.9f, 0.6f, 0.9f);
			glTranslatef(0.05f, 0.3f, 0.0f);
			draw_square();
			glEnable(GL_STENCIL_TEST);
		}
		else
		{
			glPushMatrix();
			if(current_dir == FORWARD)
			{

				glScalef(0.13f, 0.25f, 0.2f);
				glRotatef(180.0f, 0.0f,0.0f,1.0f);
				glTranslatef(-3.5f, -1.0f, 0.0f);
			}
			else if(current_dir == BACKWARD)
			{
				glScalef(0.13f, 0.25f, 0.2f);
				glTranslatef(3.5f, 2.0f, 0.0f);
			}
			else if(current_dir == RIGHT)
			{

				glScalef(0.18f, 0.1f, 0.2f);
				glRotatef(90, 0,0,1);
				glTranslatef(2.5f, -1.5f, 0.0f);
			}
			else if(current_dir == LEFT)
			{

				glScalef(0.18f, 0.1f, 0.2f);
				glRotatef(-90, 0,0,1);
				glTranslatef(-2.5f, 3.5f, 0.0f);
			}
			glDisable(GL_STENCIL_TEST);
			draw_md3_file(frogmodel);
			glEnable(GL_STENCIL_TEST);
			glPopMatrix();
		}
	glPopMatrix();


	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPushMatrix();
		// Draw left vertical no entry line
		glEnable(GL_BLEND);
		glColor4f(1.0f, 0.0f, 0.0f, 0.5f);
		glTranslatef((GLfloat)left_no_entry-1.0f, 0.0f, 0.0f);
		glBegin(GL_QUADS);
			glNormal3f(1.0f, 0.0f, 0.0f);
			glVertex3f(0.5f, 0.5f, 0.0f);
			glVertex3f(0.5f, 0.5f, 1.4f);
			glVertex3f(0.5f, grid_height-0.5, 1.4f);
			glVertex3f(0.5f, grid_height-0.5, 0.0f);
		glEnd();
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
		glDisable(GL_BLEND);
    glPopMatrix();

	glPushMatrix();
		// Draw right vertical no entry line
		glEnable(GL_BLEND);
		glColor4f(1.0f, 0.0f, 0.0f, 0.5f);
		glTranslatef((GLfloat)right_no_entry+1.0f, 0.0f, 0.0f);
		glBegin(GL_QUADS);
			glNormal3f(1.0f, 0.0f, 0.0f);
			glVertex3f(0.5f, 0.5f, 0.0f);
			glVertex3f(0.5f, 0.5f, 1.4f);
			glVertex3f(0.5f, grid_height-0.5, 1.4f);
			glVertex3f(0.5f, grid_height-0.5, 0.0f);
		glEnd();
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
		glDisable(GL_BLEND);
    glPopMatrix();

}

float camera_rotation = 0.0f;

static void draw_md3_file(libmd3_file *modelFile)
{   
	int i;
    libmd3_mesh * meshp;

	if (modelFile->header->mesh_count == 0) {
        printf("[No meshes in file]\n");
        return;
    }

    glPushMatrix(); 
	glScalef(0.001f, 0.001f, 0.001f);
    meshp = modelFile->meshes;
    for(i = 0; i < modelFile->header->mesh_count; ++i, ++meshp) {
        draw_one_mesh(meshp);
    }
	glPopMatrix(); 
}

void camera_pos()
{
    // Set up the modelview
    glMatrixMode(GL_MODELVIEW);
    // Reset the camera
    glLoadIdentity();

	gluLookAt(		0.5, -(grid_height/2.0)-10, 10.0,
					0.5, -(grid_height/2.0)+3,0,
					0,0,1
				 );
}

void render_scene()
{
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // Enable the depth test
    glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);

    // Set the projection transform
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (float)screen_width/screen_height, 1.f, 100.f);

    // Set the camera position
    camera_pos();

    // Draw the scene
    draw_grid();
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

    glColor3f(1.f, 1.f, 1.f);

	glPushMatrix();

		glTranslatef(0.f, 20.f, 0);
		sprintf(buf, "Score: %d", game.score);
		gl_print(buf);
		
		glTranslatef(0.f, 20.f, 0);
		sprintf(buf, "Level: %d", game.level);
		gl_print(buf);

		if (!game.over) {
			glTranslatef(0.f, 20.f, 0);
			sprintf(buf, "Lives: %d", game.lives);
			gl_print(buf);
		}	
		
	glPopMatrix();

	if(messageString != EMPTY_STRING)
	{
		switch(messageString)
		{
			case INSTRUCTIONS:		if (countdown > 0) {
										sprintf(buf, "How many times can you cross the road?...");
										countdown--;
									}
									else {
										messageString = EMPTY_STRING;
									}
									break;
			case GAME_OVER:			sprintf(buf, "GAME OVER!  Press Space to Restart...");
									break;
			case SPACE_TO_CONTINUE: sprintf(buf, "You Lose a Life!  Press Space to Continue...");
									break;
			default:				break;
		}
		glPushMatrix();
		glTranslatef(40.f, 370.f, 0);
		gl_print(buf);
		glPopMatrix();
	}
}

void reset_level()
{
	game.you_lose = false;
	block_x = (int)(grid_width/2.0);
	block_y = game.curr_start_block;
	messageString = EMPTY_STRING;
}

void you_lose() {
	game.you_lose = true;
	game.lives--;
	if(game.lives < 0) {
		game.over = true;
		Mix_HaltMusic();
		glDisable(GL_LIGHT1);
		glEnable(GL_LIGHT2);
		messageString = GAME_OVER;
		Mix_PlayMusic(gameover, -1);
	}
	else
	{
		messageString = SPACE_TO_CONTINUE;
	}
	play_audio(SPLAT_CHANNEL, splatChunk, 0);
}

void increase_score() {
	game.score++;
	if(game.score%LEVEL_UP == 0) {
		int l = ((game.level-1)%LEVEL_LOOP);
		switch(level[l].change)
		{
			case CAR_FREQ:	if(level[l].type == DELTA)
								game.car_frequency += level[l].amount;
							else if(level[l].type == SETTING)
								game.car_frequency = level[l].amount;
							break;
			case TIME_STEP:	if(level[l].type == DELTA)
								game.time_step += (int)level[l].amount;
							else if(level[l].type == SETTING)
								game.time_step = (int)level[l].amount;
							break;
			case CAR_GAP:	if(level[l].type == DELTA)
								if (game.car_gap > 2)
									game.car_gap += (int)level[l].amount;
							else if(level[l].type == SETTING)
								game.car_gap = (int)level[l].amount;
							break;
		}
		game.level++;
		if(game.level%ADDED_LIFE == 0) {
			game.lives++;
			play_audio(ONEUP_CHANNEL, one_upChunk, 0);
		}
		else {
			play_audio(APPLAUSE_CHANNEL, applauseChunk, 0);
		}
	}
}

void reset_game() {
	block_x = grid_width/2;
	block_y = 0;
	game.you_lose = false;
	game.over = false;
	game.curr_start_block = 0;
	game.curr_end_block = grid_height-1;
	game.level = 1;
	game.lives = 3;
	game.score = 0;
	game.car_frequency = 0.2;
	game.time_step = 200;
	game.car_gap = 6;
	glEnable(GL_LIGHT1);
	glDisable(GL_LIGHT2);
	for(int i=0; i<100; i++)
		step();

	Mix_HaltMusic();
	Mix_PlayMusic(music, -1);
	messageString = EMPTY_STRING;
}

void check_beep()
{
	if (!game.you_lose) {
		if(lane[block_y].going_right)
		{
			if(block_x > 1)
			{
				if(properties[block_x-2][block_y].type != EMPTY)
				{
					play_audio(BEEP_CHANNEL, beepChunk, 0);
				}
			}
		}
		else
		{
			if(block_x < grid_width-2)
			{
				if(properties[block_x+2][block_y].type != EMPTY)
				{
					play_audio(BEEP_CHANNEL, beepChunk, 0);
				}
			}
		}
	}
}

bool car_gap(int x, int y, int dir, int gap) {
	bool check = true;
	for(int i = 1; i <= gap; i++) {
		if (properties[x+(-1*dir*i)][y].type != EMPTY) {
			check = false;
			continue;
		}
	}
	return check;
}

// This function is called every step_time milliseconds. In many games you
// will want to use this function to update the game state. For example in
// a Tetris game, this would be the place where the falling blocks were
// moved down the grid.
void step()
{
	// Draw blocks whereever one should be placed on the grid.
    for(int j = 0; j < grid_height; ++j) {
		if(lane[j].counter < lane[j].timesteps) {
			lane[j].counter++;
			continue;
		}
		lane[j].counter = 0;
		int dir;
		int start, end;
		// MOVING RIGHT
		if(lane[j].going_right) {
			dir = -1;
			start = 0;
			end = grid_width-1;
			switch(properties[end][j].type)
			{
				case CAR:
				case VAN_BACK:
				case TRUCK_BACK:	lane[j].capacity++;
									Mix_SetPosition(properties[end][j].channel, 0, 0);
									Mix_HaltChannel(properties[end][j].channel);
									break;
				default:			break;
			}

			// MOVE RIGHT ONE BLOCK
			for(int i = grid_width-1; i > 0; --i) {
				properties[i][j].type = properties[i-1][j].type;
				properties[i][j].channel = properties[i-1][j].channel;
				if(properties[i][j].type != EMPTY && i==block_x && j==block_y && !game.you_lose) {
					you_lose();
				}
			}
		}
		// MOVING LEFT
		else {
			dir = +1;
			start = grid_width-1;
			end = 0;
			switch(properties[end][j].type)
			{
				case CAR:
				case VAN_BACK:
				case TRUCK_BACK:	lane[j].capacity++;
									Mix_SetPosition(properties[end][j].channel, 0, 0);
									Mix_HaltChannel(properties[end][j].channel);
									break;
				default:			break;
			}

			// MOVE LEFT ONE BLOCK
			for(int i = 0; i < grid_width-1; ++i) {
				properties[i][j].type = properties[i+1][j].type;
				properties[i][j].channel = properties[i+1][j].channel;
				if(properties[i][j].type != EMPTY && i==block_x && j==block_y && !game.you_lose) {
					you_lose();
				}
			}
		}
		// SET FIRST BLOCK TO EMPTY
		int set_to;
		switch (properties[start][j].type)
		{
			case VAN_FRONT:		set_to = VAN_BACK;
								break;
			case TRUCK_FRONT:	set_to = TRUCK_MIDDLE;
								break;
			case TRUCK_MIDDLE:	set_to = TRUCK_BACK;
								break;
			default:			set_to = EMPTY;
								break;
		}
		properties[start][j].type = set_to;
		properties[start][j].channel = -1;
		
		// IF SPARE CAPACITY AND NEXT 2 BLOCKS EMPTY
		// RANDOMLY ADD A NEW CAR
		if(lane[j].capacity > 0 && car_gap(start, j, dir, game.car_gap)) {//properties[start+(-1*dir*1)][j].type == EMPTY && properties[start+(-1*dir*2)][j].type == EMPTY) {
			double r = ((double)rand())/ ((double)RAND_MAX);
			if(r < game.car_frequency)	// ADDS VEHICLE
			{
				properties[start][j].type = lane[j].vehicle_type;
				lane[j].capacity--;
				if(block_x == start && block_y == j)
				{
					you_lose();
				}
			}
		}
    }
	check_beep();
}

// The main program loop function. This does not return until the program
// has finished.
void loop()
{
    SDL_Event event;
    int elapsed_time = SDL_GetTicks();
    int last_step = elapsed_time;
    int frame_count = 0;

    // This is the main program loop. It will run until something sets
    // the flag to indicate we are done.
    while (!program_finished) {
        // Check for events
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    // The user closed the window
                    program_finished = true;
                    break;
                case SDL_KEYDOWN:
                    // We have a keypress
					if(ribbitChannel > -1)
						Mix_HaltChannel(ribbitChannel);
                    if ( event.key.keysym.sym == SDLK_ESCAPE ) {
                        // quit
                        program_finished = true;
                    }
					if(!game.you_lose)
					{
						if ( event.key.keysym.sym == SDLK_UP )
						{
							current_dir = FORWARD;
							if((block_y < (grid_height-1)) && properties[block_x][block_y + 1].type != EMPTY) 
							{
								you_lose();
							}
							if (block_y < (grid_height-1))
							{
								++block_y;
								ribbitChannel = play_audio(RIBBIT_CHANNEL, ribbitChunk, 0);
								Mix_Volume(ribbitChannel, 32);
							}
						}
						if ( event.key.keysym.sym == SDLK_DOWN ) {
							current_dir = BACKWARD;
							// Move block down
							if ((block_y > 0) && properties[block_x][block_y - 1].type != EMPTY) 
							{
								you_lose();
							}
							if (block_y > 0) 
							{
								--block_y;
								ribbitChannel = play_audio(RIBBIT_CHANNEL, ribbitChunk, 0);
								Mix_Volume(ribbitChannel, 32);
							}
						}
						if(block_y == game.curr_end_block)
						{
							int temp;
							temp = game.curr_end_block;
							game.curr_end_block = game.curr_start_block;
							game.curr_start_block = temp;
							increase_score();
						}
						if ( event.key.keysym.sym == SDLK_LEFT ) 
						{
							current_dir = LEFT;
							// Move block left
							if ((block_x > 0) && properties[block_x - 1][block_y].type != EMPTY)
							{
								you_lose();
							}
							if (block_x > left_no_entry) 
							{
								--block_x;
								ribbitChannel = play_audio(RIBBIT_CHANNEL, ribbitChunk, 0);
								Mix_Volume(ribbitChannel, 32);
							}
						}
						if ( event.key.keysym.sym == SDLK_RIGHT ) 
						{
							current_dir = RIGHT;
							// Move block right
							if ((block_x < (grid_width-1)) && properties[block_x + 1][block_y].type != EMPTY) 
							{
								you_lose();
							}
							if (block_x < right_no_entry)
							{
								++block_x;
								ribbitChannel = play_audio(RIBBIT_CHANNEL, ribbitChunk, 0);
								Mix_Volume(ribbitChannel, 32);
							}
						}
					}
					if(event.key.keysym.sym == SDLK_p)
					{
						game.paused = !game.paused;
					}
					if(event.key.keysym.sym == SDLK_SPACE)
					{
						if(game.you_lose)
						{
							if(game.over) {
								reset_game();
							}
							else {
								reset_level();
							}
						}
						else
						{
							game.paused = !game.paused;
						}
					}
                    break;
                default:
                    break;
            }
        }

        // Get the time and check if a complete time step has passed.
        // For step based games like Tetris, this is used to update the
        // the game state
        const int ticks = SDL_GetTicks();
		if ((ticks - last_step) > game.time_step) {
            last_step = ticks;
			if(!game.paused) {
	            step();
			}
        }

        // Calculate the time in seconds since the last frame
        // For a real time program this would be used to update the game state
        float delta = (ticks - elapsed_time) / 1000.0f;
        elapsed_time = ticks;

        // Update the rotation on the camera
        camera_rotation += delta;
        if (camera_rotation > (2 * M_PI)) {
            camera_rotation -= (2 * M_PI);
        }

        // Render the screen
        render_scene();
        render_interface();

        SDL_GL_SwapBuffers();
    }
}

int main()
{
    // Initialise the graphics
    if (!init_graphics()) {
        return 1;
    }

    if (!init_audio()) {
        return 1;
    }

	if(!load_files()) {
		return 1;
	}

	// Intialise the game state
    setup();

    // Run the game
    loop();

	//Close all audio stuff
	uninit_audio();

    return 0;
}

#ifdef WIN32

int __stdcall WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow
)
{
    main();
}

#endif

#include "Level.h"

/*void hello(const int i)
{
	float **p3 = new float *[i];
	printf("const int i = %d\n", i);
}*/

Level::Level(int x, int y, int z)
{
	printf("Constructing Level!\n");

	char buf[256];
	//GetPrivateProfileString("1,1,1", "type", NULL, buf, 256, ".\\test.ini");
	char *filename = ".\\levels\\level1.ini";

	char width[256], height[256], depth[256];
	GetPrivateProfileString("settings", "width", NULL, width, 256, filename);
	GetPrivateProfileString("settings", "height", NULL, height, 256, filename);
	GetPrivateProfileString("settings", "depth", NULL, depth, 256, filename);
	//GetPrivateProfileString("settings", "", NULL, , 256, filename);
	int x1 = atoi(width);
	int y1 = atoi(height);
	int z1 = atoi(depth);
	
	this->grid_width = x1;
	this->grid_height = y1;
	this->grid_depth = z1;

	// allocate block array
	// thanks to http://www.phptr.com/articles/article.asp?p=31783&seqNum=7&rl=1
	this->block = new AbstractBlock *[this->grid_width * this->grid_height * this->grid_depth];

	GLuint texture = LoadGLTexture("texture.bmp");

	//printf("texture: %u", texture);

	for(int k=0; k<this->grid_depth; k++)
	{
		for(int j=0; j<this->grid_height; j++)
		{
			for(int i=0; i<this->grid_width; i++)
			{
				int offset = this->getOffset(i, j, k);
				char block_string[256], type[256];
				sprintf(block_string, "%d,%d,%d", i, j, k);
				GetPrivateProfileString(block_string, "type", NULL, type, 256, filename);
				//printf("type = %s\n", type);
				if(strcmp(type, "Normal") == 0)
				{
					block[offset] = new NormalBlock(texture);
				}
				//else if(strcmp(buf, "Special") == 0)
				else
				{
					block[offset] = NULL;
				}
			}
		}
	}
}

Level::~Level()
{
	printf("Destructing Level!\n");

	// clean up block array
	delete [] block;
}

void Level::render()
{
	//printf("Render Level here\n");
	draw_grid();
}


void Level::draw_grid()
{
	glPushMatrix();
    float horizontal_line_vertices[] = {
        0.f, 0.f, 0.f,
        this->grid_width, 0.f, 0.f,
    };
    float vertical_line_vertices[] = {
        0.f, 0.f, 0.f,
        0.f, this->grid_height, 0.f,
    };
    float depth_line_vertices[] = {
        0.f, 0.f, 0.f,
        0.f, 0.f, this->grid_depth,
    };

    glColor3f(0.3f, 0.3f, 0.3f);

    // Move to the origin of the grid
    glTranslatef(-(float)this->grid_width/2.0f, -(float)this->grid_height/2.0f, -(float)this->grid_depth/2.0f);


	// Store this position
	glPushMatrix();

	//for each z value of the grid
	for (int k = 0; k <= this->grid_depth; ++k)
	{
		glPushMatrix();
		// Draw vertical lines
		glVertexPointer(3, GL_FLOAT, 0, vertical_line_vertices);
		for (int i = 0; i <= this->grid_width; ++i) {
			glDrawArrays(GL_LINES, 0, 2);
			glTranslatef(1.0f, 0.0f, 0.0f);
		}
		glPopMatrix();


		glPushMatrix();
		// Draw horizontal lines
		glVertexPointer(3, GL_FLOAT, 0, horizontal_line_vertices);
		for (int j = 0; j <= this->grid_height; ++j) {
			glDrawArrays(GL_LINES, 0, 2);
			glTranslatef(0.0f, 1.0f, 0.0f);
		}
	    glPopMatrix();

        glTranslatef(0.0f, 0.0f, 1.0f);
    }
	glPopMatrix();


	glPushMatrix();
    // Draw blocks whereever one should be placed on the grid.
    for(int i = 0; i < this->grid_width; ++i) {
        for(int j = 0; j < this->grid_height; ++j) {
			for(int k = 0; k < this->grid_depth; ++k) {
				/*if ((properties[i][j][k].block)) {
					//draw_unit_cube();
				}*/
				//AbstractBlock *block = this->block[this->getOffset(i, j, k)];
				int offset = this->getOffset(i, j, k);
				if(block[offset] != NULL)
				{
					//block->render();
					block[offset]->render();
				}
				/*else
				{
					glPushMatrix();
					//glTranslatef(0.5f, 0.5f, 0.5f);
					draw_md3_file(model);
					glPopMatrix();
				}*/
				glTranslatef(0.0f, 0.0f, 1.0f);
			}
            glTranslatef(0.0f, 1.0f, -this->grid_depth);
        }
        glTranslatef(1.0f, -this->grid_height, 0.0f);
    }
	glPopMatrix();

    glColor3f(1.0, 1.0, 1.0);


    // Draw the user controlled block
    //glTranslatef(block_x, block_y, block_z);
    //draw_unit_cube();
	glPopMatrix();
}

int Level::getOffset(int i, int j, int k)
{
	return (i + (j * this->grid_width) + (k * this->grid_width * this->grid_height));
}

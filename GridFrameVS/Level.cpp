#include "Level.h"

/*void hello(const int i)
{
	float **p3 = new float *[i];
	printf("const int i = %d\n", i);
}*/

Level::Level(int x, int y, int z)
{
	printf("Constructing Level!\n");

	this->rotF = 0;
	this->rotS = 0;

	this->camera = new Camera();

	this->blockSize = 1.0;

//	char buf[256];
	//GetPrivateProfileString("1,1,1", "type", NULL, buf, 256, ".\\test.ini");
	char *filename = ".\\levels\\level2.ini";

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

	char sx[256], sy[256], sz[256];
	GetPrivateProfileString("start", "x", NULL, sx, 256, filename);
	GetPrivateProfileString("start", "y", NULL, sy, 256, filename);
	GetPrivateProfileString("start", "z", NULL, sz, 256, filename);
//	ini.getInt("start", "x");
	int startX = atoi(sx);
	int startY = atoi(sy);
	int startZ = atoi(sz);

	/*char or[256];
	ini.getString("start", "orientation", 256, buf);
	GetPrivateProfileString("start", "orientation", NULL, or, 256, filename);*/

	this->offsetX = -(float)(this->grid_width*this->blockSize)/2.0f;
	this->offsetY = -(float)(this->grid_height*this->blockSize)/2.0f;
	this->offsetZ = -(float)(this->grid_depth*this->blockSize)/2.0f;

	//this->ball = new Ball(this->blockSize, this->offsetX + (startX * this->blockSize), this->offsetY + (startY * this->blockSize), this->offsetZ + (startZ * this->blockSize));
	this->ball = new Ball(this->blockSize, (startX * this->blockSize), (startY * this->blockSize), (startZ * this->blockSize));

	this->velF = 0;
	this->velS = 0;
}

Level::~Level()
{
	printf("Destructing Level!\n");

	// clean up block array
	delete [] block;
}

void Level::up()
{
	this->rotF-=1;
	this->accF-=0.01f;
}

void Level::down()
{
	this->rotF+=1;
	this->accF+=0.01f;
}

void Level::left()
{
	this->rotS-=1;
	this->accS-=0.01f;
}

void Level::right()
{
	this->rotS+=1;
	this->accS+=0.01f;
}

void Level::stop()
{
	this->rotS = 0;
	this->rotF = 0;
	this->accS = 0;
	this->accF = 0;
	this->velS = 0;
	this->velF = 0;
}

GLfloat friction = -0.5f;

// http://www.thescripts.com/forum/thread128445.html
inline int sign(int a) { return (a == 0) ? 0 : (a<0 ? -1 : 1); }

void Level::update()
{
	/*for(double i = -5.4; i<5; i++)
	{
		printf("%f floor=%f ceil=%f\n", i, floor(i), ceil(i));
	}*/
	GLfloat prevX = ball->getX();
	GLfloat prevY = ball->getY();
	GLfloat prevZ = ball->getZ();

	int dirF=0;
	if(ball->getFacingX()!=0)
		dirF=ball->getFacingX();
	else if(ball->getFacingY()!=0)
		dirF=ball->getFacingY();
	else if(ball->getFacingZ()!=0)
		dirF=ball->getFacingZ();
	
	int dirS=0;
	if(ball->getSideX()!=0)
		dirS=ball->getSideX();
	else if(ball->getSideY()!=0)
		dirS=ball->getSideY();
	else if(ball->getSideZ()!=0)
		dirS=ball->getSideZ();

	GLfloat prevF = (fabs(ball->getFacingX())*ball->getX()) + (fabs(ball->getFacingY())*ball->getY()) + (fabs(ball->getFacingZ())*ball->getZ());
	GLfloat prevS = (fabs(ball->getSideX())*ball->getX()) + (fabs(ball->getSideY())*ball->getY()) + (fabs(ball->getSideZ())*ball->getZ());
	//printf("prevX=%f, prevY=%f, prevZ=%f, prevF=%f\n\n", prevX, prevY, prevZ, prevF);

	this->velF += accF + (sign(accF)*friction);
	this->velS += accS + (sign(accS)*friction);

	ball->move(this->velF, this->velS);
	//printf("Ball:   (%f,%f,%f)\n", ball->getX(), ball->getY(), ball->getZ());

	GLfloat nextF = (fabs(ball->getFacingX())*ball->getX()) + (fabs(ball->getFacingY())*ball->getY()) + (fabs(ball->getFacingZ())*ball->getZ());
	GLfloat nextS = (fabs(ball->getSideX())*ball->getX()) + (fabs(ball->getSideY())*ball->getY()) + (fabs(ball->getSideZ())*ball->getZ());

	//printf("prev: %f, next: %f, comp:%f\n", prevF, nextF, ceil(nextF)-(this->blockSize/2.0));

	GLfloat halfBlock = this->blockSize/2.0;

	// backward edges
	if(prevF > ceil(nextF)-(this->blockSize/2.0) && nextF <= ceil(nextF)-(this->blockSize/2.0))
	{
		//printf("got in here!\n");
		int last = (int)ceil(prevF);
		int next = (int)floor(nextF);
		/*printf("prevZ=%f, floor(prevZ)=%f, ceil(prevZ)=%f\n", prevZ, floor(prevZ), ceil(prevZ));
		printf("ballZ=%f, floor(ballZ)=%f, ceil(ballZ)=%f\n", ball->getZ(), floor(ball->getZ()), ceil(ball->getZ()));*/
		//printf("Changed at %d <- %d\n", next, last);

		//printf("Ball:   (%f,%f,%f)\n", ball->getX(), ball->getY(), ball->getZ());

		int x = ((ball->getFacingX()!=0)?next:floor(ball->getX()+this->blockSize/2.0)) - (ball->getOrientationX() * this->blockSize);
		int y = ((ball->getFacingY()!=0)?next:floor(ball->getY()+this->blockSize/2.0)) - (ball->getOrientationY() * this->blockSize);
		int z = ((ball->getFacingZ()!=0)?next:floor(ball->getZ()+this->blockSize/2.0)) - (ball->getOrientationZ() * this->blockSize);

		//printf("next under block (%d,%d,%d)\n", x,y,z);

		if(next < 0)
		{
			printf("1 FALLOUT!\n");
			/*if(ball->getFacingX()!=0)
				printf("x: 
			else if(ball->getFacingY()!=0)
			else if(ball->getFacingZ()!=0)*/
			if(dirF > 0)
			{
				ball->move(halfBlock, 0);
				ball->fallBackward();
				ball->realign();
				ball->move(halfBlock, 0);
			}
			else
			{
				ball->move(-halfBlock, 0);
				ball->fallForward();
				ball->realign();
				ball->move(-halfBlock, 0);
			}
		}
		else if(this->block[this->getOffset(x,y,z)] == NULL)
		{
			printf("2 FALLOUT!\n");
			/*ball->move(halfBlock, 0);
			ball->fallBackward();
			ball->move(halfBlock, 0);*/
			if(dirF > 0)
			{
				ball->move(halfBlock, 0);
				ball->fallBackward();
				ball->realign();
				ball->move(halfBlock, 0);
			}
			else
			{
				ball->move(-halfBlock, 0);
				ball->fallForward();
				ball->realign();
				ball->move(-halfBlock, 0);
			}
		}
	}
	// forward edges
	else if(prevF <= floor(nextF)+(this->blockSize/2.0) && nextF > floor(nextF)+(this->blockSize/2.0))
	{
		int last = (int)floor(prevF);
		int next = (int)ceil(nextF);
		//printf("Changed at %d -> %d\n", last, next);

		//printf("Ball1:   (%f,%f,%f)\n", ball->getX(), ball->getY(), ball->getZ());

		int x = ((ball->getFacingX()!=0)?next:floor(ball->getX()+this->blockSize/2.0)) - (ball->getOrientationX() * this->blockSize);
		int y = ((ball->getFacingY()!=0)?next:floor(ball->getY()+this->blockSize/2.0)) - (ball->getOrientationY() * this->blockSize);
		int z = ((ball->getFacingZ()!=0)?next:floor(ball->getZ()+this->blockSize/2.0)) - (ball->getOrientationZ() * this->blockSize);

		//printf("next under block (%d,%d,%d)\n", x,y,z);

		if(next >= this->grid_width)
		{
			printf("3 FALLOUT!\n");
			/*ball->move(-halfBlock, 0);
			ball->fallForward();
			ball->move(-halfBlock, 0);*/
			if(dirF < 0)
			{
				ball->move(halfBlock, 0);
				ball->fallBackward();
				ball->realign();
				ball->move(halfBlock, 0);
			}
			else
			{
				ball->move(-halfBlock, 0);
				ball->fallForward();
				ball->realign();
				ball->move(-halfBlock, 0);
			}
		}
		else if(this->block[this->getOffset(x,y,z)] == NULL)
		{
			printf("4 FALLOUT!\n");
			/*ball->move(-halfBlock, 0);
			ball->fallForward();
			ball->move(-halfBlock, 0);*/
			if(dirF < 0)
			{
				ball->move(halfBlock, 0);
				ball->fallBackward();
				ball->realign();
				ball->move(halfBlock, 0);
			}
			else
			{
				ball->move(-halfBlock, 0);
				ball->fallForward();
				ball->realign();
				ball->move(-halfBlock, 0);
			}
		}
	}
	
	// backward blocks
	else if(prevF > ceil(nextF) && nextF <= ceil(nextF))
	{
		//printf("got in here!\n");
		int last = (int)ceil(prevF);
		int next = (int)floor(nextF);
		/*printf("prevZ=%f, floor(prevZ)=%f, ceil(prevZ)=%f\n", prevZ, floor(prevZ), ceil(prevZ));
		printf("ballZ=%f, floor(ballZ)=%f, ceil(ballZ)=%f\n", ball->getZ(), floor(ball->getZ()), ceil(ball->getZ()));*/
		printf("Changed at %d <- %d\n", next, last);

		printf("Ball: (%f,%f,%f)\n", ball->getX(), ball->getY(), ball->getZ());

		int x = ((ball->getFacingX()!=0)?next:floor(ball->getX()+this->blockSize/2.0))/* - (ball->getOrientationX() * this->blockSize)*/;
		int y = ((ball->getFacingY()!=0)?next:floor(ball->getY()+this->blockSize/2.0))/* - (ball->getOrientationY() * this->blockSize)*/;
		int z = ((ball->getFacingZ()!=0)?next:floor(ball->getZ()+this->blockSize/2.0))/* - (ball->getOrientationZ() * this->blockSize)*/;

		printf("block in front (%d,%d,%d)\n", x,y,z);

		if(validBlock(x,y,z) && this->block[this->getOffset(x,y,z)] != NULL)
		{
			printf("11 COLLISION!\n");
			/*ball->setX(prevX);
			ball->setY(prevY);
			ball->setZ(prevZ);*/
			if(dirF > 0)
			{
				//ball->move(-halfBlock, 0);
				ball->fallForward();
				//ball->realign();
				//ball->move(-halfBlock, 0);*/
			}
			else
			{
				//ball->move(halfBlock, 0);
				ball->fallBackward();
				//ball->realign();
				//ball->move(halfBlock, 0);*/
			}
		}
	}
	// forward blocks
	else if(prevF <= floor(nextF) && nextF > floor(nextF))
	{
		int last = (int)floor(prevF);
		int next = (int)ceil(nextF);
		printf("Changed at %d -> %d\n", last, next);

		printf("Ball: (%f,%f,%f)\n", ball->getX(), ball->getY(), ball->getZ());

		int x = ((ball->getFacingX()!=0)?next:floor(ball->getX()+this->blockSize/2.0))/* - (ball->getOrientationX() * this->blockSize)*/;
		int y = ((ball->getFacingY()!=0)?next:floor(ball->getY()+this->blockSize/2.0))/* - (ball->getOrientationY() * this->blockSize)*/;
		int z = ((ball->getFacingZ()!=0)?next:floor(ball->getZ()+this->blockSize/2.0))/* - (ball->getOrientationZ() * this->blockSize)*/;

		printf("block in front (%d,%d,%d)\n", x,y,z);

		if(validBlock(x,y,z) && this->block[this->getOffset(x,y,z)] != NULL)
		{
			printf("13 COLLISION!\n");
			/*ball->setX(prevX);
			ball->setY(prevY);
			ball->setZ(prevZ);*/
			if(dirF > 0)
			{
				//ball->move(halfBlock, 0);
				ball->fallBackward();
				//ball->realign();
				//ball->move(halfBlock, 0);*/
			}
			else
			{
				//ball->move(-halfBlock, 0);
				ball->fallForward();
				//ball->realign();
				//ball->move(-halfBlock, 0);*/
			}
		}
	}

	
	// right edge
	if(prevS > ceil(nextS)-(this->blockSize/2.0) && nextS <= ceil(nextS)-(this->blockSize/2.0))
	{
		//printf("got in here!\n");
		int last = (int)ceil(prevS);
		int next = (int)floor(nextS);
		/*printf("prevZ=%f, floor(prevZ)=%f, ceil(prevZ)=%f\n", prevZ, floor(prevZ), ceil(prevZ));
		printf("ballZ=%f, floor(ballZ)=%f, ceil(ballZ)=%f\n", ball->getZ(), floor(ball->getZ()), ceil(ball->getZ()));*/
		//printf("Changed at %d <- %d\n", next, last);

		//printf("Ball:   (%f,%f,%f)\n", ball->getX(), ball->getY(), ball->getZ());

		int x = ((ball->getSideX()!=0)?next:floor(ball->getX()+this->blockSize/2.0)) - (ball->getOrientationX() * this->blockSize);
		int y = ((ball->getSideY()!=0)?next:floor(ball->getY()+this->blockSize/2.0)) - (ball->getOrientationY() * this->blockSize);
		int z = ((ball->getSideZ()!=0)?next:floor(ball->getZ()+this->blockSize/2.0)) - (ball->getOrientationZ() * this->blockSize);

		//printf("next under block (%d,%d,%d)\n", x,y,z);

		if(next < 0)
		{
			printf("5 FALLOUT!\n");
			/*if(ball->getFacingX()!=0)
				printf("x: 
			else if(ball->getFacingY()!=0)
			else if(ball->getFacingZ()!=0)*/
			if(dirS > 0)
			{
				ball->move(0, -halfBlock);
				ball->fallLeft();
				ball->realign();
				ball->move(0, -halfBlock);
				printf("new ball: (%f,%f,%f)\n", ball->getX(), ball->getY(), ball->getZ());
			}
			else
			{
				ball->move(0, halfBlock);
				ball->fallRight();
				ball->realign();
				ball->move(0, halfBlock);
				printf("new ball: (%f,%f,%f)\n", ball->getX(), ball->getY(), ball->getZ());
			}
		}
		else if(this->block[this->getOffset(x,y,z)] == NULL)
		{
			printf("6 FALLOUT!\n");
			/*ball->move(halfBlock, 0);
			ball->fallBackward();
			ball->move(halfBlock, 0);*/
			if(dirS > 0)
			{
				ball->move(0, -halfBlock);
				ball->fallLeft();
				ball->realign();
				ball->move(0, -halfBlock);
				printf("new ball: (%f,%f,%f)\n", ball->getX(), ball->getY(), ball->getZ());
			}
			else
			{
				ball->move(0, halfBlock);
				ball->fallRight();
				ball->realign();
				ball->move(0, halfBlock);
				printf("new ball: (%f,%f,%f)\n", ball->getX(), ball->getY(), ball->getZ());
			}
		}
	}
	// left edges
	else if(prevS <= floor(nextS)+(this->blockSize/2.0) && nextS > floor(nextS)+(this->blockSize/2.0))
	{
		int last = (int)floor(prevS);
		int next = (int)ceil(nextS);
		//printf("Changed at %d -> %d\n", last, next);

		//printf("Ball:   (%f,%f,%f)\n", ball->getX(), ball->getY(), ball->getZ());

		int x = ((ball->getSideX()!=0)?next:floor(ball->getX()+this->blockSize/2.0)) - (ball->getOrientationX() * this->blockSize);
		int y = ((ball->getSideY()!=0)?next:floor(ball->getY()+this->blockSize/2.0)) - (ball->getOrientationY() * this->blockSize);
		int z = ((ball->getSideZ()!=0)?next:floor(ball->getZ()+this->blockSize/2.0)) - (ball->getOrientationZ() * this->blockSize);

		//printf("next under block (%d,%d,%d)\n", x,y,z);

		if(next >= this->grid_width)
		{
			printf("7 FALLOUT! ");
			
			if(dirS < 0)
			{
				printf("a\n");
				ball->move(0, -halfBlock);
				ball->fallLeft();
				ball->realign();
				ball->move(0, -halfBlock);
				printf("new ball: (%f,%f,%f)\n", ball->getX(), ball->getY(), ball->getZ());
			}
			else
			{
				printf("b\n");
				ball->move(0, halfBlock);
				ball->fallRight();
				ball->realign();
				ball->move(0, halfBlock);
				printf("new ball: (%f,%f,%f)\n", ball->getX(), ball->getY(), ball->getZ());
			}
		}
		else if(this->block[this->getOffset(x,y,z)] == NULL)
		{
			printf("8 FALLOUT! ");
			if(dirS < 0)
			{
				printf("a\n");
				ball->move(0, -halfBlock);
				ball->fallLeft();
				ball->realign();
				ball->move(0, -halfBlock);
				printf("new ball: (%f,%f,%f)\n", ball->getX(), ball->getY(), ball->getZ());
			}
			else
			{
				printf("b\n");
				ball->move(0, halfBlock);
				ball->fallRight();
				ball->realign();
				ball->move(0, halfBlock);
				printf("new ball: (%f,%f,%f)\n", ball->getX(), ball->getY(), ball->getZ());
			}
		}
	}
	
	
	// right blocks
	else if(prevS > ceil(nextS) && nextS <= ceil(nextS))
	{
		//printf("got in here!\n");
		int last = (int)ceil(prevS);
		int next = (int)floor(nextS);
		/*printf("prevZ=%f, floor(prevZ)=%f, ceil(prevZ)=%f\n", prevZ, floor(prevZ), ceil(prevZ));
		printf("ballZ=%f, floor(ballZ)=%f, ceil(ballZ)=%f\n", ball->getZ(), floor(ball->getZ()), ceil(ball->getZ()));*/
		printf("Changed at %d <- %d\n", next, last);

		printf("Ball: (%f,%f,%f)\n", ball->getX(), ball->getY(), ball->getZ());

		int x = ((ball->getSideX()!=0)?next:floor(ball->getX()+this->blockSize/2.0))/* - (ball->getOrientationX() * this->blockSize)*/;
		int y = ((ball->getSideY()!=0)?next:floor(ball->getY()+this->blockSize/2.0))/* - (ball->getOrientationY() * this->blockSize)*/;
		int z = ((ball->getSideZ()!=0)?next:floor(ball->getZ()+this->blockSize/2.0))/* - (ball->getOrientationZ() * this->blockSize)*/;

		printf("block in front (%d,%d,%d)\n", x,y,z);

		if(validBlock(x,y,z) && this->block[this->getOffset(x,y,z)] != NULL)
		{
			printf("15 COLLISION!\n");
			/*ball->setX(prevX);
			ball->setY(prevY);
			ball->setZ(prevZ);*/
			if(dirS > 0)
			{
				//ball->move(-halfBlock, 0);
				ball->fallRight();
				//ball->realign();
				//ball->move(-halfBlock, 0);*/
			}
			else
			{
				//ball->move(halfBlock, 0);
				ball->fallLeft();
				//ball->realign();
				//ball->move(halfBlock, 0);*/
			}
		}
	}
	// left blocks
	else if(prevS <= floor(nextS) && nextS > floor(nextS))
	{
		int last = (int)floor(prevS);
		int next = (int)ceil(nextS);
		printf("Changed at %d -> %d\n", last, next);

		printf("Ball: (%f,%f,%f)\n", ball->getX(), ball->getY(), ball->getZ());

		int x = ((ball->getSideX()!=0)?next:floor(ball->getX()+this->blockSize/2.0))/* - (ball->getOrientationX() * this->blockSize)*/;
		int y = ((ball->getSideY()!=0)?next:floor(ball->getY()+this->blockSize/2.0))/* - (ball->getOrientationY() * this->blockSize)*/;
		int z = ((ball->getSideZ()!=0)?next:floor(ball->getZ()+this->blockSize/2.0))/* - (ball->getOrientationZ() * this->blockSize)*/;

		printf("block in front (%d,%d,%d)\n", x,y,z);

		if(validBlock(x,y,z) && this->block[this->getOffset(x,y,z)] != NULL)
		{
			printf("17 COLLISION!\n");
			/*ball->setX(prevX);
			ball->setY(prevY);
			ball->setZ(prevZ);*/
			if(dirS > 0)
			{
				//ball->move(halfBlock, 0);
				ball->fallLeft();
				//ball->realign();
				//ball->move(halfBlock, 0);*/
			}
			else
			{
				//ball->move(-halfBlock, 0);
				ball->fallRight();
				//ball->realign();
				//ball->move(-halfBlock, 0);*/
			}
		}
	}
}

bool Level::validBlock(int x, int y, int z)
{
	return (x >= 0 && x < grid_width && y >= 0 && y < grid_height && z >= 0 && z < grid_depth);
}

void Level::render()
{
	//printf("Render Level here\n");
	glPushMatrix();
	//printf("rotating: %d\n", this->rotate);
	
//	glTranslatef(camera->getEyeX(), camera->getEyeY(), camera->getEyeZ());
	glTranslatef(ball->getX()+this->offsetX+(this->blockSize/2), ball->getY()+this->offsetY+(this->blockSize/2), ball->getZ()+this->offsetZ+(this->blockSize/2));
	
	//glRotatef(this->rotate, 1.0, 0.0, 0.0);
	//glTranslatef(-1.5,-0.5,-1.5);
//	glRotatef(this->rotate, 1.0, 0.0, 0.0);
		glRotatef(this->rotF, ball->getSideX(), ball->getSideY(), ball->getSideZ());
		glRotatef(this->rotS, ball->getFacingX(), ball->getFacingY(), ball->getFacingZ());
	//glTranslatef(1.5,0.5,1.5);
	//camera->print();

	glTranslatef(-ball->getX()-this->offsetX-(this->blockSize/2), -ball->getY()-this->offsetY-(this->blockSize/2), -ball->getZ()-this->offsetZ-(this->blockSize/2));
//	glTranslatef(-camera->getEyeX(), -camera->getEyeY(), -camera->getEyeZ());

	glPushMatrix();
	draw_grid();
	glPopMatrix();

	/*glPushMatrix();
	glTranslatef(offsetX, offsetY, offsetZ);
	draw_unit_cube();
	glPopMatrix();*/

	/*glPushMatrix();
	this->ball->render();
	glPopMatrix();*/

	glPopMatrix();
}

void Level::renderCamera()
{
	camera->render();
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
    //glTranslatef(-(float)this->grid_width/2.0f, -(float)this->grid_height/2.0f, -(float)this->grid_depth/2.0f);
	glTranslatef(this->offsetX, this->offsetY, this->offsetZ);


	// Store this position
	/*glPushMatrix();

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
	glPopMatrix();*/


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
	this->ball->render();
	glPopMatrix();
}

int Level::getOffset(int i, int j, int k)
{
	return (i + (j * this->grid_width) + (k * this->grid_width * this->grid_height));
}

void Level::updateCameraPosition(bool lookat)
{
	ball->lookAtMe(camera);
	camera->addEye(this->offsetX, this->offsetY, this->offsetZ);
	camera->addAt(this->offsetX, this->offsetY, this->offsetZ);

	if(lookat)
		camera->updateLookAt();
}

void Level::turnLeft()
{
	ball->turnLeft();
}

void Level::turnRight()
{
	ball->turnRight();
}

#include "Ball.h"

Ball::Ball(GLfloat blockSize, GLfloat offsetX, GLfloat offsetY, GLfloat offsetZ)
{
	this->scale = 1.0f;

	printf("x=%f, y=%f, z=%f\n", offsetX, offsetY, offsetZ);

	this->x = 0 + offsetX;
	this->y = 0 + offsetY;
	this->z = 0 + offsetZ;

	this->radius = (blockSize * this->scale)/2;

	this->orientation = UP;
	this->facing = FRONT;

	this->model = NULL;

	if((this->model = load_model("models/ball.md3")) == NULL)
	{
		printf("Could not load model!\n");
	}
}

Ball::~Ball()
{
}

void Ball::render()
{
	glPushMatrix();

	glTranslatef(this->x, this->y, this->z);
	if(this->model == NULL)
		draw_unit_cube();
	else
		draw_md3_file(this->model);


	glPopMatrix();
}

void Ball::lookAtMe(Camera *c)
{
	GLfloat dist = 5;
	GLfloat ux=0, uy=1, uz=0;
	GLfloat ax=0, ay=0, az=0;
	switch(this->orientation)
	{
		case UP:	ux= 0; uy= 1; uz= 0; break;
		case DOWN:	ux= 0; uy=-1; uz= 0; break;
		case LEFT:	ux= 0; uy= 0; uz= 0; break;
		case RIGHT:	ux= 0; uy= 0; uz= 0; break;
		case FRONT:	ux= 0; uy= 0; uz= 0; break;
		case BACK:	ux= 0; uy= 0; uz= 0; break;
		default: ux=0; uy=1; ux=0; break;
	}
	switch(this->facing)
	{
		case UP:	ax= 0; ay= 0; az= 0; break;
		case DOWN:	ax= 0; ay= 0; az= 0; break;
		case LEFT:	ax= 1; ay= 0; az= 0; break;
		case RIGHT:	ax=-1; ay= 0; az= 0; break;
		case FRONT:	ax= 0; ay= 0; az=-1; break;
		case BACK:	ax= 0; ay= 0; az= 1; break;
		default: ax=0; ay=0; ax=1; break;
	}
	/*gluLookAt(	(this->x+this->radius) + (ax*dist) + (ux*dist), (this->y+this->radius) + (ay*dist) + (uy*dist), (this->z+this->radius) + (az*dist) + (uz*dist),
				this->x+this->radius, this->y+this->radius, this->z+this->radius,
				ux,uy,uz
			 );*/
	c->setEye( (this->x+this->radius) + (ax*dist) + (ux*dist), (this->y+this->radius) + (ay*dist) + (uy*dist), (this->z+this->radius) + (az*dist) + (uz*dist) );
	c->setAt( this->x+this->radius, this->y+this->radius, this->z+this->radius );
	c->setUp(ux, uy, uz);
}

void Ball::turnLeft()
{
	printf("Turn left!\n");
	if(this->orientation == UP && this->facing == FRONT)
		this->facing = RIGHT;
	else if(this->orientation == UP && this->facing == RIGHT)
		this->facing = BACK;
	else if(this->orientation == UP && this->facing == BACK)
		this->facing = LEFT;
	else if(this->orientation == UP && this->facing == LEFT)
		this->facing = FRONT;

}

void Ball::turnRight()
{
	printf("Turn right!\n");
	if(this->orientation == UP && this->facing == FRONT)
		this->facing = LEFT;
}


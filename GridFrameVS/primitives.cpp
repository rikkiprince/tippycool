
#include "primitives.h"

void draw_textured_unit_cube(GLuint frontTexture, GLuint backTexture, GLuint leftTexture, GLuint rightTexture, GLuint topTexture, GLuint bottomTexture)
{
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	//glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);

	glTexEnvi(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	static const float texture_coords[] = {
		1.f, 0.f,
		0.f, 0.f,
		0.f, 1.f,
		1.f, 1.f,
	};

    static const float front_vertices[] = {
        0.f, 0.f, 1.f,
        1.f, 0.f, 1.f,
        1.f, 1.f, 1.f,
        0.f, 1.f, 1.f,
    };
    static const float front_normals[] = {
        0.f, 0.f, 1.f,
        0.f, 0.f, 1.f,
        0.f, 0.f, 1.f,
        0.f, 0.f, 1.f,
    };
	glBindTexture(GL_TEXTURE_2D, frontTexture);
	glTexCoordPointer(2, GL_FLOAT, 0, texture_coords);
    glVertexPointer(3, GL_FLOAT, 0, front_vertices);
	glNormalPointer(GL_FLOAT, 0, front_normals);
    glDrawArrays(GL_QUADS, 0, 4);
	
    static const float back_vertices[] = {
        1.f, 0.f, 0.f,
        0.f, 0.f, 0.f,
        0.f, 1.f, 0.f,
        1.f, 1.f, 0.f,
    };
    static const float back_normals[] = {
        0.f, 0.f, -1.f,
        0.f, 0.f, -1.f,
        0.f, 0.f, -1.f,
        0.f, 0.f, -1.f,
    };
	glBindTexture(GL_TEXTURE_2D, backTexture);
	glTexCoordPointer(2, GL_FLOAT, 0, texture_coords);
    glVertexPointer(3, GL_FLOAT, 0, back_vertices);
	glNormalPointer(GL_FLOAT, 0, back_normals);
    glDrawArrays(GL_QUADS, 0, 4);

    static const float left_vertices[] = {
        0.f, 0.f, 0.f,
        0.f, 0.f, 1.f,
        0.f, 1.f, 1.f,
        0.f, 1.f, 0.f,
    };
    static const float left_normals[] = {
        -1.f, 0.f, 0.f,
        -1.f, 0.f, 0.f,
        -1.f, 0.f, 0.f,
        -1.f, 0.f, 0.f,
    };
	glBindTexture(GL_TEXTURE_2D, leftTexture);
	glTexCoordPointer(2, GL_FLOAT, 0, texture_coords);
    glVertexPointer(3, GL_FLOAT, 0, left_vertices);
	glNormalPointer(GL_FLOAT, 0, left_normals);
    glDrawArrays(GL_QUADS, 0, 4);

    static const float right_vertices[] = {
        1.f, 0.f, 1.f,
        1.f, 0.f, 0.f,
        1.f, 1.f, 0.f,
        1.f, 1.f, 1.f,
    };
    static const float right_normals[] = {
        1.f, 0.f, 0.f,
        1.f, 0.f, 0.f,
        1.f, 0.f, 0.f,
        1.f, 0.f, 0.f,
    };
	glBindTexture(GL_TEXTURE_2D, rightTexture);
	glTexCoordPointer(2, GL_FLOAT, 0, texture_coords);
    glVertexPointer(3, GL_FLOAT, 0, right_vertices);
	glNormalPointer(GL_FLOAT, 0, right_normals);
    glDrawArrays(GL_QUADS, 0, 4);

    static const float top_vertices[] = {
        0.f, 1.f, 1.f,
        1.f, 1.f, 1.f,
        1.f, 1.f, 0.f,
        0.f, 1.f, 0.f,
    };
    static const float top_normals[] = {
        0.f, 1.f, 0.f,
        0.f, 1.f, 0.f,
        0.f, 1.f, 0.f,
        0.f, 1.f, 0.f,
    };
	glBindTexture(GL_TEXTURE_2D, topTexture);
	glTexCoordPointer(2, GL_FLOAT, 0, texture_coords);
    glVertexPointer(3, GL_FLOAT, 0, top_vertices);
	glNormalPointer(GL_FLOAT, 0, top_normals);
    glDrawArrays(GL_QUADS, 0, 4);

    static const float bottom_vertices[] = {
        0.f, 0.f, 0.f,
        1.f, 0.f, 0.f,
        1.f, 0.f, 1.f,
        0.f, 0.f, 1.f,
    };
    static const float bottom_normals[] = {
        0.f, -1.f, 0.f,
        0.f, -1.f, 0.f,
        0.f, -1.f, 0.f,
        0.f, -1.f, 0.f,
    };
	glBindTexture(GL_TEXTURE_2D, bottomTexture);
	glTexCoordPointer(2, GL_FLOAT, 0, texture_coords);
    glVertexPointer(3, GL_FLOAT, 0, bottom_vertices);
	glNormalPointer(GL_FLOAT, 0, bottom_normals);
    glDrawArrays(GL_QUADS, 0, 4);

	
	glDisable(GL_TEXTURE_2D);
	//glShadeModel(GL_FLAT);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	//glEnableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}

void draw_textured_unit_cube(GLuint sameTexture)
{
	draw_textured_unit_cube(sameTexture, sameTexture, sameTexture, sameTexture, sameTexture, sameTexture);
}

void draw_unit_cube()
{
	glEnableClientState(GL_NORMAL_ARRAY);

    static const float front_vertices[] = {
        0.f, 0.f, 1.f,
        1.f, 0.f, 1.f,
        1.f, 1.f, 1.f,
        0.f, 1.f, 1.f,
    };
    static const float front_normals[] = {
        0.f, 0.f, 1.f,
        0.f, 0.f, 1.f,
        0.f, 0.f, 1.f,
        0.f, 0.f, 1.f,
    };
    glVertexPointer(3, GL_FLOAT, 0, front_vertices);
	glNormalPointer(GL_FLOAT, 0, front_normals);
    glDrawArrays(GL_QUADS, 0, 4);
	
    static const float back_vertices[] = {
        1.f, 0.f, 0.f,
        0.f, 0.f, 0.f,
        0.f, 1.f, 0.f,
        1.f, 1.f, 0.f,
    };
    static const float back_normals[] = {
        0.f, 0.f, -1.f,
        0.f, 0.f, -1.f,
        0.f, 0.f, -1.f,
        0.f, 0.f, -1.f,
    };
    glVertexPointer(3, GL_FLOAT, 0, back_vertices);
	glNormalPointer(GL_FLOAT, 0, back_normals);
    glDrawArrays(GL_QUADS, 0, 4);

    static const float left_vertices[] = {
        0.f, 0.f, 0.f,
        0.f, 0.f, 1.f,
        0.f, 1.f, 1.f,
        0.f, 1.f, 0.f,
    };
    static const float left_normals[] = {
        -1.f, 0.f, 0.f,
        -1.f, 0.f, 0.f,
        -1.f, 0.f, 0.f,
        -1.f, 0.f, 0.f,
    };
    glVertexPointer(3, GL_FLOAT, 0, left_vertices);
	glNormalPointer(GL_FLOAT, 0, left_normals);
    glDrawArrays(GL_QUADS, 0, 4);

    static const float right_vertices[] = {
        1.f, 0.f, 1.f,
        1.f, 0.f, 0.f,
        1.f, 1.f, 0.f,
        1.f, 1.f, 1.f,
    };
    static const float right_normals[] = {
        1.f, 0.f, 0.f,
        1.f, 0.f, 0.f,
        1.f, 0.f, 0.f,
        1.f, 0.f, 0.f,
    };
    glVertexPointer(3, GL_FLOAT, 0, right_vertices);
	glNormalPointer(GL_FLOAT, 0, right_normals);
    glDrawArrays(GL_QUADS, 0, 4);

    static const float top_vertices[] = {
        0.f, 1.f, 1.f,
        1.f, 1.f, 1.f,
        1.f, 1.f, 0.f,
        0.f, 1.f, 0.f,
    };
    static const float top_normals[] = {
        0.f, 1.f, 0.f,
        0.f, 1.f, 0.f,
        0.f, 1.f, 0.f,
        0.f, 1.f, 0.f,
    };
    glVertexPointer(3, GL_FLOAT, 0, top_vertices);
	glNormalPointer(GL_FLOAT, 0, top_normals);
    glDrawArrays(GL_QUADS, 0, 4);

    static const float bottom_vertices[] = {
        0.f, 0.f, 0.f,
        1.f, 0.f, 0.f,
        1.f, 0.f, 1.f,
        0.f, 0.f, 1.f,
    };
    static const float bottom_normals[] = {
        0.f, -1.f, 0.f,
        0.f, -1.f, 0.f,
        0.f, -1.f, 0.f,
        0.f, -1.f, 0.f,
    };
    glVertexPointer(3, GL_FLOAT, 0, bottom_vertices);
	glNormalPointer(GL_FLOAT, 0, bottom_normals);
    glDrawArrays(GL_QUADS, 0, 4);
}
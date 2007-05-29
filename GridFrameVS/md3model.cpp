#include "headers.h"

void fixPath(char * filename)
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

void draw_md3_file(libmd3_file *modelFile)
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

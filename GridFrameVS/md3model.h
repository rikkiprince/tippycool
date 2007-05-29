
void fixPath(char * filename);
void draw_md3_file(libmd3_file *modelFile);
int get_normals(libmd3_mesh * mesh);
void libmd3_unpack_normals(libmd3_mesh * mesh);
libmd3_file * load_model(const char * filename);

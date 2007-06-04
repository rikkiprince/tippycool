#ifndef __TYPES_H__
#define __TYPES_H__

				//   0    1      2      3     4      5      6       7
enum Orientation { none, TOP, BOTTOM, LEFT, RIGHT, FRONT, BACK, MAX_DIRS };

typedef struct
{
	int x;
	int y;
	int z;
} intXYZ;


#endif // __TYPES_H__


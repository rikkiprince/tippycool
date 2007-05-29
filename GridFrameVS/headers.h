#ifndef __HEADERS_H__
#define __HEADERS_H__

#ifdef WIN32
#include <Windows.h>
#define M_PI 3.14159265f
#endif

#include <SDL.h>

extern "C" {
#include <libmd3/structure.h>
#include <libmd3/loader.h>
#include <libmd3/convert.h>
#include <libmd3/mesh.h>
}

#include <GL/gl.h>
#include <GL/glu.h>

#include "font.h"

#include <iostream>

#include <cmath>
#include <cstring>

#include "AbstractBlock.h"
#include "NormalBlock.h"

#include "textures.h"
#include "primitives.h"
#include "md3model.h"

#include "Level.h"

#endif // __HEADERS_H__

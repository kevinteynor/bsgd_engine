#ifndef ENGINE_TYPE_DEFS_H
#define ENGINE_TYPE_DEFS_H

typedef unsigned int uint;
typedef unsigned short ushort;

#define PI 3.14159265359f
#define float_abs fabsf
#define INDEFINITE_DURATION (float)((uint)-1)

// DEBUGING DEFINES -- MUST BE DEFINED TO BE ENABLED!
#define USE_DEBUG_SHAPES
#define USE_DEBUG_MENUS
#define USE_VERTICAL_DEBUG_MENUS
#define LOGGING

#endif // ENGINE_TYPE_DEFS_H
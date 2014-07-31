#ifndef EXPORT_HEADER_H
#define EXPORT_HEADER_H

#ifdef DEBUG_EXPORTS
#define DEBUG_SHARED __declspec( dllexport )
#else
#define DEBUG_SHARED __declspec( dllimport )
#endif

#endif // EXPORT_HEADER_H
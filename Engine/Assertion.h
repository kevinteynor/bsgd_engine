#ifndef _ASSERTION_H_
#define _ASSERTION_H_

#ifdef ASSERTS
#include <cstdlib>
#include <intrin.h>
#include "Logger.h"
#define Assert(exp) do { if(!(exp)) { LOG_D("Assertion failed", Error, __FILE__, __LINE__); __debugbreak(); }} while(0)
#endif // ASSERTS

#ifndef ASSERTS
#define Assert(exp)
#endif // ASSERTS

#endif //_ASSERTION_H_
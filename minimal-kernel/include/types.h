#ifndef TYPES_H
#define TYPES_H

#ifndef NULL
#define NULL ((void*) 0)
#endif

typedef int bool;

typedef __signed char int8_t;
typedef unsigned char uint8_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef int int32_t;
typedef unsigned int uint32_t;
typedef uint32_t uintptr_t;

typedef unsigned int size_t;

#ifndef true
  #define true 1
#endif

#ifndef false
  #define false 0
#endif

#endif

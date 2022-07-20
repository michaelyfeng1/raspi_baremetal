#ifndef _TYPES_H_
#define _TYPES_H_

#define true    1
#define false   0
#define null    0
#define nullptr 0

#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef volatile uint32_t reg32;
typedef volatile uint64_t reg64;

#endif
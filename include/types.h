#ifndef TYPES_H
#define TYPES_H

// Include this here to basically include it everywhere
#include "config.h"

#define TRUE 1
#define FALSE 0
#define NULL ((void*)0)

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
typedef unsigned long long u64;
typedef signed long long s64;
typedef volatile unsigned char vu8;
typedef volatile signed char vs8;
typedef volatile unsigned short vu16;
typedef volatile signed short vs16;
typedef volatile unsigned int vu32;
typedef volatile signed int vs32;
typedef volatile unsigned long long vu64;
typedef volatile signed long long vs64;

#define SCHAR_MIN (-128)
#define SCHAR_MAX (127)
#define UCHAR_MAX (255)

#define SHORT_MIN (-32768)
#define SHORT_MAX (32767)
#define USHORT_MAX (65535)

#define INT_MIN (-2147483648)
#define INT_MAX (2147483647)
#define UINT_MAX (0xFFFFFFFFu)

typedef void(*Func_T)(void);

/**
 * @brief The amount of sub pixels a single pixel contains
 * 
 */
#define SUB_PIXEL_RATIO 4

/**
 * @brief The amount of pixels a full block contains
 * 
 */
#define PIXEL_PER_BLOCK 8

/**
 * @brief The size, in sub pixels, of a single full block
 * 
 */
#define BLOCK_SIZE (PIXEL_PER_BLOCK * SUB_PIXEL_RATIO)

/**
 * @brief The size, in sub pixels, of a single pixel
 * 
 */
#define PIXEL_SIZE (BLOCK_SIZE / PIXEL_PER_BLOCK)

/**
 * @brief A single sub pixel
 * 
 */
#define ONE_SUB_PIXEL (PIXEL_SIZE / PIXEL_SIZE)

/**
 * @brief The size, in sub pixels, of half of a block
 * 
 */
#define HALF_BLOCK_SIZE (BLOCK_SIZE / 2)

/**
 * @brief The size, in sub pixels, of a quarter of a block
 * 
 */
#define QUARTER_BLOCK_SIZE (BLOCK_SIZE / 4)

/**
 * @brief The size, in sub pixels, of three quarters of a block
 * 
 */
#define THREE_QUARTER_BLOCK_SIZE (HALF_BLOCK_SIZE + QUARTER_BLOCK_SIZE)

/**
 * @brief Mask to filter out the sub pixel coordinates, effectively clamping to a block
 * 
 */
#define BLOCK_POSITION_FLAG ((u16)~BLOCK_SIZE + ONE_SUB_PIXEL)

/**
 * @brief Mask to filter out the sub pixel coordinates, effectively clamping to a block
 * 
 */
#define SUB_PIXEL_POSITION_FLAG (BLOCK_SIZE - ONE_SUB_PIXEL)

#endif /* TYPES_H */

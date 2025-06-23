#ifndef TYPES_H
#define TYPES_H

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
#define UINT_MAX ((u32)0xFFFFFFFF)

typedef void(*Func_T)(void);

// Enables some hacky optimizations
#define HACKY_OPTIMIZATIONS

#endif /* TYPES_H */

#ifndef MACROS_H
#define MACROS_H

#include "types.h"

/**
 * @brief Gets the size of an array
 * 
 */
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

/**
 * @brief Performs a modulo (value % mod) operation on a value using the and operation (WARNING only use a value for mod that is a power of 2)
 * 
 * @param value Value
 * @param mod Modulo
 */
#define MOD_AND(value, mod) ((value) & ((mod) - 1))

/**
 * @brief This macro does nothing, it's only used to indicate that a function is implemented in asm rather than C
 * 
 */
#define ASM_IMPL

/**
 * @brief Adds an asm block
 *  
 * @param Asm code
 */
#define ASM(...) __asm__(__VA_ARGS__)

#ifdef MODERN
/**
 * @brief Performs a static assertion
 * 
 * @param expr Expression to assert
 * @param id Error message
 */
#define STATIC_ASSERT(expr, id) static_assert(expr, "Assert failed : " #id)
#else
/**
 * @brief Performs a static assertion
 * 
 * @param expr Expression to assert
 * @param id Struct name, this will be printed in the error message
 */
#define STATIC_ASSERT(expr, id) static char id[(expr) ? 1 : -1];
#endif /* MODERN */

#ifdef HACKY_OPTIMIZATIONS
// This is kind of bullshit.
// Since the compiler can't know if the array indexing (i.e. index * sizeof(arrayType)) will overflow above 255,
// it has to promote it to a u16, which makes the array indexing slower because the gameboy only has 8bit registers.
// So the optimization boils down to doing the array indexing ourselves by treating it as a u8* and performing the
// multiplication ourselves, which keeps the array index as an u8 during the multiplication.
// This does have a side effect of breaking indexing if index > 85, so it can't be applied everywhere.
#define HACKY_ARRAY_INDEXING(array, index, arrayType)                               \
((arrayType*)(((u8*)array) + (u8)(index * sizeof(arrayType))))
#endif /* HACKY_OPTIMIZATIONS */

#endif /* MACROS_H */

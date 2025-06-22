#ifndef MACROS_H
#define MACROS_H

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


#endif /* MACROS_H */

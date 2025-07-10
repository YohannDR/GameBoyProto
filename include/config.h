#ifndef CONFIG_H
#define CONFIG_H

// Enables some hacky optimizations
#define HACKY_OPTIMIZATIONS

// Indicates that the code is being compiled for modern platform
// #define MODERN

#ifdef MODERN
// Modern should not have hacky optimizations, those don't matter and some ot them are probably UB anyway
#undef HACKY_OPTIMIZATIONS
#endif /* MODERN */

#endif /* CONFIG_H */

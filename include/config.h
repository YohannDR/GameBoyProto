#ifndef CONFIG_H
#define CONFIG_H

// Enables some hacky optimizations
#define HACKY_OPTIMIZATIONS

// Enables fixes for some compiler bugs (SDCC 4.5.1)
#define COMPILER_FIXES

// Indicates that the code is being compiled for modern platform
// #define MODERN

#ifdef MODERN
// Modern should not have hacky optimizations, those don't matter and some ot them are probably UB anyway
#undef HACKY_OPTIMIZATIONS
#endif /* MODERN */

#endif /* CONFIG_H */

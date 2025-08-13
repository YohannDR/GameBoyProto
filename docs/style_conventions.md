# Conventions

This document shows the coding conventions of the project.
It is based on the [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html) and is trimmed to remove C++ specific conventions.
There is also a specific section dedicated to conventions for ASM.

## Summary

- [Conventions](#conventions)
  - [Summary](#summary)
- [C](#c)
  - [Header files](#header-files)
    - [The #define Guard](#the-define-guard)
    - [Forward declarations](#forward-declarations)
    - [Inline Functions](#inline-functions)
    - [Names and Order of Includes](#names-and-order-of-includes)
  - [Scoping](#scoping)
    - [Internal Linkage](#internal-linkage)
    - [Local Variables](#local-variables)
  - [Functions](#functions)
    - [Inputs](#inputs)
    - [Outputs](#outputs)
  - [Other C features](#other-c-features)
    - [Casting](#casting)
    - [Preincrement and Predecrement](#preincrement-and-predecrement)
    - [Use of `const`](#use-of-const)
      - [Integer types](#integer-types)
    - [Preprocessor macros](#preprocessor-macros)
    - [`sizeof`](#sizeof)
  - [Naming](#naming)
    - [General naming rules](#general-naming-rules)
    - [File names](#file-names)
    - [Type names](#type-names)
    - [Variable names](#variable-names)
  - [Comments](#comments)
    - [Spaces vs. Tabs](#spaces-vs-tabs)
    - [Function Calls](#function-calls)
    - [Looping and Branching Statements](#looping-and-branching-statements)
    - [Pointer expressions](#pointer-expressions)
    - [Preprocessor Directives](#preprocessor-directives)
    - [Horizontal Whitespace](#horizontal-whitespace)
    - [Vertical Whitespace](#vertical-whitespace)
- [ASM](#asm)
  - [Carry over from C conventions](#carry-over-from-c-conventions)
  - [Naming](#naming-1)
    - [Labels](#labels)
    - [Registers](#registers)
  - [Style](#style)

# C

## Header files

Header files should have the `.h` file extension.

Every `.c` file should have an associated `.h` file, with the only exception being the file containing the `main` function.

The opposite isn't forced, as such standalone `.h` are allowed, in which case the file should only contains preprocessor directives or typedefs.

### The #define Guard

The classic define guard must be used in every header file.
```c
#ifndef HEADER_H
#define HEADER_H

...

#endif // HEADER_H
```

### Forward declarations

The use of forward declarations is entirely fordibben, the proper header file must always be included when necessary.

### Inline Functions

Inline functions are forbidden.

### Names and Order of Includes

Include headers in the following order: Related header, Game Boy headers, project code headers, project data headers. These categories should be split using a blank line between each.

An `#include` directive should never use a path with directory aliases such as `.` or `..`. They should also only use forward slashes.

All includes should use quotes (`""`), the usage of brackets (`<>`) is forbidden.

The `#include` directives should also be sorted by alphabetic order.

For example, for a source file `sprite.c` the includes should be as follow:

```c
// Related header
#include "sprite.h"

// Game Boy headers
#include "gb/oam.h"
#include "gb/io.h"

// Project code headers
#include "macros.h"
#include "bg.h"

// Project data headers
#include "data/sprite_data.h"
```

## Scoping

### Internal Linkage

When definitions in a source file do not need to be referenced outside of this file (e.g. they are not declared in any header file), give them internal linkage by declaring them `static`.

### Local Variables

Local variables should always be declared at the beginning of the function, regardless of where they are used.
The declaration and initialization should also always be separated, even when it could possibly be merged.

```c
void Function(void)
{
    u8 i;

    // ...

    // Good, declared at the beginning of the function, declaration and initialization separated.
    i = 0;
}
```

```c
void Function(void)
{
    // ...

    // Bad, not declared at the beginning of the function.
    u8 i;
    i = 0;
}
```

```c
void Function(void)
{
    // Bad, declaration and initialization not separated.
    u8 i = 0;

    // ...
}
```

As a side effect, `if` initializers (initialization inside a control flow) are prohibited as well.

## Functions

### Inputs

Functions should limit their inputs as much as possible, ideally 3 or 4 maximum, otherwise consider passing some inputs via global variables.

In the case of a function having no inputs, declare it with `void` as the parameter.

### Outputs

Functions should only has a single output, only via its return value, using parameters as output values is fordibben.
In the case that a function needs multiple output values, write them to global variables.

```c
u8 ComputeSomething(void)
{
    // Good, the output is the return value
    return 8 * 8;
}
```

```c
void ComputeSomething(u8* result)
{
    // Bad, a parameter is an output
    *result = 8 * 8;
}
```

```c
u8 gResult;

u8 ComputeMuchThings(void)
{
    // Good, a global variable is an output
    gResult = 8 + 8;
    return 8 * 8;
}
```

```c
u8 ComputeMuchThings(u8* result)
{
    // Bad, a parameter is an output
    *result = 8 + 8;
    return 8 * 8;
}
```

## Other C features

### Casting

Only use casting if it's strictly necessary, or if it helps improve performance without changing the semantics of the code.

### Preincrement and Predecrement

Use the postfix form (`i++`) of the increment and decrement operators unless prefix semantics are required.

### Use of `const`

Use `const` only on data, to signify the compiler that it should be read-only. Any other usage is prohibited.

#### Integer types

Only use the integer types defined in the `types.h` header file.

```c
u8 num;             // Good
unsigned char num;  // Bad
```

```c
u16 num;            // Good
unsigned short num; // Bad
```

### Preprocessor macros

All constants should be defined using a `#define`.

Macros should be limited to simple operations where using a function would be overkill.

### `sizeof`

Prefer `sizeof(varName)` to `sizeof(varType)`.

This allows the `sizeof` to automatically change if the type of the variable changes.

`sizeof(varType)` is allowed when there is no variable in the context of the `sizeof`.

## Naming

### General naming rules

The priority when naming should always be clarity, the name should reflect the purpose of the object, abbreviation for the sake of abbreviation or to save space is strongly discouraged.

### File names

Files names should be in `snke_case`.

### Type names

Type names should be in `PascalCase`.

### Variable names

- Common variable : `camelCase`
- Struct variable : `camelCase`
- Function : `PascalCase`
- Global variables : `PascalCase` with a `g` prefix
- Constant data : `PascalCase` with a `s` prefix
- `#define` macros: `UPPER_CASE`
- `enum` constants: `UPPER_CASE`

## Comments

Comments should be used in multiple places to explain what the code does and to document it.

Comments for the purpose of explanations should use double forward slashes (`//`) for each line.

Comments for the purpose of documentation should be formatted in the [Javadoc style](https://www.oracle.com/fr/technical-resources/articles/java/javadoc-tool.html).

```c
/**
 * @brief This is helpful documentation for gSomeGlobal
 * 
 */
u8 gSomeGlobal;

/**
 * @brief This is helpful documentation for SomeFunction
 * 
 * @param param1 This is documentation for the parameter "param1"
 * @param param2 This is documentation for the parameter "param2"
 * @return u8 This is documentation for the return value
 */
u8 SomeFunction(u8 param1, u8 param2);
```

### Spaces vs. Tabs

Use only spaces, and indent 4 spaces at a time.

### Function Calls

Function calls have the following format:

```c++
result = DoSomething(arg1, arg2, arg3);
```

If the arguments do not all fit on one line:

```c++
result = VeryVeryVeryVeryLongFunctionName(
    arg1,
    arg2,
    arg3,
    arg4,
    arg5,
    arg6
);
```

### Looping and Branching Statements

At a high level, looping or branching statements consist of the following components:

- One or more statement keywords (e.g. if, else, switch, while, do, or for).
- One condition or iteration specifier, inside parentheses.
- One or more controlled statements, or blocks of controlled statements.

For these statements:

- The components of the statement should be separated by single spaces (not line breaks).
- Inside the condition or iteration specifier, put one space (or a line break) between each semicolon and the next token, except if the token is a closing parenthesis or another semicolon.
- Inside the condition or iteration specifier, do not put a space after the opening parenthesis or before the closing parenthesis.
- Put any controlled statements inside blocks (i.e. use curly braces) except if is only a single line.
- Inside the controlled blocks, put one line break immediately before and after the opening and closing braces.

```c++
if (condition)
{
    DoSomething();
    ...
}
else
{
    DoSomethingElse();
    ...
}

while (condition)
{
    Foo();
    ...
}

do
{
    Bar();
    ...
}
while (condition);

for (int i = 0; i < 10; i++)
    gSomeGlobal[i] = 0;

for (;;)
{
    // Infinite loop
    DoSomethingForever();
}

switch (value)
{
    case 1:
        DoSomething();
        break;

    // Add a scope because we initialize a local with the same name of another case
    case 2:
    {
        float f = 5.f;
        Consume(f);
        break;
    }

    case 3:
    {
        float f = 6.f;
        ConsumeDifferently(f);
        break;
    }
}
```

### Pointer expressions

No spaces around period or arrow. Pointer operators do not have trailing spaces.

The following are examples of correctly-formatted pointer expressions :

```c
x = *p;
p = &x;
x = r.y;
x = v->y;
```

It is disallowed to declare multiple variables in the same declaration.

Good declarations :

```c
u8 i;
u8 j;
```

Bad declarations :

```c
u8 i, j;
```

### Preprocessor Directives

The hash mark that starts a preprocessor directive should always be at the beginning of the line.

Even when preprocessor directives are within the body of indented code, the directives should start at the beginning of the line.

```c++
void Func()
{
    DoSomething()
#ifdef DEFINE
    Foo();
#else
    Bar()
#endif
}
```

### Horizontal Whitespace

Use of horizontal whitespace depends on location. Never put trailing whitespace at the end of a line.

### Vertical Whitespace

Do not use excessive empty lines. You should almost never have two empty lines following one another.

# ASM

## Carry over from C conventions

Most of conventions declared in the C section also apply to ASM, however ASM as a few more cases to cover.

## Naming

### Labels

Labels should be `PascalCase` and always at the beginning of the line.

### Registers

Registers should be fully lower case.

## Style

All instructions should be indented once, and all parameters should have only a single space separating them (with a comma if necessary).

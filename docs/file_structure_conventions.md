# Structure

This document shows the file structure conventions of the project.

## Summary

- [Structure](#structure)
  - [Summary](#summary)
  - [Root folders](#root-folders)
  - [Root files](#root-files)
  - [Mirror structure](#mirror-structure)
  - [Subfolders](#subfolders)
  - [Intermediate files](#intermediate-files)
  - [Build output](#build-output)

## Root folders

Only these 5 folders are allowed at the root of the project :

- `asm` : Contains hand-written assembly files (`.s` files).
- `docs` : Contains markdown files for the sole purpose of documentation.
- `include` : Contains every header file (`.h` files).
- `obj` : Contains the build output files.
- `src` : Contains every source code file (`.c` files).

And any special folders that need to be at the root (`.vscode` and `.github`).

## Root files

Only these 3 files are allowed at the root of the project due to being strictly required to be here :

- `.gitignore`
- `MakeFile`
- `README.md`

## Mirror structure

The `include` and `src` folder should be mirrors to each other, that is to say they must have the same sub-folder structure, and they must each have an equivalent for every file in the same place (with some exceptions, explained in the style document).

For example :
```
src
│   player.c
│   sprite.c
│
└───sprites_AI
    │   fire.c
    |   moving_platform.c
```
```
include
│   player.h
│   sprite.h
│
└───sprites_AI
    │   fire.h
    |   moving_platform.h
```

## Subfolders

There are 4 subfolders in the project :

- `data` : Data only files.
- `gb` : Game Boy hardware related files.
- `sprites_AI` : AIs for the sprites.
- `data/rooms` : Room data only files.

## Intermediate files

Intermediate files are located next to the file used to generate them. As such, both the folders `asm` and `src` contain intermediate files.

There are 2 types of intermediate files :

- `.s` files, generated from `.c` files.
- `.o` files, generated from `.s` files.

## Build output

The `obj` folder contains the build output files, there are currently 3 output files :

- A `.gb` file, the output ROM.
- A `.ihx` file, a pre-rom intermediate file that's used to perform validation.
- A `.map` file, a file containing a map of all the ROM's symbols, used for debugging purposes.

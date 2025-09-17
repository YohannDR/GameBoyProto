# Bank structure

This document describes how the data is laid out between the different banks.

# Bank 0, main bank

Contains critical code, main game loop, game systems (sprites, fire, door, fading...).
Basically everything that is or could be called every frame during the main loop is there.

# Bank 1, data bank

Contains tilesets, room data and door data.
Everything in this bank is copied to buffers in RAM, or directly uploaded to VRAM, so it can be separated from the rest.

# Bank 2, sprite bank

Contains sprite related code and data.

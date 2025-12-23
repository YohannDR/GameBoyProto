# What is the project ?

Microvania's coffee is a pacifist and minimalist mini Metroidvania, crafted for the good old Game Boy (DMG001) to be released on a real cartridge.

#### Pacifist
- There will be no enemies, no battles, and no bosses.
- The (single) new skill will exclusively be used for exploration and puzzle-solving.
- It is not possible to die
- No destruction, the main goal is even to repair something : the fantastic coffee machine.
##### Minimalist
- No text: playable in any language, even if the player can't read.
- Only one skill to unlock.
- 2-hour playtime, so no need to save.
The game is easily achieved on a rainy Sunday afternoon.
- ROM smaller than 32 KB, compatible with inexpensive cartridges.
#### Metroidvania
- Classic 2D platformer.
- Non-linear map to explore.
- Only one skill to unlock areas of the map.

## 3C

### Camera 
Microvania's coffee is a side-scrolling 2D platformer. The map is divided into many rooms of 32 × 32 tiles of 8 × 8 pixels each to comply with the technical limitations of the DMG 001. The camera will follow the player with a smooth transition between each room. 

**Optional:** The player can sometimes use a door to travel from one room to another one with a smooth fading effect.
### Character 
The single skill that will be unlocked is an ability to change the size of the character if he goes over a specific portal.

There are only 2 sizes: a big one and a small one.
The game starts with the character in his big size.
He will become small when the player finds the shrinking portal
He will then need an expansion portal to become big back.

The shrinking portal cannot make the character bigger. 
The expansion portal cannot make the character smaller.

This change will impact the physics of the character to let him jump higher and go to smaller locations, but he will not be able to activate the switches on the floor anymore.

| Metric | Big | Small |
| ------------- | :--: | :---: |
| Size X | X | X | 
| Size Y | X | X | 
| Acceleration | X | X | 
| Deceleration | X | X |
| VelocityCap X | X | X |
| VelocityCap Y | X | X |
| gravityUpwards | X | X |
| gravityDownwards | X | X |
| jumpingVelocity | X | X |

### Controls
 The game will use the classical controls for a side scrolling platformer : 
 - Move left and right with the D-Pad
 - Jump with the A button
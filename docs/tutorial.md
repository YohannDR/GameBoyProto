# Tutorial

This document describes how to best perform certain actions.

## Summary

- [Tutorial](#tutorial)
  - [Summary](#summary)
  - [Build](#build)
  - [Where to put what](#where-to-put-what)
- [Editor](#editor)
  - [Launching the editor](#launching-the-editor)
  - [Room editor](#room-editor)
  - [Graphics editor](#graphics-editor)
  - [Animation editor](#animation-editor)
  - [Tileset editor](#tileset-editor)
  - [Collision table editor](#collision-table-editor)
  - [Physics editor](#physics-editor)
  - [Adding](#adding)
  - [Other](#other)
- [Git](#git)
  - [Push](#push)
  - [Pull](#pull)
  - [Commit](#commit)

## Build

In order to build the ROM, you need to have a WSL window open in the root folder of the project.

To do this you can either open it directly via the file explorer by typing WSL in the path, or you can open a WSL window and cd all the way to the project.

Then in order to build, simply use the command `make -j`.

The ROM will be located in the `obj` folder.

## Where to put what

When creating a new resource, you should be mindful of where you put it, the location should both make sense, and be compliant with the structure convention of the project.

You should first look to see if there's already a suitable file for your resource, for example, if you're creating something related to the player, you should put it in the already existing `src/player.c` file.

If a suitable file doesn't exist, you should create a new one, the name and location should make sense and be compliant with the structure convention of the project.

# Editor

## Launching the editor

Every time you launch the editor, you have to locate the project folder in `File -> Locate Project`.

The path you need to input is the root folder of the project, when pressing enter to validate, 3 things can happen :

- The input window closes and the room editor appears, this means the project was successfully loaded.
- Nothing happens, this means your path is invalid.
- The editor crashes, make sure you have the latest version of both the editor and the project.

## Room editor

The room editor is split into 3 parts :

- On the top left are the settings/controls, this is where you can select the room and edit its properties.
- Right below is the graphics view, here you can select with which to view the room, as well as see the graphics themselves.
- On the right is the room view, this is where the actual room is visible.

In the room view, the background is rendered, as well as any placed sprites and doors, using green and blue rectangles respectively.

The room editor has 2 editing modes, tile mode and object mode, as the names imply, in tile mode you can only edit the tiles (aka the background) and in object mode you only edit the objects (aka sprites and doors). You can switch modes by either selecting the one you want in the settings panel, or by pressing the middle mouse button to toggle between them.

While in tile editing mode, you can hold right click to select a region of tiles in either the room or in the graphics to copy it.
You can then use left click to paste the tiles in the room.

While in object edit mode, you can drag around any object, or you can press right click to open a context menu that allows you to add, edit, or delete objects. You can add object anywhere (provided you are below the limit for said object) but you need to right click on a specific object to edit or delete it. Choosing to edit an object will open a small window that allows you change the properties of the object.

In the settings window, you can change the size of the room, simply change either the width or height value and press to `Resize` button to apply.
This operation is lossy, meaning that if you shrink the room, you will lose what was removed so be careful.

## Graphics editor

The graphics editor is split into 3 parts :

- At the top are the settings, this is where you can select your graphics and edit the palette.
- On the left is selected tile.
- On the right are the graphics.

You can select a tile by using the slider in the selected tile window, or by clicking on a tile in the graphics window.

In the selected tile window, you can use left click to paint over the pixels using the selected color, you can also hold left control while clicking to perform a fill.

In the graphics window, you can click on `Add tile` to add a blank tile at the very end of the graphics, and you can click on `Delete tile` to delete the currently selected tile.

## Animation editor

The animation editor is split into 6 parts :

- At the top are the settings, this is where you select the animation and which graphics to use.
- On the left are the controls.
- Below that is the frame section.
- Below that is the part section.
- On the right are the graphics.
- Below that is the rendered animation.

In the controls, you can change the zoom of the rendered animation, choose whether to draw the origin (coordinate 0;0) and play/stop the animation.

In the frame section, you can change the currently selected frame and edit its duration, you can also add a new frame using the + button (it will be added before the currently selected one) or delete the currently selected frame with the - button.

In the part section, you can change the currently selected part and edit all of its [properties](https://gbdev.io/pandocs/OAM.html). You can also add or remove parts.

The graphics view is purely visual.

The rendered animation is also purely visual, with the only exception being that you can click on a tile to select its corresponding part.

## Tileset editor

The tileset editor is split into 2 parts :

- The selection to add a new tileset.
- The list of the current tilesets.

You can add a new tileset by selecting graphics and pressing `Add`. You can remove a tileset by pressing on the - button next to its name.

## Collision table editor

The collision table editor is split into 3 parts :

- At the top is the graphics selector.
- On the left is the collision table view.
- On the right are the graphics.

It's not required to select graphics, however since each entry in the collision table is associated with a tile, it helps visualize.

The association is shown using the `x` on the right of the entries, the red one is the tile you have selected in the graphics, it also works the other way around so you can click on an `x` to select the tile and vice versa.

If you change the graphics, you can automatically resize the collision table by pressing the `Resize table to fit graphics`. The default collision will be CLIPDATA_AIR.

## Physics editor

The physics editor simply has inputs for the the different player physics values, if you hover over them they provide a tooltip to further explain what the value signifies.

Note that the values can be either signed or unsigned.

## Adding

The only way to add a resource is by using the `Add` menu, then selecting the type of resource you wish to create.
The required information will vary depending on the type of resource :

- Graphics : Requires a file and a name.
- Animation : Requires a file and a name.
- Room : Requires a collision table and an origin location (you may ignore origin location for now).
- Collision table : Requires a name.

When providing a name, keep in mind the naming rules for data (`PascalCase` with a `s` prefix).

When providing a file, input the whole relative path from the root folder using forward slashes only, for example : `src/player.c`.

## Other

You can freely resize almost every window and sub window.

You can also move, re-order and dock all of the main editor windows.

You can either use `File -> Save` to save or the keyboard shortcut `ctrl + S`.

You can either use the arrow buttons (`<` and `>`) in the menu bars of the main windows to undo/redo, or use `ctrl + Z` and `ctrl + Y` respectively.

**DO NOT** use the `File -> Build` button, build externally using WSL as explained in [this section](#build).

The editor does not hot reload, meaning that if you manually change the source code, you have to restart it.

# Git

## Push

Pushing is the action of publishing your changes to the repository.

To push you first need to [commit](#commit), then press the rightmost button at the top below the menu bar.

You should only push when you "finish" doing something, and it's properly working. For example, finishing the level design of a room, or creating new graphics. Don't wait until the end of the day to push, it's generally better to do multiple small pushes rather than a big one.

If you get a message when pushing, **DON'T DO ANYTHING** and immediately contact me.

## Pull

Pulling is the action of getting the latest changes from the repository.

You first need to fetch before pulling, both actions are done using the same button in GitHub Desktop, it's the rightmost button at the top below the menu bar.

It's generally good practice to always pull when there's changes available, so do it often, especially at the start of the day or when you start working.

If you get a message when pulling, **DON'T DO ANYTHING** and immediately contact me.

## Commit

Commiting involves selecting which files you want to commit and writing a summary message. By default, all files are selected, but you can cherry pick them at will by simply ticking or unticking them.

You have to write your commit message in the box in the bottom left corner, the thin one above the description.

Commit messages should be a clear as possible, I should be able to know **exactly** what you've done just by reading the message. They should be concise, so feel free to omit words to get to the point, for example do `Room 0 LD` instead of `Made level design for the room 0`.
They should begin with an uppercase letter (`Add thing` instead of `add thing`) and be written in English exclusively.

If you get a message when commiting, **DON'T DO ANYTHING** and immediately contact me.

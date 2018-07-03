# 3D Roguelike

An engine test for a potential 3D roguelike project with a unique rendering technique.
## Current Features

  - Map generation using noise;
  - Entity system.
  - Map rendering

## Controls

 - UP, DOWN, LEFT, RIGHT, PGUP, PGDOWN -> movement
 - HOME   -> enable/disable collision
 - END    -> build Non Blocking walls
 - INSERT -> build walls
 - DELETE -> destroy walls
 - F1/F2  -> change downward vision
 - F3     -> change rendering method

## Requirements

 - [SDL](https://www.libsdl.org/)
 - [LIBTCOD (1.5.2)](http://roguecentral.org/doryen/libtcod/)

## Building

I am not aware how to do it in other systems but the way I do it is on Windows letting VS2017+ compile it while linking the LIBTCOD 1.5.2 library.
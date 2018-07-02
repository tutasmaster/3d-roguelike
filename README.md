# TutorialRL

This is a roguelike I made in C++ and Libtcod as an attempt to wrap my mind around game/engine development. 
This code was heavily based on Roguebasin's libtcod tutorial at 
http://www.roguebasin.com/index.php?title=Complete_roguelike_tutorial_using_C%2B%2B_and_libtcod_-_part_1:_setting_up .

## Features

  - Map generation using libtcod's BspListener;
  - Basic inventory system;
  - Versatile entity system.

## Controls

 - UP, DOWN, LEFT, RIGHT or 8,2,4,6 -> movement
 - I -> inventory/use
 - G -> pick up item
 - 5 -> wait a turn
 - D -> drop item
 - L -> look around

## Requirements

 - [SDL](https://www.libsdl.org/)
 - [LIBTCOD (1.5.2)](http://roguecentral.org/doryen/libtcod/)

## Building

I am not aware how to do it in other systems but the way I do it is on Windows letting VS2017+ compile it while linking the LIBTCOD 1.5.2 library.

## Todo

 - Adding armor system;
 - Adding weapon system;
 - More content;
 - Better ai using A* for pathfinding;
 - Spells;
 - Scoring system.

****
**Thank you for your attention!**
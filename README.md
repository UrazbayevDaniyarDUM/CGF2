# Assignment 2: OpenGL / GLUT

## Part 1: Windows, menus and animation
Main window with a square, a subwindow with an ellipse, and "window 2" with
a circle and a triangle. Right-click menus, keyboard color control
(r, g, b, y, o, p, w), idle-function animation.

## Part 2: 3D cube
Colored cube with depth test. Transformations (scale, rotate, translate)
are applied in the vertex shader in this order: scale, rotation, translation.

Controls:
- Right click: choose SCALE / ROTATE / TRANSLATE
- q/a, w/s, e/d: increase/decrease X, Y, Z
- + / -: increase/decrease delta
- r: reset, Esc: exit

## Build
```
cmake -B build
cmake --build build
```
Requires: freeglut, GLEW, OpenGL.
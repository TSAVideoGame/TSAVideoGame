# (M)jir Enjin

2D game 'engine' framework thingy

As of rn it's still unfinished, probably never going to be but I'm gonna make some progress

C99 engine ig

## Big thanks to these libraries
- OpenAL
- OpenGL
- cglm
- lodepng

## Res note
In this repo I've left out res/images and res/sounds, cloning and building will
fail if there is no .wav bgm in res/sounds, the res/images might fail, so find
resources to use for those and then it should be good. (Errors on resource loading
are a todo...)

## Main repo layout
- src
- - driver
- - - Runs the code
- - core
- - - Windowing, inputs, stuff for game loop, there's a seperate repo with just this (JirJin organization)
- - gfx
- - - Graphics
- - resm
- - - Resource manager
- - snd
- - - Sound
- - stm
- - - State manager

These folders can be thought of as modules. Modules not listed here are extra stuff.

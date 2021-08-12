# (M)jir Enjin

2D game 'engine' framework thingy

As usual this isn't finished yet, here is a todo

## Todo
PNG loading
Animations
Game loop

## Next step
Bgm music

## Finished
States
Resource Manager
Sfx loading

## Big thanks to these libraries
OpenAL
OpenGL (nothing in rn, but will be used for graphics when I get there)

## Memory leaks
I am using valgrind on linux (Ubuntu) to check for leaks

There will be bytes in 'still reachable', but those are usually going
to be because of libraries, the goal should be 0 bytes in 0 blocks for
every other category.

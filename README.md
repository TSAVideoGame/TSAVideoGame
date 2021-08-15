# (M)jir Enjin

2D game 'engine' framework thingy

As usual this isn't finished yet, here is a todo

## Big thanks to these libraries
OpenAL
OpenGL
glfw
GLEW

## Memory leaks
I am using valgrind on linux (Ubuntu) to check for leaks

There will be bytes in 'still reachable', but those are usually going
to be because of libraries, the goal should be 0 bytes in 0 blocks for
every other category.

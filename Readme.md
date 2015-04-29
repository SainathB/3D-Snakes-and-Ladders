3D Snakes and Ladders
====

A snakes and ladders game implementation in OpenGL.

Warning: Sphaggeti code ahead :P

Here's a screenshot:
![Screenshot](/img/screenshot.png)

## Building
On Linux, install opengl (freeglut3), SOIL and make
`sudo apt-get install freeglut3-dev libsoil-dev make`
To compile, run following command
` g++  -std=c++11 main.cpp Game.cpp  -o game -lGL -lGLU -lglut -lSOIL
Run the game executable
`./game`


![Bezier Curves
](https://i.imgur.com/ABPrMdq.png)
## Cubic Bézier Curves
A Very simple program that draws cubic [Bézier curves](https://en.wikipedia.org/wiki/B%C3%A9zier_curve).

## Requirements
To compile you need [LibSDL2](https://www.libsdl.org/) development libraries installed.
### Linux
On Linux you need to install the following packages: `libsdl2-dev`  `libsdl2_image-dev`.
### Windows
On Windows you have to download `SDL2-devel-2.x.x-mingw.zip` from the [SDL github releases page](https://github.com/libsdl-org/SDL/releases)
and [MinGW](https://sourceforge.net/projects/mingw/). 
Add the MinGW bin directory to your `PATH` environment variable.
In the file `compile.bat` change `-I"include"` and `-L"lib"` replacing `include` and `lib` with the path to the corresponding folders inside the `SDL2-devel-2.x.x-mingw` you've downloaded earlier.

## Compiling 
### Linux
Run `compile.sh`
### Windows
Run `compile.bat`

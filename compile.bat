gcc main.c game.c bcurve.c -static -I"C:\SDL2-2.26.4-mingw\i686-w64-mingw32\include" -L"C:\SDL2-2.26.4-mingw\i686-w64-mingw32\lib" -lSDL2 -lm -lmingw32 -mwindows -Wl,--dynamicbase -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lsetupapi -lversion -luuid -o bcurves.exe
pause


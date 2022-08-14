@echo off
gcc main.c src/asset_manager.c src/logging.c src/graphics.c src/engine.c -IC:\sdl-mingw\include\SDL2 -LC:\sdl-mingw\lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -std=c99 -o synthesis
"synthesis.exe"

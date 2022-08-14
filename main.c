#include <stdio.h>

//read(enter, only write numbers, arrowkeys write in middle of string)
//window sizes
#include "src/engine.h"

int main(){
	ENGINE engine = {0};
	GRAPHICS window = {"Synthesis", 800, 640, SDL_RENDER};
	ENGINE_init(&engine, window, 60);
	SYNTH_RECT r = {0, 100, 400, 3*8};
	SYNTH_RECT dr = {3, 300, 400, 100};
	SYNTH_COLOR c = {0, 0, 255};
	u_int font = GRAPHICS_loadBMP("rsc/mc_font.bmp", &engine.window);
	u_int mages = GRAPHICS_loadBMP("rsc/mages.bmp", &engine.window);
	FONT mc_font = GRAPHICS_loadFontBMP(engine.window, font);
	SPRITE slime[4];
	for(u_char i = 0; i < 4; i++)
		slime[i] = GRAPHICS_createSprite(i, 8, 8, mages, engine.window);
	while(engine.running == 1){
		ENGINE_startTimer(&engine.fps);
        ENGINE_event(&engine);
		GRAPHICS_renderClear(engine.window);
		GRAPHICS_printS(0, 500, 3.0, "Hello World", engine.window, mc_font);
		GRAPHICS_renderBMP(0, 0, 3.5, slime[engine.fps.count/16%4], engine.window);
		GRAPHICS_renderUpdate(engine.window);
		ENGINE_fps(&engine.fps);
	}
	ENGINE_exit(&engine);
}



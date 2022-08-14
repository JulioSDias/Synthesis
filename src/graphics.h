#include "utils.h"
#include "logging.h"
#include "asset_manager.h"

struct{
	int x;
	int y;
	int width;
	int height;
}typedef SYNTH_RECT;

struct{
	SYNTH_RECT on_map;
	void *texture;
}typedef SPRITE;

struct{
	u_int r;
	u_int g;
	u_int b;
}typedef SYNTH_COLOR;

struct{
	float r;
	float g;
	float b;
}typedef F_SYNTH_COLOR;

struct{
	SPRITE chars[80];
	u_char array_chars[80];
}typedef FONT;

struct{
	char *window_title;
	int width;
	int height;
	int render_flag; 
	void *window;
	void *renderer;
	SYNTH_COLOR color;
	ASSET_STACK texture_manager;
}typedef GRAPHICS;


//	Initialize renderer and create window.
bool GRAPHICS_init(GRAPHICS *graphics);

//	Destroy renderer and window. Free asset manager.
bool GRAPHICS_free(GRAPHICS *graphics);

//	Loads bitmap to asset manager.
u_int GRAPHICS_loadBMP(u_char *path, GRAPHICS *graphics);

//	Creates texture(sprite) of position in bitmap.
SPRITE GRAPHICS_createSprite(u_int position, u_int width, u_int height, u_int address, GRAPHICS graphics);

//	Places texture(sprite)
bool GRAPHICS_renderBMP(u_int x, u_int y, float scale, SPRITE sprite, GRAPHICS graphics);
FONT GRAPHICS_loadFontBMP(GRAPHICS graphics, u_int address);

void GRAPHICS_printS(u_int x, u_int y, float scale, char *output, GRAPHICS graphics, FONT font);
void GRAPHICS_printI(u_int x, u_int y, float scale, int output, GRAPHICS graphics, FONT font);
void GRAPHICS_printF(u_int x, u_int y, float scale, float output, GRAPHICS graphics, FONT font);
void GRAPHICS_fillRect(SYNTH_RECT rect, SYNTH_COLOR color, GRAPHICS graphics);
void GRAPHICS_drawRect(SYNTH_RECT rect, SYNTH_COLOR color, GRAPHICS graphics, u_int thickness);
void GRAPHICS_drawRectS(SYNTH_RECT rect, SYNTH_COLOR color, GRAPHICS graphics);
void GRAPHICS_line(u_int x1, u_int y1, u_int x2, u_int y2, SYNTH_COLOR color, GRAPHICS graphics);

void GRAPHICS_renderClear(GRAPHICS graphics);
void GRAPHICS_renderUpdate(GRAPHICS graphics);

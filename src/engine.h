#include "utils.h"
#include "graphics.h"

enum {SYNTH_UP, SYNTH_DOWN, SYNTH_LEFT, SYNTH_RIGHT, SYNTH_ESCAPE, SYNTH_BACKSPACE, SYNTH_SPACE, SYNTH_LSHIFT, SYNTH_RSHIFT, SYNTH_RETURN, SYNTH_F1, SYNTH_F2, SYNTH_F3, SYNTH_F4, SYNTH_F5, SYNTH_F6, SYNTH_F7, SYNTH_F8, SYNTH_F9, SYNTH_F10, SYNTH_F11, SYNTH_F12, BUTTON_COUNT};

struct{
	int x;
	int y;
}typedef MOUSE;


/*	x, y -> top left corner coords
	scale -> how big
	size -> nº of chars
	stack_num -> must be initialized to -1. Value atributed by function, stored in INPUT string_manager.
	active -> enable/disable box. */
struct{
	u_int x;
	u_int y;
	float scale;
	u_int size;
	int stack_num;
	bool active;
}typedef INPUT_BOX;

struct{
    bool is_down;
    bool changed;
}typedef BUTTON;

struct{
    BUTTON button[BUTTON_COUNT];
	u_int key[BUTTON_COUNT];
	MOUSE mouse;
	ASSET_STACK string_manager;
}typedef INPUT;

struct{
	u_int count;
	float raw;
	float cooked;
	float accum;
	float start_time;
	float target_time;
}typedef FPS;

struct{
	GRAPHICS window;
	INPUT input;
	FPS fps;
	bool running;
}typedef ENGINE;


/* 	Start func to enable lib.
	Engine -> main struct(always initialize '= {0};').
	Window -> basic window definitions.
	Target_fps -> desired fps target. */
int ENGINE_init(ENGINE *engine, GRAPHICS window, u_int target_fps);

// 	Link SDL input with Synthesis input.
bool ENGINE_inputInit(ENGINE *engine);

// 	Wrapper SDL_Delay.
void ENGINE_delay(u_int time);


//	Convert target FPS to ms.
float ENGINE_initFPS(u_int target_fps);

//	Called in the beggining of main loop. Gets SDL_GetTicks() to start count.
bool ENGINE_startTimer(FPS *fps);

//	Calcs fps in ms. call at the end of main loop.
float ENGINE_fps(FPS *fps);

//	Creates a text box for input. Outputs pointer to string written.
//	Needs an INPUT_BOX  struct initialized.
char *ENGINE_inputText(ENGINE *engine, FONT font, INPUT_BOX *input_box);

//	Event Manager.
int ENGINE_event(ENGINE *engine);

/*	Press a key(1 frame). 
	b -> key in enum top of engine.h. 
	input -> struct in ENGINE */
bool PRESSED(u_int b, INPUT input);

/*	Release a key(1 frame). 
	b -> key in enum top of engine.h. 
	input -> struct in ENGINE */
bool RELEASED(u_int b, INPUT input);

// 	INCOMPLETE
bool HOLD(u_int b, INPUT input);

// 	Gives mouse coords
void MOUSE_COORDS(u_int *x, u_int *y, INPUT input);

//	Function called on exit and clean up
int ENGINE_exit(ENGINE *engine);

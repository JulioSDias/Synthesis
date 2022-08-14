#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#include "engine.h"

bool SYNTH_LBUTTON, SYNTH_RBUTTON;

int ENGINE_init(ENGINE *engine, GRAPHICS window, u_int target_fps)
{
    LOG_init();
    
    if (ENGINE_inputInit(engine) != 1)
        LOG_error("FAILED TO INIT INPUT");
    
    if(SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        LOG_info("SDL STARTED");
        engine->window = window;
        
        engine->running = 1;
        
        if(GRAPHICS_init(&engine->window) == 1)
            LOG_info("WINDOW CREATION SUCCESSFUL");
        
        engine->fps.target_time = ENGINE_initFPS(target_fps);
        
        if(MANAGER_assetInit(&engine->input.string_manager, ASSET_STRING) == 1)
            LOG_info("TEXT MANAGER INITIALIZED");
        SDL_StartTextInput();
    }
    return 1;
}

bool ENGINE_inputInit(ENGINE *engine){
    
    u_int SDLkey[BUTTON_COUNT] = {SDLK_UP, SDLK_DOWN, SDLK_LEFT, SDLK_RIGHT, SDLK_ESCAPE, SDLK_BACKSPACE, SDLK_SPACE, SDLK_LSHIFT, SDLK_RSHIFT, SDLK_RETURN, SDLK_F1, SDLK_F2, SDLK_F3, SDLK_F4, SDLK_F5, SDLK_F6, SDLK_F7, SDLK_F8, SDLK_F9, SDLK_F10, SDLK_F11, SDLK_F12};
    
    for(u_int i = 0; i < BUTTON_COUNT; i++)
        engine->input.key[i] = SDLkey[i];
    
    return 1;
}

int ENGINE_event(ENGINE *engine)
{
    SDL_Event event;
    
    for(u_int i = 0; i < BUTTON_COUNT; i++)
        engine->input.button[i].changed = 0;
    
    SDL_GetMouseState(&engine->input.mouse.x, &engine->input.mouse.y);
    while (SDL_PollEvent(&event) != 0){
        switch(event.type){
            case SDL_QUIT:{
                engine->running = 0;
                return 0;   
            }break;
            
            case SDL_KEYDOWN:{
                for(u_int i = 0; i < BUTTON_COUNT; i++){
                    if(event.key.keysym.sym == engine->input.key[i]){
                        if(engine->input.button[i].is_down == 0) 
                            engine->input.button[i].changed = 1; 
                        engine->input.button[i].is_down = 1;
                    }
                }
            }break;
            
            case SDL_KEYUP:{
                for(u_int i = 0; i < BUTTON_COUNT; i++){
                    if(event.key.keysym.sym == engine->input.key[i]){
                        if(engine->input.button[i].is_down == 1) 
                            engine->input.button[i].changed = 1; 
                        engine->input.button[i].is_down = 0;
                    }
                }
            }break;
            
            case SDL_MOUSEBUTTONDOWN:{
                if(event.button.button == SDL_BUTTON_LEFT)
                    SYNTH_LBUTTON = 1;
                if(event.button.button == SDL_BUTTON_RIGHT)
                    SYNTH_RBUTTON = 1;
            }break;
            
            case SDL_MOUSEBUTTONUP:{
                if(event.button.button == SDL_BUTTON_LEFT)
                    SYNTH_LBUTTON = 0;
                if(event.button.button == SDL_BUTTON_RIGHT)
                    SYNTH_RBUTTON = 0;
            }break;
            
            case SDL_TEXTINPUT:{
                TEXT_INPUT *string;
                for(u_int i = 0; i <= engine->input.string_manager.top + 1; i++){
                    string = MANAGER_assetReturn(&engine->input.string_manager, i);
                    if(i == engine->input.string_manager.top + 1)
                        return 0;
                    if(string->active == 1)
                        break;
                }
                
                if(string->size > 0 && string->count < string->size){
                    string->count++;
                    if(string->count > 1)
                        string->buffer = (char *)realloc(string->buffer, (string->count + 2));
                    string->buffer[string->count - 1] = *event.text.text;
                    string->buffer[string->count] = '_';
                    string->buffer[string->count + 1] = '\0';
                }
                if(string->size <= 0){
                    string->count++;
                    if(string->count > 1)
                        string->buffer = (char *)realloc(string->buffer, (string->count + 2));
                    string->buffer[string->count - 1] = *event.text.text;
                    string->buffer[string->count] = '_';
                    string->buffer[string->count + 1] = '\0';
                }
            }break;
        }
    }   
}

char *ENGINE_inputText(ENGINE *engine, FONT font, INPUT_BOX *input_box)
{
    if(input_box->stack_num == -1){
        TEXT_INPUT *new_string = (TEXT_INPUT *)malloc(sizeof(TEXT_INPUT));
        new_string->count = 0;
        new_string->size = input_box->size;
        new_string->buffer = (char *)calloc(3, 1);
        input_box->stack_num = MANAGER_assetPush(&engine->input.string_manager, new_string);
    }
    TEXT_INPUT *string = MANAGER_assetReturn(&engine->input.string_manager, input_box->stack_num);
    if(input_box->active == 1){
        string->active = 1;
        if(PRESSED(SYNTH_BACKSPACE, engine->input) && string->count > 0){
            string->count--;
            string->buffer[string->count] = string->buffer[string->count + 1];
            string->buffer[string->count + 1] = string->buffer[string->count + 2];
            string->buffer = (char *)realloc(string->buffer, (string->count + 2));
        }
    }
    
    if(input_box->active == 0){
        string->active = 0;
        string->buffer[string->count] = string->buffer[string->count + 1];
        string->buffer[string->count + 1] = 0;
        string->buffer = (char *)realloc(string->buffer, (string->count + 1));
    }
    
    if(string->count > 0){
        for(u_int i = 0; i <= 80; i++){
            if(string->buffer[string->count - 1] == font.array_chars[i])
                break;
            if(i == 80)
                string->buffer[string->count - 1] = '?';
        }
    }
    GRAPHICS_printS(input_box->x, input_box->y, input_box->scale, string->buffer, engine->window, font);
    return string->buffer;
}

float ENGINE_initFPS(u_int target_fps)
{
    float target_time = 1000.0f/target_fps;
    return target_time;
}

bool ENGINE_startTimer(FPS *fps)
{
    fps->start_time = SDL_GetTicks();
    return 1;
}

float ENGINE_fps(FPS *fps)
{
    fps->count++;
    
    float elapsed_time = SDL_GetTicks() - fps->start_time;
    
    if(elapsed_time < fps->target_time)
        SDL_Delay(fps->target_time - elapsed_time);
    
    fps->raw = 1000.0f/(SDL_GetTicks() - fps->start_time);
    fps->accum += fps->raw;
    
    if(fps->count == 1000){
        fps->cooked = fps->accum/(float)(fps->count);
        
        fps->accum = 0;
        fps->count = 0;
    }
    return fps->cooked;
}

bool PRESSED(u_int b, INPUT input){
    if((input.button[b].is_down == 1) && (input.button[b].changed == 1))
        return 1;
    return 0;
}

bool RELEASED(u_int b, INPUT input){
    if((input.button[b].is_down == 0) && (input.button[b].changed == 1))
        return 1;
    return 0;
}

bool HOLD(u_int b, INPUT input){
    if(input.button[b].is_down == 1)
        return 1;
    return 0;
}

void MOUSE_COORDS(u_int *x, u_int *y, INPUT input){
    *x = input.mouse.x;
    *y = input.mouse.y;
}

int ENGINE_exit(ENGINE *engine)
{
    
    if(GRAPHICS_free(&engine->window) == 1)
        LOG_info("GRAPHICS DESTROYED");
    if(MANAGER_assetFree(&engine->input.string_manager) == 1)
        LOG_info("TEXT MANAGER DESTROYED");
    SDL_StopTextInput();
    SDL_Quit();
    return 1;
}

void ENGINE_delay(u_int time)
{
    SDL_Delay(time);
}

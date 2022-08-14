#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#include "graphics.h"

bool GRAPHICS_init(GRAPHICS *graphics)
{
    
	graphics->window = SDL_CreateWindow(graphics->window_title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, graphics->width, graphics->height, SDL_WINDOW_SHOWN|SDL_WINDOW_OPENGL);
	if(graphics->window == NULL) 
		LOG_error("FAIL WINDOW CREATION");

    graphics->renderer = SDL_CreateRenderer(graphics->window, -1, 0);
    if(graphics->renderer == NULL)
        LOG_error("SDL RENDER FAIL INITIALIZATION");
    LOG_info("SDL RENDER STARTED");
	
	if(MANAGER_assetInit(&graphics->texture_manager, ASSET_TEXTURE) == 1)
		LOG_info("ASSET MANAGER CREATED");
	return 1;
}

u_int GRAPHICS_loadBMP(u_char *path, GRAPHICS *graphics)
{
	
	SDL_Surface *surface = SDL_LoadBMP(path);
	SDL_SetColorKey(surface, SDL_TRUE, STD_DARK_GREY);
	if(surface == NULL)
		LOG_error("LOADING BMP SURFACE");
	void *texture = SDL_CreateTextureFromSurface(graphics->renderer, surface);
	u_int address = MANAGER_assetPush(&graphics->texture_manager, texture);
	if(texture == NULL)
		LOG_error("LOADING BMP TEXTURE");
	SDL_FreeSurface(surface);
	return address;
}

SPRITE GRAPHICS_createSprite(u_int position, u_int width, u_int height, u_int address, GRAPHICS graphics)
{
	
	SYNTH_RECT rect = {position%10 * 8, position/10 * 8, width, height};
	SPRITE sprite = {rect, NULL};
	sprite.texture = MANAGER_assetReturn(&graphics.texture_manager, address);
	return sprite;
}

bool GRAPHICS_renderBMP(u_int x, u_int y, float scale, SPRITE sprite, GRAPHICS graphics)
{
	SYNTH_RECT rect = {x, y, sprite.on_map.width*scale, sprite.on_map.height*scale};
	SDL_RenderCopy(graphics.renderer, sprite.texture, (SDL_Rect *)&sprite.on_map, (SDL_Rect *)&rect);
	return 1;
}

FONT GRAPHICS_loadFontBMP(GRAPHICS graphics, u_int address)
{
	u_char buffer_array[80] = 
    {
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 
        'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 
        'U', 'V', 'W', 'X', 'Y', 'Z', ' ', ' ', ' ', ' ',
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
        'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
        'u', 'v', 'w', 'x', 'y', 'z', ' ', ' ', ' ', '_',
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        '.', ',', ';', ':', '!', '?', '(', ')', '-', '$'
    };
	
	FONT font = {0};
    
	for(u_int i = 0; i < 80; i++){
		font.array_chars[i] = buffer_array[i];
		font.chars[i].on_map.x = i%10 * 8;
		font.chars[i].on_map.y = i/10 * 8;
		font.chars[i].on_map.width = 7;
		font.chars[i].on_map.height = 8;
		font.chars[i].texture = MANAGER_assetReturn(&graphics.texture_manager, address);
	}
	return font;
}

void GRAPHICS_printS(u_int x, u_int y, float scale, char *output, GRAPHICS graphics, FONT font)
{
    
	u_int k = 0;
	if(output == NULL)
		return;
	while(output[k] != '\0'){
		for(u_int i = 0; i <= 80; i++){
			if(output[k] == font.array_chars[i]){
				GRAPHICS_renderBMP(x + ((k * 6) * scale), y, scale, font.chars[i], graphics);
				k++;
				break;
			}
		}
	}
	return;
}

void GRAPHICS_printI(u_int x, u_int y, float scale, int output, GRAPHICS graphics, FONT font)
{
	u_int size = snprintf(NULL, 0, "%d", output);
	char *str =(char *)malloc(size + 1);
	snprintf(str, size + 1,"%d", output);
	u_int k = 0;
	
	if(str == NULL){
		free(str);
		return;
	}
	while(str[k] != '\0'){
		for(u_int i = 0; i <= 80; i++){
			if(str[k] == font.array_chars[i]){
				GRAPHICS_renderBMP(x + ((k * 6) * scale), y, scale, font.chars[i], graphics);
				k++;
				break;
			}
		}
	}
	free(str);
	return;
}

void GRAPHICS_printF(u_int x, u_int y, float scale, float output, GRAPHICS graphics, FONT font)
{
	u_int size = snprintf(NULL, 0, "%f", output);
	char *str =(char *)malloc(size + 1);
	snprintf(str, size + 1,"%.2f", output);
	u_int k = 0;
	
	if(str == NULL){
		free(str);
		return;
	}
	while(str[k] != '\0'){
		for(u_int i = 0; i <= 80; i++){
			if(str[k] == font.array_chars[i]){
				GRAPHICS_renderBMP(x + ((k * 6) * scale), y, scale, font.chars[i], graphics);
				k++;
				break;
			}
		}
	}
	free(str);
	return;
}

void GRAPHICS_fillRect(SYNTH_RECT rect, SYNTH_COLOR color, GRAPHICS graphics)
{
	SDL_SetRenderDrawColor(graphics.renderer, color.r, color.g, color.b, 255);
	SDL_RenderFillRect(graphics.renderer, (SDL_Rect *)&rect);
	return;
}

void GRAPHICS_drawRect(SYNTH_RECT rect, SYNTH_COLOR color, GRAPHICS graphics, u_int thickness)
{
	SDL_SetRenderDrawColor(graphics.renderer, color.r, color.g, color.b, 255);
	SYNTH_RECT rect1 = {rect.x - thickness, rect.y - thickness, rect.width + thickness * 2, thickness};
	SYNTH_RECT rect2 = {rect.x - thickness, rect.y - thickness, thickness, rect.height + thickness * 2};
	SYNTH_RECT rect3 = {rect.x + rect.width, rect.y, thickness, rect.height};
	SYNTH_RECT rect4 = {rect.x, rect.y + rect.height, rect.width + thickness, thickness};
	SDL_RenderFillRect(graphics.renderer, (SDL_Rect *)&rect1);
	SDL_RenderFillRect(graphics.renderer, (SDL_Rect *)&rect2);
	SDL_RenderFillRect(graphics.renderer, (SDL_Rect *)&rect3);
	SDL_RenderFillRect(graphics.renderer, (SDL_Rect *)&rect4);
	return;
}

void GRAPHICS_drawRectS(SYNTH_RECT rect, SYNTH_COLOR color, GRAPHICS graphics)
{
	SDL_SetRenderDrawColor(graphics.renderer, color.r, color.g, color.b, 255);
	SDL_RenderDrawRect(graphics.renderer, (SDL_Rect *)&rect);
	return;
}

void GRAPHICS_line(u_int x1, u_int y1, u_int x2, u_int y2, SYNTH_COLOR color, GRAPHICS graphics)
{
	SDL_SetRenderDrawColor(graphics.renderer, color.r, color.g, color.b, 255);
	SDL_RenderDrawLine(graphics.renderer, x1, y1, x2, y2);
	return;
}

void GRAPHICS_renderClear(GRAPHICS graphics)
{
	if(graphics.render_flag == 1){
        SDL_SetRenderDrawColor(graphics.renderer, graphics.color.r, graphics.color.g, graphics.color.b, 255);
        SDL_RenderClear(graphics.renderer);
    }
	return;
}

void GRAPHICS_renderUpdate(GRAPHICS graphics)
{
	if(graphics.render_flag == 1) SDL_RenderPresent(graphics.renderer);
	return;
}

bool GRAPHICS_free(GRAPHICS *graphics)
{
	SDL_DestroyWindow(graphics->window);
	SDL_DestroyRenderer(graphics->renderer);
	if(MANAGER_assetFree(&graphics->texture_manager) == 1)
		LOG_info("TEXTURE MANAGER DESTROYED");
	return 1;
}

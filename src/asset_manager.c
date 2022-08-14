#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#include "asset_manager.h"

bool MANAGER_assetInit(ASSET_STACK *asset_manager, u_int data_type)
{
	asset_manager->capacity = 3;
	asset_manager->stack = malloc(asset_manager->capacity * (sizeof(u_char) * 4));
	asset_manager->top = -1;
	return 1;
}

bool MANAGER_assetFree(ASSET_STACK *asset_manager)
{
	
	while(asset_manager->top > -1)
		MANAGER_assetPop(asset_manager);
	free(asset_manager);
	asset_manager->stack = NULL;
	return 1;
}

int MANAGER_assetPush(ASSET_STACK *asset_manager, void *asset)
{
	asset_manager->top++;
	if(asset_manager->top == asset_manager->capacity)
		MANAGER_assetScale(asset_manager);
	asset_manager->stack[asset_manager->top] = asset;
	return asset_manager->top;
}

int MANAGER_assetPop(ASSET_STACK *asset_manager)
{
	if(asset_manager->top == -1)
		return 0;
	if(asset_manager->type == ASSET_TEXTURE)
		SDL_DestroyTexture(MANAGER_assetTop(asset_manager));
	if(asset_manager->type == ASSET_STRING){
		free(((TEXT_INPUT *)MANAGER_assetTop(asset_manager))->buffer);
		free(MANAGER_assetTop(asset_manager));
	}
	asset_manager->top--;
	
	return asset_manager->top;
}

void *MANAGER_assetTop(ASSET_STACK *asset_manager)
{
	return asset_manager->stack[asset_manager->top];
}

void *MANAGER_assetReturn(ASSET_STACK *asset_manager, int address)
{
	if(address > -1)
		return asset_manager->stack[address];
	else
		return NULL;
}

u_int MANAGER_assetScale(ASSET_STACK *asset_manager)
{
	asset_manager->capacity += 3;
	asset_manager->stack = realloc(asset_manager->stack, asset_manager->capacity * (sizeof(u_char) * 4));
	return asset_manager->capacity;
}

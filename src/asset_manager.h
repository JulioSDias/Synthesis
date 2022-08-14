#include "utils.h"

struct{
	void **stack;
	int capacity;
	int top;
	u_int type;
}typedef ASSET_STACK;

struct{
	u_int count;
	char *buffer;
	bool active;
	u_int size;
}typedef TEXT_INPUT;


bool MANAGER_assetInit(ASSET_STACK *asset_manager, u_int data_type);
int MANAGER_assetPush(ASSET_STACK *asset_manager, void *asset);
int MANAGER_assetPop(ASSET_STACK *asset_manager);
bool MANAGER_assetFree(ASSET_STACK *asset_manager);
void *MANAGER_assetTop(ASSET_STACK *asset_manager);
void *MANAGER_assetReturn(ASSET_STACK *asset_manager, int address);
u_int MANAGER_assetScale(ASSET_STACK *asset_manager);

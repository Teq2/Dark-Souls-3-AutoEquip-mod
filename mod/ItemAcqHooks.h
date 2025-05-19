#pragma once
#include <basetsd.h>

size_t ItemPickupHook(UINT_PTR pWorldChrMan, UINT_PTR pItemBuffer, UINT_PTR pItemData);
extern decltype(&ItemPickupHook) ItemPickupHookOrig;


bool ItemBuyHook(unsigned int* itemId, int itemQuantity);
extern decltype(&ItemBuyHook) ItemBuyHookOrig;
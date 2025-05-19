#include "ItemAcqHooks.h"
#include "Core.h"
#include "WeaponsReforger.h"
#include "AutoEquip.h"

decltype(&ItemPickupHook) ItemPickupHookOrig = 0;
decltype(&ItemBuyHook) ItemBuyHookOrig = 0;

size_t ItemPickupHook(UINT_PTR pWorldChrMan, UINT_PTR pItemBuffer, UINT_PTR pItemData)
{
	auto itemsAmount = *(int*)pItemBuffer;
	ModCore::DebugPrint("[ItemPickupHook] - items:%d", itemsAmount);
	if (itemsAmount > 99) {
		ModCore::DebugPrint("Too many items, skip");
		return ItemPickupHookOrig(pWorldChrMan, pItemBuffer, pItemData);
	};

	auto droppedByPlayer = *(int*)(pItemData) == -1;
	if (!droppedByPlayer)
	{
		WeaponReforger::Reforge(pWorldChrMan, pItemBuffer, pItemData);
	}

	auto ret = ItemPickupHookOrig(pWorldChrMan, pItemBuffer, pItemData);

	if (Settings::AutoEquipEnabled && !droppedByPlayer)
	{
		AutoEquip::AutoEquipItems(pItemBuffer);
	}
	return ret;
}

bool ItemBuyHook(unsigned int* itemId, int itemQuantity)
{
	ModCore::DebugPrint("[ItemBuyHook] - ItemId:%08X", *itemId);

	// optional reforge
	if (Settings::ReinforceShopWeapons && *itemId > 0)
	{
		*itemId = WeaponReforger::Reforge(*itemId);
	}

	auto ret = ItemBuyHookOrig(itemId, itemQuantity);

	if (*itemId > 0)
	{
		AutoEquip::AutoEquipItem(*itemId);
	}
	return ret;
}
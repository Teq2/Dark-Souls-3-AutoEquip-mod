#include "AutoEquip.h"
#include "Core.h"
#include "Addresses.h"
#include "ParamHelpers.h"
#include "ItemStructs.h"

using DS3EquipItem = void (*)(int newSlot, EquipBuffer* equipData);
using DS3FindEquippedSlot = BOOL(*) (UINT_PTR playerEquips, DWORD inventorySlot);

void AutoEquip::AutoEquipItem(int itemId)
{
	EquipBuffer equipBuffer = { 0 };

	if (ChooseInventorySlot(itemId, &equipBuffer)) {
		if (!IsInventoryItemEquipped(equipBuffer.dInventorySlot)) {
			if (equipBuffer.dEquipSlot == 0x11) CycleRingsRight();

			LockUnlockEquipSlots(1);
			// sometimes it just breaks a weapon, investigation is needed
			reinterpret_cast<DS3EquipItem>(Addresses::GetEquipItem())(equipBuffer.dEquipSlot, &equipBuffer);
			ModCore::DebugPrint("[AutoEquip] Equipped item:%d to slot:%d", itemId, equipBuffer.dEquipSlot);
		}
	};
};

void AutoEquip::AutoEquipItems(UINT_PTR itemsBuff)
{
	auto itemsAmount = *(int*)itemsBuff;
	itemsBuff += 4;

	while (itemsAmount)
	{
		ItemBuff* item = (ItemBuff*)itemsBuff;
		//ModCore::DebugPrint("Item: id=%i quantity=%i durability=%i", item->id, item->quantity, item->durability);

		AutoEquipItem(item->id);
		itemsAmount--;
		itemsBuff += 0x0C;
	};
};

bool AutoEquip::ChooseInventorySlot(int itemID, EquipBuffer* equipData)
{
	DWORD equipSlot = 0;
	DWORD itemType = (ItemType)(itemID >> 0x1C);

    switch (itemType) {
    case(Weapon):
		// An ammo?
        if ((itemID >> 0x10) == 6) 
		{
			return false; 
		}
		if (!ParamHelpers::IsShield(itemID - ParamHelpers::WeaponGetUpgrade(itemID)))
		{
			// Equip staves, chimes, talismans in the left hand (0)
			if (!(Settings::LeftHandedCatalysts && ParamHelpers::IsCatalyst(itemID)))
			{
				equipSlot = 1;
			}
		}
        break;
	case(Protector):
		if (Settings::AutoEquipArmor)
		{
			equipSlot = GetProtectorEquipSlot(itemID);
		}
		break;
	case(Accessory):
        if (Settings::AutoEquipRings && ParamHelpers::IsRing(itemID))
		{
			equipSlot = 0x11; // the leftmost ring-slot
		}
		break;
	case(Goods): 
		return false;
	default:
		ModCore::DebugPrint("[AutoEquip] Invalid item type: %i (%d)", itemType, itemID);
		return false;
	};

	equipData->dEquipSlot = equipSlot;
	equipData->dInventorySlot = GetInventorySlotID(itemID);

	if (equipData->dInventorySlot == (-1)) {
		ModCore::DebugPrint("[AutoEquip] Unable to find the item: %d", itemID);
		return false;
	};

	return true;
};

DWORD AutoEquip::GetProtectorEquipSlot(DWORD itemId)
{
    // slot is based on decimal value of item id
    auto itemActualId = itemId & 0x0FFFFFFF;
    switch (itemActualId % 10000) {
    case 0:
        return 0x0C;
    case 1000:
        return 0x0D;
    case 2000:
        return 0x0E;
    case 3000:
        return 0x0F;
    }

	ModCore::DebugPrint("[AutoEquip] Unable to find slot for the item: %d", itemId);
    return 0;
};

DWORD AutoEquip::GetInventorySlotID(DWORD itemID)
{
	auto inventoryPtr = *(UINT_PTR*)Addresses::GetLocalPlayer();
	inventoryPtr = *(UINT_PTR*)(inventoryPtr + 0x10);
	inventoryPtr = *(UINT_PTR*)(inventoryPtr + 0x470);
	inventoryPtr = *(UINT_PTR*)(inventoryPtr + 0x10);
	inventoryPtr += 0x1B8;

	DWORD inventoryID = 0;
	while (inventoryID < *(DWORD*)(inventoryPtr + 0x04))
	{
		auto inventoryScanPtr = (static_cast<UINT_PTR>(inventoryID) << 0x04);
		inventoryScanPtr += *(UINT_PTR*)(inventoryPtr + 0x38);

		if (*(DWORD*)(inventoryScanPtr + 0x04) == itemID)
		{
			return (inventoryID + *(DWORD*)(inventoryPtr + 0x14));
		};
	
		inventoryID++;
	};

	return -1;
};

void AutoEquip::LockUnlockEquipSlots(int iIsUnlock)
{
	UINT_PTR qWorldChrMan = *(UINT_PTR*)(Addresses::GetWorldChrMan());
	if (!qWorldChrMan)
	{
		ModCore::Panic("WorldChrMan", "AutoEquip.cpp", 1);
	};

	qWorldChrMan = *(UINT_PTR*)(qWorldChrMan + 0x80);
	if (!qWorldChrMan)
	{
		ModCore::Panic("'WorldChr Player' does not exist", "AutoEquip.cpp", 1);
	};

	qWorldChrMan = *(UINT_PTR*)(qWorldChrMan + 0x1F90);
	if (!qWorldChrMan)
	{
		ModCore::Panic("'WorldChr Data' does not exist", "AutoEquip.cpp", 0);
	};

	qWorldChrMan = *(UINT_PTR*)(qWorldChrMan);
	if (!qWorldChrMan)
	{
		ModCore::Panic("'WorldChr Flags' does not exist", "AutoEquip.cpp", 0);
	};

	DWORD dChrEquipAnimFlags = *(DWORD*)(qWorldChrMan + 0x10);

	if (iIsUnlock) dChrEquipAnimFlags |= 1;
	else dChrEquipAnimFlags &= 0xFFFFFFFE;

	*(DWORD*)(qWorldChrMan + 0x10) = dChrEquipAnimFlags;
};

BOOL AutoEquip::IsInventoryItemEquipped(DWORD inventoryId)
{
    auto playerEquips = *(UINT_PTR*)Addresses::GetLocalPlayer();
    playerEquips = *(UINT_PTR*)(playerEquips + 0x10) + 0x228;

    return reinterpret_cast<DS3FindEquippedSlot>(Addresses::GetFindEquippedSlot())(playerEquips, inventoryId) != -1;
}

void AutoEquip::CycleRingsRight()
{
	EquipBuffer equipBuffer = { 0 };
	// offsets from 'EquipItem' func
	auto playerEquips = *(UINT_PTR*)Addresses::GetLocalPlayer();
	playerEquips = *(UINT_PTR*)(playerEquips + 0x10) + 0x228;
	auto slots = (int*)(playerEquips + 0x24);

	// [0x11, 0x12, 0x13, 0x14] - rings slots)
	// can't just overwrite 'slots' array here, it won't be enough
	for (int i = 0x14; i > 0x11; i--)
	{
		equipBuffer.dInventorySlot = slots[i - 1];
		if (equipBuffer.dInventorySlot == -1) continue; // kinda unnecessary check, it's done inside GetEquipItem
		equipBuffer.dEquipSlot = i - 1;
		reinterpret_cast<DS3EquipItem>(Addresses::GetEquipItem())(i, &equipBuffer);
	}
}
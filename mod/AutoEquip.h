#pragma once
#include <Windows.h>

typedef struct {
    DWORD dUn1;
    DWORD dUn2;
    DWORD dEquipSlot;
    char unkBytes[0x2C];
    DWORD dInventorySlot;
    DWORD dFlag;
    char Bytes[0x5c];
} EquipBuffer;

class AutoEquip {
public:
    static void AutoEquipItem(int itemId);
    static void AutoEquipItems(UINT_PTR itemsBuffer);

private:
    static bool ChooseInventorySlot(int itemID, EquipBuffer* eb);
    static DWORD GetProtectorEquipSlot(DWORD item);
    static DWORD GetInventorySlotID(DWORD itemID);
    static void LockUnlockEquipSlots(int iIsUnlock);
    static BOOL IsInventoryItemEquipped(DWORD inventoryId);
    static void CycleRingsRight();
};
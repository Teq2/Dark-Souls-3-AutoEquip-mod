#include "ParamHelpers.h"
#include "Addresses.h"

struct EquipParamWeapon;

EquipParamWeapon* ParamHelpers::GetEquipParamWeapon(int itemId)
{
    //char result[44];
    //GetEquipParamWeaponInternal(result, itemId);
    //return (EquipParamWeapon*)*(long long*)(result + 8);
    return (EquipParamWeapon*)ReversedGetParams(itemId, EquipParamWeaponTable);
}

EquipParamProtector* ParamHelpers::GetEquipParamProtector(int itemId)
{
    return (EquipParamProtector*)ReversedGetParams(itemId, EquipParamProtectorTable);
}

EquipParamAccessory* ParamHelpers::GetEquipParamAccessory(int itemId)
{
    return (EquipParamAccessory*)ReversedGetParams(itemId, EquipParamAccessoryTable);
}

EquipParamGoods* ParamHelpers::GetEquipParamGoods(int itemId)
{
    return (EquipParamGoods*)ReversedGetParams(itemId, EquipParamGoodsTable);
}

unsigned char ParamHelpers::GetWeaponCategory(int itemId)
{
    EquipParamWeapon* params = GetEquipParamWeapon(itemId);

    if (params)
    {
        return params->weaponCategory;
    }
    return -1;
}

unsigned long ParamHelpers::GetWeaponUpgrade(int itemId)
{
    // Every infusion gives a step of 100, everything in between - upgrade level
    return itemId % 100;
}

bool ParamHelpers::IsRing(int itemId)
{
    EquipParamAccessory* params = GetEquipParamAccessory(itemId & 0x0FFFFFFF);

    if (params)
    {
        return params->RingCompatibilityId != -1;
    }
    return false;
}

bool ParamHelpers::IsWeaponFullyUpgradable(int itemId)
{
    EquipParamWeapon* params = GetEquipParamWeapon(itemId);

    if (params)
    {
        return params->originEquipWep10 != -1;
    }
    return false;
}

bool ParamHelpers::IsWeaponInfusable(int itemId)
{
    const int infusionId = 1500;
    EquipParamWeapon* params = GetEquipParamWeapon(itemId + infusionId);

    if (params)
    {
        // if (params->reinforceTypeId == infusionId) - this check is not reliable
        // `params` isn't null => weapon with infusion exists
        // +1500 Hollow - offset of the last infusion
        // if weapon is already infused (reinforceWeaponId might be 0) it will help to not infuse weapon twice. 

        return true;
    }
    return false;
}

bool ParamHelpers::IsCatalyst(unsigned char wepCategory)
{
    return wepCategory == 8;
}

bool ParamHelpers::IsRanged(unsigned char wepCategory)
{
    return wepCategory == 10 /* a bow */ || wepCategory == 11 /* a crossbow */;
}

bool ParamHelpers::IsShield(unsigned char wepCategory)
{
    return wepCategory == 12;
}

/// reversed this function just for better understanding GetParamX funcs
void* ParamHelpers::ReversedGetParams(unsigned int itemId, ParamTableType tableId)
{
    const unsigned long long soloParamRepository = *(unsigned long long*)Addresses::GetSoloParamRepository();

    if (itemId >= 0)
    {
        long long params = tableId < 0x61? *(unsigned long long*)(soloParamRepository + 8 * (9 * tableId) + 0x70): 0;

        if (params)
        {
            auto paramTable = *(unsigned long long*)(*(unsigned long long*)(params + 104) + 104);
            auto indexesArray = paramTable + ((*(unsigned int*)(paramTable - 16) + 15) & 0xFFFFFFFFFFFFFFF0);
            int lowIdx = 0;
            int highIdx = *(unsigned short*)(paramTable + 10) - 1;
            if (highIdx >= 0)
            {
                while (lowIdx <= highIdx)
                {
                    int midIdx = (highIdx + lowIdx) >> 1;
                    auto searchId = *(unsigned int*)(indexesArray + 8 * midIdx);
                    if (itemId == searchId)
                    {
                        unsigned long long itemIndex = *(unsigned int*)(indexesArray + 8 * midIdx + 4);
                        if ((int)itemIndex >= 0)
                        {
                            unsigned long long offset = 0;

                            switch (*(unsigned char*)(paramTable + 45) & 0x7F)
                            {
                            case 2:
                                offset = *(unsigned int*)(paramTable + 12 * itemIndex + 52);
                                break;
                            case 3:
                                offset = *(unsigned int*)(paramTable + 12 * itemIndex + 68);
                                break;
                            case 5:
                                if (*(signed char *)(paramTable + 45) > 0)
                                {
                                    offset = *(unsigned int*)(paramTable + 12 * itemIndex + 68);
                                    break;
                                }
                                // no break;
                            case 4:
                                if (*(unsigned char*)(paramTable + 46) & 2)
                                    offset = *(unsigned long long*)(paramTable + 24 * (itemIndex + 3));
                                else
                                    offset = *(unsigned int*)(paramTable + 12 * itemIndex + 68);
                                break;
                            }
                            return (void*)(offset + paramTable);
                        }
                    }
                    if (itemId < searchId)
                        highIdx = midIdx - 1;
                    else
                        lowIdx = midIdx + 1;
                }
            }
        }
    }
    return nullptr;
}
#include "WeaponsReforger.h"
#include "Addresses.h"
#include "Core.h"
#include "ParamHelpers.h"
#include "ItemStructs.h"

int WeaponReforger::Reforge(int itemID)
{
	auto itemType = (ItemType)(itemID >> 0x1C);

	if (itemType == Weapon && (itemID >> 0x10) != 6 /* Ammo */ && itemID != 0x000A87500 /* Dark Hand */)
	{
		if (Settings::RandomWeaponUpgrades || Settings::RandomInfusionChance)
		{
			UINT_PTR playerState = *(UINT_PTR*)Addresses::GetLocalPlayer();
			if (!playerState)
			{
				ModCore::Panic("'Local Player' does not exist", "WeaponsReforger.cpp", 1);
			};

			playerState = *(UINT_PTR*)(playerState + 0x10);
			BYTE playerUpgradeLevel = *(BYTE*)(playerState + 0xB3);
			
			auto preupgrade = ParamHelpers::GetWeaponUpgrade(itemID);
			auto infusable = ParamHelpers::IsWeaponInfusable(itemID - preupgrade);
			auto regular = ParamHelpers::IsWeaponFullyUpgradable(itemID - preupgrade);
			auto minUpgrade = preupgrade;
			decltype(minUpgrade) maxUpgrade = regular? playerUpgradeLevel: playerUpgradeLevel >> 1;

			if (Settings::RandomWeaponUpgrades && maxUpgrade > preupgrade)
			{
				if (Settings::AdjustUpgrades == UpgradeAdjustment::Higher)
				{
					decltype(minUpgrade) newBottom = maxUpgrade / 2 + (maxUpgrade & 1);
					if (minUpgrade < newBottom)
					{
						minUpgrade = newBottom;
					}
				}
				else if (Settings::AdjustUpgrades == UpgradeAdjustment::Max)
				{
					minUpgrade = maxUpgrade;
				}

				itemID += RandomizeNumber<DWORD>(minUpgrade, maxUpgrade) - preupgrade;
			}

			if (Settings::RandomInfusionChance && infusable)
			{
				// infuse? 
				if (Settings::RandomInfusionChance >= RandomizeNumber(0, 100))
				{
					int infusionId = RandomizeNumber(0, 15) * 100;
					itemID += infusionId;
				}
			}

			ModCore::DebugPrint("Weapon: infusable=%i regular=%i preupgrade=%i min=%i max=%i", infusable, regular, preupgrade, minUpgrade, maxUpgrade);

		} // Settings
	} // ItemType::Weapon
	return itemID;
}

void WeaponReforger::Reforge(UINT_PTR gWorldChrMan, UINT_PTR itemsBuff, UINT_PTR itemData)
{
	auto itemsAmount = *(int*)itemsBuff;
	itemsBuff += 4;

	for (; itemsAmount--; itemsBuff += 0x0C)
	{
		auto item = (ItemBuff*)itemsBuff;
		ModCore::DebugPrint("Item: id=%08X quantity=%i durability=%i", item->id, item->quantity, item->durability);
		auto newItemID = Reforge(item->id);

		if (item->id != newItemID)
		{
			ModCore::DebugPrint("Weapon id changed from %08X to %08X", item->id, newItemID);
			item->id = newItemID;
		}
	};
};

template <class T>
T WeaponReforger::RandomizeNumber(T min, T max)
{
	if (min >= max) return max;

	std::uniform_int_distribution<T> dist{ min, max };
	return dist(engine);
};


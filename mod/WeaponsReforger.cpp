#include "WeaponsReforger.h"
#include "Addresses.h"
#include "Core.h"
#include "ParamHelpers.h"
#include "ItemStructs.h"

int WeaponReforger::Reforge(int itemID)
{
	DWORD itemType = (itemID >> 0x1C);

	if (itemType == ItemType::Weapon)
	{
		if (Settings::RandomWeaponUpgrades && (itemID >> 0x10) != 6 /* Ammo */ && itemID != 0x000A87500 /* Dark Hand */)
		{
			UINT_PTR playerState = *(UINT_PTR*)Addresses::GetLocalPlayer();
			if (!playerState)
			{
				ModCore::Panic("'Local Player' does not exist", "WeaponsReforger.cpp", 1);
			};

			playerState = *(UINT_PTR*)(playerState + 0x10);
			BYTE playerUpgradeLevel = *(BYTE*)(playerState + 0xB3);

			if (playerUpgradeLevel)
			{
				auto preupgrade = ParamHelpers::WeaponGetUpgrade(itemID);
				auto infusable = ParamHelpers::IsWeaponInfusable(itemID - preupgrade);
				auto regular = ParamHelpers::IsWeaponFullyUpgradable(itemID - preupgrade);

				if (!regular)
				{
					playerUpgradeLevel >>= 1;
				}

				DWORD lowestUpgrade = !Settings::MoreUpgradedWeapons ? 0 : playerUpgradeLevel / 2 + (playerUpgradeLevel & 1);
				if (lowestUpgrade < preupgrade)
				{
					lowestUpgrade = preupgrade;
				}

				if (lowestUpgrade < playerUpgradeLevel)
				{
					itemID += RandomizeNumber<DWORD>(lowestUpgrade, playerUpgradeLevel) - preupgrade;
				}

				if (infusable && Settings::RandomWeaponInfusions)
				{
					int infusionId = (RandomizeNumber(0, 15) * 100);
					itemID += infusionId;
				}

				ModCore::DebugPrint("Weapon: infusable=%i regular=%i preupgrade=%i min=%i max=%i", infusable, regular, preupgrade, lowestUpgrade, playerUpgradeLevel);
			} // if bPlayerUpgradeLevel
		} // if weapon
	} // itemType
	return itemID;
}

void WeaponReforger::Reforge(UINT_PTR gWorldChrMan, UINT_PTR itemsBuff, UINT_PTR itemData)
{
	auto itemsAmount = *(int*)itemsBuff;
	itemsBuff += 4;

	for (; itemsAmount--; itemsBuff += 0x0C)
	{
		ItemBuff* item = (ItemBuff*)itemsBuff;
		ModCore::DebugPrint("Item: id=%i quantity=%i durability=%i", item->id, item->quantity, item->durability);
		auto newItemID = Reforge(item->id);

		if (item->id != newItemID)
		{
			ModCore::DebugPrint("Weapon id changed from %d to %d", item->id, newItemID);
			item->id = newItemID;
		}
	};
};

template <typename T>
T WeaponReforger::RandomizeNumber(T min, T max)
{
	if (min >= max) return max;

	std::uniform_int_distribution<T> dist{ min, max };
	return dist(engine);
};


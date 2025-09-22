#include "Core.h"
#include "Hooks.h"
#include "Addresses.h"
#include "ItemAcqHooks.h"
#include "ItemRequirements.h"
#include <filesystem>

DWORD ModCore::Start(HINSTANCE dllInstance)
{
	DebugInit();

	// Full dll path is needed for Modengine
	char dllPath[MAX_PATH + 1];
	GetModuleFileNameA(dllInstance, dllPath, MAX_PATH);

	if (!Settings::LoadSettings(std::filesystem::path(dllPath).parent_path().append("AutoEquipSettings.ini").string()))
	{
		// try again in .exe folder
		if (!Settings::LoadSettings("AutoEquipSettings.ini"))
		{
			Panic("Failed to load 'AutoEquipSettings.ini'\r\nFallback values were used.", "Core\\Core.cpp", false);
		}
	}

#ifdef DEBUG
	DebugPrint("[AutoEquip] - AutoEquipWeapons = %i", Settings::AutoEquipWeapons);
	DebugPrint("[AutoEquip] - AutoEquipArmor = %i", Settings::AutoEquipArmor);
	DebugPrint("[AutoEquip] - AutoEquipRings = %i", Settings::AutoEquipRings);
	DebugPrint("[AutoEquip] - LeftHandedCatalysts = %i", Settings::LeftHandedCatalysts);
	DebugPrint("[AutoEquip] - LeftHandedRanged = %i", Settings::LeftHandedRanged);
	DebugPrint("[Randomizer] - RandomizeWeaponUpgrade = %i", Settings::RandomWeaponUpgrades);
	DebugPrint("[Randomizer] - RandomInfusionChance = %i", Settings::RandomInfusionChance);
	DebugPrint("[Randomizer] - ReinforceShopWeapons = %i", Settings::ReinforceShopWeapons);
	DebugPrint("[Randomizer] - AdjustUpgrades = %i", Settings::AdjustUpgrades);
	DebugPrint("[Requirements] - ReduceWeaponRequirements = %i", Settings::WeaponRequirements);
	DebugPrint("[Requirements] - ReduceMagicRequirements = %i", Settings::MagicRequirements);
#endif

	Addresses::Rebase();
	bool hooked = false;
	if (MH_Initialize() == MH_OK)
	{
		hooked = Hooks::SetHook(Addresses::GetItemPickup(), &ItemPickupHook, &ItemPickupHookOrig);
		hooked &= Hooks::SetHook(Addresses::GetItemBuy(), &ItemBuyHook, &ItemBuyHookOrig);

		if (Settings::WeaponRequirements != StatRequirements::Full)
		{
			hooked &= Hooks::SetHook(Addresses::GetGetWeaponParams(), &GetWeaponParams, &GetWeaponParams_org);
		}

		if (Settings::MagicRequirements != StatRequirements::Full)
		{
			hooked &= Hooks::SetHook(Addresses::GetGetMagicParams(), &GetMagicParams, &GetMagicParams_org);
		}
	}

	if (!hooked)
	{
		Panic("Failed to initialize hooks", "Core\\Core.cpp", true);
		int3
	};
	return 0;
};

void ModCore::Panic(const char* message, const char* sort, bool isFatalError)
{
	char outmsg[MAX_PATH];
	sprintf_s(outmsg, "[%s] %s", sort, message);

	if (IsDebuggerPresent())
	{
		OutputDebugStringA(outmsg);
	};

#ifdef DEBUG
	DebugPrint("ModCore::Panic is outputting debug-mode error information\n");
	DebugPrint(outmsg);
	if (isFatalError) int3;
#else
	auto title = isFatalError ? "[AutoEquip - Fatal Error]" : "[AutoEquip - Error]";
	MessageBoxA(NULL, outmsg, title, MB_ICONERROR);
#endif

	return;
};

void ModCore::DebugInit()
{
#ifdef DEBUG
	FILE* fp;

	AllocConsole();
	SetConsoleTitleA("DS3 AutoEquip Debug Console");
	freopen_s(&fp, "CONOUT$", "w", stdout);
	printf_s("Starting...\n");
#endif
	return;
};

void ModCore::DebugPrint(const char* message, ...)
{
#ifdef DEBUG
	va_list argptr;
	va_start(argptr, message);
	vprintf_s(message, argptr);
	va_end(argptr);
	printf_s("\r\n");
#endif
};

bool Settings::IsAutoEquipEnabled()
{
	return AutoEquipWeapons || AutoEquipArmor || AutoEquipRings;
}

bool Settings::LoadSettings(const std::string& filename)
{
	INIReader reader(filename);

	AutoEquipWeapons = reader.GetBoolean("AutoEquip", "AutoEquipWeapons", true);
	AutoEquipArmor = reader.GetBoolean("AutoEquip", "AutoEquipArmor", true);
	AutoEquipRings = reader.GetBoolean("AutoEquip", "AutoEquipRings", true);
	LeftHandedCatalysts = reader.GetBoolean("AutoEquip", "LeftHandedCatalysts", true);
	LeftHandedRanged = reader.GetBoolean("AutoEquip", "LeftHandedRanged", true);

	RandomWeaponUpgrades = reader.GetBoolean("Randomizer", "RandomizeWeaponUpgrade", true);
	RandomInfusionChance = reader.GetInteger("Randomizer", "RandomInfusionChance", 50);

	RandomInfusionChance = RandomInfusionChance < 0 ? 0 : RandomInfusionChance;
	RandomInfusionChance = RandomInfusionChance > 100 ? 100 : RandomInfusionChance;

	ReinforceShopWeapons = reader.GetBoolean("Randomizer", "ReinforceShopWeapons", false);
	AdjustUpgrades = static_cast<UpgradeAdjustment>(reader.GetInteger("Randomizer", "AdjustUpgrades", 0));

	WeaponRequirements = static_cast<StatRequirements>(reader.GetInteger("Requirements", "ReduceWeaponRequirements", 0));
	MagicRequirements = static_cast<StatRequirements>(reader.GetInteger("Requirements", "ReduceMagicRequirements", 0));

	return reader.ParseError() != -1;
}
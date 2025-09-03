#pragma once
#include "INIReader/INIReader.h"
#include "MinHook/include/MinHook.h"
#include <Windows.h>
#include <string>

#ifdef _DEBUG
#define DEBUG
#define int3 __debugbreak();
#else
#define int3 ;
#endif

enum class UpgradeAdjustment { None, Higher, Max };
enum class WeaponRequirements { Full, Halved, NoReq } ;

class ModCore {
public:
	static DWORD Start();
	static bool Initialize();

	static void Panic(char* message, char* sort, bool isFatalError);
	static void DebugInit();
	static void DebugPrint(const char* message, ...);
};

class Settings
{
public:
	static bool IsAutoEquipEnabled();
	static bool LoadSettings(const std::string& filename);

	static inline bool AutoEquipWeapons;
	static inline bool AutoEquipArmor;
	static inline bool AutoEquipRings;
	static inline bool LeftHandedCatalysts;
	static inline bool LeftHandedRanged;
	static inline WeaponRequirements LessWeaponRequirements;

	static inline bool RandomWeaponUpgrades;
	static inline int RandomInfusionChance;
	static inline bool ReinforceShopWeapons;
	static inline UpgradeAdjustment AdjustUpgrades;
};

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

class ModCore {
public:
	static DWORD Start();
	static bool Initialize();

	static void Panic(char* message, char* sort, bool isFatalError);
	static void DebugInit();
	static void DebugPrint(const char* message, ...);
};

typedef enum { Full, Halved, NoReq } WeaponRequirements;

class Settings
{
public:
	static bool LoadSettings(const std::string& filename);

	static inline bool AutoEquipEnabled;
	static inline bool AutoEquipArmor;
	static inline bool AutoEquipRings;
	static inline bool LeftHandedCatalysts;
	static inline WeaponRequirements LessWeaponRequirements;

	static inline bool RandomWeaponUpgrades;
	static inline bool RandomWeaponInfusions;
	static inline bool ReinforceShopWeapons;
	static inline bool MoreUpgradedWeapons;
};

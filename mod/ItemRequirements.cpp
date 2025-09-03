#include "ParamHelpers.h"
#include "Core.h"
#include <basetsd.h>
#include <IntSafe.h>

float CalcWeaponDamage(DWORD, std::uint32_t, float, std::uint64_t, std::uint64_t, std::uint64_t, float, BYTE);

decltype(&CalcWeaponDamage) CalcWeaponDamage_org = 0;

DWORD NoWeaponRequirements(UINT_PTR thisPtr, UINT_PTR itemPtr)
{
	auto params = *(UINT_PTR*)(itemPtr + 8);
	if (params) {
		// 4bytes: str, agi, int, faith
		*(DWORD*)(params + 0xee) = 0;
	}
	return 0;
};

static DWORD FilterRequirements(DWORD prev)
{
	switch (Settings::LessWeaponRequirements)
	{
	case WeaponRequirements::Halved:
		return prev >> 1;
	case WeaponRequirements::NoReq:
		return 0;
	default:
		return prev;
	}
}

float CalcWeaponDamage(DWORD weaponReq, std::uint32_t playerStat, float param_3, std::uint64_t param_4,
	std::uint64_t param_5, std::uint64_t param_6, float param_7, BYTE param_8)
{
	return CalcWeaponDamage_org(FilterRequirements(weaponReq), playerStat, param_3, param_4, param_5, param_6, param_7, param_8);
}

DWORD StrRequirements(UINT_PTR thisPtr, UINT_PTR itemPtr)
{
	auto params = *(EquipParamWeapon**)(itemPtr + 8);
	if (params) {
		return FilterRequirements(params->properStrength);
	}
	return 0;
};

DWORD AgiRequirements(UINT_PTR thisPtr, UINT_PTR itemPtr)
{
	auto params = *(EquipParamWeapon**)(itemPtr + 8);
	if (params) {
		return FilterRequirements(params->properAgility);
	}
	return 0;
};

DWORD IntRequirements(UINT_PTR thisPtr, UINT_PTR itemPtr)
{
	auto params = *(EquipParamWeapon**)(itemPtr + 8);
	if (params) {
		return FilterRequirements(params->properMagic);
	}
	return 0;
};

DWORD FthRequirements(UINT_PTR thisPtr, UINT_PTR itemPtr)
{
	auto params = *(EquipParamWeapon**)(itemPtr + 8);
	if (params) {
		return FilterRequirements(params->properFaith);
	}
	return 0;
};
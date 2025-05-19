#include "ParamHelpers.h"
#include "Core.h"
#include <basetsd.h>
#include <IntSafe.h>

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
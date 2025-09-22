#include "ParamHelpers.h"
#include "Core.h"
#include "ItemRequirements.h"
#include <set>

decltype(&GetMagicParams) GetMagicParams_org = 0;
decltype(&GetWeaponParams) GetWeaponParams_org = 0;

template <class ParamsStruct, auto ParamsStruct::* ...  fld>
void ParamsProc(StatRequirements mode, UINT_PTR* buff, std::uint32_t id)
{
	static std::set<std::uint32_t> idsAffected{};
	if (static_cast<std::uint32_t>(buff[0]) == id && buff[1])
	{
		ParamsStruct* params = reinterpret_cast<ParamsStruct*>(buff[1]);

		switch (mode)
		{
		case StatRequirements::Halved:
			if (!idsAffected.count(id))
			{
				idsAffected.insert(id);
				((params->*fld = params->*fld >> 1), ...);
			}
			return;
		case StatRequirements::NoReq:
			((params->*fld) = ... = 0);
			return;
		}
	}
}

void GetWeaponParams(UINT_PTR* buff, std::uint32_t weaponId)
{
	GetWeaponParams_org(buff, weaponId);
	ParamsProc<EquipParamWeapon, 
		&EquipParamWeapon::properStrength, 
		&EquipParamWeapon::properAgility, 
		&EquipParamWeapon::properMagic, 
		&EquipParamWeapon::properFaith
	>(Settings::WeaponRequirements, buff, weaponId);
}

void GetMagicParams(UINT_PTR* buff, std::uint32_t spellId)
{
	GetMagicParams_org(buff, spellId);
	ParamsProc<MagicParams,
		&MagicParams::requirementIntellect,
		&MagicParams::requirementFaith
	>(Settings::MagicRequirements, buff, spellId);
}

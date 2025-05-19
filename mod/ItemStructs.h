#pragma once

struct ItemBuff
{
	int id;
	int quantity;
	int durability;
};

enum ItemType { Weapon, Protector, Accessory, Goods = 4 };
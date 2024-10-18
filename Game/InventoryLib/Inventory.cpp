#include "Inventory.h"

Inventory::Inventory()
{
	items = std::vector<std::pair<std::shared_ptr<Item>, int>>();
}


void Inventory::AddItem(const std::shared_ptr<Item>& _item, int _amount)
{
	for (int i = 0; i < items.size(); i++)
	{
		if (items[i].first->GetID() == _item->GetID())
		{
			items[i].second += _amount;
			return;
		}
	}
	items.push_back(std::make_pair(_item, _amount));
}

void Inventory::RemoveItem(const std::shared_ptr<Item>& _item, int _amount)
{
	for (int i = 0; i < items.size(); i++)
	{
		if (items[i].first->GetID() == _item->GetID())
		{
			items[i].second -= _amount;
			if (items[i].second == 0)
			{
				items.erase(items.begin() + i);
			}
			return;
		}
	}
}

void Inventory::LoadInventory(const std::string& _filePath)
{
	Parsing parsing(_filePath);

	parsing.ShowTargetItems();
	auto sections = parsing.GetListItems();

	CreateItem(sections);
}

void Inventory::CreateItem(std::vector<std::vector<std::pair<std::string, ParsingOption>>>& _listItems) {
	ItemBuild itemBuild;
    for (const auto& listItem : _listItems) {
        std::string type;

		// stock le type pour le transmettre � "ItemBuild" qui pourra d�terminer c'est quel type d'item
		// puis traiter en fonction du type d'item les valeurs � attribu�
        for (const auto& [key, value] : listItem) {
            std::visit([&](auto&& arg) {
                if (key == "type") {
                    if constexpr (std::is_same_v<std::decay_t<decltype(arg)>, std::string>) {
                        type = arg;
                    }
                }
            }, value);
        }

		auto item = itemBuild.CreateItemByType(type, listItem);
		if (item) {
			AddItem(item);
		}
    }
}

// Sorting functions
template <typename T, typename CompareFunc>
struct CompareBy {
	bool ascending;
	CompareFunc compareFunc;

	CompareBy(bool _ascending, CompareFunc _compareFunc) : ascending(_ascending), compareFunc(_compareFunc) {}

	bool operator()(const std::pair<std::shared_ptr<Item>, int>& a, const std::pair<std::shared_ptr<Item>, int>& b) {
		auto itemA = std::dynamic_pointer_cast<T>(a.first);
		auto itemB = std::dynamic_pointer_cast<T>(b.first);
		if (itemA && itemB) {
			return ascending ? compareFunc(itemA, itemB) : compareFunc(itemB, itemA);
		}
		if (itemA) return true;
		if (itemB) return false;

		return false; // If either is not of type T, they are considered equal for sorting purposes
	}
};

struct CompareByQuantity {
	bool ascending = true;
	CompareByQuantity(bool _ascending) : ascending(_ascending) {};
	bool operator()(const std::pair<std::shared_ptr<Item>, int>& a, const std::pair<std::shared_ptr<Item>, int>& b)
	{
		return ascending ? a.second < b.second : a.second > b.second;
	};
};

void Inventory::SortByID(bool _ascending) {
	auto compareFunc = [](const std::shared_ptr<Item>& a, const std::shared_ptr<Item>& b) {
		return a->GetID() < b->GetID();
		};
	std::sort(items.begin(), items.end(), CompareBy<Item, decltype(compareFunc)>(_ascending, compareFunc));
}

void Inventory::SortByName(bool _ascending) {
	auto compareFunc = [](const std::shared_ptr<Item>& a, const std::shared_ptr<Item>& b) {
		return a->GetName() < b->GetName();
		};
	std::sort(items.begin(), items.end(), CompareBy<Item, decltype(compareFunc)>(_ascending, compareFunc));
}

void Inventory::SortByType(bool _ascending) {
	auto compareFunc = [](const std::shared_ptr<Item>& a, const std::shared_ptr<Item>& b) {
		return a->GetItemType() < b->GetItemType();
		};
	std::sort(items.begin(), items.end(), CompareBy<Item, decltype(compareFunc)>(_ascending, compareFunc));
}

void Inventory::SortByWeaponType(bool _ascending) {
	auto compareFunc = [](const std::shared_ptr<Weapon>& a, const std::shared_ptr<Weapon>& b) {
		return a->GetWeaponType() < b->GetWeaponType();
		};
	std::sort(items.begin(), items.end(), CompareBy<Weapon, decltype(compareFunc)>(_ascending, compareFunc));
}

void Inventory::SortByArmorType(bool _ascending) {
	auto compareFunc = [](const std::shared_ptr<Armor>& a, const std::shared_ptr<Armor>& b) {
		return a->GetArmorType() < b->GetArmorType();
		};
	std::sort(items.begin(), items.end(), CompareBy<Armor, decltype(compareFunc)>(_ascending, compareFunc));
}

void Inventory::SortByPrice(bool _ascending) {
	auto compareFunc = [](const std::shared_ptr<Item>& a, const std::shared_ptr<Item>& b) {
		return a->GetSellPrice() < b->GetSellPrice();
		};
	std::sort(items.begin(), items.end(), CompareBy<Item, decltype(compareFunc)>(_ascending, compareFunc));
}

void Inventory::SortByLevel(bool _ascending) {
	auto compareFunc = [](const std::shared_ptr<Equipment>& a, const std::shared_ptr<Equipment>& b) {
		return a->GetLevel() < b->GetLevel();
		};
	std::sort(items.begin(), items.end(), CompareBy<Equipment, decltype(compareFunc)>(_ascending, compareFunc));
}

void Inventory::SortByDefense(bool _ascending) {
	auto compareFunc = [](const std::shared_ptr<Equipment>& a, const std::shared_ptr<Equipment>& b) {
		return a->GetDefense() < b->GetDefense();
		};
	std::sort(items.begin(), items.end(), CompareBy<Equipment, decltype(compareFunc)>(_ascending, compareFunc));
}

void Inventory::SortByAttack(bool _ascending) {
	auto compareFunc = [](const std::shared_ptr<Weapon>& a, const std::shared_ptr<Weapon>& b) {
		return a->GetAttack() < b->GetAttack();
		};
	std::sort(items.begin(), items.end(), CompareBy<Weapon, decltype(compareFunc)>(_ascending, compareFunc));
}

void Inventory::SortByAttackSpeed(bool _ascending)
{
	auto compareFunc = [](const std::shared_ptr<Weapon>& a, const std::shared_ptr<Weapon>& b) {
		return a->GetAttackSpeed() < b->GetAttackSpeed();
		};
	std::sort(items.begin(), items.end(), CompareBy<Weapon, decltype(compareFunc)>(_ascending, compareFunc));
}

void Inventory::SortByCritRate(bool _ascending)
{
	auto compareFunc = [](const std::shared_ptr<Weapon>& a, const std::shared_ptr<Weapon>& b) {
		return a->GetCritRate() < b->GetCritRate();
		};
	std::sort(items.begin(), items.end(), CompareBy<Weapon, decltype(compareFunc)>(_ascending, compareFunc));
}

void Inventory::SortByCritDamage(bool _ascending)
{
	auto compareFunc = [](const std::shared_ptr<Weapon>& a, const std::shared_ptr<Weapon>& b) {
		return a->GetCritDamage() < b->GetCritDamage();
		};
	std::sort(items.begin(), items.end(), CompareBy<Weapon, decltype(compareFunc)>(_ascending, compareFunc));
}

void Inventory::SortByAccuracy(bool _ascending)
{
	auto compareFunc = [](const std::shared_ptr<Weapon>& a, const std::shared_ptr<Weapon>& b) {
		return a->GetAccuracy() < b->GetAccuracy();
		};
	std::sort(items.begin(), items.end(), CompareBy<Weapon, decltype(compareFunc)>(_ascending, compareFunc));
}

void Inventory::SortByCooldownReduction(bool _ascending)
{
	auto compareFunc = [](const std::shared_ptr<Weapon>& a, const std::shared_ptr<Weapon>& b) {
		return a->GetCooldownReduction() < b->GetCooldownReduction();
		};
	std::sort(items.begin(), items.end(), CompareBy<Weapon, decltype(compareFunc)>(_ascending, compareFunc));
}

void Inventory::SortByLifeSteal(bool _ascending)
{
	auto compareFunc = [](const std::shared_ptr<Weapon>& a, const std::shared_ptr<Weapon>& b) {
		return a->GetLifeSteal() < b->GetLifeSteal();
		};
	std::sort(items.begin(), items.end(), CompareBy<Weapon, decltype(compareFunc)>(_ascending, compareFunc));
}

void Inventory::SortByHealth(bool _ascending)
{
	auto compareFunc = [](const std::shared_ptr<Armor>& a, const std::shared_ptr<Armor>& b) {
		return a->GetHealth() < b->GetHealth();
		};
	std::sort(items.begin(), items.end(), CompareBy<Armor, decltype(compareFunc)>(_ascending, compareFunc));
}


void Inventory::SortByDodgeRate(bool _ascending)
{
	auto compareFunc = [](const std::shared_ptr<Armor>& a, const std::shared_ptr<Armor>& b) {
		return a->GetDodgeRate() < b->GetDodgeRate();
		};
	std::sort(items.begin(), items.end(), CompareBy<Armor, decltype(compareFunc)>(_ascending, compareFunc));
}

void Inventory::SortByHealthRegen(bool _ascending)
{
	auto compareFunc = [](const std::shared_ptr<Armor>& a, const std::shared_ptr<Armor>& b) {
		return a->GetHealthRegen() < b->GetHealthRegen();
		};
	std::sort(items.begin(), items.end(), CompareBy<Armor, decltype(compareFunc)>(_ascending, compareFunc));
}

void Inventory::SortByResistance(bool _ascending)
{
	auto compareFunc = [](const std::shared_ptr<Armor>& a, const std::shared_ptr<Armor>& b) {
		return a->GetResistance() < b->GetResistance();
		};
	std::sort(items.begin(), items.end(), CompareBy<Armor, decltype(compareFunc)>(_ascending, compareFunc));
}

void Inventory::SortByLuck(bool _ascending)
{
	auto compareFunc = [](const std::shared_ptr<Armor>& a, const std::shared_ptr<Armor>& b) {
		return a->GetLuck() < b->GetLuck();
		};
	std::sort(items.begin(), items.end(), CompareBy<Armor, decltype(compareFunc)>(_ascending, compareFunc));
}

void Inventory::SortByQuantity(bool _ascending)
{

	std::sort(items.begin(), items.end(), CompareByQuantity(_ascending));
}

// Filter functions
void Inventory::AddItemTypeFilter(ItemType _itemType)
{
	filters.push_back(_itemType);
}

void Inventory::RemoveItemTypeFilter(ItemType _itemType)
{
	for (int i = 0; i < filters.size(); i++)
	{
		filters.erase(filters.begin() + i);
	}
}

void Inventory::RemoveFilteredItemFromInventory(std::shared_ptr<Item>& _itemToFilter)
{
	for (int i = 0; i < items.size(); i++)
	{
		if (items[i].first == _itemToFilter)
		{
			items.erase(items.begin()+ i);
		}
	}
}

void Inventory::FilterInventory()
{
	for (int i = 0; i < items.size(); i++)
	{
		if (std::find(filters.begin(), filters.end(), items[i].first->GetItemType()) == filters.end())
		{
			items_filtered.push_back(items[i]);
		}
	}
	for (int i = 0; i < items_filtered.size(); i++)
	{
		RemoveFilteredItemFromInventory(items_filtered[i].first);
	}
}


// Print functions

template <typename T>
void AppendIfNotZero(std::ostream& os, const std::string& label, T value) {
	if (value != 0) {
		os << " " << label << ": " << value << " ";
	}
}

void AppendIfNotZero(std::ostream& os, const std::string& label, const std::string& value) {
	if (!value.compare("")) {
		os << " " << label << ": " << value;
	}
}


void Inventory::PrintWeapon(const std::shared_ptr<Weapon>& _weapon, const int _quantity, const int _num)
{
	PrintItem(_weapon, _quantity, _num, RED);
	std::cout << " Weapon Type: " << _weapon->GetWeaponType() << std::endl;
	PrintEquipment(_weapon);

	AppendIfNotZero(std::cout, "Attack", _weapon->GetAttack());
	AppendIfNotZero(std::cout, "Attack Speed", _weapon->GetAttackSpeed());
	AppendIfNotZero(std::cout, "Critical Rate", _weapon->GetCritRate());
	AppendIfNotZero(std::cout, "Critical Damage", _weapon->GetCritDamage());
	AppendIfNotZero(std::cout, "Accuracy", _weapon->GetAccuracy());
	AppendIfNotZero(std::cout, "Cooldown Reduction", _weapon->GetCooldownReduction());
	AppendIfNotZero(std::cout, "Life Steal", _weapon->GetLifeSteal());
	AppendIfNotZero(std::cout, "Skill", _weapon->GetSkill());
	std::cout << std::endl;
}

void Inventory::PrintArmor(const std::shared_ptr<Armor>& _armor, const int _quantity, const int _num)
{
	PrintItem(_armor, _quantity, _num, BLUE);
	std::cout << " Armor Type: " << _armor->GetArmorType() << std::endl;
	PrintEquipment(_armor);

	AppendIfNotZero(std::cout, "Health", _armor->GetHealth());
	AppendIfNotZero(std::cout, "Dodge Rate", _armor->GetDodgeRate());
	AppendIfNotZero(std::cout, "Resistance", _armor->GetResistance());
	AppendIfNotZero(std::cout, "Health Regen", _armor->GetHealthRegen());
	AppendIfNotZero(std::cout, "Luck", _armor->GetLuck());
	AppendIfNotZero(std::cout, "Skill", _armor->GetSkill());
	std::cout << std::endl;

}

void Inventory::PrintItem(const std::shared_ptr<Item>& _item, const int _quantity, const int _num, const std::string _color)
{
	std::cout << _num << ") " << "ID: " << _item->GetID() << _color << " Name: " << _item->GetName() << RESET << std::endl << " Description: " << _item->GetDescription() << std::endl
		<< " Amount: " << _quantity << " Price: " << _item->GetSellPrice() << std::endl;
}

void Inventory::PrintEquipment(const std::shared_ptr<Equipment>& _equipment)
{
	AppendIfNotZero(std::cout, "Level", _equipment->GetLevel());
	AppendIfNotZero(std::cout, "Defense", _equipment->GetDefense());
	
}


void Inventory::ShowInventory()
{
	std::cout << "Inventory: " << std::endl;
	for (int i = 0; i < items.size(); i++)
	{
		switch (items[i].first->GetItemType())
		{
		case ItemType::item_Weapon:
			PrintWeapon(std::dynamic_pointer_cast<Weapon>(items[i].first), items[i].second, i);
			break;
		case ItemType::item_Armor:
			PrintArmor(std::dynamic_pointer_cast<Armor>(items[i].first), items[i].second, i);
			break;
		default:
			PrintItem(items[i].first, items[i].second, i, WHITE);
			break;
		}
		std::cout << std::endl;
	}
}

void Inventory::ShowInventoryTemp()
{
	std::cout << "Inventory: " << std::endl;
	for (int i = 0; i < items_filtered.size(); i++)
	{
		switch (items_filtered[i].first->GetItemType())
		{
		case ItemType::item_Weapon:
			PrintWeapon(std::dynamic_pointer_cast<Weapon>(items_filtered[i].first), items_filtered[i].second, i);
			break;
		case ItemType::item_Armor:
			PrintArmor(std::dynamic_pointer_cast<Armor>(items_filtered[i].first), items_filtered[i].second, i);
			break;
		default:
			PrintItem(items_filtered[i].first, items_filtered[i].second, i, WHITE);
			break;
		}
		std::cout << std::endl;
	}
}

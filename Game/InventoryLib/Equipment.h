#pragma once
#include "Item.h"
//#include "Skill.h"

class Equipment : public Item
{
private:
	int level = 1;
	int defense = 0;
	std::string skill = ""; //Equipment can have
protected:
	//void SetSkill(std::string _skill) { skill = _skill; }
public:
	Equipment(int _type, int _ID, std::string _name,std::string _desc, int _sell_price = 0, bool _isStackable = true, int _level = 1, int _defense = 0, std::string _skill = "");
	~Equipment() = default;
	int GetLevel() const { return level; }
	int GetDefense() const { return defense; }
	std::string GetSkill() const { return skill; }
};
#pragma once

#include <d3dx9.h>
#include <set>
#include <string>
#include <unordered_map>

class Player;
class Weapon;

class Skins
{
public:
	std::unordered_map<std::string, std::string> m_icon_overrides = { };
	Skins() {}
	void skinfix(int itemfix);
	void ParseSkins() const;
	void FrameStageNotify(bool frame_end);
	const char* GetIconOverride(const std::string& original);
	void EraseOverrideIfExistsByIndex(short definition_index);
	bool ApplyCustomSkin(Player* localPlayer, Weapon* pWeapon, short nWeaponIndex);
	void iconfix(std::string fix);
};

extern Skins g_Skins;

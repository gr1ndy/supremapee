#pragma once

#pragma warning( disable : 4307 ) // '*': integral constant overflow
#pragma warning( disable : 4244 ) // possible loss of data
#pragma warning( disable : 4800 ) // forcing value to bool 'true' or 'false'
#pragma warning( disable : 4838 ) // conversion from '::size_t' to 'int' requires a narrowing conversion

// You can define _SILENCE_CXX17_OLD_ALLOCATOR_MEMBERS_DEPRECATION_WARNING or _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS to acknowledge that you have received this warning.
#define _SILENCE_CXX17_OLD_ALLOCATOR_MEMBERS_DEPRECATION_WARNING
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

using ulong_t = unsigned long;

// windows / stl includes.
#include <Windows.h>
#include <cstdint>
#include <intrin.h>
#include <xmmintrin.h>
#include <array>
#include <vector>
#include <algorithm>
#include <cctype>
#include <string>
#include <chrono>
#include <thread>
#include <memory>
#include <sstream>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <deque>
#include <functional>
#include <map>
#include <shlobj.h>
#include <filesystem>
#include <streambuf>
#include <d3d9.h>
#include <d3dx9.h>
// our custom wrapper.
#include "unique_vector.h"
#include "tinyformat.h"

// other includes.
#include "hash.h"
#include "xorstr.h"
#include "pe.h"
#include "winapir.h"
#include "address.h"
#include "util.h"
#include "modules.h"
#include "pattern.h"
#include "vmt.h"
#include "stack.h"
#include "nt.h"
#include "x86.h"
#include "syscall.h"
#include "detours.h"
// hack includes.
#include "interfaces.h"
#include "sdk.h"
#include "csgo.h"
#include "penetration.h"
#include "netvars.h"
#include "entoffsets.h"
#include "entity.h"
#include "client.h"
#include "gamerules.h"
#include "vmt for testing/vfunc_hook.hpp"
#include "hooks.h"
#include "render.h"
#include "pred.h"
#include "lagrecord.h"
#include "visuals.h"
#include "movement.h"
#include "bonesetup.h"
#include "hvh.h"
#include "lagcomp.h"
#include "aimbot.h"
#include "netdata.h"
#include "chams.h"
#include "notify.h"
#include "resolver.h"
#include "grenades.h"
#include "skins.h"
#include "events.h"
#include "shots.h"
#include "tickbase_shift.h"

// gui includes.
#include "json.h"
#include "base64.h"
#include "element.h"
#include "checkbox.h"
#include "dropdown.h"
#include "multidropdown.h"
#include "slider.h"
#include "colorpicker.h"
#include "edit.h"
#include "keybind.h"
#include "button.h"
#include "tab.h"
#include "form.h"
#include "gui.h"
#include "callbacks.h"
#include "menu.h"
#include "config.h"


enum ItemDefinitionIndex
{
	WEAPON_NONE = 0,
	WEAPON_DEAGLE,
	WEAPON_ELITE,
	WEAPON_FIVESEVEN,
	WEAPON_GLOCK,
	WEAPON_AK47 = 7,
	WEAPON_AUG,
	WEAPON_AWP,
	WEAPON_FAMAS,
	WEAPON_G3SG1,
	WEAPON_GALIL = 13,
	WEAPON_M249,
	WEAPON_M4A4 = 16,
	WEAPON_MAC10,
	WEAPON_P90 = 19,
	WEAPON_MP5SD = 23,
	WEAPON_UMP45,
	WEAPON_XM1014,
	WEAPON_BIZON,
	WEAPON_MAG7,
	WEAPON_NEGEV,
	WEAPON_SAWEDOFF,
	WEAPON_TEC9,
	WEAPON_ZEUS,
	WEAPON_P2000,
	WEAPON_MP7,
	WEAPON_MP9,
	WEAPON_NOVA,
	WEAPON_P250,
	WEAPON_SHIELD,
	WEAPON_SCAR20,
	WEAPON_SG553,
	WEAPON_SSG08,
	WEAPON_KNIFEGG,
	WEAPON_KNIFE,
	WEAPON_FLASHBANG,
	WEAPON_HEGRENADE,
	WEAPON_SMOKEGRENADE,
	WEAPON_MOLOTOV,
	WEAPON_DECOY,
	WEAPON_INC,
	WEAPON_C4,
	WEAPON_HEALTHSHOT = 57,
	WEAPON_KNIFE_T = 59,
	WEAPON_M4A1S,
	WEAPON_USPS,
	WEAPON_CZ75 = 63,
	WEAPON_REVOLVER,
	WEAPON_TAGRENADE = 68,
	WEAPON_FISTS,
	WEAPON_BREACHCHARGE,
	WEAPON_TABLET = 72,
	WEAPON_MELEE = 74,
	WEAPON_AXE,
	WEAPON_HAMMER,
	WEAPON_SPANNER = 78,
	WEAPON_KNIFE_GHOST = 80,
	WEAPON_FIREBOMB,
	WEAPON_DIVERSION,
	WEAPON_FRAG_GRENADE,
	WEAPON_SNOWBALL,
	WEAPON_BUMPMINE,
	WEAPON_KNIFE_BAYONET = 500,
	WEAPON_KNIFE_CSS = 503,
	WEAPON_KNIFE_FLIP = 505,
	WEAPON_KNIFE_GUT,
	WEAPON_KNIFE_KARAMBIT,
	WEAPON_KNIFE_M9_BAYONET,
	WEAPON_KNIFE_TACTICAL,
	WEAPON_KNIFE_FALCHION = 512,
	WEAPON_KNIFE_SURVIVAL_BOWIE = 514,
	WEAPON_KNIFE_BUTTERFLY,
	WEAPON_KNIFE_PUSH = 516,
	WEAPON_KNIFE_CORD = 517,
	WEAPON_KNIFE_CANIS = 518,
	WEAPON_KNIFE_URSUS = 519,
	WEAPON_KNIFE_GYPSY_JACKKNIFE,
	WEAPON_KNIFE_OUTDOOR = 521,
	WEAPON_KNIFE_STILETTO = 522,
	WEAPON_KNIFE_WIDOWMAKER,
	WEAPON_KNIFE_SKELETON = 525,
	GLOVE_STUDDED_BLOODHOUND = 5027,
	GLOVE_T_SIDE = 5028,
	GLOVE_CT_SIDE = 5029,
	GLOVE_SPORTY = 5030,
	GLOVE_SLICK = 5031,
	GLOVE_LEATHER_WRAP = 5032,
	GLOVE_MOTORCYCLE = 5033,
	GLOVE_SPECIALIST = 5034,
	GLOVE_HYDRA = 5035
};
#include "steam_sdk/steam_api.h"


struct skinInfo
{
	std::string name;
	std::string cdnName;
	std::vector<std::string> weaponName;
	int rarity;
};


struct skins_s
{
	struct sides
	{
		bool enabled = false;
		int definitionIndex = 0;
		int paintKit = 0;
		int seed = 0;
		int stattrak = -1;
		int quality = 4;
		float wear = FLT_MIN;
	};

	sides t;
	sides ct;
};
struct weapons
{
	skins_s skins;
};

class inv_fix
{
public:
	std::map<short, weapons> weapons;
	struct xd
	{
		std::unordered_map<int, skinInfo> skinInfo;
		int itemCount = 0;
		bool enabled = false;
	} inventory;
};

inline inv_fix _inv;
#include "inventorychanger.h"
#include "../protobuffs/Protobuffs.h"
#include "../protobuffs/ProtoWriter.h"
#include "../protobuffs/Messages.h"
#include "../includes.h"


static const char* weaponnames(const short id)
{
	switch (id)
	{
	case WEAPON_DEAGLE:
		return "deagle";
	case WEAPON_ELITE:
		return "elite";
	case WEAPON_FIVESEVEN:
		return "fiveseven";
	case WEAPON_GLOCK:
		return "glock";
	case WEAPON_AK47:
		return "ak47";
	case WEAPON_AUG:
		return "aug";
	case WEAPON_AWP:
		return "awp";
	case WEAPON_FAMAS:
		return "famas";
	case WEAPON_G3SG1:
		return "g3sg1";
	case WEAPON_GALIL:
		return "galilar";
	case WEAPON_M249:
		return "m249";
	case WEAPON_M4A1S:
		return "m4a1_silencer";
	case WEAPON_M4A4:
		return "m4a1";
	case WEAPON_MAC10:
		return "mac10";
	case WEAPON_P90:
		return "p90";
	case WEAPON_UMP45:
		return "ump45";
	case WEAPON_XM1014:
		return "xm1014";
	case WEAPON_BIZON:
		return "bizon";
	case WEAPON_MAG7:
		return "mag7";
	case WEAPON_NEGEV:
		return "negev";
	case WEAPON_SAWEDOFF:
		return "sawedoff";
	case WEAPON_TEC9:
		return "tec9";
	case WEAPON_P2000:
		return "hkp2000";
	case WEAPON_MP5SD:
		return "mp5sd";
	case WEAPON_MP7:
		return "mp7";
	case WEAPON_MP9:
		return "mp9";
	case WEAPON_NOVA:
		return "nova";
	case WEAPON_P250:
		return "p250";
	case WEAPON_SCAR20:
		return "scar20";
	case WEAPON_SG553:
		return "sg556";
	case WEAPON_SSG08:
		return "ssg08";
	case WEAPON_USPS:
		return "usp_silencer";
	case WEAPON_CZ75:
		return "cz75a";
	case WEAPON_REVOLVER:
		return "revolver";
	case WEAPON_KNIFE:
		return "knife";
	case WEAPON_KNIFE_T:
		return "knife_t";
	case WEAPON_KNIFE_M9_BAYONET:
		return "knife_m9_bayonet";
	case WEAPON_KNIFE_BAYONET:
		return "bayonet";
	case WEAPON_KNIFE_FLIP:
		return "knife_flip";
	case WEAPON_KNIFE_GUT:
		return "knife_gut";
	case WEAPON_KNIFE_KARAMBIT:
		return "knife_karambit";
	case WEAPON_KNIFE_TACTICAL:
		return "knife_tactical";
	case WEAPON_KNIFE_FALCHION:
		return "knife_falchion";
	case WEAPON_KNIFE_SURVIVAL_BOWIE:
		return "knife_survival_bowie";
	case WEAPON_KNIFE_BUTTERFLY:
		return "knife_butterfly";
	case WEAPON_KNIFE_PUSH:
		return "knife_push";
	case WEAPON_KNIFE_URSUS:
		return "knife_ursus";
	case WEAPON_KNIFE_GYPSY_JACKKNIFE:
		return "knife_gypsy_jackknife";
	case WEAPON_KNIFE_STILETTO:
		return "knife_stiletto";
	case WEAPON_KNIFE_WIDOWMAKER:
		return "knife_widowmaker";
	case WEAPON_KNIFE_SKELETON:
		return "knife_skeleton";
	case WEAPON_KNIFE_OUTDOOR:
		return "knife_outdoor";
	case WEAPON_KNIFE_CANIS:
		return "knife_canis";
	case WEAPON_KNIFE_CORD:
		return "knife_cord";
	case WEAPON_KNIFE_CSS:
		return "knife_css";
	case GLOVE_STUDDED_BLOODHOUND:
		return "studded_bloodhound_gloves";
	case GLOVE_T_SIDE:
		return "t_gloves";
	case GLOVE_CT_SIDE:
		return "ct_gloves";
	case GLOVE_SPORTY:
		return "sporty_gloves";
	case GLOVE_SLICK:
		return "slick_gloves";
	case GLOVE_LEATHER_WRAP:
		return "leather_handwraps";
	case GLOVE_MOTORCYCLE:
		return "motorcycle_gloves";
	case GLOVE_SPECIALIST:
		return "specialist_gloves";
	case GLOVE_HYDRA:
		return "studded_hydra_gloves";
	default:
		return "";
	}
}

std::string Inventory::Changer(void* pubDest, uint32_t* pcubMsgSize)
{
	ProtoWriter msg((void*)((DWORD)pubDest + 8), *pcubMsgSize - 8, 11);
	if (msg.getAll(CMsgClientWelcome::outofdate_subscribed_caches).empty())
		return msg.serialize();

	ProtoWriter cache(msg.get(CMsgClientWelcome::outofdate_subscribed_caches).String(), 4);
	// If not have items in inventory, Create null inventory
	FixNullInventory(cache);
	// Add custom items
	auto objects = cache.getAll(CMsgSOCacheSubscribed::objects);
	for (size_t i = 0; i < objects.size(); i++)
	{
		ProtoWriter object(objects[i].String(), 2);

		if (!object.has(SubscribedType::type_id))
			continue;

		if (object.get(SubscribedType::type_id).Int32() == 1)
		{
			object.clear(SubscribedType::object_data);

			ClearEquipState(object);
			AddAllItems(object);
			cache.replace(Field(CMsgSOCacheSubscribed::objects, TYPE_STRING, object.serialize()), i);
		}
	}
	msg.replace(Field(CMsgClientWelcome::outofdate_subscribed_caches, TYPE_STRING, cache.serialize()), 0);

	return msg.serialize();
}

void Inventory::FixNullInventory(ProtoWriter& cache)
{
	bool inventory_exist = false;
	auto objects = cache.getAll(CMsgSOCacheSubscribed::objects);
	for (size_t i = 0; i < objects.size(); i++)
	{
		ProtoWriter object(objects[i].String(), 2);
		if (!object.has(SubscribedType::type_id))
			continue;
		if (object.get(SubscribedType::type_id).Int32() != 1)
			continue;
		inventory_exist = true;
		break;
	}
	if (!inventory_exist)
	{
		ProtoWriter null_object(2);
		null_object.add(Field(SubscribedType::type_id, TYPE_INT32, (int64_t)1));

		cache.add(Field(CMsgSOCacheSubscribed::objects, TYPE_STRING, null_object.serialize()));
	}
}

void Inventory::ClearEquipState(ProtoWriter& object)
{
	auto object_data = object.getAll(SubscribedType::object_data);
	for (size_t j = 0; j < object_data.size(); j++)
	{
		ProtoWriter item(object_data[j].String(), 19);

		if (item.getAll(CSOEconItem::equipped_state).empty())
			continue;

		// create NOT equiped state for item 
		ProtoWriter null_equipped_state(2);
		null_equipped_state.replace(Field(CSOEconItemEquipped::new_class, TYPE_UINT32, (int64_t)0));
		null_equipped_state.replace(Field(CSOEconItemEquipped::new_slot, TYPE_UINT32, (int64_t)0));
		// unequip all 
		auto equipped_state = item.getAll(CSOEconItem::equipped_state);
		for (size_t k = 0; k < equipped_state.size(); k++)
			item.replace(Field(CSOEconItem::equipped_state, TYPE_STRING, null_equipped_state.serialize()), k);

		object.replace(Field(SubscribedType::object_data, TYPE_STRING, item.serialize()), j);
	}
}

static auto is_knife(const int i) -> bool
{
	return (i >= 500 && i < 5027) || i == 59 || i == 42;
}

static auto is_glove(const int i) -> bool
{
	return (i >= 5027 && i <= 5035);
}

void Inventory::AddAllItems(ProtoWriter& object)
{
	for (auto& x : g_InventorySkins)
		AddItem(object, x.first, x.second.wId, _inv.inventory.skinInfo[x.second.paintKit].rarity, x.second.paintKit, x.second.seed, x.second.wear, x.second.name);
}

bool is_uncommon(int index)
{
	switch (index)
	{
	case WEAPON_DEAGLE:
	case WEAPON_GLOCK:
	case WEAPON_AK47:
	case WEAPON_AWP:
	case WEAPON_M4A4:
	case WEAPON_M4A1S:
	case WEAPON_P2000:
	case WEAPON_USPS:
		return true;
	default:
		return false;
	}
}

void Inventory::AddItem(ProtoWriter& object, int index, int itemIndex, int rarity, int paintKit, int seed, float wear, std::string name)
{
	uint32_t steamid = SteamUser->GetSteamID().GetAccountID();
	if (!steamid)
		return;
	ProtoWriter item(19);
	item.add(Field(CSOEconItem::id, TYPE_UINT64, (int64_t)index));
	item.add(Field(CSOEconItem::account_id, TYPE_UINT32, (int64_t)steamid));
	item.add(Field(CSOEconItem::def_index, TYPE_UINT32, (int64_t)itemIndex));
	item.add(Field(CSOEconItem::inventory, TYPE_UINT32, (int64_t)index));
	item.add(Field(CSOEconItem::origin, TYPE_UINT32, (int64_t)24));
	item.add(Field(CSOEconItem::quantity, TYPE_UINT32, (int64_t)1));
	item.add(Field(CSOEconItem::level, TYPE_UINT32, (int64_t)1));
	item.add(Field(CSOEconItem::style, TYPE_UINT32, (int64_t)0));
	item.add(Field(CSOEconItem::flags, TYPE_UINT32, (int64_t)0));
	item.add(Field(CSOEconItem::in_use, TYPE_BOOL, (int64_t)false));
	item.add(Field(CSOEconItem::original_id, TYPE_UINT64, (int64_t)itemIndex));
	if (is_uncommon(itemIndex))
		rarity++;
	if (is_knife(itemIndex) || is_glove(itemIndex))
	{
		item.add(Field(CSOEconItem::quality, TYPE_UINT32, (int64_t)3));
		rarity = 6;
	}
	item.add(Field(CSOEconItem::rarity, TYPE_UINT32, (int64_t)rarity));

	if (name.length() > 0)
		item.add(Field(CSOEconItem::custom_name, TYPE_STRING, name));
	// Paint Kit
	float _PaintKitAttributeValue = (float)paintKit;
	auto PaintKitAttributeValue = std::string{ reinterpret_cast<const char*>((void*)&_PaintKitAttributeValue), 4 };
	ProtoWriter PaintKitAttribute(3);
	PaintKitAttribute.add(Field(CSOEconItemAttribute::def_index, TYPE_UINT32, (int64_t)6));
	PaintKitAttribute.add(Field(CSOEconItemAttribute::value_bytes, TYPE_STRING, PaintKitAttributeValue));
	item.add(Field(CSOEconItem::attribute, TYPE_STRING, PaintKitAttribute.serialize()));

	// Paint Seed
	float _SeedAttributeValue = (float)seed;
	auto SeedAttributeValue = std::string{ reinterpret_cast<const char*>((void*)&_SeedAttributeValue), 4 };
	ProtoWriter SeedAttribute(3);
	SeedAttribute.add(Field(CSOEconItemAttribute::def_index, TYPE_UINT32, (int64_t)7));
	SeedAttribute.add(Field(CSOEconItemAttribute::value_bytes, TYPE_STRING, SeedAttributeValue));
	item.add(Field(CSOEconItem::attribute, TYPE_STRING, SeedAttribute.serialize()));

	// Paint Wear
	float _WearAttributeValue = wear;
	auto WearAttributeValue = std::string{ reinterpret_cast<const char*>((void*)&_WearAttributeValue), 4 };
	ProtoWriter WearAttribute(3);
	WearAttribute.add(Field(CSOEconItemAttribute::def_index, TYPE_UINT32, (int64_t)8));
	WearAttribute.add(Field(CSOEconItemAttribute::value_bytes, TYPE_STRING, WearAttributeValue));
	item.add(Field(CSOEconItem::attribute, TYPE_STRING, WearAttribute.serialize()));

	object.add(Field(SubscribedType::object_data, TYPE_STRING, item.serialize()));
}

uint8_t* ssdgfadefault(HMODULE hModule, const char* szSignature)
{
	static auto pattern_to_byte = [](const char* pattern) {
		auto bytes = std::vector<int>{};
		auto start = const_cast<char*>(pattern);
		auto end = const_cast<char*>(pattern) + strlen(pattern);

		for (auto current = start; current < end; ++current) {
			if (*current == '?') {
				++current;
				if (*current == '?')
					++current;
				bytes.push_back(-1);
			}
			else {
				bytes.push_back(strtoul(current, &current, 16));
			}
		}
		return bytes;
	};

	//auto Module = GetModuleHandleA(szModule);

	auto dosHeader = (PIMAGE_DOS_HEADER)hModule;
	auto ntHeaders = (PIMAGE_NT_HEADERS)((std::uint8_t*)hModule + dosHeader->e_lfanew);

	auto sizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;
	auto patternBytes = pattern_to_byte(szSignature);
	auto scanBytes = reinterpret_cast<std::uint8_t*>(hModule);

	auto s = patternBytes.size();
	auto d = patternBytes.data();

	for (auto i = 0ul; i < sizeOfImage - s; ++i) {
		bool found = true;
		for (auto j = 0ul; j < s; ++j) {
			if (scanBytes[i + j] != d[j] && d[j] != -1) {
				found = false;
				break;
			}
		}
		if (found) {
			return &scanBytes[i];
		}
	}

	return nullptr;
};

template<class T>
static T* FindHudElement(const char* name)
{
	static auto pThis = *reinterpret_cast<DWORD**>(ssdgfadefault(GetModuleHandleA("client.dll"), ("B9 ? ? ? ? E8 ? ? ? ? 8B 5D 08")) + 1);

	static auto find_hud_element = reinterpret_cast<DWORD(__thiscall*)(void*, const char*)>(ssdgfadefault(GetModuleHandleA("client.dll"), ("55 8B EC 53 8B 5D 08 56 57 8B F9 33 F6 39 77 28")));
	return (T*)find_hud_element(pThis, name);
}

struct hud_weapons_t {
	std::int32_t* get_weapon_count() {
		return reinterpret_cast<std::int32_t*>(std::uintptr_t(this) + 0x80);
	}
};



void force_full_update()
{
	static auto fn = reinterpret_cast<std::int32_t(__thiscall*)(void*, std::int32_t)>(ssdgfadefault(GetModuleHandleA("client.dll"), ("55 8B EC 51 53 56 8B 75 08 8B D9 57 6B FE 2C")));
	
	auto element = FindHudElement<std::uintptr_t*>(("CCSGO_HudWeaponSelection"));

	auto hud_weapons = reinterpret_cast<hud_weapons_t*>(std::uintptr_t(element) - 0xA0);

	if (hud_weapons == nullptr)
		return;

	if (!*hud_weapons->get_weapon_count())
		return;

	for (std::int32_t i = 0; i < *hud_weapons->get_weapon_count(); i++)
		i = fn(hud_weapons, i);


}






bool Inventory::Presend(uint32_t& unMsgType, void* pubData, uint32_t& cubData)
{
	uint32_t MessageType = unMsgType & 0x7FFFFFFF;

	if (MessageType == k_EMsgGCAdjustItemEquippedState) {

		ProtoWriter msg((void*)((DWORD)pubData + 8), cubData - 8, 19);

		if (!msg.has(CMsgAdjustItemEquippedState::item_id)
			|| !msg.has(CMsgAdjustItemEquippedState::new_class)
			|| !msg.has(CMsgAdjustItemEquippedState::new_slot))
			return true;

		uint32_t item_id = msg.get(CMsgAdjustItemEquippedState::item_id).UInt32();
		uint32_t new_class = msg.get(CMsgAdjustItemEquippedState::new_class).UInt32();

		auto weapon = g_InventorySkins[item_id];

		if (new_class == 2)
		{
			for (auto& skins : g_InventorySkins)
			{
				if (Protobuffs::GetSlotID(skins.second.wId) == Protobuffs::GetSlotID(weapon.wId))
					skins.second.in_use_t = false;
			}

			g_InventorySkins[item_id].in_use_t = true;
			_inv.weapons[weapon.wId].skins.t.enabled = true;
			_inv.weapons[weapon.wId].skins.t.paintKit = weapon.paintKit;
			_inv.weapons[weapon.wId].skins.t.wear = weapon.wear;
			_inv.weapons[weapon.wId].skins.t.seed = weapon.seed;

			if (is_knife(weapon.wId))
			{
				_inv.weapons[WEAPON_KNIFE].skins.t.enabled = true;
				_inv.weapons[WEAPON_KNIFE].skins.t.definitionIndex = weapon.wId;
				_inv.weapons[WEAPON_KNIFE].skins.t.paintKit = weapon.paintKit;
				_inv.weapons[WEAPON_KNIFE].skins.t.wear = weapon.wear;
				_inv.weapons[WEAPON_KNIFE].skins.t.seed = weapon.seed;
			}
			else if (is_glove(weapon.wId))
			{
				_inv.weapons[GLOVE_T_SIDE].skins.t.enabled = true;
				_inv.weapons[GLOVE_T_SIDE].skins.t.definitionIndex = weapon.wId;
				_inv.weapons[GLOVE_T_SIDE].skins.t.paintKit = weapon.paintKit;
				_inv.weapons[GLOVE_T_SIDE].skins.t.wear = weapon.wear;
				_inv.weapons[GLOVE_T_SIDE].skins.t.seed = weapon.seed;
			}
		}
		else
		{
			for (auto& skins : g_InventorySkins)
			{
				if (Protobuffs::GetSlotID(skins.second.wId) == Protobuffs::GetSlotID(weapon.wId))
					skins.second.in_use_ct = false;
			}

			_inv.weapons[weapon.wId].skins.ct.enabled = true;
			_inv.weapons[weapon.wId].skins.ct.paintKit = weapon.paintKit;
			_inv.weapons[weapon.wId].skins.ct.wear = weapon.wear;
			_inv.weapons[weapon.wId].skins.ct.seed = weapon.seed;

			if (is_knife(weapon.wId))
			{
				_inv.weapons[WEAPON_KNIFE].skins.ct.enabled = true;
				_inv.weapons[WEAPON_KNIFE].skins.ct.definitionIndex = weapon.wId;
				_inv.weapons[WEAPON_KNIFE].skins.ct.paintKit = weapon.paintKit;
				_inv.weapons[WEAPON_KNIFE].skins.ct.wear = weapon.wear;
				_inv.weapons[WEAPON_KNIFE].skins.ct.seed = weapon.seed;
			}
			else if (is_glove(weapon.wId))
			{
				_inv.weapons[GLOVE_T_SIDE].skins.ct.enabled = true;
				_inv.weapons[GLOVE_T_SIDE].skins.ct.definitionIndex = weapon.wId;
				_inv.weapons[GLOVE_T_SIDE].skins.ct.paintKit = weapon.paintKit;
				_inv.weapons[GLOVE_T_SIDE].skins.ct.wear = weapon.wear;
				_inv.weapons[GLOVE_T_SIDE].skins.ct.seed = weapon.seed;
			}
		}
		force_full_update();
		g_csgo.m_cl->ForceFullUpdate();

		return false;
	}
	return true;
}

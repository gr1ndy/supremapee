#include <Windows.h>
#include <WinInet.h>
#include "../includes.h"
#include "skins.h"
#include "../inventory/items.h"
#include "parser.h"
#include <array>


struct skin_info
{
	int seed = -1;
	int paintkit;
	int rarity = 0;
	std::string tag_name;
	std::string cdn_name;
};

#pragma comment(lib, "Wininet")

CreateClientClass_t GetWearableCreateFn()
{
	auto clazz = g_csgo.m_client->GetAllClasses();
	// Please, if you gonna paste it into a cheat use classids here. I use names because they
	// won't change in the foreseeable future and i dont need high speed, but chances are
	// you already have classids, so use them instead, they are faster.
	while (strcmp(clazz->m_pNetworkName, "CEconWearable"))
		clazz = clazz->m_pNext;
	return clazz->m_pCreate;
}

const weapon_info* GetWeaponInfo(const short& defindex)
{
	const auto entry = k_weapon_info.find(defindex);

	return entry == k_weapon_info.end() ? nullptr : &entry->second;
}

void Skins::EraseOverrideIfExistsByIndex(const short definition_index)
{
	if (const auto original_item = GetWeaponInfo(definition_index))
	{
		if (!original_item->icon)
			return;

		const auto override_entry = m_icon_overrides.find(original_item->icon);
		if (override_entry != std::end(m_icon_overrides))
			m_icon_overrides.erase(override_entry);
	}
}

const char* Skins::GetIconOverride(const std::string& original)
{
	return m_icon_overrides.count(original) > 0 ? m_icon_overrides.at(original).data() : nullptr;
}

int GetWeaponRarity(std::string rarity)
{
	if (rarity == "default")
		return 0;
	else if (rarity == "common")
		return 1;
	else if (rarity == "uncommon")
		return 2;
	else if (rarity == "rare")
		return 3;
	else if (rarity == "mythical")
		return 4;
	else if (rarity == "legendary")
		return 5;
	else if (rarity == "ancient")
		return 6;
	else if (rarity == "immortal")
		return 7;
	else if (rarity == "unusual")
		return 99;

	return 0;
}

void Skins::ParseSkins() const
{
	std::unordered_map<std::string, std::set<std::string>> weaponSkins;
	std::unordered_map<std::string, skin_info> skinMap;
	std::unordered_map<std::string, std::string> skinNames;

	valve_parser::Document doc;
	auto r = doc.Load(R"(.\csgo\scripts\items\items_game.txt)", valve_parser::ENCODING::UTF8);
	if (!r)
		return;

	valve_parser::Document english;
	r = english.Load(R"(.\csgo\resource\csgo_english.txt)", valve_parser::ENCODING::UTF16_LE);
	if (!r)
		return;

	auto weapon_skin_combo = doc.BreadthFirstSearch("weapon_icons");
	if (!weapon_skin_combo || !weapon_skin_combo->ToObject())
		return;

	auto paint_kits_rarity = doc.BreadthFirstSearchMultiple("paint_kits_rarity");
	if (paint_kits_rarity.empty())
		return;

	auto skin_data_vec = doc.BreadthFirstSearchMultiple("paint_kits");
	if (skin_data_vec.empty())
		return;

	auto paint_kit_names = english.BreadthFirstSearch("Tokens");
	if (!paint_kit_names || !paint_kit_names->ToObject())
		return;

	std::array weaponNames = {
		std::string("deagle"),
		std::string("elite"),
		std::string("fiveseven"),
		std::string("glock"),
		std::string("ak47"),
		std::string("aug"),
		std::string("awp"),
		std::string("famas"),
		std::string("g3sg1"),
		std::string("galilar"),
		std::string("m249"),
		std::string("m4a1_silencer"),
		std::string("m4a1"),
		std::string("mac10"),
		std::string("p90"),
		std::string("ump45"),
		std::string("xm1014"),
		std::string("bizon"),
		std::string("mag7"),
		std::string("negev"),
		std::string("sawedoff"),
		std::string("tec9"),
		std::string("hkp2000"),
		std::string("mp5sd"),
		std::string("mp7"),
		std::string("mp9"),
		std::string("nova"),
		std::string("p250"),
		std::string("scar20"),
		std::string("sg556"),
		std::string("ssg08"),
		std::string("usp_silencer"),
		std::string("cz75a"),
		std::string("revolver"),
		std::string("knife_m9_bayonet"),
		std::string("bayonet"),
		std::string("knife_flip"),
		std::string("knife_gut"),
		std::string("knife_karambit"),
		std::string("knife_tactical"),
		std::string("knife_falchion"),
		std::string("knife_survival_bowie"),
		std::string("knife_butterfly"),
		std::string("knife_push"),
		std::string("knife_ursus"),
		std::string("knife_gypsy_jackknife"),
		std::string("knife_stiletto"),
		std::string("knife_widowmaker"),
		std::string("knife_skeleton"),
		std::string("knife_outdoor"),
		std::string("knife_canis"),
		std::string("knife_cord"),
		std::string("knife_css"),
		std::string("studded_bloodhound_gloves"),
		std::string("sporty_gloves"),
		std::string("slick_gloves"),
		std::string("leather_handwraps"),
		std::string("motorcycle_gloves"),
		std::string("specialist_gloves"),
		std::string("studded_hydra_gloves")
	};

	for (const auto& child : weapon_skin_combo->children)
	{
		if (child->ToObject())
		{
			for (const auto& weapon : weaponNames)
			{
				auto skin_name = child->ToObject()->GetKeyByName("icon_path")->ToKeyValue()->Value.toString();
				const auto pos = skin_name.find(weapon);
				if (pos != std::string::npos)
				{
					const auto pos2 = skin_name.find_last_of('_');
					weaponSkins[weapon].insert(
						skin_name.substr(pos + weapon.length() + 1,
							pos2 - pos - weapon.length() - 1)
					);
					break;
				}
			}
		}
	}

	for (const auto& skin_data : skin_data_vec)
	{
		if (skin_data->ToObject())
		{
			for (const auto& skin : skin_data->children)
			{
				if (skin->ToObject())
				{
					skin_info si;
					si.paintkit = skin->ToObject()->name.toInt();

					if (si.paintkit == 0)
						continue;

					auto skin_name = skin->ToObject()->GetKeyByName("name")->ToKeyValue()->Value.toString();
					si.cdn_name = skin_name;
					auto tag_node = skin->ToObject()->GetKeyByName("description_tag");
					if (tag_node)
					{
						auto tag = tag_node->ToKeyValue()->Value.toString();
						tag = tag.substr(1, std::string::npos);
						std::transform(tag.begin(), tag.end(), tag.begin(), towlower);
						si.tag_name = tag;
					}



					auto key_val = skin->ToObject()->GetKeyByName("seed");
					if (key_val != nullptr)
						si.seed = key_val->ToKeyValue()->Value.toInt();

					skinMap[skin_name] = si;
				}
			}
		}
	}

	for (const auto& child : paint_kit_names->children)
	{
		if (child->ToKeyValue())
		{
			auto key = child->ToKeyValue()->Key.toString();
			std::transform(key.begin(), key.end(), key.begin(), towlower);
			if (key.find("paintkit") != std::string::npos &&
				key.find("tag") != std::string::npos)
			{
				skinNames[key] = child->ToKeyValue()->Value.toString();
			}
		}
	}

	for (const auto& rarity : paint_kits_rarity)
	{
		if (rarity->ToObject())
		{
			for (const auto& child : rarity->children)
			{
				if (child->ToKeyValue())
				{
					std::string paint_kit_name = child->ToKeyValue()->Key.toString();
					std::string paint_kit_rarity = child->ToKeyValue()->Value.toString();

					auto skinInfo = &skinMap[paint_kit_name];

					skinInfo->rarity = GetWeaponRarity(paint_kit_rarity);
				}
			}
		}
	}

	for (auto weapon : weaponNames)
	{
		for (auto skin : weaponSkins[weapon])
		{
			skinInfo* info = &_inv.inventory.skinInfo[skinMap[skin].paintkit];
			info->weaponName.push_back(weapon);
			info->cdnName = skin;
			info->name = skinNames[skinMap[skin].tag_name].c_str();
			info->rarity = skinMap[skin].rarity;
			_inv.inventory.skinInfo.insert({ skinMap[skin].paintkit, *info });
		}
	}
}


bool Skins::ApplyCustomSkin(Player* localPlayer, Weapon* pWeapon, short nWeaponIndex)
{
	switch (nWeaponIndex)
	{
	case WEAPON_KNIFE_BAYONET:
	case WEAPON_KNIFE_BUTTERFLY:
	case WEAPON_KNIFE_FALCHION:
	case WEAPON_KNIFE_FLIP:
	case WEAPON_KNIFE_GUT:
	case WEAPON_KNIFE_GYPSY_JACKKNIFE:
	case WEAPON_KNIFE_KARAMBIT:
	case WEAPON_KNIFE_M9_BAYONET:
	case WEAPON_KNIFE_PUSH:
	case WEAPON_KNIFE_STILETTO:
	case WEAPON_KNIFE_SURVIVAL_BOWIE:
	case WEAPON_KNIFE_TACTICAL:
	case WEAPON_KNIFE_URSUS:
	case WEAPON_KNIFE_WIDOWMAKER:
	case WEAPON_KNIFE_SKELETON:
	case WEAPON_KNIFE_OUTDOOR:
	case WEAPON_KNIFE_CANIS:
	case WEAPON_KNIFE_CORD:
	case WEAPON_KNIFE_CSS:
	case WEAPON_KNIFE_T:
	case WEAPON_KNIFE:
		nWeaponIndex = WEAPON_KNIFE;
	}
	
	skins_s::sides skin_changer_cfg;
	if (localPlayer->m_iTeamNum() == 2)
		skin_changer_cfg = _inv.weapons[nWeaponIndex].skins.t;
	else
		skin_changer_cfg = _inv.weapons[nWeaponIndex].skins.ct;
	
	if (!skin_changer_cfg.enabled && nWeaponIndex != WEAPON_KNIFE && nWeaponIndex != WEAPON_KNIFE_T)
		return false;
	
	pWeapon->m_nFallbackPaintKit() = skin_changer_cfg.paintKit;
	pWeapon->m_iEntityQuality() = skin_changer_cfg.quality;
	pWeapon->m_nFallbackSeed() = skin_changer_cfg.seed;
	pWeapon->m_nFallbackStatTrak() = skin_changer_cfg.stattrak;
	pWeapon->m_flFallbackWear() = std::max(skin_changer_cfg.wear, 0.00001f);
	
	pWeapon->m_iItemIDHigh() = -1;
	
	if (skin_changer_cfg.definitionIndex != WEAPON_NONE) {
		static auto old_definition_index = pWeapon->m_iItemDefinitionIndex();
	
		pWeapon->m_iItemDefinitionIndex() = skin_changer_cfg.definitionIndex;
		pWeapon->m_iEntityQuality() = 3;
		const auto& replacement_item = k_weapon_info.at(pWeapon->m_iItemDefinitionIndex());
	
		skinfix(g_csgo.m_model_info->GetModelIndex(replacement_item.model));


	    pWeapon->GetClientNetworkable()->PreDataUpdate(DATA_UPDATE_CREATED);
	
		//const auto original_item = GetWeaponInfo(old_definition_index);
		//if (original_item && original_item->icon && replacement_item.icon && false)
			//iconfix(replacement_item.icon);


		//if (original_item && original_item->icon && replacement_item.icon)
		//	m_icon_overrides[original_item->icon] = replacement_item.icon;
	}
	return true;
}

void Skins::FrameStageNotify(bool frame_end)
{
	
	if (!g_cl.m_local)
		return;
	
	player_info_t info;
	g_csgo.m_engine->GetPlayerInfo(g_csgo.m_engine->GetLocalPlayer(), &info);
	const auto wearables = g_cl.m_local->m_hMyWearables();


	if (frame_end && !g_csgo.m_entlist->GetClientEntityFromHandle(wearables[0]))
	{
		skins_s::sides gloveConfig;
		if (g_cl.m_local->m_iTeamNum() == 2)
			gloveConfig = _inv.weapons[GLOVE_T_SIDE].skins.t;
		else
			gloveConfig = _inv.weapons[GLOVE_T_SIDE].skins.ct;

		static auto gloveHandle = CBaseHandle(0);
		auto glove = reinterpret_cast<Weapon*>(g_csgo.m_entlist->GetClientEntityFromHandle(wearables[0]));

		if (!glove)
		{
			const auto ourGlove = (Weapon*)g_csgo.m_entlist->GetClientEntityFromHandle(gloveHandle);
			if (ourGlove)
			{
				wearables[0] = gloveHandle;
				glove = ourGlove;
			}
		}

		if (!g_cl.m_local->alive())
		{
			if (glove)
			{
				glove->GetClientNetworkable()->SetDestroyedOnRecreateEntities();
				glove->GetClientNetworkable()->Release();
			}
			return;
		}

		if (gloveConfig.definitionIndex)
		{
			if (!glove)
			{
				static auto create_wearable_fn = GetWearableCreateFn();
				const auto entry = g_csgo.m_entlist->GetHighestEntityIndex() + 1;
				const auto serial = rand() % 0x1000;

				create_wearable_fn(entry, serial);
				glove = reinterpret_cast<Weapon*>(g_csgo.m_entlist->GetClientEntity(entry));
				glove->SetAbsOrigin({ 10000.f, 10000.f, 10000.f });
				const auto wearable_handle = reinterpret_cast<CBaseHandle*>(&wearables[0]);
				*wearable_handle = entry | serial << 16;
				gloveHandle = wearables[0];

			}

			if (glove)
			{
				glove->m_iItemDefinitionIndex() = gloveConfig.definitionIndex;
				glove->m_iItemIDHigh() = -1;
				glove->m_iEntityQuality() = 4;
				glove->m_iAccountID() = info.m_xuid;
				glove->m_nFallbackSeed() = gloveConfig.seed;
				glove->m_nFallbackStatTrak() = -1;
				glove->m_flFallbackWear() = gloveConfig.wear;
				glove->m_nFallbackPaintKit() = gloveConfig.paintKit;
				glove->SetModelIndex(-1);
				glove->PreDataUpdate(DataUpdateType_t::DATA_UPDATE_CREATED);
			}
		}
	}
	else
	{
		const auto weapons = g_cl.m_local->m_hMyWeapons();

		if (!weapons)
			return;

		for (auto i = 0; weapons[i].IsValid(); i++)
		{
			auto weapon = static_cast<Weapon*>(g_csgo.m_entlist->GetClientEntityFromHandle(weapons[i]));

			if (!weapon)
				continue;

			const auto weaponIndex = weapon->m_iItemDefinitionIndex();

			
			ApplyCustomSkin(g_cl.m_local, weapon, weaponIndex);

			weapon->m_iAccountID() = info.m_xuid;
		}

	
		//if (view_model_weapon->m_Item().m_iItemDefinitionIndex() == 42 || view_model_weapon->m_Item().m_iItemDefinitionIndex() == 59
		//	|| (view_model_weapon->m_Item().m_iItemDefinitionIndex() >= 500 && view_model_weapon->m_Item().m_iItemDefinitionIndex() <= 530))
		//{
		//	if (k_weapon_info.count(view_model_weapon->m_Item().m_iItemDefinitionIndex()))
		//	{
		//
		//		const auto override_model = k_weapon_info.at(view_model_weapon->m_Item().m_iItemDefinitionIndex()).model;
		//		const auto override_model_index = VModelInfoClient->GetModelIndex(override_model);
		//		view_model->m_nModelIndex() = override_model_index;
		//		const auto world_model_handle = view_model_weapon->m_hWeaponWorldModel();
		//
		//		if (!world_model_handle.IsValid())
		//			return;
		//
		//		const auto world_model = static_cast<C_BaseWeaponWorldModel*>(ClientEntityList->GetClientEntityFromHandle(world_model_handle));
		//
		//		if (!world_model)
		//			return;
		//
		//		world_model->m_nModelIndex() = override_model_index + 1;
		//	}
		//}

		
	}
	
}

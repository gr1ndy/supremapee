#include "Protobuffs.h"
#include "Messages.h"
#include "ProtoWriter.h"
#include <array>
#include "../inventory/inventorychanger.h"
#include "../includes.h"


#define CAST(cast, address, add) reinterpret_cast<cast>((uint32_t)address + (uint32_t)add)

#define _gc2ch MatchmakingGC2ClientHello
#define _pci PlayerCommendationInfo
#define _pri PlayerRankingInfo
static std::string ProfileChanger(void* pubDest, uint32_t* pcubMsgSize)
{
	ProtoWriter msg((void*)((DWORD)pubDest + 8), *pcubMsgSize - 8, 19);
	//auto _commendation = msg.has(_gc2ch::commendation) ? msg.get(_gc2ch::commendation).String() : std::string("");
	//ProtoWriter commendation(_commendation, 4);
	//
	//commendation.replace(Field(_pci::cmd_friendly, TYPE_UINT32, (int64_t)C::Get<float>(shonaxcfg.profile.profile_friend)));
	//commendation.replace(Field(_pci::cmd_teaching, TYPE_UINT32, (int64_t)C::Get<float>(shonaxcfg.profile.profile_teach)));
	//commendation.replace(Field(_pci::cmd_leader, TYPE_UINT32, (int64_t)C::Get<float>(shonaxcfg.profile.profile_leader)));
	//msg.replace(Field(_gc2ch::commendation, TYPE_STRING, commendation.serialize()));
	//auto _ranking = msg.has(_gc2ch::ranking) ? msg.get(_gc2ch::ranking).String() : std::string("");
	//ProtoWriter ranking(_ranking, 6);
	//
	//ranking.replace(Field(_pri::rank_id, TYPE_UINT32, (int64_t)(C::Get<int>(shonaxcfg.profile.profile_rank))));
	//ranking.replace(Field(_pri::wins, TYPE_UINT32, (int64_t)C::Get<float>(shonaxcfg.profile.profile_rank_wins)));
	//msg.replace(Field(_gc2ch::ranking, TYPE_STRING, ranking.serialize()));
	//msg.replace(Field(_gc2ch::player_level, TYPE_INT32, (int64_t)C::Get<float>(shonaxcfg.profile.profile_lvl)));
	//msg.replace(Field(_gc2ch::player_cur_xp, TYPE_INT32, (int64_t)C::Get<float>(shonaxcfg.profile.profile_xp)));
	//
	////if (g_Options.ban_type != 0 && g_Options.ban_time != 0)
	//{
	//	msg.replace(Field(_gc2ch::penalty_reason, TYPE_INT32, (int64_t)g_Options.ban_type));
	//	msg.replace(Field(_gc2ch::penalty_seconds, TYPE_INT32, (int64_t)g_Options.ban_time));
	//}

	return msg.serialize();
}


void Protobuffs::WritePacket(std::string packet, void* thisPtr, void* oldEBP, void* pubDest, uint32_t cubDest, uint32_t* pcubMsgSize)
{
	if ((uint32_t)packet.size() <= cubDest - 8)
	{
		memcpy((void*)((DWORD)pubDest + 8), (void*)packet.data(), packet.size());
		*pcubMsgSize = packet.size() + 8;
	}
	else if (g_csgo.m_mem_alloc)
	{
		auto memPtr = *CAST(void**, thisPtr, 0x14);
		auto memPtrSize = *CAST(uint32_t*, thisPtr, 0x18);
		auto newSize = (memPtrSize - cubDest) + packet.size() + 8;

		auto memory = g_csgo.m_mem_alloc->Realloc(memPtr, newSize + 4);

		*CAST(void**, thisPtr, 0x14) = memory;
		*CAST(uint32_t*, thisPtr, 0x18) = newSize;
		*CAST(void**, oldEBP, -0x14) = memory;

		memcpy(CAST(void*, memory, 24), (void*)packet.data(), packet.size());

		*pcubMsgSize = packet.size() + 8;
	}
}

#include "pbwrap.hpp"

using namespace pbwrap;


void Protobuffs::ReceiveMessage(void* thisPtr, void* oldEBP, uint32_t messageType, void* pubDest, uint32_t cubDest, uint32_t* pcubMsgSize)
{
	if (messageType == k_EMsgGCCStrike15_v2_MatchmakingGC2ClientHello /*&& C::Get<bool>(shonaxcfg.profile.activate_changer)*/)
	{
		//auto packet = ProfileChanger(pubDest, pcubMsgSize);
		//WritePacket(packet, thisPtr, oldEBP, pubDest, cubDest, pcubMsgSize);
	}
	else if (messageType == 9194)
	{
		//CMsgGCCStrike15_v2_ClientGCRankUpdate msg((void*)((DWORD)pubDest + 8), *pcubMsgSize - 8);
		//
		//auto ranking = msg.ranking().get();
		//
		//if (ranking.rank_type_id().get() == 7)
		//{
		//	ranking.rank_id().set(C::Get<int>(shonaxcfg.profile.r_rank));
		//	ranking.wins().set(C::Get<float>(shonaxcfg.profile.r_wins));
		//	msg.ranking().set(ranking);
		//	auto packet = msg.serialize();
		//	WritePacket(packet, thisPtr, oldEBP, pubDest, cubDest, pcubMsgSize);
		//}
		//else
		//	if (ranking.rank_type_id().get() == 10) {
		//		ranking.rank_id().set(C::Get<int>(shonaxcfg.profile.t_rank));
		//		ranking.wins().set(C::Get<float>(shonaxcfg.profile.t_wins));
		//		msg.ranking().set(ranking);
		//		auto packet = msg.serialize();
		//		WritePacket(packet, thisPtr, oldEBP, pubDest, cubDest, pcubMsgSize);
		//	}
	}else if (messageType == k_EMsgGCClientWelcome && g_menu.main.skins.activate_inventory.get())
	{
		auto packet = g_Inventory->Changer(pubDest, pcubMsgSize);
		WritePacket(packet, thisPtr, oldEBP, pubDest, cubDest, pcubMsgSize);
	}
}

bool Protobuffs::PreSendMessage(uint32_t& unMsgType, void* pubData, uint32_t& cubData)
{
	g_Inventory->Presend(unMsgType, pubData, cubData);

	return true;
}

bool Protobuffs::SendClientHello()
{
	ProtoWriter msg(7);
	msg.add(Field(3, TYPE_UINT32, (int64_t)1));
	auto packet = msg.serialize();

	void* ptr = malloc(packet.size() + 8);

	if (!ptr)
		return false;

	((uint32_t*)ptr)[0] = k_EMsgGCClientHello | ((DWORD)1 << 31);
	((uint32_t*)ptr)[1] = 0;

	memcpy((void*)((DWORD)ptr + 8), (void*)packet.data(), packet.size());
	bool result = SteamGameCoordinator->__SendMessage(k_EMsgGCClientHello | ((DWORD)1 << 31), ptr, packet.size() + 8) == k_EGCResultOK;
	free(ptr);

	return result;
}


bool Protobuffs::SendMatchmakingClient2GCHello()
{
	ProtoWriter msg(0);
	auto packet = msg.serialize();
	void* ptr = malloc(packet.size() + 8);

	if (!ptr)
		return false;

	((uint32_t*)ptr)[0] = k_EMsgGCCStrike15_v2_MatchmakingClient2GCHello | ((DWORD)1 << 31);
	((uint32_t*)ptr)[1] = 0;

	memcpy((void*)((DWORD)ptr + 8), (void*)packet.data(), packet.size());
	bool result = SteamGameCoordinator->__SendMessage(k_EMsgGCCStrike15_v2_MatchmakingClient2GCHello | ((DWORD)1 << 31), ptr, packet.size() + 8) == k_EGCResultOK;
	free(ptr);

	return result;
}
bool Protobuffs::SendClientGcRankUpdate1()
{
	
	return false;
}
bool Protobuffs::SendClientGcRankUpdate2()
{
	return false;
}


int Protobuffs::GetSlotID(int definition_index)
{
	switch (definition_index)
	{
	case WEAPON_KNIFE_BAYONET:
	case WEAPON_KNIFE_FLIP:
	case WEAPON_KNIFE_GUT:
	case WEAPON_KNIFE_KARAMBIT:
	case WEAPON_KNIFE_M9_BAYONET:
	case WEAPON_KNIFE_TACTICAL:
	case WEAPON_KNIFE_FALCHION:
	case WEAPON_KNIFE_SURVIVAL_BOWIE:
	case WEAPON_KNIFE_BUTTERFLY:
	case WEAPON_KNIFE_PUSH:
		return 0;
	case WEAPON_USPS:
	case WEAPON_P2000:
	case WEAPON_GLOCK:
		return 2;
	case WEAPON_ELITE:
		return 3;
	case WEAPON_P250:
		return 4;
	case WEAPON_TEC9:
	case WEAPON_CZ75:
	case WEAPON_FIVESEVEN:
		return 5;
	case WEAPON_DEAGLE:
	case WEAPON_REVOLVER:
		return 6;
	case WEAPON_MP9:
	case WEAPON_MAC10:
		return 8;
	case WEAPON_MP7:
		return 9;
	case WEAPON_UMP45:
		return 10;
	case WEAPON_P90:
		return 11;
	case WEAPON_BIZON:
		return 12;
	case WEAPON_FAMAS:
	case WEAPON_GALIL:
		return 14;
	case WEAPON_M4A1S:
	case WEAPON_M4A4:
	case WEAPON_AK47:
		return 15;
	case WEAPON_SSG08:
		return 16;
	case WEAPON_SG553:
	case WEAPON_AUG:
		return 17;
	case WEAPON_AWP:
		return 18;
	case WEAPON_G3SG1:
	case WEAPON_SCAR20:
		return 19;
	case WEAPON_NOVA:
		return 20;
	case WEAPON_XM1014:
		return 21;
	case WEAPON_SAWEDOFF:
	case WEAPON_MAG7:
		return 22;
	case WEAPON_M249:
		return 23;
	case WEAPON_NEGEV:
		return 24;
	case GLOVE_STUDDED_BLOODHOUND:
	case GLOVE_SPORTY:
	case GLOVE_SLICK:
	case GLOVE_LEATHER_WRAP:
	case GLOVE_MOTORCYCLE:
	case GLOVE_SPECIALIST:
		return 41;
	default:
		return -1;
	}
}

void Protobuffs::ParseSkins()
{
	/*
	valve_parser::Document doc;
	auto r = doc.Load(".\\csgo\\scripts\\items\\items_game.txt", valve_parser::ENCODING::UTF8);
	valve_parser::Document english;
	r = english.Load(".\\csgo\\resource\\csgo_english.txt", valve_parser::ENCODING::UTF16_LE);
	auto weaponSkinCombo = doc.BreadthFirstSearch("weapon_icons");
	auto skinDataVec = doc.BreadthFirstSearchMultiple("paint_kits");
	auto PaintKitNames = english.BreadthFirstSearch("Tokens");
	std::vector<std::string> weaponNames = {
		"deagle",
		"elite",
		"fiveseven",
		"glock",
		"ak47",
		"aug",
		"awp",
		"famas",
		"g3sg1",
		"galilar",
		"m249",
		"m4a1_silencer", //needs to be before m4a1 else silencer doesnt get filtered out :D
		"m4a1",
		"mac10",
		"p90",
		"mp5",
		"ump45",
		"xm1014",
		"bizon",
		"mag7",
		"negev",
		"sawedoff",
		"tec9",
		"hkp2000",
		"mp7",
		"mp9",
		"nova",
		"p250",
		"scar20",
		"sg556",
		"ssg08",
		"usp_silencer",
		"cz75a",
		"revolver",
		"knife_m9_bayonet", //needs to be before bayonet else knife_m9_bayonet doesnt get filtered out :D
		"bayonet",
		"knife_flip",
		"knife_gut",
		"knife_karambit",
		"knife_tactical",
		"knife_falchion",
		"knife_survival_bowie",
		"knife_butterfly",
		"knife_push",
		"knife_ursus",
		"knife_gypsy_jackknife",
		"knife_stiletto",
		"knife_widowmaker"

	};

	for (auto child : weaponSkinCombo->children)
	{
		if (child->ToObject())
		{
			for (auto weapon : weaponNames)
			{
				auto skinName = child->ToObject()->GetKeyByName("icon_path")->ToKeyValue()->Value.toString();
				auto pos = skinName.find(weapon);
				if (pos != std::string::npos)
				{
					auto pos2 = skinName.find_last_of('_');
					g_Options.weaponSkins[weapon].insert(
						skinName.substr(pos + weapon.length() + 1,
							pos2 - pos - weapon.length() - 1)
					);
					break;
				}
			}
		}
	}

	//populate skinData
	for (auto skinData : skinDataVec)
	{
		if (skinData->ToObject())
		{
			for (auto skin : skinData->children)
			{
				if (skin->ToObject())
				{
					skinInfo si;
					si.paintkit = skin->ToObject()->name.toInt();

					auto skinName = skin->ToObject()->GetKeyByName("name")->ToKeyValue()->Value.toString();
					auto tagNode = skin->ToObject()->GetKeyByName("description_tag");
					if (tagNode)
					{
						std::string tag = tagNode->ToKeyValue()->Value.toString();
						tag = tag.substr(1, std::string::npos); //skip #
						std::transform(tag.begin(), tag.end(), tag.begin(), towlower);
						si.tagName = tag;
					}

					auto keyVal = skin->ToObject()->GetKeyByName("seed");
					if (keyVal != nullptr)
						si.seed = keyVal->ToKeyValue()->Value.toInt();

					g_Options.skinMap[skinName] = si;
				}
			}
		}
	}

	//populate G::skinNames
	for (auto child : PaintKitNames->children)
	{
		if (child->ToKeyValue())
		{
			std::string key = child->ToKeyValue()->Key.toString();
			std::transform(key.begin(), key.end(), key.begin(), towlower);
			if (key.find("paintkit") != std::string::npos &&
				key.find("tag") != std::string::npos)
			{
				g_Options.skinNames[key] = child->ToKeyValue()->Value.toString();
			}
		}
	}


	*/
}
#include "includes.h"




uint8_t* pattern_scandefault(HMODULE hModule, const char* szSignature)
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


vec3_t Player::GetShootPosition()
{
	
		if (g_hvh.m_fake_duck && this->index() == g_csgo.m_engine->GetLocalPlayer()) {
			vec3_t origin = m_vecOrigin();

			vec3_t vDuckHullMin = g_csgo.m_game_movement->GetPlayerMins(true);
			vec3_t vStandHullMin = g_csgo.m_game_movement->GetPlayerMins(false);

			float fMore = (vDuckHullMin.z - vStandHullMin.z);

			vec3_t vecDuckViewOffset = g_csgo.m_game_movement->GetPlayerViewOffset(true);
			vec3_t vecStandViewOffset = g_csgo.m_game_movement->GetPlayerViewOffset(false);
			float duckFraction = m_flDuckAmount();

			float tempz = ((vecDuckViewOffset.z - fMore) * duckFraction) +
				(vecStandViewOffset.z * (1 - duckFraction));

			origin.z += tempz;

			return origin;
		}
		vec3_t pos;

		GetEyePos(&pos);
		//0x39E1 == 0x3AB4
		if (*reinterpret_cast <int32_t*> (uintptr_t(this) + 0x3AB4)) {
			auto v3 = m_PlayerAnimState();
			if (v3) {
				ModifyEyePosition(v3, &pos);
			}
		}

		return pos;
	

}

int Player::GetSequenceActivity(int iSequence)
{
	studiohdr_t* pStudioHdr = g_csgo.m_model_info->GetStudioModel(GetModel());
	if (pStudioHdr == nullptr)
		return -1;

	using GetSequenceActivityFn = int(__fastcall*)(void*, void*, int);
	// @xref: "Need to handle the activity %d\n"
	static auto oGetSequenceActivity = reinterpret_cast<GetSequenceActivityFn>(pattern_scandefault(GetModuleHandleA("client.dll"), ("55 8B EC 53 8B 5D 08 56 8B F1 83")));
	return oGetSequenceActivity(this, pStudioHdr, iSequence);
}
#include "cmenu.h"
#include "bgtexture.h"
#include "bindsfont.h"
#include <algorithm>
#include "../imgui_internal.h"
#include <fstream>
#include <WinInet.h>


#include "../../includes.h"

#pragma comment(lib, "wininet.lib")
c_menu skeet_menu;
static char ConfigNamexd[64] = { 0 };
IDirect3DTexture9* imgxd = nullptr;
ImFont* tabs;
ImFont* def;
ImFont* Normal;
DWORD stream;
ImFont* weapon_icons = nullptr;
static int weapon_tab = 0;
IDirect3DDevice9* dev;
static float pfix = 226;
#pragma comment(lib, "d3dx9.lib")
static const char* zweaponnames(const short id)
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






adding_menu skin_menu;





void c_menu::initialize(IDirect3DDevice9* pDevice) {
	if (this->m_bInitialized)
		return;

	ui::CreateContext();
	auto io = ui::GetIO();
	auto style = &ui::GetStyle();

	style->WindowRounding = 0.f;
	style->AntiAliasedLines = true;
	style->AntiAliasedFill = true;
	style->ScrollbarRounding = 0.f;
	style->ScrollbarSize = 0.f;
	style->WindowPadding = ImVec2(0, 0);
	style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(45 / 255.f, 45 / 255.f, 45 / 255.f, 1.f);
	style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(65 / 255.f, 65 / 255.f, 65 / 255.f, 1.f);
	

	dev = pDevice;

	this->m_bIsOpened = true;

	IDirect3DSwapChain9* pChain = nullptr;
	D3DPRESENT_PARAMETERS pp = {};
	D3DDEVICE_CREATION_PARAMETERS param = {};
	pDevice->GetCreationParameters(&param);
	pDevice->GetSwapChain(0, &pChain);

	if (pChain)
		pChain->GetPresentParameters(&pp);

	ImGui_ImplWin32_Init(param.hFocusWindow);
	ImGui_ImplDX9_Init(pDevice);

	ImFontConfig cfg;
	io.Fonts->AddFontFromFileTTF("C:/windows/fonts/verdana.ttf", 12.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
	io.Fonts->AddFontFromFileTTF("C:/windows/fonts/verdanab.ttf", 12.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
	io.Fonts->AddFontFromMemoryTTF(keybinds_font, 25600, 10.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
	tabs = io.Fonts->AddFontFromMemoryTTF(skeet, 15400, 50.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
	def = io.Fonts->AddFontFromMemoryTTF(skeet, 15400, 10.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
	io.Fonts->AddFontDefault();




	ImGuiFreeType::BuildFontAtlas(io.Fonts, 0x00);

	this->m_bInitialized = true;
}

void c_menu::draw_begin() {
	if (!this->m_bInitialized)
		return;

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ui::NewFrame();
}

static inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y); }
void ui::ClosePopup(ImGuiID id)
{
	if (!IsPopupOpen(id))
		return;
	ImGuiContext& g = *GImGui;
	ClosePopupToLevel(g.OpenPopupStack.Size - 1, false);
}









std::string FindURl(std::string paintName, std::string weapon)
{
	std::ifstream inFile;
	inFile.open(".\\csgo\\scripts\\items\\items_game_cdn.txt");//path: to item cdn list
	std::string line;
	//int founds = 0;
	unsigned int curLine = 0;
	std::string search = paintName.append("=");
	//string weapon = "aug";
	while (getline(inFile, line)) {
		curLine++;
		if (line.find(search, 0) != std::string::npos) {
			if (line.find(weapon, 0) != std::string::npos)
			{
				//cout << "link: " << << endl;
				return line.substr(line.find(search)).erase(0, search.length()); //DOSTAEM SSILKU
			}
		}
	}
	return "nf";
}

std::string DownloadBytes(const char* const szUrl)
{
	HINTERNET hOpen = NULL;
	HINTERNET hFile = NULL;
	char* lpBuffer = NULL;
	DWORD dwBytesRead = 0;
	//Pointer to dynamic buffer.
	char* data = 0;
	//Dynamic data size.
	DWORD dataSize = 0;

	hOpen = InternetOpenA("smef.pw INDIGO/4.3 paste", NULL, NULL, NULL, NULL);
	if (!hOpen) return (char*)"";

	hFile = InternetOpenUrlA(hOpen, szUrl, NULL, NULL, INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE, NULL);

	if (!hFile) {
		InternetCloseHandle(hOpen);
		return (char*)"";
	}

	std::string output;
	do {
		char buffer[2000];
		InternetReadFile(hFile, (LPVOID)buffer, _countof(buffer), &dwBytesRead);
		output.append(buffer, dwBytesRead);
	} while (dwBytesRead);

	InternetCloseHandle(hFile);
	InternetCloseHandle(hOpen);

	return output;
}
void UpdatePic(IDirect3DDevice9* thisptr, std::string link, IDirect3DTexture9* texture, bool* show)
{
	texture = nullptr;

	if (texture == nullptr) {

		std::string imData = DownloadBytes(link.c_str());

		D3DXCreateTextureFromFileInMemoryEx(thisptr
			, imData.data(), imData.length(),
			512, 384, D3DX_DEFAULT,
			0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
			D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &texture);
	}


}



#include "unlockmanual.h"


void EnableHiddenCVars()
{
	
}





static bool test_work_1;

#include "../../inventory/inventorychanger.h"
#include "../../protobuffs/Protobuffs.h"
#include "../../inventory/items.h"


void c_menu::draw()
{


	if (!this->m_bIsOpened && ui::GetStyle().Alpha > 0.f) {
		float fc = 255.f / 0.2f * ui::GetIO().DeltaTime;
		ui::GetStyle().Alpha = std::clamp(ui::GetStyle().Alpha - fc / 255.f, 0.f, 1.f);
	}

	if (this->m_bIsOpened && ui::GetStyle().Alpha < 1.f) {
		float fc = 255.f / 0.2f * ui::GetIO().DeltaTime;
		ui::GetStyle().Alpha = std::clamp(ui::GetStyle().Alpha + fc / 255.f, 0.f, 1.f);
	}

	if (!this->m_bIsOpened && ui::GetStyle().Alpha == 0.f)
		return;


	ui::SetNextWindowSizeConstraints(ImVec2(760, 660), ImVec2(4096, 4096));
	static auto calculateChildWindowPositionDouble = [](int num) -> ImVec2 {
		return ImVec2(ui::GetWindowPos().x + 10 + (ui::GetWindowSize().x / 2 - 70) * num + 20 * num, ui::GetWindowPos().y + 20);
	};
	ui::Begin("shonax", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);
	{
		ui::SetNextWindowPos(calculateChildWindowPositionDouble(0));
		ui::BeginChild("Inventory changer", ImVec2(600, 500));
		{
			static wskin weaponSkin;
			if (weaponSkin.wId == WEAPON_NONE)
				weaponSkin.wId = WEAPON_DEAGLE;
			if (ui::BeginCombo("Weapon", k_inventory_names.at(weaponSkin.wId), ImGuiComboFlags_HeightLargest, k_inventory_names.size()))
			{
				for (const auto& weapon : k_inventory_names)
				{
					if (ui::Selectable(weapon.second, weaponSkin.wId == weapon.first))
					{
						weaponSkin.wId = weapon.first;
						weaponSkin.paintKit = 0;
						skinImage = nullptr;
					}
				}
				ui::EndCombo();
			}
			
		
			auto weaponName = zweaponnames(weaponSkin.wId);
			ui::Text("Skin");
		
			if (ui::BeginCombo("##Paint Kit", weaponSkin.paintKit > 0 ? _inv.inventory.skinInfo[weaponSkin.paintKit].name.c_str() : "", ImGuiComboFlags_HeightLargest, _inv.inventory.skinInfo.size()))
			{
				int lastID = ui::GetItemID();
		
				for (auto skin : _inv.inventory.skinInfo)
				{
					for (auto names : skin.second.weaponName)
					{
						if (weaponName != names)
							continue;
		
						ui::PushID(lastID++);
		
						if (ui::Selectable(skin.second.name.c_str(), skin.first == weaponSkin.paintKit))
							weaponSkin.paintKit = skin.first;
		
						ui::PopID();
					}
				}
				ui::EndCombo();
			}
		
		
			ui::SliderFloat("Wear", &weaponSkin.wear, 0.f, 1.f, "%.2f");
			ui::SliderInt("Seed", &weaponSkin.seed, 0.f, 100.f, "%.0f");
			if (ui::Button("Add"))
			{
				g_InventorySkins.insert({ g_csgo.RandomInt(20000, 200000), weaponSkin });
				_inv.inventory.itemCount = g_InventorySkins.size();
			}
			if (ui::Button("Apply"))
			{
				write.SendClientHello();
				write.SendMatchmakingClient2GCHello();
			}
			skin_menu.write(weaponSkin.paintKit, weaponSkin.wId);
			static int selectedId = 0;
		
		//	if (ui::BeginChild("Skins", ImVec2(ui::GetWindowWidth() - 30, 200)))
		//	{
		//		int lastID = ui::GetItemID();
		//		for (auto weapon : g_InventorySkins)
		//		{
		//			if (!weapon.second.wId || !weapon.second.paintKit)
		//				continue;
		//
		//			ui::PushID(lastID++);
		//
		//			if (ui::Selectable((k_inventory_names.at(weapon.second.wId) + std::string(" | ") + _inv.inventory.skinInfo[weapon.second.paintKit].name).c_str(), selectedId == weapon.first))
		//				selectedId = weapon.first;
		//			ui::PopID();
		//		}
		//
		//		ui::EndChild();
		//	}
		//	
		//	if (selectedId != 0)
		//	{
		//		if (ui::Button("Delete", ImVec2(-1, 25)))
		//		{
		//			g_InventorySkins.erase(selectedId);
		//			_inv.inventory.itemCount = g_InventorySkins.size();
		//		}
		//	}
		//
		}
		ui::EndChild();
	}
	ui::End();

	

}

void c_menu::draw_end() {
	if (!this->m_bInitialized)
		return;

	ui::EndFrame();
	ui::Render();
	ImGui_ImplDX9_RenderDrawData(ui::GetDrawData());
}

bool c_menu::is_menu_initialized() {
	return this->m_bInitialized;
}

bool c_menu::is_menu_opened() {
	return this->m_bIsOpened;
}

void c_menu::set_menu_opened(bool v) {
	this->m_bIsOpened = v;
}

void c_menu::cjec()
{
	//CloseLeftoverPicker();
}

ImColor c_menu::get_accent_color() {
	return ImColor(
		1.0f,
		1.0f,
		1.0f,
		ui::GetStyle().Alpha
	);
}



void adding_menu::write(int hex1, int hex2)
{
	
}






void dmt(std::string key) {

}




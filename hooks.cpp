#include "includes.h"
#include "inventory/items.h"
Hooks                g_hooks{ };;
Skins g_Skins;
CustomEntityListener g_custom_entity_listener{ };;
uint8_t* wtf_no(HMODULE hModule, const char* szSignature)
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


void Pitch_proxy( CRecvProxyData *data, Address ptr, Address out ) {
	// normalize this fucker.
	math::NormalizeAngle( data->m_Value.m_Float );

	// clamp to remove retardedness.
	math::clamp( data->m_Value.m_Float, -90.f, 90.f );

	// call original netvar proxy.
	if ( g_hooks.m_Pitch_original )
		g_hooks.m_Pitch_original( data, ptr, out );
}

void Body_proxy( CRecvProxyData *data, Address ptr, Address out ) {
	Stack stack;

	static Address RecvTable_Decode{ pattern::find( g_csgo.m_engine_dll, XOR( "EB 0D FF 77 10" ) ) };

	// call from entity going into pvs.
	if ( stack.next( ).next( ).ReturnAddress( ) != RecvTable_Decode ) {
		// convert to player.
		Player *player = ptr.as< Player * >( );

		// store data about the update.
		g_resolver.OnBodyUpdate( player, data->m_Value.m_Float );
	}

	// call original proxy.
	if ( g_hooks.m_Body_original )
		g_hooks.m_Body_original( data, ptr, out );
}

void AbsYaw_proxy( CRecvProxyData *data, Address ptr, Address out ) {
	
	if ( g_hooks.m_AbsYaw_original )
		g_hooks.m_AbsYaw_original( data, ptr, out );
}

void Force_proxy( CRecvProxyData *data, Address ptr, Address out ) {
	// convert to ragdoll.
	Ragdoll *ragdoll = ptr.as< Ragdoll * >( );

	// get ragdoll owner.
	Player *player = ragdoll->GetPlayer( );

	// we only want this happening to noobs we kill.
	if ( g_menu.main.misc.ragdoll_force.get( ) && g_cl.m_local && player && player->enemy( g_cl.m_local ) ) {
		// get m_vecForce.
		vec3_t vel = { data->m_Value.m_Vector[ 0 ], data->m_Value.m_Vector[ 1 ], data->m_Value.m_Vector[ 2 ] };

		// give some speed to all directions.
		vel *= 1000.f;

		// boost z up a bit.
		if ( vel.z <= 1.f )
			vel.z = 2.f;

		vel.z *= 2.f;

		// don't want crazy values for this... probably unlikely though?
		math::clamp( vel.x, std::numeric_limits< float >::lowest( ), std::numeric_limits< float >::max( ) );
		math::clamp( vel.y, std::numeric_limits< float >::lowest( ), std::numeric_limits< float >::max( ) );
		math::clamp( vel.z, std::numeric_limits< float >::lowest( ), std::numeric_limits< float >::max( ) );

		// set new velocity.
		data->m_Value.m_Vector[ 0 ] = vel.x;
		data->m_Value.m_Vector[ 1 ] = vel.y;
		data->m_Value.m_Vector[ 2 ] = vel.z;
	}

	if ( g_hooks.m_Force_original )
		g_hooks.m_Force_original( data, ptr, out );
}





enum ESequence
{
	SEQUENCE_DEFAULT_DRAW = 0,
	SEQUENCE_DEFAULT_IDLE1 = 1,
	SEQUENCE_DEFAULT_IDLE2 = 2,
	SEQUENCE_DEFAULT_LIGHT_MISS1 = 3,
	SEQUENCE_DEFAULT_LIGHT_MISS2 = 4,
	SEQUENCE_DEFAULT_HEAVY_MISS1 = 9,
	SEQUENCE_DEFAULT_HEAVY_HIT1 = 10,
	SEQUENCE_DEFAULT_HEAVY_BACKSTAB = 11,
	SEQUENCE_DEFAULT_LOOKAT01 = 12,

	SEQUENCE_BUTTERFLY_DRAW = 0,
	SEQUENCE_BUTTERFLY_DRAW2 = 1,
	SEQUENCE_BUTTERFLY_LOOKAT01 = 13,
	SEQUENCE_BUTTERFLY_LOOKAT02 = 14,
	SEQUENCE_BUTTERFLY_LOOKAT03 = 15,

	SEQUENCE_FALCHION_IDLE1 = 1,
	SEQUENCE_FALCHION_HEAVY_MISS1 = 8,
	SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP = 9,
	SEQUENCE_FALCHION_LOOKAT01 = 12,
	SEQUENCE_FALCHION_LOOKAT02 = 13,

	SEQUENCE_DAGGERS_IDLE1 = 1,
	SEQUENCE_DAGGERS_LIGHT_MISS1 = 2,
	SEQUENCE_DAGGERS_LIGHT_MISS5 = 6,
	SEQUENCE_DAGGERS_HEAVY_MISS2 = 11,
	SEQUENCE_DAGGERS_HEAVY_MISS1 = 12,

	SEQUENCE_BOWIE_IDLE1 = 1,
};

int random(const int& min, const int& max)
{
	return rand() % (max - min + 1) + min;
}

int GetNewAnimation(std::string model, const int sequence)
{
	if (model == "models/weapons/v_knife_butterfly.mdl")
	{
		switch (sequence)
		{
		case SEQUENCE_DEFAULT_DRAW:
			return random(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
		case SEQUENCE_DEFAULT_LOOKAT01:
			return random(SEQUENCE_BUTTERFLY_LOOKAT01, SEQUENCE_BUTTERFLY_LOOKAT02);
		default:
			return sequence + 1;
		}
	}
	if (model == "models/weapons/v_knife_falchion_advanced.mdl")
	{
		switch (sequence)
		{
		case SEQUENCE_DEFAULT_IDLE2:
			return SEQUENCE_FALCHION_IDLE1;
		case SEQUENCE_DEFAULT_HEAVY_MISS1:
			return random(SEQUENCE_FALCHION_HEAVY_MISS1, SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP);
		case SEQUENCE_DEFAULT_LOOKAT01:
			return random(SEQUENCE_FALCHION_LOOKAT01, SEQUENCE_FALCHION_LOOKAT02);
		case SEQUENCE_DEFAULT_DRAW:
		case SEQUENCE_DEFAULT_IDLE1:
			return sequence;
		default:
			return sequence - 1;
		}
	}
	if (model == "models/weapons/v_knife_push.mdl")
	{
		switch (sequence)
		{
		case SEQUENCE_DEFAULT_IDLE2:
			return SEQUENCE_DAGGERS_IDLE1;
		case SEQUENCE_DEFAULT_LIGHT_MISS1:
		case SEQUENCE_DEFAULT_LIGHT_MISS2:
			return random(SEQUENCE_DAGGERS_LIGHT_MISS1, SEQUENCE_DAGGERS_LIGHT_MISS5);
		case SEQUENCE_DEFAULT_HEAVY_MISS1:
			return random(SEQUENCE_DAGGERS_HEAVY_MISS2, SEQUENCE_DAGGERS_HEAVY_MISS1);
		case SEQUENCE_DEFAULT_HEAVY_HIT1:
		case SEQUENCE_DEFAULT_HEAVY_BACKSTAB:
		case SEQUENCE_DEFAULT_LOOKAT01:
			return sequence + 3;
		case SEQUENCE_DEFAULT_DRAW:
		case SEQUENCE_DEFAULT_IDLE1:
			return sequence;
		default:
			return sequence + 2;
		}
	}
	if (model == "models/weapons/v_knife_survival_bowie.mdl")
	{
		switch (sequence)
		{
		case SEQUENCE_DEFAULT_DRAW:
		case SEQUENCE_DEFAULT_IDLE1:
			return sequence;
		case SEQUENCE_DEFAULT_IDLE2:
			return SEQUENCE_BOWIE_IDLE1;
		default:
			return sequence - 1;
		}
	}
	if (model == "models/weapons/v_knife_ursus.mdl" || model == "models/weapons/v_knife_cord.mdl" || model == "models/weapons/v_knife_canis.mdl" || model == "models/weapons/v_knife_outdoor.mdl" || model == "models/weapons/v_knife_skeleton.mdl")
	{
		switch (sequence)
		{
		case SEQUENCE_DEFAULT_DRAW:
			return random(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
		case SEQUENCE_DEFAULT_LOOKAT01:
			return random(SEQUENCE_BUTTERFLY_LOOKAT01, 14);
		default:
			return sequence + 1;
		}
	}
	if (model == "models/weapons/v_knife_stiletto.mdl")
	{
		switch (sequence)
		{
		case SEQUENCE_DEFAULT_LOOKAT01:
			return random(12, 13);
		}
	}
	if (model == "models/weapons/v_knife_widowmaker.mdl")
	{
		switch (sequence)
		{
		case SEQUENCE_DEFAULT_LOOKAT01:
			return random(14, 15);
		case SEQUENCE_DEFAULT_HEAVY_BACKSTAB:
			return sequence - 1;
		}
	}
	return sequence;
}

void Remapping(CRecvProxyData* data, Weapon* entity)
{
	if (!g_cl.m_local || !g_cl.m_local->alive())
		return;

	const auto owner = g_csgo.m_entlist->GetClientEntityFromHandle(entity->m_hOwner());
	if (owner != g_cl.m_local)
		return;


	const auto view_model_weapon = static_cast<Weapon*>(g_csgo.m_entlist->GetClientEntityFromHandle(entity->m_hWeapon()));
	if (!view_model_weapon)
		return;

	const auto entry = k_weapon_info.find(view_model_weapon->m_iItemDefinitionIndex());

	if (entry == k_weapon_info.end())
		return;

	if (&entry->second == nullptr)
		return;

	const auto weaponInfo = &entry->second;
	auto& sequence = data->m_Value.m_Int;
	sequence = GetNewAnimation(weaponInfo->model, sequence);
}

RecvVarProxy_t sequence_hook;
void RecvProxy(CRecvProxyData* pData, void* entity, void* output)
{
	const auto proxy_data = const_cast<CRecvProxyData*>(pData);
	const auto view_model = static_cast<Weapon*>(entity);
	Remapping(proxy_data, view_model);
	if (sequence_hook)
		sequence_hook(pData, entity, output);
}




#include "skinchanger/skins.h"
static int fixed_p;
void Skins::skinfix(int itemfix)
{
	fixed_p = itemfix;
}


RecvVarProxy_t viewmodel_hook;
void Viewmodel(CRecvProxyData* pData, void* pStruct, void* pOut)
{


	int default_t = g_csgo.m_model_info->GetModelIndex("models/weapons/v_knife_default_t.mdl");
	int default_ct = g_csgo.m_model_info->GetModelIndex("models/weapons/v_knife_default_ct.mdl");
	int iBayonet = g_csgo.m_model_info->GetModelIndex("models/weapons/v_knife_bayonet.mdl");
	int iButterfly = g_csgo.m_model_info->GetModelIndex("models/weapons/v_knife_butterfly.mdl");
	int iFlip = g_csgo.m_model_info->GetModelIndex("models/weapons/v_knife_flip.mdl");
	int iGut = g_csgo.m_model_info->GetModelIndex("models/weapons/v_knife_gut.mdl");
	int iKarambit = g_csgo.m_model_info->GetModelIndex("models/weapons/v_knife_karam.mdl");
	int iM9Bayonet = g_csgo.m_model_info->GetModelIndex("models/weapons/v_knife_m9_bay.mdl");
	int iHuntsman = g_csgo.m_model_info->GetModelIndex("models/weapons/v_knife_tactical.mdl");
	int iFalchion = g_csgo.m_model_info->GetModelIndex("models/weapons/v_knife_falchion_advanced.mdl");
	int iDagger = g_csgo.m_model_info->GetModelIndex("models/weapons/v_knife_push.mdl");
	int iBowie = g_csgo.m_model_info->GetModelIndex("models/weapons/v_knife_survival_bowie.mdl");
	int iGunGame = g_csgo.m_model_info->GetModelIndex("models/weapons/v_knife_gg.mdl");
	int Navaja = g_csgo.m_model_info->GetModelIndex("models/weapons/v_knife_gypsy_jackknife.mdl");
	int Stiletto = g_csgo.m_model_info->GetModelIndex("models/weapons/v_knife_stiletto.mdl");
	int Ursus = g_csgo.m_model_info->GetModelIndex("models/weapons/v_knife_ursus.mdl");
	int Talon = g_csgo.m_model_info->GetModelIndex("models/weapons/v_knife_widowmaker.mdl");
	int d1 = g_csgo.m_model_info->GetModelIndex("models/weapons/v_knife_css.mdl");
	int d2 = g_csgo.m_model_info->GetModelIndex("models/weapons/v_knife_skeleton.mdl");
	int d3 = g_csgo.m_model_info->GetModelIndex("models/weapons/v_knife_outdoor.mdl");
	int d4 = g_csgo.m_model_info->GetModelIndex("models/weapons/v_knife_canis.mdl");
	int d5 = g_csgo.m_model_info->GetModelIndex("models/weapons/v_knife_cord.mdl");


	const auto local = g_cl.m_local;


	if (local && g_menu.main.skins.activate_inventory.get())
	{
		if (local->alive() && (
			pData->m_Value.m_Int == default_t ||
			pData->m_Value.m_Int == default_ct ||
			pData->m_Value.m_Int == iBayonet ||
			pData->m_Value.m_Int == iFlip ||
			pData->m_Value.m_Int == iGunGame ||
			pData->m_Value.m_Int == iGut ||
			pData->m_Value.m_Int == iKarambit ||
			pData->m_Value.m_Int == iM9Bayonet ||
			pData->m_Value.m_Int == iHuntsman ||
			pData->m_Value.m_Int == iBowie ||
			pData->m_Value.m_Int == iButterfly ||
			pData->m_Value.m_Int == iFalchion ||
			pData->m_Value.m_Int == iDagger ||
			pData->m_Value.m_Int == Navaja ||
			pData->m_Value.m_Int == Stiletto ||
			pData->m_Value.m_Int == Ursus ||
			pData->m_Value.m_Int == Talon ||
			pData->m_Value.m_Int == d1 ||
			pData->m_Value.m_Int == d2 ||
			pData->m_Value.m_Int == d3 ||
			pData->m_Value.m_Int == d4 ||
			pData->m_Value.m_Int == d5))
		{


			if (fixed_p)
				pData->m_Value.m_Int = fixed_p;




		}
	}
	if (viewmodel_hook)
		viewmodel_hook(pData, pStruct, pOut);

}







IDirect3DStateBlock9* pixel_state = NULL; IDirect3DVertexDeclaration9* vertDec; IDirect3DVertexShader9* vertShader;
DWORD dwOld_D3DRS_COLORWRITEENABLE;

void SaveState(IDirect3DDevice9* pDevice)
{
	pDevice->GetRenderState(D3DRS_COLORWRITEENABLE, &dwOld_D3DRS_COLORWRITEENABLE);
	//	pDevice->CreateStateBlock(D3DSBT_PIXELSTATE, &pixel_state); // This seam not to be needed anymore because valve fixed their shit
	pDevice->GetVertexDeclaration(&vertDec);
	pDevice->GetVertexShader(&vertShader);
	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);
	pDevice->SetRenderState(D3DRS_SRGBWRITEENABLE, false);
	pDevice->SetSamplerState(NULL, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	pDevice->SetSamplerState(NULL, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	pDevice->SetSamplerState(NULL, D3DSAMP_ADDRESSW, D3DTADDRESS_WRAP);
	pDevice->SetSamplerState(NULL, D3DSAMP_SRGBTEXTURE, NULL);
}

void RestoreState(IDirect3DDevice9* pDevice) // not restoring everything. Because its not needed.
{
	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, dwOld_D3DRS_COLORWRITEENABLE);
	pDevice->SetRenderState(D3DRS_SRGBWRITEENABLE, true);
	//pixel_state->Apply(); 
	//pixel_state->Release();
	pDevice->SetVertexDeclaration(vertDec);
	pDevice->SetVertexShader(vertShader);
}
#include "UI/Menu/cmenu.h"
HRESULT WINAPI Present(IDirect3DDevice9* pDevice, RECT* pRect1, const RECT* pRect2, HWND hWnd, const RGNDATA* pRGNData) {
	


	IDirect3DStateBlock9* pixel_state = NULL; IDirect3DVertexDeclaration9* vertDec; IDirect3DVertexShader9* vertShader;
	pDevice->CreateStateBlock(D3DSBT_PIXELSTATE, &pixel_state);
	pDevice->GetVertexDeclaration(&vertDec);
	pDevice->GetVertexShader(&vertShader);
	//latch_timer();
	static auto wanted_ret_address = _ReturnAddress();
	if (_ReturnAddress() == wanted_ret_address)
	{

		DWORD colorwrite, srgbwrite;
		pDevice->GetRenderState(D3DRS_COLORWRITEENABLE, &colorwrite);
		pDevice->GetRenderState(D3DRS_SRGBWRITEENABLE, &srgbwrite);
		pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);
		pDevice->SetRenderState(D3DRS_SRGBWRITEENABLE, false);

		//static PDIRECT3DTEXTURE9 m_pTexture;
		skeet_menu.initialize(pDevice);
		skeet_menu.draw_begin();

	
		if (g_menu.main.skins.activate_inventory.get())
			skeet_menu.draw();

		skeet_menu.draw_end();
		pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, colorwrite);
		pDevice->SetRenderState(D3DRS_SRGBWRITEENABLE, srgbwrite);
	}
	
	g_visuals.spread_crosshair();
	pixel_state->Apply();
	pixel_state->Release();
	pDevice->SetVertexDeclaration(vertDec);
	pDevice->SetVertexShader(vertShader);
	return g_hooks.m_device.GetOldMethod<decltype(&Present)>(17)(pDevice, pRect1, pRect2, hWnd, pRGNData);
}
long __stdcall ResetHook(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	static auto oReset = g_hooks.m_device.GetOldMethod<decltype(&ResetHook)>(16);
	if (!skeet_menu.is_menu_initialized())
		return oReset(pDevice, pPresentationParameters);
	ImGui_ImplDX9_InvalidateDeviceObjects();
	auto result = oReset(pDevice, pPresentationParameters);
	ImGui_ImplDX9_CreateDeviceObjects();
	return result;

}


vfunc_hook gc;

using GCRetrieveMessage = unsigned long(__thiscall*)(void*, uint32_t * punMsgType, void* pubDest, uint32_t cubDest, uint32_t * pcubMsgSize);
using GCSendMessage = unsigned long(__thiscall*)(void*, uint32_t unMsgType, const void* pubData, uint32_t cubData);

#include "protobuffs/Protobuffs.h"
unsigned long __fastcall GCRetrieveMessageHook(void* ecx, void*, uint32_t* punMsgType, void* pubDest, uint32_t cubDest, uint32_t* pcubMsgSize)
{
	//static auto oGCRetrieveMessage = SteamGame.GetOriginal<GCRetrieveMessage>();
	static auto oGCRetrieveMessage = gc.get_original<GCRetrieveMessage>(2);
	auto status = oGCRetrieveMessage(ecx, punMsgType, pubDest, cubDest, pcubMsgSize);
	if (status == k_EGCResultOK)
	{

		void* thisPtr = nullptr;
		__asm mov thisPtr, ebx;
		auto oldEBP = *reinterpret_cast<void**>((uint32_t)_AddressOfReturnAddress() - 4);

		uint32_t messageType = *punMsgType & 0x7FFFFFFF;
		write.ReceiveMessage(thisPtr, oldEBP, messageType, pubDest, cubDest, pcubMsgSize);
	}
	return status;
}

unsigned long __fastcall GCSendMessageHook(void* ecx, void*, uint32_t unMsgType, const void* pubData, uint32_t cubData)
{
	//static auto oGCSendMessage = SteamGame.GetOriginal<GCSendMessage>();
	static auto oGCSendMessage = gc.get_original<GCSendMessage>(0);
	bool sendMessage = write.PreSendMessage(unMsgType, const_cast<void*>(pubData), cubData);
	if (!sendMessage)
		return k_EGCResultOK;
	return oGCSendMessage(ecx, unMsgType, const_cast<void*>(pubData), cubData);
}
vfunc_hook clientstate_hook;
vfunc_hook partition_hook;
void __fastcall ReadPacketEntitiesHook(void* _this, int, void* entInfo)
{
	static auto ofunct = clientstate_hook.get_original<decltype(&ReadPacketEntitiesHook)>(64);
	
	ofunct(_this, 0, entInfo);
	if (g_csgo.m_engine->IsInGame())
		g_Skins.FrameStageNotify(false);
}
//--------------------------------------------------------------------------------
void __fastcall SuppressListsHook(void* _this, int, int a2, bool a3)
{
	static auto ofunct = partition_hook.get_original<decltype(&SuppressListsHook)>(16);

	static auto FrameNetUpdateEnd_Return = wtf_no(GetModuleHandleA("client.dll"), "5F 5E 5D C2 04 00 83 3D ? ? ? ? ?");
	
	if (_ReturnAddress() == FrameNetUpdateEnd_Return)
		g_Skins.FrameStageNotify(true);

	ofunct(SpatialPartition, 0, a2, a3);
}
template< typename T >
T* Interface(const char* strModule, const char* strInterface)
{
	typedef T* (*CreateInterfaceFn)(const char* szName, int iReturn);
	CreateInterfaceFn CreateInterface = (CreateInterfaceFn)GetProcAddress(GetModuleHandleA(strModule), "CreateInterface");

	return CreateInterface(strInterface, 0);
}


void Hooks::init( ) {
	// hook wndproc.
	auto m_hWindow = FindWindowA(XOR("Valve001"), NULL); //fix shonax
	m_old_wndproc = ( WNDPROC )g_winapi.SetWindowLongA( m_hWindow, GWL_WNDPROC, util::force_cast< LONG >( Hooks::WndProc ) );

	// setup normal VMT hooks.
	m_panel.init( g_csgo.m_panel );
	m_panel.add( IPanel::PAINTTRAVERSE, util::force_cast( &Hooks::PaintTraverse ) );

	m_client.init( g_csgo.m_client );
	m_client.add( CHLClient::LEVELINITPREENTITY, util::force_cast( &Hooks::LevelInitPreEntity ) );
	m_client.add( CHLClient::LEVELINITPOSTENTITY, util::force_cast( &Hooks::LevelInitPostEntity ) );
	m_client.add( CHLClient::LEVELSHUTDOWN, util::force_cast( &Hooks::LevelShutdown ) );
	m_client.add( CHLClient::FRAMESTAGENOTIFY, util::force_cast( &Hooks::FrameStageNotify ) );
	m_client.add(CHLClient::USRCMDTODELTABUFFER, util::force_cast(&Hooks::WriteUsercmdDeltaToBuffer));




	
	shonax_3_fix.setup(g_csgo.m_engine);
	shonax_3_fix.hook_index(IVEngineClient::ISCONNECTED, util::force_cast(&Hooks::IsConnected));
	shonax_3_fix.hook_index(IVEngineClient::ISHLTV, util::force_cast(&Hooks::IsHLTV));
	
	shonax_2_fix.setup(g_csgo.m_prediction);
	shonax_2_fix.hook_index(CPrediction::INPREDICTION, util::force_cast(&Hooks::InPrediction));
	shonax_2_fix.hook_index(CPrediction::RUNCOMMAND, util::force_cast(&Hooks::RunCommand));

	
	m_client_mode.init( g_csgo.m_client_mode );
	m_client_mode.add( IClientMode::SHOULDDRAWPARTICLES, util::force_cast( &Hooks::ShouldDrawParticles ) );
	m_client_mode.add( IClientMode::SHOULDDRAWFOG, util::force_cast( &Hooks::ShouldDrawFog ) );
	m_client_mode.add( IClientMode::OVERRIDEVIEW, util::force_cast( &Hooks::OverrideView ) );
	m_client_mode.add( IClientMode::CREATEMOVE, util::force_cast( &Hooks::CreateMove ) );
	m_client_mode.add( IClientMode::DOPOSTSPACESCREENEFFECTS, util::force_cast( &Hooks::DoPostScreenSpaceEffects ) );
	
	m_surface.init( g_csgo.m_surface );
	m_surface.add( ISurface::LOCKCURSOR, util::force_cast( &Hooks::LockCursor ) );
	m_surface.add( ISurface::PLAYSOUND, util::force_cast( &Hooks::PlaySound ) );
	m_surface.add( ISurface::ONSCREENSIZECHANGED, util::force_cast( &Hooks::OnScreenSizeChanged ) );
	
	m_model_render.init( g_csgo.m_model_render );
	m_model_render.add( IVModelRender::DRAWMODELEXECUTE, util::force_cast( &Hooks::DrawModelExecute ) );

	
	m_render_view.init( g_csgo.m_render_view );
	m_render_view.add( IVRenderView::SCENEEND, util::force_cast( &Hooks::SceneEnd ) );
	
	m_shadow_mgr.init( g_csgo.m_shadow_mgr );
	m_shadow_mgr.add( IClientShadowMgr::COMPUTESHADOWDEPTHTEXTURES, util::force_cast( &Hooks::ComputeShadowDepthTextures ) );
	
	m_view_render.init( g_csgo.m_view_render );
	m_view_render.add( CViewRender::ONRENDERSTART, util::force_cast( &Hooks::OnRenderStart ) );
	m_view_render.add( CViewRender::RENDERVIEW, util::force_cast( &Hooks::RenderView ) );
	m_view_render.add( CViewRender::RENDER2DEFFECTSPOSTHUD, util::force_cast( &Hooks::Render2DEffectsPostHUD ) );
	m_view_render.add( CViewRender::RENDERSMOKEOVERLAY, util::force_cast( &Hooks::RenderSmokeOverlay ) );
	
	m_match_framework.init( g_csgo.m_match_framework );
	m_match_framework.add( CMatchFramework::GETMATCHSESSION, util::force_cast( &Hooks::GetMatchSession ) );
	
	m_material_system.init( g_csgo.m_material_system );
	m_material_system.add( IMaterialSystem::OVERRIDECONFIG, util::force_cast( &Hooks::OverrideConfig ) );
	
	m_fire_bullets.init( g_csgo.TEFireBullets );
	m_fire_bullets.add( 7, util::force_cast( &Hooks::PostDataUpdate ) );
	
	
	m_temp.init(((uintptr_t*)((uintptr_t)g_csgo.m_cl + 0x8)));
	m_temp.add(CClientState::TEMPENTITIES, util::force_cast(&Hooks::TempEntities));


	
	m_device.init(reinterpret_cast<uintptr_t**>(g_csgo.m_device));
	m_device.add(17, util::force_cast(&Present));
	m_device.add(16, util::force_cast(&ResetHook));



	g_custom_entity_listener.init( );
	
	// cvar hooks.
	m_debug_spread.init( g_csgo.weapon_debug_spread_show );
	m_debug_spread.add( ConVar::GETINT, util::force_cast( &Hooks::DebugSpreadGetInt ) );
	
	shonax_4_fix.setup( g_csgo.net_showfragments );
	shonax_4_fix.hook_index( ConVar::GETBOOL, util::force_cast( &Hooks::NetShowFragmentsGetBool ) );

	// set netvar proxies.
	g_netvars.SetProxy( HASH( "DT_CSPlayer" ), HASH( "m_angEyeAngles[0]" ), Pitch_proxy, m_Pitch_original );
	g_netvars.SetProxy( HASH( "DT_CSPlayer" ), HASH( "m_flLowerBodyYawTarget" ), Body_proxy, m_Body_original );
	g_netvars.SetProxy( HASH( "DT_CSRagdoll" ), HASH( "m_vecForce" ), Force_proxy, m_Force_original );
	g_netvars.SetProxy( HASH( "DT_CSRagdoll" ), HASH( "m_flAbsYaw" ), AbsYaw_proxy, m_AbsYaw_original );


	g_netvars.SetProxy(HASH("CBaseViewModel"), HASH("m_nSequence"), RecvProxy, sequence_hook);
	g_netvars.SetProxy(HASH("DT_BaseViewModel"), HASH("m_nModelIndex"), Viewmodel, viewmodel_hook);



	gc.setup(SteamGameCoordinator);
	gc.hook_index(0, GCSendMessageHook);
	gc.hook_index(2, GCRetrieveMessageHook);


	SpatialPartition = Interface<ISpatialPartition>("engine.dll", "SpatialPartition001"); //newest from csgo sdk

	clientstate_hook.setup(((uintptr_t*)((uintptr_t)g_csgo.m_cl + 0x8)));
	clientstate_hook.hook_index(64, ReadPacketEntitiesHook);

	partition_hook.setup(SpatialPartition);
	partition_hook.hook_index(16, SuppressListsHook);

	

	

}
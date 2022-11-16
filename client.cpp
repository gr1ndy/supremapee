#include "includes.h"

Client g_cl{ };

// loader will set this fucker.
char username[ 33 ] = "\x90\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x90";

// init routine.
ulong_t __stdcall Client::init( void* arg ) {
	// if not in interwebz mode, the driver will not set the username.
	g_cl.m_user = XOR( "user" );

	// stop here if we failed to acquire all the data needed from csgo.
	if( !g_csgo.init( ) )
		return 0;


	return 1;
}

void Client::DrawHUD( ) {
	if( !g_csgo.m_engine->IsInGame( ) )
		return;

	// get time.
	time_t t = std::time( nullptr );
	std::ostringstream time;
	time << std::put_time( std::localtime( &t ), ( "%H:%M:%S" ) );

	// get round trip time in milliseconds.
	int ms = std::max( 0, ( int ) std::round( g_cl.m_latency * 1000.f ) );

	// get tickrate.
	int rate = ( int ) std::round( 1.f / g_csgo.m_globals->m_interval );

	std::string text = tfm::format( XOR( "supremacy | rtt: %ims | rate: %i | %s" ), ms, rate, time.str( ).data( ) );
	render::FontSize_t size = render::hud.size( text );

	// background.
	render::rect_filled( m_width - size.m_width - 20, 10, size.m_width + 10, size.m_height + 2, { 15, 15, 15, 130 } );

	// text.
	render::hud.string( m_width - 15, 10, { 255, 255, 255, 255 }, text, render::ALIGN_RIGHT );
}
uint8_t* sfafsafddefault(HMODULE hModule, const char* szSignature)
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
static T* zFindHudElement(const char* name)
{
	static auto pThis = *reinterpret_cast<DWORD**>(sfafsafddefault(GetModuleHandleA("client.dll"), ("B9 ? ? ? ? E8 ? ? ? ? 8B 5D 08")) + 1);

	static auto find_hud_element = reinterpret_cast<DWORD(__thiscall*)(void*, const char*)>(sfafsafddefault(GetModuleHandleA("client.dll"), ("55 8B EC 53 8B 5D 08 56 57 8B F9 33 F6 39 77 28")));
	return (T*)find_hud_element(pThis, name);
}

void Client::KillFeed( ) {

	


	//if( !g_menu.main.misc.killfeed.get( ) )
	//	return;
	//
	//if( !g_csgo.m_engine->IsInGame( ) )
	//	return;
	//
	//// get the addr of the killfeed.
	//KillFeed_t* feed = ( KillFeed_t* ) g_csgo.m_hud->FindElement( HASH( "SFHudDeathNoticeAndBotStatus" ) );
	//if( !feed )
	//	return;
	//
	//int size = feed->notices.Count( );
	//if( !size )
	//	return;
	//
	//for( int i{ }; i < size; ++i ) {
	//	NoticeText_t* notice = &feed->notices[ i ];
	//
	//	// this is a local player kill, delay it.
	//	if( notice->fade == 1.5f )
	//		notice->fade = FLT_MAX;
	//}

	static bool clear_notices = false;
	static bool old_setting = g_menu.main.misc.killfeed.get();
	static void(__thiscall * clear_death_notices_addr)(uintptr_t);
	static uintptr_t* death_notices_addr;

	if (!g_csgo.m_engine->IsInGame() || !g_cl.m_local)
		return;

	if (!g_cl.m_local) {

		// invalidate the clear death notice address if local goes invalid.
		if (clear_death_notices_addr) {
			clear_death_notices_addr = nullptr;
		}

		// invalidate the death notice address if local goes invalid.
		if (death_notices_addr) {
			death_notices_addr = nullptr;
		}

		// that's it, exit out.
		return;
	}

	// only do the following if local is alive.
	if (g_cl.m_local->alive()) {
		// get the death notice addr.
		if (!death_notices_addr) {
			death_notices_addr = zFindHudElement<uintptr_t>(XOR("CCSGO_HudDeathNotice"));
		}

		// get the clear death notice addr.
		if (!clear_death_notices_addr) {
			clear_death_notices_addr = pattern::find(g_csgo.m_client_dll, XOR("55 8B EC 83 EC 0C 53 56 8B 71 58")).as< void(__thiscall*)(uintptr_t) >();
		}

		// only do the following if both addresses were found and are valid.
		if (clear_death_notices_addr && death_notices_addr) {
			// grab the local death notice time.
			float* local_death_notice_time = (float*)((uintptr_t)death_notices_addr + 0x50);
			static float backup_local_death_notice_time = *local_death_notice_time;

			// extend killfeed time.
			if (g_cl.m_round_flags == 1) {
				if (local_death_notice_time && g_menu.main.misc.killfeed.get()) {
					*local_death_notice_time = std::numeric_limits<float>::max();
				}
			}

			// if we disable the option, clear the death notices.
			if (old_setting != g_menu.main.misc.killfeed.get()) {
				if (!g_menu.main.misc.killfeed.get()) {
					if (local_death_notice_time) {
						// reset back to the regular death notice time.
						*local_death_notice_time = backup_local_death_notice_time;
					}

					clear_notices = true;
				}
				old_setting = g_menu.main.misc.killfeed.get();
			}

			// if we have the option enabled and we need to clear the death notices.
			if (g_menu.main.misc.killfeed.get()) {
				if (g_cl.m_round_flags == 0 && death_notices_addr - 0x14) {
					clear_notices = true;
				}
			}

			// clear the death notices.
			if (clear_notices) {
				clear_death_notices_addr(((uintptr_t)death_notices_addr - 0x14));
				clear_notices = false;
			}
		}
	}
	else {
		// invalidate clear death notice address.
		if (clear_death_notices_addr) {
			clear_death_notices_addr = nullptr;
		}

		// invalidate death notice address.
		if (death_notices_addr) {
			death_notices_addr = nullptr;
		}
	}








}

void Client::OnPaint( ) {
	// update screen size.
	g_csgo.m_engine->GetScreenSize( m_width, m_height );

	// render stuff.
	g_visuals.think( );
	g_grenades.paint( );
	g_notify.think( );

	DrawHUD( );
	KillFeed( );
	
	// menu goes last.
	g_gui.think( );
}

void Client::OnMapload( ) {
	// store class ids.
	g_netvars.SetupClassData( );

	// createmove will not have been invoked yet.
	// but at this stage entites have been created.
	// so now we can retrive the pointer to the local player.
	m_local = g_csgo.m_entlist->GetClientEntity< Player* >( g_csgo.m_engine->GetLocalPlayer( ) );

	// world materials.
	Visuals::ModulateWorld( );

	m_sequences.clear( );




	// if the INetChannelInfo pointer has changed, store it for later.
	g_csgo.m_net = g_csgo.m_engine->GetNetChannelInfo( );
	
	
	
	if( g_csgo.m_net ) {
		g_hooks.m_net_channel.reset( );
		g_hooks.m_net_channel.init( g_csgo.m_net );
		g_hooks.m_net_channel.add( INetChannel::PROCESSPACKET, util::force_cast( &Hooks::ProcessPacket ) );
		g_hooks.m_net_channel.add( INetChannel::SENDDATAGRAM, util::force_cast( &Hooks::SendDatagram ) );
	}
}

void Client::StartMove( CUserCmd* cmd ) {
	// save some usercmd stuff.
	m_cmd = cmd;
	m_tick = cmd->m_tick;
	m_view_angles = cmd->m_view_angles;
	m_buttons = cmd->m_buttons;
	m_pressing_move = (m_buttons & (IN_LEFT) || m_buttons & (IN_FORWARD) || m_buttons & (IN_BACK) ||
		m_buttons & (IN_RIGHT) || m_buttons & (IN_MOVELEFT) || m_buttons & (IN_MOVERIGHT) ||
		m_buttons & (IN_JUMP));
	// get local ptr.
	m_local = g_csgo.m_entlist->GetClientEntity< Player* >( g_csgo.m_engine->GetLocalPlayer( ) );
	if( !m_local )
		return;

	// store max choke
	// TODO; 11 -> m_bIsValveDS
	m_max_lag = ( m_local->m_fFlags( ) & FL_ONGROUND ) ? 16 : 15;
	m_lag = g_csgo.m_cl->m_choked_commands;
	m_lerp = game::GetClientInterpAmount( );
	m_latency = g_csgo.m_net->GetLatency( INetChannel::FLOW_OUTGOING );
	math::clamp( m_latency, 0.f, 1.f );
	m_latency_ticks = game::TIME_TO_TICKS( m_latency );
	m_server_tick = g_csgo.m_cl->clock_drift_mgr.m_server_tick;
	m_arrival_tick = m_server_tick + m_latency_ticks;

	// processing indicates that the localplayer is valid and alive.
	m_processing = m_local && m_local->alive( );
	if( !m_processing )
		return;

	// make sure prediction has ran on all usercommands.
	// because prediction runs on frames, when we have low fps it might not predict all usercommands.
	// also fix the tick being inaccurate.
	g_inputpred.update( );

	// store some stuff about the local player.
	m_flags = m_local->m_fFlags( );
	g_tickbase.PreMovement();
	// ...
	m_shot = false;
}

void Client::BackupPlayers( bool restore ) {
	if( restore ) {
		// restore stuff.
		for( int i{ 1 }; i <= g_csgo.m_globals->m_max_clients; ++i ) {
			Player* player = g_csgo.m_entlist->GetClientEntity< Player* >( i );

			if( !g_aimbot.IsValidTarget( player ) )
				continue;

			g_aimbot.m_backup[ i - 1 ].restore( player );
		}
	}

	else {
		// backup stuff.
		for( int i{ 1 }; i <= g_csgo.m_globals->m_max_clients; ++i ) {
			Player* player = g_csgo.m_entlist->GetClientEntity< Player* >( i );

			if( !g_aimbot.IsValidTarget( player ) )
				continue;

			g_aimbot.m_backup[ i - 1 ].store( player );
		}
	}
}

void Client::DoMove( ) {
	penetration::PenetrationOutput_t tmp_pen_data{ };

	// run movement code before input prediction.
	g_movement.JumpRelated( );
	g_movement.Strafe( );
	g_movement.FakeWalk( );
	g_movement.AutoPeek( );
	g_hvh.FakeDuck();



	// backup strafe angles (we need them for input prediction)
	m_strafe_angles = m_cmd->m_view_angles;

	// predict input.
	g_inputpred.run( );

	// restore original angles after input prediction
	m_cmd->m_view_angles = m_view_angles;

	// convert viewangles to directional forward vector.
	math::AngleVectors( m_view_angles, &m_forward_dir );

	// store stuff after input pred.
	m_shoot_pos = m_local->GetShootPosition( );

	// reset shit.
	m_weapon = nullptr;
	m_weapon_info = nullptr;
	m_weapon_id = -1;
	m_weapon_type = WEAPONTYPE_UNKNOWN;
	m_player_fire = m_weapon_fire = false;

	// store weapon stuff.
	m_weapon = m_local->GetActiveWeapon( );

	if( m_weapon ) {
		m_weapon_info = m_weapon->GetWpnData( );
		m_weapon_id = m_weapon->m_iItemDefinitionIndex( );
		m_weapon_type = m_weapon_info->WeaponType;

		// ensure weapon spread values / etc are up to date.
		if( m_weapon_type != WEAPONTYPE_GRENADE )
			m_weapon->UpdateAccuracyPenalty( );

		// run autowall once for penetration crosshair if we have an appropriate weapon.
		if( m_weapon_type != WEAPONTYPE_KNIFE && m_weapon_type != WEAPONTYPE_C4 && m_weapon_type != WEAPONTYPE_GRENADE ) {
			penetration::PenetrationInput_t in;
			in.m_from = m_local;
			in.m_target = nullptr;
			in.m_pos = m_shoot_pos + ( m_forward_dir * m_weapon_info->flRange );
			in.m_damage = 1.f;
			in.m_damage_pen = 1.f;
			in.m_can_pen = true;

			// run autowall.
			penetration::run( &in, &tmp_pen_data );
		}

		// set pen data for penetration crosshair.
		m_pen_data = tmp_pen_data;

		// can the player fire.
		m_player_fire = g_csgo.m_globals->m_curtime >= m_local->m_flNextAttack( ) && !g_csgo.m_gamerules->m_bFreezePeriod( ) && !( g_cl.m_flags & FL_FROZEN );

		UpdateRevolverCock( );
		m_weapon_fire = CanFireWeapon( );
	}

	// last tick defuse.
	// todo - dex;  figure out the range for CPlantedC4::Use?
	//              add indicator if valid (on ground, still have time, not being defused already, etc).
	//              move this? not sure where we should put it.
	if( g_input.GetKeyState( g_menu.main.misc.last_tick_defuse.get( ) ) && g_visuals.m_c4_planted ) {
		float defuse = ( m_local->m_bHasDefuser( ) ) ? 5.f : 10.f;
		float remaining = g_visuals.m_planted_c4_explode_time - g_csgo.m_globals->m_curtime;
		float dt = remaining - defuse - ( g_cl.m_latency / 2.f );

		m_cmd->m_buttons &= ~IN_USE;
		if( dt <= game::TICKS_TO_TIME( 2 ) )
			m_cmd->m_buttons |= IN_USE;
	}

	// grenade prediction.
	g_grenades.think( );

	// run fakelag.
	g_hvh.SendPacket( );

	// run aimbot.
	g_aimbot.think( );

	// run antiaims.
	g_hvh.AntiAim( );
}

void Client::EndMove( CUserCmd* cmd ) {
	// update client-side animations.
	UpdateInformation( );

	// if matchmaking mode, anti untrust clamp.
	if( g_menu.main.config.mode.get( ) == 0 )
		m_cmd->m_view_angles.SanitizeAngle( );

	// fix our movement.
	g_movement.FixMove( cmd, m_strafe_angles );

	// this packet will be sent.
	if( *m_packet ) {
		g_hvh.m_step_switch = ( bool ) g_csgo.RandomInt( 0, 1 );

		// we are sending a packet, so this will be reset soon.
		// store the old value.
		m_old_lag = m_lag;

		// get radar angles.
		m_radar = cmd->m_view_angles;
		m_radar.normalize( );

		// get current origin.
		vec3_t cur = m_local->m_vecOrigin( );

		// get prevoius origin.
		vec3_t prev = m_net_pos.empty( ) ? cur : m_net_pos.front( ).m_pos;

		// check if we broke lagcomp.
		m_lagcomp = ( cur - prev ).length_sqr( ) > 4096.f;

		// save sent origin and time.
		m_net_pos.emplace_front( g_csgo.m_globals->m_curtime, cur );
	}

	// store some values for next tick.
	m_old_packet = *m_packet;
	m_old_shot = m_shot;

	g_tickbase.PostMovement();
}
bool Client::IsFiring(float curtime) {
	const auto weapon = g_cl.m_weapon;
	if (!weapon)
		return false;

	const auto IsZeus = m_weapon_id == Weapons_t::ZEUS;
	const auto IsKnife = !IsZeus && m_weapon_type == WEAPONTYPE_KNIFE;

	if (weapon->IsGrenade())
		return !weapon->m_bPinPulled() && weapon->m_fThrowTime() > 0.f && weapon->m_fThrowTime() < curtime;
	else if (IsKnife)
		return (g_cl.m_cmd->m_buttons & (IN_ATTACK) || g_cl.m_cmd->m_buttons & (IN_ATTACK2)) && CanFireWeapon(curtime);
	else
		return g_cl.m_cmd->m_buttons & (IN_ATTACK) && CanFireWeapon(curtime);
}
bool Client::CanFireWeapon(float curtime) {
	// the player cant fire.
	if (!m_player_fire || !m_weapon)
		return false;

	if (m_weapon->IsGrenade())
		return false;

	// if we have no bullets, we cant shoot.
	if (m_weapon_type != WEAPONTYPE_KNIFE && m_weapon->m_iClip1() < 1)
		return false;

	// do we have any burst shots to handle?
	if ((m_weapon_id == GLOCK || m_weapon_id == FAMAS) && m_weapon->m_iBurstShotsRemaining() > 0) {
		// new burst shot is coming out.
		if (curtime >= m_weapon->m_fNextBurstShot())
			return true;
	}

	// r8 revolver.
	if (m_weapon_id == REVOLVER) {
		// mouse1.
		if (m_revolver_fire) {
			return true;
		}
		else {
			return false;
		}
	}

	// yeez we have a normal gun.
	if (curtime >= m_weapon->m_flNextPrimaryAttack())
		return true;

	return false;
}

void Client::OnTick( CUserCmd* cmd ) {
	// TODO; add this to the menu.
	if( g_menu.main.misc.ranks.get( ) && cmd->m_buttons & IN_SCORE ) {
		static CCSUsrMsg_ServerRankRevealAll msg{ };
		g_csgo.ServerRankRevealAll( &msg );
	}

	// store some data and update prediction.
	StartMove( cmd );

	// not much more to do here.
	if( !m_processing )
		return;

	// save the original state of players.
	BackupPlayers( false );

	// run all movement related code.
	DoMove( );

	// store command number at the time we fired.
	if (IsFiring(g_csgo.m_globals->m_curtime)) {
		g_cl.m_shot_command_number = cmd->m_command_number;
		g_cl.m_shot_tickbase = g_cl.m_local->m_nTickBase();

		if (!g_hvh.m_fake_duck) {
			*g_cl.m_packet = true;
		}

		if (!m_shot)
			m_shot = true;
	}

	// store stome additonal stuff for next tick
	// sanetize our usercommand if needed and fix our movement.
	EndMove( cmd );

	// restore the players.
	BackupPlayers( true );

	// restore curtime/frametime
	// and prediction seed/player.
	g_inputpred.restore( );
}

void Client::SetAngles( ) {
	if( !g_cl.m_local || !g_cl.m_processing )
		return;

	// set the nointerp flag.
	g_cl.m_local->m_fEffects( ) |= EF_NOINTERP;

	// apply the rotation.
	g_cl.m_local->SetAbsAngles( m_rotation );
	g_cl.m_local->m_angRotation( ) = m_rotation;
	g_cl.m_local->m_angNetworkAngles( ) = m_rotation;

	// set radar angles.
	if( g_csgo.m_input->CAM_IsThirdPerson( ) )
		g_csgo.m_prediction->SetLocalViewAngles( m_radar );
}


bool Client::GenerateMatrix(Player* pEntity, BoneArray* pBoneToWorldOut, int boneMask, float currentTime)
{
	if (!pEntity || !pEntity->IsPlayer() || !pEntity->alive())
		return false;

	CStudioHdr* pStudioHdr = pEntity->GetModelPtr();
	if (!pStudioHdr) {
		return false;
	}

	// get ptr to bone accessor.
	CBoneAccessor* accessor = &pEntity->m_BoneAccessor();
	if (!accessor)
		return false;

	// store origial output matrix.
	// likely cachedbonedata.
	BoneArray* backup_matrix = accessor->m_pBones;
	if (!backup_matrix)
		return false;

	vec3_t vecAbsOrigin = pEntity->GetAbsOrigin();
	ang_t vecAbsAngles = pEntity->GetAbsAngles();

	vec3_t vecBackupAbsOrigin = pEntity->GetAbsOrigin();
	ang_t vecBackupAbsAngles = pEntity->GetAbsAngles();

	matrix3x4a_t parentTransform;
	math::AngleMatrix(vecAbsAngles, vecAbsOrigin, parentTransform);

	pEntity->m_fEffects() |= 8;

	pEntity->SetAbsOrigin(vecAbsOrigin);
	pEntity->SetAbsAngles(vecAbsAngles);

	vec3_t pos[128];
	__declspec(align(16)) quaternion_t     q[128];

	g_bones.m_running = true;

	uint32_t fBackupOcclusionFlags = pEntity->m_iOcclusionFlags();

	pEntity->m_iOcclusionFlags() |= 0xA; // skipp call to accumulatelayers in standardblendingrules

	pEntity->StandardBlendingRules(pStudioHdr, pos, q, currentTime, boneMask);

	pEntity->m_iOcclusionFlags() = fBackupOcclusionFlags; // standardblendingrules was called now restore niggaaaa

	accessor->m_pBones = pBoneToWorldOut;

	uint8_t computed[0x100];
	std::memset(computed, 0, 0x100);

	pEntity->BuildTransformations(pStudioHdr, pos, q, parentTransform, boneMask, computed);

	accessor->m_pBones = backup_matrix;

	pEntity->SetAbsOrigin(vecBackupAbsOrigin);
	pEntity->SetAbsAngles(vecBackupAbsAngles);

	g_bones.m_running = false;

	return true;
}


void Client::UpdateFakeAnimations() {
	if (!g_cl.m_cmd || !g_cl.m_processing)
		return;

	if (!g_csgo.m_cl)
		return;

	bool allocate = (g_cl.m_fake_state == nullptr);
	bool change = (!allocate) && (g_cl.m_local->GetRefEHandle() != g_cl.m_ent_handle);
	bool reset = (!allocate && !change) && (g_cl.m_local->m_flSpawnTime() != g_cl.m_spawn_time);

	// player changed, free old animation state.
	if (change && g_cl.m_fake_state_allocated)
		g_csgo.m_mem_alloc->Free(g_cl.m_fake_state_allocated);

	// need to reset? (on respawn)
	if (reset) {
		// reset animation state.
		game::ResetAnimationState(g_cl.m_fake_state);

		// note new spawn time.
		g_cl.m_spawn_time = g_cl.m_local->m_flSpawnTime();
	}

	// need to allocate or create new due to player change.
	if (allocate || change) {
		// only works with games heap alloc.
		g_cl.m_fake_state_allocated = (CCSGOPlayerAnimState*)g_csgo.m_mem_alloc->Alloc(sizeof(CCSGOPlayerAnimState));

		if (g_cl.m_fake_state_allocated != nullptr)
			game::CreateAnimationState(g_cl.m_local, g_cl.m_fake_state_allocated);

		// used to detect if we need to recreate / reset.
		g_cl.m_ent_handle = g_cl.m_local->GetRefEHandle();
		g_cl.m_spawn_time = g_cl.m_local->m_flSpawnTime();

		// note anim state for future use.
		g_cl.m_fake_state = g_cl.m_fake_state_allocated;
	}
	else {
		// make sure our state isn't null.
		if (!g_cl.m_fake_state)
			return;

		// allow re-animating in the same frame.
		g_cl.m_fake_state->m_frame = 0.f;

		// update anim update delta as server would.
		g_cl.m_fake_state->m_update_delta = fmaxf(g_csgo.m_globals->m_curtime - g_cl.m_fake_state->m_frame, 0.0);

		// update fake animations per tick.
		if (g_cl.m_update_fake) {
			// update fake animations when we send commands.
			if (!g_csgo.m_cl->m_choked_commands) {

				// update fake animation state.
				game::UpdateAnimationState(g_cl.m_fake_state, g_cl.m_angle);

				g_cl.m_local->GetAnimLayers(g_cl.m_fake_layers);
				g_cl.m_local->GetPoseParameters(g_cl.m_fake_poses);
			}

			// remove model sway.
			g_cl.m_fake_layers[12].m_weight = 0.f;

			// make sure to only animate once per tick.
			g_cl.m_update_fake = false;
		}

		// note - alpha; 
		// you don't need to set animation layers/poses to the fake ones here 
		// since they will already be fake, as UpdateAnimationState called 
		// UpdateClientsideAnimations - And since this is being called before
		// we update local player angles/layers, it will get overwritten by them
		// in the end.

		// replace current animation layers and poses with the fake ones.
		g_cl.m_local->SetAnimLayers(g_cl.m_fake_layers);
		g_cl.m_local->SetPoseParameters(g_cl.m_fake_poses);

		// replace the model rotation and build a matrix with our fake data.
		g_cl.m_local->SetAbsAngles(ang_t(0.f, g_cl.m_fake_state->m_goal_feet_yaw, 0.f));

		// generate a fake matrix.
		GenerateMatrix(g_cl.m_local, g_cl.m_fake_matrix, 0x7FF00, g_cl.m_local->m_flSimulationTime());

		// fix interpolated model.
		for (auto& i : g_cl.m_fake_matrix) {
			i[0][3] -= g_cl.m_local->GetRenderOrigin().x;
			i[1][3] -= g_cl.m_local->GetRenderOrigin().y;
			i[2][3] -= g_cl.m_local->GetRenderOrigin().z;
		}

		// revert our layers and poses back.
		g_cl.m_local->SetAnimLayers(g_cl.m_real_layers);
		g_cl.m_local->SetPoseParameters(g_cl.m_real_poses);

		// replace the model rotation with the proper rotation.
		g_cl.m_local->SetAbsAngles(ang_t(0.f, g_cl.m_rotation.y, 0.f));
	}
}
void Client::UpdateAnimations( ) {
	if( !g_cl.m_local || !g_cl.m_processing )
		return;

	CCSGOPlayerAnimState* state = g_cl.m_local->m_PlayerAnimState( );
	if( !state )
		return;

	// prevent model sway on player.
	g_cl.m_local->m_AnimOverlay( )[ 12 ].m_weight = 0.f;

	// update animations with last networked data.
	g_cl.m_local->SetPoseParameters( g_cl.m_poses );

	// update abs yaw with last networked abs yaw.
	g_cl.m_local->SetAbsAngles( ang_t( 0.f, g_cl.m_abs_yaw, 0.f ) );
}

void Client::UpdateInformation( ) {
	if( g_cl.m_lag > 0 )
		return;

	CCSGOPlayerAnimState* state = g_cl.m_local->m_PlayerAnimState( );
	if( !state )
		return;

	// update time.
	m_anim_frame = g_csgo.m_globals->m_curtime - m_anim_time;
	m_anim_time = g_csgo.m_globals->m_curtime;

	// current angle will be animated.
	m_angle = g_cl.m_cmd->m_view_angles;

	// fix landing anim.
	if( state->m_land && !state->m_dip_air && state->m_dip_cycle > 0.f )
		m_angle.x = -12.f;

	math::clamp( m_angle.x, -90.f, 90.f );
	m_angle.normalize( );

	// write angles to model.
	g_csgo.m_prediction->SetLocalViewAngles( m_angle );

	// set lby to predicted value.
	g_cl.m_local->m_flLowerBodyYawTarget( ) = m_body;

	// CCSGOPlayerAnimState::Update, bypass already animated checks.
	if( state->m_frame == g_csgo.m_globals->m_frame )
		state->m_frame -= 1;

	// call original, bypass hook.
	if (g_hooks.m_UpdateClientSideAnimation) //not real fix but why not
		g_hooks.m_UpdateClientSideAnimation( g_cl.m_local );

	// get last networked poses.
	g_cl.m_local->GetPoseParameters( g_cl.m_poses );

	// store updated abs yaw.
	g_cl.m_abs_yaw = state->m_goal_feet_yaw;

	// we landed.
	if( !m_ground && state->m_ground ) {
		m_body = m_angle.y;
		m_body_pred = m_anim_time;
	}

	// walking, delay lby update by .22.
	else if( state->m_speed > 0.1f ) {
		if( state->m_ground )
			m_body = m_angle.y;

		m_body_pred = m_anim_time + 0.22f;
	}

	// standing update every 1.1s
	else if( m_anim_time > m_body_pred ) {
		m_body = m_angle.y;
		m_body_pred = m_anim_time + 1.1f;
	}

	// save updated data.
	m_rotation = g_cl.m_local->m_angAbsRotation( );
	m_speed = state->m_speed;
	m_ground = state->m_ground;
}

void Client::print( const std::string text, ... ) {
	va_list     list;
	int         size;
	std::string buf;

	if( text.empty( ) )
		return;

	va_start( list, text );

	// count needed size.
	size = std::vsnprintf( 0, 0, text.c_str( ), list );

	// allocate.
	buf.resize( size );

	// print to buffer.
	std::vsnprintf( buf.data( ), size + 1, text.c_str( ), list );

	va_end( list );

	// print to console.
	g_csgo.m_cvar->ConsoleColorPrintf( colors::burgundy, XOR( "[supremacy] " ) );
	g_csgo.m_cvar->ConsoleColorPrintf( colors::white, buf.c_str( ) );
}

bool Client::CanFireWeapon( ) {
	// the player cant fire.
	if( !m_player_fire )
		return false;

	if( m_weapon_type == WEAPONTYPE_GRENADE )
		return false;

	// if we have no bullets, we cant shoot.
	//if( m_weapon_type != WEAPONTYPE_KNIFE && m_weapon->m_iClip1( ) < 1 )
	//	return false;
	//i replace in other shit bcuz for autoknife -_-

	// do we have any burst shots to handle?
	if( ( m_weapon_id == GLOCK || m_weapon_id == FAMAS ) && m_weapon->m_iBurstShotsRemaining( ) > 0 ) {
		// new burst shot is coming out.
		if( g_csgo.m_globals->m_curtime >= m_weapon->m_fNextBurstShot( ) )
			return true;
	}

	// r8 revolver.
	if( m_weapon_id == REVOLVER ) {
		int act = m_weapon->m_Activity( );

		// mouse1.
		if( !m_revolver_fire ) {
			if( ( act == 185 || act == 193 ) && m_revolver_cock == 0 )
				return g_csgo.m_globals->m_curtime >= m_weapon->m_flNextPrimaryAttack( );

			return false;
		}
	}

	// yeez we have a normal gun.
	if( g_csgo.m_globals->m_curtime >= m_weapon->m_flNextPrimaryAttack( ) )
		return true;

	return false;
}

void Client::UpdateRevolverCock( ) {
	// default to false.
	m_revolver_fire = false;

	// reset properly.
	if( m_revolver_cock == -1 )
		m_revolver_cock = 0;

	// we dont have a revolver.
	// we have no ammo.
	// player cant fire
	// we are waiting for we can shoot again.
	if( m_weapon_id != REVOLVER || m_weapon->m_iClip1( ) < 1 || !m_player_fire || g_csgo.m_globals->m_curtime < m_weapon->m_flNextPrimaryAttack( ) ) {
		// reset.
		m_revolver_cock = 0;
		m_revolver_query = 0;
		return;
	}

	// calculate max number of cocked ticks.
	// round to 6th decimal place for custom tickrates..
	int shoot = ( int ) ( 0.25f / ( std::round( g_csgo.m_globals->m_interval * 1000000.f ) / 1000000.f ) );

	// amount of ticks that we have to query.
	m_revolver_query = shoot - 1;

	// we held all the ticks we needed to hold.
	if( m_revolver_query == m_revolver_cock ) {
		// reset cocked ticks.
		m_revolver_cock = -1;

		// we are allowed to fire, yay.
		m_revolver_fire = true;
	}

	else {
		// we still have ticks to query.
		// apply inattack.
		if( g_menu.main.config.mode.get( ) == 0 && m_revolver_query > m_revolver_cock )
			m_cmd->m_buttons |= IN_ATTACK;

		// count cock ticks.
		// do this so we can also count 'legit' ticks
		// that didnt originate from the hack.
		if( m_cmd->m_buttons & IN_ATTACK )
			m_revolver_cock++;

		// inattack was not held, reset.
		else m_revolver_cock = 0;
	}

	// remove inattack2 if cocking.
	if( m_revolver_cock > 0 )
		m_cmd->m_buttons &= ~IN_ATTACK2;
}

void Client::UpdateIncomingSequences( ) {
	if( !g_csgo.m_net )
		return;

	if( m_sequences.empty( ) || g_csgo.m_net->m_in_seq > m_sequences.front( ).m_seq ) {
		// store new stuff.
		m_sequences.emplace_front( g_csgo.m_globals->m_realtime, g_csgo.m_net->m_in_rel_state, g_csgo.m_net->m_in_seq );
	}

	// do not save too many of these.
	while( m_sequences.size( ) > 2048 )
		m_sequences.pop_back( );
}
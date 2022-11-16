#include "includes.h"

bool Hooks::ShouldDrawParticles( ) {
	return g_hooks.m_client_mode.GetOldMethod< ShouldDrawParticles_t >( IClientMode::SHOULDDRAWPARTICLES )( this );
}

bool Hooks::ShouldDrawFog( ) {
	// remove fog.
	if( g_menu.main.visuals.nofog.get( ) )
		return false;

	return g_hooks.m_client_mode.GetOldMethod< ShouldDrawFog_t >( IClientMode::SHOULDDRAWFOG )( this );
}

void Hooks::OverrideView( CViewSetup* view ) {
	// damn son.
	g_cl.m_local = g_csgo.m_entlist->GetClientEntity< Player* >( g_csgo.m_engine->GetLocalPlayer( ) );

	// g_grenades.think( );
	g_visuals.ThirdpersonThink( );

	if (g_cl.m_processing) {
		if (g_hvh.m_fake_duck) {
			view->m_origin.z = g_cl.m_local->GetAbsOrigin().z + 64.f;
		}
	}

    // call original.
	g_hooks.m_client_mode.GetOldMethod< OverrideView_t >( IClientMode::OVERRIDEVIEW )( this, view );

    // remove scope edge blur.
	//no need
	if( g_menu.main.visuals.noscope.get( ) ) {
		if( g_cl.m_local && g_cl.m_local->m_bIsScoped( ) )
            view->m_edge_blur = 0;
	}
}

bool Hooks::CreateMove( float time, CUserCmd* cmd ) {
	Stack   stack;
	// called from CInput::ExtraMouseSample -> return original.

	if (!cmd || !cmd->m_command_number)
		return g_hooks.m_client_mode.GetOldMethod< CreateMove_t >(IClientMode::CREATEMOVE)(this, time, cmd);

	// if we arrived here, called from -> CInput::CreateMove
	// call EngineClient::SetViewAngles according to what the original returns.
	if (g_hooks.m_client_mode.GetOldMethod< CreateMove_t >(IClientMode::CREATEMOVE)(this, time, cmd))
		g_csgo.m_engine->SetViewAngles(cmd->m_view_angles);
	bool old_tickbase = g_tickbase.m_shift_data.m_should_attempt_shift;
	g_cl.m_negate_desync = g_input.GetKeyState(g_menu.main.antiaim.rage_aa_negate_key.get());
	g_hvh.m_fake_duck = g_input.GetKeyState(g_menu.main.antiaim.fakeduck.get());
	g_cl.m_goal_shift = g_menu.main.aimbot.exploit.get() == 1 ? 13 : 7;
	g_tickbase.m_shift_data.m_should_attempt_shift = g_menu.main.aimbot.exploit.get() > 0;
	if (old_tickbase != g_tickbase.m_shift_data.m_should_attempt_shift) {
		//g_notify.add( tfm::format( XOR( "Tried shifting tickbase" )) );

		if (g_tickbase.m_shift_data.m_should_attempt_shift)
			g_tickbase.m_shift_data.m_needs_recharge = g_cl.m_goal_shift;
		else
			g_tickbase.m_shift_data.m_needs_recharge = 0;

		g_tickbase.m_shift_data.m_did_shift_before = false;
	}




	// random_seed isn't generated in ClientMode::CreateMove yet, we must set generate it ourselves.
	cmd->m_random_seed = g_csgo.MD5_PseudoRandom( cmd->m_command_number ) & 0x7fffffff;

	// get bSendPacket off the stack.
	g_cl.m_packet = stack.next( ).local( 0x1c ).as< bool* >( );

	// get bFinalTick off the stack.
	g_cl.m_final_packet = stack.next( ).local( 0x1b ).as< bool* >( );

	// invoke move function.

	// let's wait till we successfully charged if we want to, hide shots. (this fixes anti-aim and shit, sorry, redundant :/)
	if (g_tickbase.m_shift_data.m_should_attempt_shift && !g_tickbase.m_shift_data.m_should_disable) {
		if (g_cl.m_goal_shift == 7 && (g_tickbase.m_shift_data.m_prepare_recharge || g_tickbase.m_shift_data.m_did_shift_before && !g_tickbase.m_shift_data.m_should_be_ready)) {
			// are we IN_ATTACK?
			if (cmd->m_buttons & IN_ATTACK) {
				// remove the flag :D!
				cmd->m_buttons &= ~IN_ATTACK;
			}
		}
	}

	g_cl.OnTick( cmd );

	// make sure to update our animations with the right angles.
	if (cmd->m_command_number >= g_cl.m_shot_command_number && g_cl.m_shot_command_number >= cmd->m_command_number - g_csgo.m_cl->m_choked_commands) {
		g_cl.m_angle = g_csgo.m_input->m_commands[g_cl.m_shot_command_number % 150].m_view_angles;
	}
	else {
		g_cl.m_angle = cmd->m_view_angles;
	}

	// let's wait till we successfully charged if we want to, hide shots.
	if (g_tickbase.m_shift_data.m_should_attempt_shift && !g_tickbase.m_shift_data.m_should_disable) {
		if (g_cl.m_goal_shift == 7 && (g_tickbase.m_shift_data.m_prepare_recharge || g_tickbase.m_shift_data.m_did_shift_before && !g_tickbase.m_shift_data.m_should_be_ready)) {
			// are we IN_ATTACK?
			if (cmd->m_buttons & IN_ATTACK) {
				// remove the flag :D!
				cmd->m_buttons &= ~IN_ATTACK;
			}
		}
	}


	return false;
}

bool Hooks::DoPostScreenSpaceEffects( CViewSetup* setup ) {
	g_visuals.RenderGlow( );

	return g_hooks.m_client_mode.GetOldMethod< DoPostScreenSpaceEffects_t >( IClientMode::DOPOSTSPACESCREENEFFECTS )( this, setup );
}
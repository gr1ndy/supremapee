#pragma once

class AimbotTab : public Tab {
public:
	// col1.
	Checkbox	  enable;
	Checkbox	  silent;
	Dropdown	  selection;
	Checkbox	  fov;
	Slider		  fov_amount;
	MultiDropdown hitbox;
	MultiDropdown hitbox_history;
	MultiDropdown multipoint;
	Slider		  scale;
	Slider		  body_scale;
	Slider		  minimal_damage;
	Checkbox	  minimal_damage_hp;
	Checkbox	  penetrate;
	Slider		  penetrate_minimal_damage;
	Checkbox	  penetrate_minimal_damage_hp;
	Checkbox      knifebot;
	Checkbox	  zeusbot;
	Dropdown exploit;
	// col2.
	Dropdown      zoom;
	Checkbox      nospread;
	Checkbox      norecoil;
	Checkbox      hitchance;
	Slider	      hitchance_amount;
	Checkbox      lagfix;
	Checkbox	  correct;
	MultiDropdown baim1;
	MultiDropdown baim2;
	Slider        baim_hp;
	Keybind       baim_key;

public:
	void init( ) {
		// title.
		SetTitle( XOR( "aimbot" ) );

		enable.setup( XOR( "enable" ), XOR( "enable" ) );
		RegisterElement( &enable );

		silent.setup( XOR( "silent aimbot" ), XOR( "silent" ) );
		RegisterElement( &silent );

		selection.setup( XOR( "target selection" ), XOR( "selection" ), { XOR( "distance" ), XOR( "crosshair" ), XOR( "damage" ), XOR( "health" ), XOR( "lag" ), XOR( "height" ) } );
		RegisterElement( &selection );

		fov.setup( XOR( "angle limit" ), XOR( "fov" ) );
		RegisterElement( &fov );

		fov_amount.setup( "", XOR( "fov_amount" ), 1.f, 180.f, false, 0, 180.f, 1.f, XOR( L"?" ) );
		fov_amount.AddShowCallback( callbacks::IsFovOn );
		RegisterElement( &fov_amount );

		hitbox.setup( XOR( "hitbox" ), XOR( "hitbox" ), { XOR( "head" ), XOR( "chest" ), XOR( "body" ), XOR( "arms" ), XOR( "legs" ) } );
		RegisterElement( &hitbox );

		hitbox_history.setup( XOR( "hitbox history" ), XOR( "hitbox_history" ), { XOR( "head" ), XOR( "chest" ), XOR( "body" ), XOR( "arms" ), XOR( "legs" ) } );
		RegisterElement( &hitbox_history );

		multipoint.setup( XOR( "multi-point" ), XOR( "multipoint" ), { XOR( "head" ), XOR( "chest" ), XOR( "body" ), XOR( "legs" ) } );
		RegisterElement( &multipoint );

		scale.setup( "", XOR( "hitbox_scale" ), 1.f, 100.f, false, 0, 90.f, 1.f, XOR( L"%" ) );
		scale.AddShowCallback( callbacks::IsMultipointOn );
		RegisterElement( &scale );

		body_scale.setup( XOR( "body hitbox scale" ), XOR( "body_hitbox_scale" ), 1.f, 100.f, true, 0, 50.f, 1.f, XOR( L"%" ) );
		body_scale.AddShowCallback( callbacks::IsMultipointBodyOn );
		RegisterElement( &body_scale );

		minimal_damage.setup( XOR( "minimal damage" ), XOR( "minimal_damage" ), 1.f, 100.f, true, 0, 40.f, 1.f );
		RegisterElement( &minimal_damage );

		minimal_damage_hp.setup( XOR( "scale damage on hp" ), XOR( "minimal_damage_hp" ) );
		RegisterElement( &minimal_damage_hp );

		penetrate.setup( XOR( "penetrate walls" ), XOR( "penetrate" ) );
		RegisterElement( &penetrate );

		penetrate_minimal_damage.setup( "", XOR( "penetrate_minimal_damage" ), 1.f, 100.f, false, 0, 30.f, 1.f );
		penetrate_minimal_damage.AddShowCallback( callbacks::IsPenetrationOn );
		RegisterElement( &penetrate_minimal_damage );

		penetrate_minimal_damage_hp.setup( XOR( "scale penetration damage on hp" ), XOR( "penetrate_minimal_damage_hp" ) );
		penetrate_minimal_damage_hp.AddShowCallback( callbacks::IsPenetrationOn );
		RegisterElement( &penetrate_minimal_damage_hp );

		knifebot.setup( XOR( "aimbot with knife" ), XOR( "knifebot" ) );
		RegisterElement( &knifebot );

		zeusbot.setup( XOR( "aimbot with taser" ), XOR( "zeusbot" ) );
		RegisterElement( &zeusbot );





		exploit.setup(XOR("exploit"), XOR("exploit"), { XOR("none"), XOR("doubletap"), XOR("hideshots") });
		RegisterElement(&exploit);


		// col2.
		zoom.setup( XOR( "auto scope" ), XOR( "zoom" ), { XOR( "off" ), XOR( "always" ), XOR( "hitchance fail" ) } );
		RegisterElement( &zoom, 1 );

		nospread.setup( XOR( "compensate spread" ), XOR( "nospread" ) );
		nospread.AddShowCallback( callbacks::IsConfigNS );
		RegisterElement( &nospread, 1 );

		norecoil.setup( XOR( "compensate recoil" ), XOR( "norecoil" ) );
		RegisterElement( &norecoil, 1 );

		hitchance.setup( XOR( "hitchance" ), XOR( "hitchance" ) );
		hitchance.AddShowCallback( callbacks::IsConfigMM );
		RegisterElement( &hitchance, 1 );

		hitchance_amount.setup( "", XOR( "hitchance_amount" ), 1.f, 100.f, false, 0, 50.f, 1.f, XOR( L"%" ) );
		hitchance_amount.AddShowCallback( callbacks::IsConfigMM );
		hitchance_amount.AddShowCallback( callbacks::IsHitchanceOn );
		RegisterElement( &hitchance_amount, 1 );

		lagfix.setup( XOR( "predict fake-lag" ), XOR( "lagfix" ) );
		RegisterElement( &lagfix, 1 );

		correct.setup( XOR( "correct anti-aim" ), XOR( "correct" ) );
		RegisterElement( &correct, 1 );

		baim1.setup( XOR( "prefer body aim" ), XOR( "baim1" ), { XOR( "always" ), XOR( "lethal" ), XOR( "lethal x2" ), XOR( "fake" ), XOR( "in air" ) } );
		RegisterElement( &baim1, 1 );

		baim2.setup( XOR( "only body aim" ), XOR( "baim2" ), { XOR( "always" ), XOR( "health" ), XOR( "fake" ), XOR( "in air" ) } );
		RegisterElement( &baim2, 1 );

		baim_hp.setup( "", XOR( "baim_hp" ), 1.f, 50.f, false, 0, 20.f, 1.f, XOR( L"hp" ) );
		baim_hp.AddShowCallback( callbacks::IsBaimHealth );
		RegisterElement( &baim_hp, 1 );

		baim_key.setup( XOR( "body aim on key" ), XOR( "body aim on key" ) );
		RegisterElement( &baim_key, 1 );
	}
};

class AntiAimTab : public Tab {
public:
	// col 1.
	Checkbox rot_enable;
	Dropdown	jitter_type;
	Slider rage_aa_custom_fake;
	Keybind rage_aa_negate_key;
	Checkbox rage_aa_real_around_fake_standing;
	Checkbox rage_aa_real_around_fake_moving;
	Checkbox rage_aa_real_around_fake_slow_motion;
	Checkbox rage_aa_real_around_fake_air;
	Checkbox rage_aa_real_around_fake_jitter;
	Dropdown yaw_direction;
	Dropdown yaw_base;
	Checkbox enable;
	Checkbox edge;
	Dropdown mode;
	Keybind fakeduck;
	Dropdown pitch_stand;
	Dropdown yaw_stand;
	Slider   jitter_range_stand;
	Slider   rot_range_stand;
	Slider   rot_speed_stand;
	Slider   rand_update_stand;
	Slider rot_range_update;
	Dropdown dir_stand;
	Slider   dir_time_stand;
	Slider   dir_custom_stand;
	Checkbox dir_lock;
	Dropdown base_angle_stand;
	Dropdown body_fake_stand;

	Dropdown pitch_walk;
	Dropdown yaw_walk;
	Slider   jitter_range_walk;
	Slider   rot_range_walk;
	Slider   rot_speed_walk;
	Slider   rand_update_walk;
	Dropdown dir_walk;
	Slider	 dir_time_walk;
	Slider   dir_custom_walk;
	Dropdown base_angle_walk;

	Dropdown pitch_air;
	Dropdown yaw_air;
	Slider   jitter_range_air;
	Slider   rot_range_air;
	Slider   rot_speed_air;
	Slider   rand_update_air;
	Dropdown dir_air;
	Slider   dir_time_air;
	Slider   dir_custom_air;
	Dropdown base_angle_air;
	Dropdown body_fake_air;

	// col 2.
	Dropdown fake_yaw;
	Slider	 fake_relative;
	Slider	 fake_jitter_range;

	Checkbox      lag_enable;
	MultiDropdown lag_active;
	Dropdown      lag_mode;
	Slider        lag_limit;
	Checkbox      lag_land;

public:
	void init( ) {
		SetTitle( XOR( "anti-aim" ) );

		enable.setup( XOR( "enable" ), XOR( "enable" ) );
		RegisterElement( &enable );

		// stand.
		pitch_stand.setup( XOR( "pitch" ), XOR( "pitch_stnd" ), { XOR( "off" ), XOR( "up" ), XOR( "down" ), XOR( "random" ), XOR( "ideal" ) } );
		pitch_stand.AddShowCallback( callbacks::IsAntiAimModeStand );
		RegisterElement( &pitch_stand );



		yaw_base.setup(XOR("static angle"), XOR("yaw_base"), { XOR("off"), XOR("away crosshair"), XOR("away distance") });
		RegisterElement(&yaw_base);

		yaw_stand.setup( XOR( "yaw" ), XOR( "yaw_stnd" ), { XOR( "off" ), XOR( "direction" ), XOR( "random" ) } );
		RegisterElement( &yaw_stand );


		yaw_direction.setup(XOR("direction"), XOR("yaw_stnd"), { XOR("backwards"), XOR("left"), XOR("right") });
		RegisterElement(&yaw_direction);

		rot_range_update.setup(XOR("random update"), XOR("rand_update_stand"), 0.f, 1.f, false, 0, 0.f, 0.1f, XOR(L"?"));
		RegisterElement(&rot_range_update);


		jitter_type.setup(XOR("jitter type"), XOR("jitter_type"), { XOR("none"), XOR("offset"), XOR("random") });
		RegisterElement(&jitter_type);

		jitter_range_stand.setup(XOR("jitter range"), XOR("jitter_range_stand"), 0.f, 360.f, false, 0, 360.f, 5.f, XOR(L"?"));
		RegisterElement(&jitter_range_stand);


		rot_enable.setup(XOR("rotation enable"), XOR("rot_enable"));
		RegisterElement(&rot_enable);

		rot_range_stand.setup(XOR("rotation range"), XOR("rot_range_stand"), 0.f, 360.f, false, 0, 360.f, 5.f, XOR(L"?"));
		RegisterElement(&rot_range_stand);

		rot_speed_stand.setup(XOR("rotation speed"), XOR("rot_speed_stand"), 0.f, 100.f, false, 0, 50.f, 5.f, XOR(L"?"));
		RegisterElement(&rot_speed_stand);



		// col2.
		


		rage_aa_real_around_fake_standing.setup(XOR("real around fake standing"), XOR("rage_aa_real_around_fake_standing"));
		RegisterElement(&rage_aa_real_around_fake_standing, 1);
		rage_aa_real_around_fake_moving.setup(XOR("real around fake moving"), XOR("rage_aa_real_around_fake_moving"));
		RegisterElement(&rage_aa_real_around_fake_moving, 1);
		rage_aa_real_around_fake_air.setup(XOR("real around fake air"), XOR("rage_aa_real_around_fake_air"));
		RegisterElement(&rage_aa_real_around_fake_air, 1);
		rage_aa_real_around_fake_slow_motion.setup(XOR("real around fake slow"), XOR("rage_aa_real_around_fake_slow_motion"));
		RegisterElement(&rage_aa_real_around_fake_slow_motion, 1);
		rage_aa_real_around_fake_jitter.setup(XOR("real around fake jitter"), XOR("rage_aa_real_around_fake_jitter"));
		RegisterElement(&rage_aa_real_around_fake_jitter, 1);

		
		rage_aa_custom_fake.setup(XOR("fake amount"), XOR( "rage_aa_custom_fake" ), -60.f, 60.f, false, 0, 0.f, 5.f, XOR( L"?" ) );
		RegisterElement( &rage_aa_custom_fake, 1 );

		
		rage_aa_negate_key.setup(XOR("invert"), XOR("rage_aa_negate_key"));
		RegisterElement(&rage_aa_negate_key, 1);

		fakeduck.setup(XOR("fake duck"), XOR("fakeduck"));
		RegisterElement(&fakeduck, 1);


		// col 2.
		lag_enable.setup( XOR( "fake-lag" ), XOR( "lag_enable" ) );
		RegisterElement( &lag_enable, 1 );

		lag_active.setup( "", XOR( "lag_active" ), { XOR( "stand" ), XOR( "move" ), XOR("air"),  XOR( "crouch" ) }, false );
		RegisterElement( &lag_active, 1 );

		lag_mode.setup( "", XOR( "lag_mode" ), { XOR( "max" ), XOR( "break" ), XOR( "random" ), XOR( "break step" ) }, false );
		RegisterElement( &lag_mode, 1 );

		lag_limit.setup( XOR( "limit" ), XOR( "lag_limit" ), 2, 16, true, 0, 2, 1.f );
		RegisterElement( &lag_limit, 1 );

		lag_land.setup( XOR( "on land" ), XOR( "lag_land" ) );
		RegisterElement( &lag_land, 1 );
	}
};

class PlayersTab : public Tab {
public:
	MultiDropdown box;
	Colorpicker   box_enemy;
	Colorpicker   box_friendly;
	Checkbox      dormant;
	Checkbox      offscreen;
	Colorpicker   offscreen_color;
	MultiDropdown name;
	Colorpicker   name_color;
	MultiDropdown health;
	MultiDropdown flags_enemy;
	MultiDropdown flags_friendly;
	MultiDropdown weapon;
	Dropdown      weapon_mode;
	Checkbox      ammo;
	Colorpicker   ammo_color;		
	Checkbox      lby_update;
	Colorpicker   lby_update_color;
	Checkbox esp_chams_desync_enable;
	Colorpicker esp_chams_desync_color;
	// col2.
	MultiDropdown skeleton;
	Colorpicker   skeleton_enemy;
	Colorpicker   skeleton_friendly;

	MultiDropdown glow;
	Colorpicker   glow_enemy;
	Colorpicker   glow_friendly;
	Slider        glow_blend;
	MultiDropdown chams_enemy;
	Colorpicker   chams_enemy_vis;
	Colorpicker   chams_enemy_invis;
	Slider        chams_enemy_blend;
	Checkbox      chams_enemy_history;
	Colorpicker   chams_enemy_history_col;
	Slider        chams_enemy_history_blend;
	MultiDropdown chams_friendly;
	Colorpicker   chams_friendly_vis;
	Colorpicker   chams_friendly_invis;
	Slider        chams_friendly_blend;
	Checkbox      chams_local;
	Colorpicker   chams_local_col;
	Slider        chams_local_blend;
	Checkbox      chams_local_scope;

public:
	void init( ) {
		SetTitle( XOR( "players" ) );

		box.setup( XOR( "boxes" ), XOR( "box" ), { XOR( "enemy" ), XOR( "friendly" ) } );
		RegisterElement( &box );

		box_enemy.setup( XOR( "box enemy color" ), XOR( "box_enemy" ), { 150, 200, 60 } );
		RegisterElement( &box_enemy );

		box_friendly.setup( XOR( "box friendly color" ), XOR( "box_friendly" ), { 255, 200, 0 } );
		RegisterElement( &box_friendly );

		dormant.setup( XOR( "dormant enemies" ), XOR( "dormant" ) );
		RegisterElement( &dormant );

		offscreen.setup( XOR( "enemy offscreen esp" ), XOR( "offscreen" ) );
		RegisterElement( &offscreen );

		offscreen_color.setup( XOR( "offscreen esp color" ), XOR( "offscreen_color" ), colors::white );
		RegisterElement( &offscreen_color );

		name.setup( XOR( "name" ), XOR( "name" ), { XOR( "enemy" ), XOR( "friendly" ) } );
		RegisterElement( &name );

		name_color.setup( XOR( "name color" ), XOR( "name_color" ), colors::white );
		RegisterElement( &name_color );

		health.setup( XOR( "health" ), XOR( "health" ), { XOR( "enemy" ), XOR( "friendly" ) } );
		RegisterElement( &health );

		flags_enemy.setup( XOR( "flags enemy" ), XOR( "flags_enemy" ), { XOR( "money" ), XOR( "armor" ), XOR( "scoped" ), XOR( "flashed" ), XOR( "reloading" ), XOR( "bomb" ) } );
		RegisterElement( &flags_enemy );

		flags_friendly.setup( XOR( "flags friendly" ), XOR( "flags_friendly" ), { XOR( "money" ), XOR( "armor" ), XOR( "scoped" ), XOR( "flashed" ), XOR( "reloading" ), XOR( "bomb" ) } );
		RegisterElement( &flags_friendly );

		weapon.setup( XOR( "weapon" ), XOR( "weapon" ), { XOR( "enemy" ), XOR( "friendly" ) } );
		RegisterElement( &weapon );

		weapon_mode.setup( "", XOR( "weapon_mode" ), { XOR( "text" ), XOR( "icon" ) }, false );
		RegisterElement( &weapon_mode );

		ammo.setup( XOR( "ammo" ), XOR( "ammo" ) );
		RegisterElement( &ammo );

		ammo_color.setup( XOR( "color" ), XOR( "ammo_color" ), colors::burgundy );
		RegisterElement( &ammo_color );	
		
		lby_update.setup( XOR( "lby update" ), XOR( "lby_update" ) );
		RegisterElement( &lby_update );

		lby_update_color.setup( XOR( "color" ), XOR( "lby_update_color" ), colors::orange );
		RegisterElement( &lby_update_color );

		// col2.
		skeleton.setup( XOR( "skeleton" ), XOR( "skeleton" ), { XOR( "enemy" ), XOR( "friendly" ) } );
		RegisterElement( &skeleton, 1 );

		skeleton_enemy.setup( XOR( "enemy color" ), XOR( "skeleton_enemy" ), { 255, 255, 255 } );
		RegisterElement( &skeleton_enemy, 1 );

		skeleton_friendly.setup( XOR( "friendly color" ), XOR( "skeleton_friendly" ), { 255, 255, 255 } );
		RegisterElement( &skeleton_friendly, 1 );

		glow.setup( XOR( "glow" ), XOR( "glow" ), { XOR( "enemy" ), XOR( "friendly" ) } );
		RegisterElement( &glow, 1 );

		glow_enemy.setup( XOR( "enemy color" ), XOR( "glow_enemy" ), { 150, 200, 60 } );
		RegisterElement( &glow_enemy, 1 );

		glow_friendly.setup( XOR( "friendly color" ), XOR( "glow_friendly" ), { 150, 200, 60 } );
		RegisterElement( &glow_friendly, 1 );

		glow_blend.setup( "", XOR( "glow_blend" ), 10.f, 100.f, false, 0, 60.f, 1.f, XOR( L"%" ) );
		RegisterElement( &glow_blend, 1 );

		chams_enemy.setup( XOR( "chams enemy" ), XOR( "chams_enemy" ), { XOR( "visible" ), XOR( "invisible" ) } );
		RegisterElement( &chams_enemy, 1 );

		chams_enemy_vis.setup( XOR( "color visible" ), XOR( "chams_enemy_vis" ), { 150, 200, 60 } );
		RegisterElement( &chams_enemy_vis, 1 );

		chams_enemy_invis.setup( XOR( "color invisible" ), XOR( "chams_enemy_invis" ), { 60, 180, 225 } );
		RegisterElement( &chams_enemy_invis, 1 );

		chams_enemy_blend.setup( "", XOR( "chams_enemy_blend" ), 10.f, 100.f, false, 0, 100.f, 1.f, XOR( L"%" ) );
		RegisterElement( &chams_enemy_blend, 1 );

		chams_enemy_history.setup( XOR( "chams history" ), XOR( "chams_history" ) );
		RegisterElement( &chams_enemy_history, 1 );

		chams_enemy_history_col.setup( XOR( "color" ), XOR( "chams_history_col" ), { 255, 255, 200 } );
		RegisterElement( &chams_enemy_history_col, 1 );

		chams_enemy_history_blend.setup( "", XOR( "chams_history_blend" ), 10.f, 100.f, false, 0, 100.f, 1.f, XOR( L"%" ) );
		RegisterElement( &chams_enemy_history_blend, 1 );

		chams_friendly.setup( XOR( "chams friendly" ), XOR( "chams_friendly" ), { XOR( "visible" ), XOR( "invisible" ) } );
		RegisterElement( &chams_friendly, 1 );

		chams_friendly_vis.setup( XOR( "color visible" ), XOR( "chams_friendly_vis" ), { 255, 200, 0 } );
		RegisterElement( &chams_friendly_vis, 1 );

		chams_friendly_invis.setup( XOR( "color invisible" ), XOR( "chams_friendly_invis" ), { 255, 50, 0 } );
		RegisterElement( &chams_friendly_invis, 1 );

		chams_friendly_blend.setup( "", XOR( "chams_friendly_blend" ), 10.f, 100.f, false, 0, 100.f, 1.f, XOR( L"%" ) );
		RegisterElement( &chams_friendly_blend, 1 );

		chams_local.setup( XOR( "chams local" ), XOR( "chams_local" ) );
		RegisterElement( &chams_local, 1 );

		chams_local_col.setup( XOR( "color" ), XOR( "chams_local_col" ), { 255, 255, 200 } );
		RegisterElement( &chams_local_col, 1 );



		chams_local_blend.setup( "", XOR( "chams_local_blend" ), 10.f, 100.f, false, 0, 100.f, 1.f, XOR( L"%" ) );
		RegisterElement( &chams_local_blend, 1 );

		chams_local_scope.setup( XOR( "blend when scoped" ), XOR( "chams_local_scope" ) );
		RegisterElement( &chams_local_scope, 1 );
	}
};

class VisualsTab : public Tab {
public:
	Checkbox      items;
	Checkbox      ammo;
	Colorpicker   item_color;
	Checkbox      proj;
	Colorpicker   proj_color;
	MultiDropdown proj_range;
	Colorpicker   proj_range_color;
	MultiDropdown planted_c4;
	Checkbox      disableteam;
	Dropdown	  world;
	Checkbox      transparent_props;
	Checkbox      enemy_radar;
	
	// col2.
	Checkbox      novisrecoil;
	Checkbox      nosmoke;
	Checkbox      nofog;
	Checkbox      noflash;
	Checkbox      noscope;
	Checkbox      fov;
	Slider        fov_amt;
	Checkbox      fov_scoped;
	Checkbox      viewmodel_fov;
	Slider        viewmodel_fov_amt;
	Checkbox      spectators;
	Checkbox      force_xhair;
	Checkbox      spread_xhair;
	Colorpicker   spread_xhair_col;
	Slider        spread_xhair_blend;
	Checkbox      pen_crosshair;
	MultiDropdown indicators;
	Checkbox      tracers;
	Checkbox      impact_beams;
	Colorpicker   impact_beams_color;
	Colorpicker   impact_beams_hurt_color;
	Slider        impact_beams_time;
	Keybind       thirdperson;

public:
	void init( ) {
		SetTitle( XOR( "visuals" ) );

		items.setup( XOR( "dropped weapons" ), XOR( "items" ) );
		RegisterElement( &items );

		ammo.setup( XOR( "dropped weapons ammo" ), XOR( "ammo" ) );
		RegisterElement( &ammo );

		item_color.setup( XOR( "color" ), XOR( "item_color" ), colors::white );
		RegisterElement( &item_color );

		proj.setup( XOR( "projectiles" ), XOR( "proj" ) );
		RegisterElement( &proj );

		proj_color.setup( XOR( "color" ), XOR( "proj_color" ), colors::white );
		RegisterElement( &proj_color );

		proj_range.setup( XOR( "projectile range" ), XOR( "proj_range" ), { XOR( "frag" ), XOR( "molly" ) } );
		RegisterElement( &proj_range );

		proj_range_color.setup( XOR( "color" ), XOR( "proj_range_color" ), colors::burgundy );
		RegisterElement( &proj_range_color );

		planted_c4.setup( XOR( "planted c4" ), XOR( "planted_c4" ), { XOR( "on screen (2D)" ), XOR( "on bomb (3D)" ) } );
		RegisterElement( &planted_c4 );

		disableteam.setup( XOR( "do not render teammates" ), XOR( "disableteam" ) );
		RegisterElement( &disableteam );

		world.setup( XOR( "world" ), XOR( "world" ), { XOR( "off" ), XOR( "night" ), XOR( "fullbright" ) } );
		world.SetCallback( Visuals::ModulateWorld );
		RegisterElement( &world );

		transparent_props.setup( XOR( "transparent props" ), XOR( "transparent_props" ) );
		transparent_props.SetCallback( Visuals::ModulateWorld );
		RegisterElement( &transparent_props );

		enemy_radar.setup( XOR( "force enemies on radar" ), XOR( "enemy_radar" ) );
		RegisterElement( &enemy_radar );

		// col2.
		novisrecoil.setup( XOR( "remove visual recoil" ), XOR( "novisrecoil" ) );
		RegisterElement( &novisrecoil, 1 );

		nosmoke.setup( XOR( "remove smoke grenades" ), XOR( "nosmoke" ) );
		RegisterElement( &nosmoke, 1 );

		nofog.setup( XOR( "remove fog" ), XOR( "nofog" ) );
		RegisterElement( &nofog, 1 );

		noflash.setup( XOR( "remove flashbangs" ), XOR( "noflash" ) );
		RegisterElement( &noflash, 1 );

		noscope.setup( XOR( "remove scope" ), XOR( "noscope" ) );
		RegisterElement( &noscope, 1 );

		fov.setup( XOR( "override fov" ), XOR( "fov" ) );
		RegisterElement( &fov, 1 );

		fov_amt.setup( "", XOR( "fov_amt" ), 60.f, 140.f, false, 0, 90.f, 1.f, XOR( L"?" ) );
		RegisterElement( &fov_amt, 1 );

		fov_scoped.setup( XOR( "override fov when scoped" ), XOR( "fov_scoped" ) );
		RegisterElement( &fov_scoped, 1 );

		viewmodel_fov.setup( XOR( "override viewmodel fov" ), XOR( "viewmodel_fov" ) );
		RegisterElement( &viewmodel_fov, 1 );

		viewmodel_fov_amt.setup( "", XOR( "viewmodel_fov_amt" ), 60.f, 140.f, false, 0, 90.f, 1.f, XOR( L"?" ) );
		RegisterElement( &viewmodel_fov_amt, 1 );

		spectators.setup( XOR( "show spectator list" ), XOR( "spectators" ) );
		RegisterElement( &spectators, 1 );

		force_xhair.setup( XOR( "force crosshair" ), XOR( "force_xhair" ) );
		RegisterElement( &force_xhair, 1 );

		spread_xhair.setup( XOR( "visualize spread" ), XOR( "spread_xhair" ) );
		RegisterElement( &spread_xhair, 1 );

		spread_xhair_col.setup( XOR( "visualize spread color" ), XOR( "spread_xhair_col" ), colors::burgundy );
		RegisterElement( &spread_xhair_col, 1 );

		spread_xhair_blend.setup( "", XOR( "spread_xhair_blend" ), 10.f, 100.f, false, 0, 100.f, 1.f, XOR( L"%" ) );
		RegisterElement( &spread_xhair_blend, 1 );

		pen_crosshair.setup( XOR( "penetration crosshair" ), XOR( "pen_xhair" ) );
		RegisterElement( &pen_crosshair, 1 );

		indicators.setup( XOR( "indicators" ), XOR( "indicators" ), { XOR( "lby" ), XOR( "lag compensation" ), XOR("fake latency") } );
		RegisterElement( &indicators, 1 );

		tracers.setup( XOR( "grenade simulation" ), XOR( "tracers" ) );
		RegisterElement( &tracers, 1 );

		impact_beams.setup( XOR( "impact beams" ), XOR( "impact_beams" ) );
		RegisterElement( &impact_beams, 1 );

		impact_beams_color.setup( XOR( "impact beams color" ), XOR( "impact_beams_color" ), colors::light_blue );
		RegisterElement( &impact_beams_color, 1 );

		impact_beams_hurt_color.setup( XOR( "impact beams hurt color" ), XOR( "impact_beams_hurt_color" ), colors::red );
		RegisterElement( &impact_beams_hurt_color, 1 );

		impact_beams_time.setup( XOR( "impact beams time" ), XOR( "impact_beams_time" ), 1.f, 10.f, true, 0, 1.f, 1.f, XOR( L"s" ) );
		RegisterElement( &impact_beams_time, 1 );

		thirdperson.setup( XOR( "thirdperson" ), XOR( "thirdperson" ) );
		thirdperson.SetToggleCallback( callbacks::ToggleThirdPerson );
		RegisterElement( &thirdperson, 1 );
	}
};

class MovementTab : public Tab {
public:
	Checkbox bhop;
	Checkbox airduck;
	Checkbox autostrafe;
	Keybind  cstrafe;
	Keybind  astrafe;
	Keybind  zstrafe;
	Slider   z_freq;
	Slider   z_dist;

	Keybind  fakewalk;
	Keybind  autopeek;
	Keybind  autostop;
	Checkbox autostop_always_on;

public:
	void init( ) {
		SetTitle( XOR( "movement" ) );

		bhop.setup( XOR( "automatic jump" ), XOR( "bhop" ) );
		RegisterElement( &bhop );

		airduck.setup( XOR( "duck in air" ), XOR( "airduck" ) );
		RegisterElement( &airduck );

		autostrafe.setup( XOR( "automatic strafe" ), XOR( "autostrafe" ) );
		RegisterElement( &autostrafe );

		cstrafe.setup( XOR( "c-strafe" ), XOR( "cstrafe" ) );
		RegisterElement( &cstrafe );

		astrafe.setup( XOR( "a-strafe" ), XOR( "astrafe" ) );
		RegisterElement( &astrafe );

		zstrafe.setup( XOR( "z-strafe" ), XOR( "zstrafe" ) );
		RegisterElement( &zstrafe );

		z_freq.setup( "", XOR( "z_freq" ), 1.f, 100.f, false, 0, 50.f, 0.5f, XOR( L"hz" ) );
		RegisterElement( &z_freq );

		z_dist.setup( "", XOR( "z_dist" ), 1.f, 100.f, false, 0, 20.f, 0.5f, XOR( L"%" ) );
		RegisterElement( &z_dist );

		fakewalk.setup( XOR( "slowmotion" ), XOR( "fakewalk" ) );
		RegisterElement( &fakewalk, 1 );

		//GUI::Controls::Hotkey(XOR("Slow motion"), XOR("movement_fakewalk_key"));

		//GUI::Controls::Label(XOR("Fake duck"));
		//GUI::Controls::Hotkey(XOR("Fake duck key"), XOR("rage_aa_fakeduck_key"));

		autopeek.setup( XOR( "automatic peek" ), XOR( "autopeek" ) );
		RegisterElement( &autopeek, 1 );

		autostop_always_on.setup( XOR( "automatic stop always on" ), XOR( "auto_stop_always" ) );
		RegisterElement( &autostop_always_on, 1 );

		autostop.setup( XOR( "automatic stop" ), XOR( "autostop" ) );
		autostop.AddShowCallback( callbacks::AUTO_STOP );
		RegisterElement( &autostop, 1 );
	}
};

class SkinsTab : public Tab {
public:
	Checkbox enable;
	Checkbox activate_inventory;
	Edit     id_deagle;
	Checkbox stattrak_deagle;
	Slider   quality_deagle;
	Slider	 seed_deagle;

	Edit     id_elite;
	Checkbox stattrak_elite;
	Slider   quality_elite;
	Slider	 seed_elite;

	Edit     id_fiveseven;
	Checkbox stattrak_fiveseven;
	Slider   quality_fiveseven;
	Slider	 seed_fiveseven;

	Edit     id_glock;
	Checkbox stattrak_glock;
	Slider   quality_glock;
	Slider	 seed_glock;

	Edit     id_ak47;
	Checkbox stattrak_ak47;
	Slider   quality_ak47;
	Slider	 seed_ak47;

	Edit     id_aug;
	Checkbox stattrak_aug;
	Slider   quality_aug;
	Slider	 seed_aug;

	Edit     id_awp;
	Checkbox stattrak_awp;
	Slider   quality_awp;
	Slider	 seed_awp;

	Edit     id_famas;
	Checkbox stattrak_famas;
	Slider   quality_famas;
	Slider	 seed_famas;

	Edit     id_g3sg1;
	Checkbox stattrak_g3sg1;
	Slider   quality_g3sg1;
	Slider	 seed_g3sg1;

	Edit     id_galil;
	Checkbox stattrak_galil;
	Slider   quality_galil;
	Slider	 seed_galil;

	Edit     id_m249;
	Checkbox stattrak_m249;
	Slider   quality_m249;
	Slider	 seed_m249;

	Edit     id_m4a4;
	Checkbox stattrak_m4a4;
	Slider   quality_m4a4;
	Slider	 seed_m4a4;

	Edit     id_mac10;
	Checkbox stattrak_mac10;
	Slider   quality_mac10;
	Slider	 seed_mac10;

	Edit     id_p90;
	Checkbox stattrak_p90;
	Slider   quality_p90;
	Slider	 seed_p90;

	Edit     id_ump45;
	Checkbox stattrak_ump45;
	Slider   quality_ump45;
	Slider	 seed_ump45;

	Edit     id_xm1014;
	Checkbox stattrak_xm1014;
	Slider   quality_xm1014;
	Slider	 seed_xm1014;

	Edit     id_bizon;
	Checkbox stattrak_bizon;
	Slider   quality_bizon;
	Slider	 seed_bizon;

	Edit     id_mag7;
	Checkbox stattrak_mag7;
	Slider   quality_mag7;
	Slider	 seed_mag7;

	Edit     id_negev;
	Checkbox stattrak_negev;
	Slider   quality_negev;
	Slider	 seed_negev;

	Edit     id_sawedoff;
	Checkbox stattrak_sawedoff;
	Slider   quality_sawedoff;
	Slider	 seed_sawedoff;

	Edit     id_tec9;
	Checkbox stattrak_tec9;
	Slider   quality_tec9;
	Slider	 seed_tec9;

	Edit     id_p2000;
	Checkbox stattrak_p2000;
	Slider   quality_p2000;
	Slider	 seed_p2000;

	Edit     id_mp7;
	Checkbox stattrak_mp7;
	Slider   quality_mp7;
	Slider	 seed_mp7;

	Edit     id_mp9;
	Checkbox stattrak_mp9;
	Slider   quality_mp9;
	Slider	 seed_mp9;

	Edit     id_nova;
	Checkbox stattrak_nova;
	Slider   quality_nova;
	Slider	 seed_nova;

	Edit     id_p250;
	Checkbox stattrak_p250;
	Slider   quality_p250;
	Slider	 seed_p250;

	Edit     id_scar20;
	Checkbox stattrak_scar20;
	Slider   quality_scar20;
	Slider	 seed_scar20;

	Edit     id_sg553;
	Checkbox stattrak_sg553;
	Slider   quality_sg553;
	Slider	 seed_sg553;

	Edit     id_ssg08;
	Checkbox stattrak_ssg08;
	Slider   quality_ssg08;
	Slider	 seed_ssg08;

	Edit     id_m4a1s;
	Checkbox stattrak_m4a1s;
	Slider   quality_m4a1s;
	Slider	 seed_m4a1s;

	Edit     id_usps;
	Checkbox stattrak_usps;
	Slider   quality_usps;
	Slider	 seed_usps;

	Edit     id_cz75a;
	Checkbox stattrak_cz75a;
	Slider   quality_cz75a;
	Slider	 seed_cz75a;

	Edit     id_revolver;
	Checkbox stattrak_revolver;
	Slider   quality_revolver;
	Slider	 seed_revolver;

	Edit     id_bayonet;
	Checkbox stattrak_bayonet;
	Slider   quality_bayonet;
	Slider	 seed_bayonet;

	Edit     id_flip;
	Checkbox stattrak_flip;
	Slider   quality_flip;
	Slider	 seed_flip;

	Edit     id_gut;
	Checkbox stattrak_gut;
	Slider   quality_gut;
	Slider	 seed_gut;

	Edit     id_karambit;
	Checkbox stattrak_karambit;
	Slider   quality_karambit;
	Slider	 seed_karambit;

	Edit     id_m9;
	Checkbox stattrak_m9;
	Slider   quality_m9;
	Slider	 seed_m9;

	Edit     id_huntsman;
	Checkbox stattrak_huntsman;
	Slider   quality_huntsman;
	Slider	 seed_huntsman;

	Edit     id_falchion;
	Checkbox stattrak_falchion;
	Slider   quality_falchion;
	Slider	 seed_falchion;

	Edit     id_bowie;
	Checkbox stattrak_bowie;
	Slider   quality_bowie;
	Slider	 seed_bowie;

	Edit     id_butterfly;
	Checkbox stattrak_butterfly;
	Slider   quality_butterfly;
	Slider	 seed_butterfly;

	Edit     id_daggers;
	Checkbox stattrak_daggers;
	Slider   quality_daggers;
	Slider	 seed_daggers;

	// col 2.
	Dropdown knife;
	Dropdown glove;
	Edit	 glove_id;

public:
	void init( ) {
		SetTitle( XOR( "inventory and skins" ) );

		activate_inventory.setup( XOR( "show inventory window" ), XOR( "activate_inventory" ) );
		RegisterElement( &activate_inventory);

		
	}
};

class MiscTab : public Tab {
public:
	// col1.
	MultiDropdown buy1;
	MultiDropdown buy2;
	MultiDropdown buy3;
	MultiDropdown notifications;
	Keybind       last_tick_defuse;
	Keybind       fake_latency;
	Slider		  fake_latency_amt;

	// col2.
	Checkbox autoaccept;
	Checkbox unlock;
	Checkbox hitmarker;
	Checkbox ragdoll_force;
	Checkbox killfeed;
	Checkbox ranks;

public:
	void init( ) {
		SetTitle( XOR( "misc" ) );

		buy1.setup( XOR( "auto buy items" ), XOR( "auto_buy1" ),
			{
				XOR( "galilar" ),
				XOR( "famas" ),
				XOR( "ak47" ),
				XOR( "m4a1" ),
				XOR( "m4a1_silencer" ),
				XOR( "ssg08" ),
				XOR( "aug" ),
				XOR( "sg556" ),
				XOR( "awp" ),
				XOR( "scar20" ),
				XOR( "g3sg1" ),
				XOR( "nova" ),
				XOR( "xm1014" ),
				XOR( "mag7" ),
				XOR( "m249" ),
				XOR( "negev" ),
				XOR( "mac10" ),
				XOR( "mp9" ),
				XOR( "mp7" ),
				XOR( "ump45" ),
				XOR( "p90" ),
				XOR( "bizon" ),
			} );
		RegisterElement( &buy1 );

		buy2.setup( "", XOR( "auto_buy2" ),
			{
				XOR( "glock" ),
				XOR( "hkp2000" ),
				XOR( "usp_silencer" ),
				XOR( "elite" ),
				XOR( "p250" ),
				XOR( "tec9" ),
				XOR( "fn57" ),
				XOR( "deagle" ),
			}, false );
		RegisterElement( &buy2 );

		buy3.setup( "", XOR( "auto_buy3" ),
			{
				XOR( "vest" ),
				XOR( "vesthelm" ),
				XOR( "taser" ),
				XOR( "defuser" ),
				XOR( "heavyarmor" ),
				XOR( "molotov" ),
				XOR( "incgrenade" ),
				XOR( "decoy" ),
				XOR( "flashbang" ),
				XOR( "hegrenade" ),
				XOR( "smokegrenade" ),
			}, false );
		RegisterElement( &buy3 );

		notifications.setup( XOR( "notifications" ), XOR( "notifications" ), { XOR( "matchmaking" ), XOR( "damage" ), XOR( "purchases" ), XOR( "bomb" ), XOR( "defuse" ) } );
		RegisterElement( &notifications );

		last_tick_defuse.setup( XOR( "last tick defuse" ), XOR( "last_tick_defuse" ) );
		RegisterElement( &last_tick_defuse );

		fake_latency.setup( XOR( "fake latency" ), XOR( "fake_latency" ) );
		fake_latency.SetToggleCallback( callbacks::ToggleFakeLatency );
		RegisterElement( &fake_latency );

		fake_latency_amt.setup( "", XOR( "fake_latency_amt" ), 50.f, 800.f, false, 0, 200.f, 50.f, XOR( L"ms" ) );
		RegisterElement( &fake_latency_amt );

		// col2.
		autoaccept.setup( XOR( "auto-accept matchmaking" ), XOR( "autoaccept" ) );
		RegisterElement( &autoaccept, 1 );

		unlock.setup( XOR( "unlock inventory in-game" ), XOR( "unlock_inventory" ) );
		RegisterElement( &unlock, 1 );

		hitmarker.setup( XOR( "hitmarker" ), XOR( "hitmarker" ) );
		RegisterElement( &hitmarker, 1 );

		ragdoll_force.setup( XOR( "ragdoll force" ), XOR( "ragdoll_force" ) );
		RegisterElement( &ragdoll_force, 1 );

		ranks.setup( XOR( "reveal matchmaking ranks" ), XOR( "ranks" ) );
		RegisterElement( &ranks, 1 );

		killfeed.setup( XOR( "preserve killfeed" ), XOR( "killfeed" ) );
		//killfeed.SetCallback( callbacks::ToggleKillfeed );
		RegisterElement( &killfeed, 1 );
	}
};

class ConfigTab : public Tab {
public:
	Colorpicker menu_color;

	Dropdown mode;
	Dropdown config;
	Keybind  key1;
	Keybind  key2;
	Keybind  key3;
	Keybind  key4;
	Keybind  key5;
	Keybind  key6;
	Button   save;
	Button   load;

public:

	void init( ) {
		SetTitle( XOR( "config" ) );

		menu_color.setup( XOR( "menu color" ), XOR( "menu_color" ), colors::burgundy, &g_gui.m_color );
		RegisterElement( &menu_color );

		mode.setup( XOR( "safety mode" ), XOR( "mode" ), { XOR( "matchmaking" ), XOR( "no-spread" ) } );
		RegisterElement( &mode, 1 );

		config.setup( XOR( "configuration" ), XOR( "cfg" ), { XOR( "1" ), XOR( "2" ), XOR( "3" ), XOR( "4" ), XOR( "5" ), XOR( "6" ) } );
		config.RemoveFlags( ElementFlags::SAVE );
		RegisterElement( &config, 1 );

		key1.setup( XOR( "configuration key 1" ), XOR( "cfg_key1" ) );
		key1.RemoveFlags( ElementFlags::SAVE );
		key1.SetCallback( callbacks::SaveHotkeys );
		key1.AddShowCallback( callbacks::IsConfig1 );
		key1.SetToggleCallback( callbacks::ConfigLoad1 );
		RegisterElement( &key1, 1 );

		key2.setup( XOR( "configuration key 2" ), XOR( "cfg_key2" ) );
		key2.RemoveFlags( ElementFlags::SAVE );
		key2.SetCallback( callbacks::SaveHotkeys );
		key2.AddShowCallback( callbacks::IsConfig2 );
		key2.SetToggleCallback( callbacks::ConfigLoad2 );
		RegisterElement( &key2, 1 );

		key3.setup( XOR( "configuration key 3" ), XOR( "cfg_key3" ) );
		key3.RemoveFlags( ElementFlags::SAVE );
		key3.SetCallback( callbacks::SaveHotkeys );
		key3.AddShowCallback( callbacks::IsConfig3 );
		key3.SetToggleCallback( callbacks::ConfigLoad3 );
		RegisterElement( &key3, 1 );

		key4.setup( XOR( "configuration key 4" ), XOR( "cfg_key4" ) );
		key4.RemoveFlags( ElementFlags::SAVE );
		key4.SetCallback( callbacks::SaveHotkeys );
		key4.AddShowCallback( callbacks::IsConfig4 );
		key4.SetToggleCallback( callbacks::ConfigLoad4 );
		RegisterElement( &key4, 1 );

		key5.setup( XOR( "configuration key 5" ), XOR( "cfg_key5" ) );
		key5.RemoveFlags( ElementFlags::SAVE );
		key5.SetCallback( callbacks::SaveHotkeys );
		key5.AddShowCallback( callbacks::IsConfig5 );
		key5.SetToggleCallback( callbacks::ConfigLoad5 );
		RegisterElement( &key5, 1 );

		key6.setup( XOR( "configuration key 6" ), XOR( "cfg_key6" ) );
		key6.RemoveFlags( ElementFlags::SAVE );
		key6.SetCallback( callbacks::SaveHotkeys );
		key6.AddShowCallback( callbacks::IsConfig6 );
		key6.SetToggleCallback( callbacks::ConfigLoad6 );
		RegisterElement( &key6, 1 );

		save.setup( XOR( "save" ) );
		save.SetCallback( callbacks::ConfigSave );
		RegisterElement( &save, 1 );

		load.setup( XOR( "load" ) );
		load.SetCallback( callbacks::ConfigLoad );
		RegisterElement( &load, 1 );
	}
};

class MainForm : public Form {
public:
	// aimbot.
	AimbotTab    aimbot;
	AntiAimTab   antiaim;

	// visuals.
	PlayersTab	 players;
	VisualsTab	 visuals;

	// misc.
	MovementTab  movement;
	SkinsTab     skins;
	MiscTab	     misc;
	ConfigTab	 config;

public:
	void init( ) {
		SetPosition( 50, 50 );
		SetSize( 630, 500 );

		// aim.
		RegisterTab( &aimbot );
		aimbot.init( );

		RegisterTab( &antiaim );
		antiaim.init( );

		// visuals.
		RegisterTab( &players );
		players.init( );

		RegisterTab( &visuals );
		visuals.init( );

		// misc.
		RegisterTab( &movement );
		movement.init( );

		RegisterTab( &skins );
		skins.init( );

		RegisterTab( &misc );
		misc.init( );

		RegisterTab( &config );
		config.init( );
	}
};

class Menu {
public:
	MainForm main;

public:
	void init( ) {
		Colorpicker::init( );

		main.init( );
		g_gui.RegisterForm( &main, VK_INSERT );
	}
};

extern Menu g_menu;
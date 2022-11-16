#include "includes.h"

Bones g_bones{};;

bool Bones::setup( Player* player, BoneArray* out, LagRecord* record ) {
	// if the record isnt setup yet.
	if( !record->m_setup ) {
		// run setupbones rebuilt. 
		if( !BuildBones( player, 0x7FF00, record->m_bones, record ) )
			return false;
		//not real fix, you should fix above shit
		//if (!player->SetupBones(record->m_bones, 128, 0x7FF00, record->m_sim_time))
		//	return false;
		// we have setup this record bones.
		record->m_setup = true;
	}

	// record is setup.
	if( out && record->m_setup )
		std::memcpy( out, record->m_bones, sizeof( BoneArray ) * 128 );

	return true;
}

bool Bones::BuildBones( Player* target, int mask, BoneArray* out, LagRecord* record ) {
	//vec3_t		     pos[ 128 ];
	//quaternion_t     q[ 128 ];
	//vec3_t           backup_origin;
	//ang_t            backup_angles;
	//float            backup_poses[ 24 ];
	//C_AnimationLayer backup_layers[ 13 ];
	//
	//// get hdr.
	//CStudioHdr* hdr = target->GetModelPtr( );
	//if( !hdr )
	//	return false;
	//
	//// get ptr to bone accessor.
	//CBoneAccessor* accessor = &target->m_BoneAccessor( );
	//if( !accessor )
	//	return false;
	//
	//
	//// store origial output matrix.
	//// likely cachedbonedata.
	//BoneArray* backup_matrix = accessor->m_pBones;
	//if( !backup_matrix )
	//	return false;
	////0x260 = 0x268
	//// prevent the game from calling ShouldSkipAnimationFrame.
	//auto bSkipAnimationFrame = *reinterpret_cast< int* >( uintptr_t( target ) + 0x260);
	//*reinterpret_cast< int* >( uintptr_t( target ) + 0x260) = NULL;
	//
	//// backup original.
	//backup_origin  = target->GetAbsOrigin( );
	//backup_angles  = target->GetAbsAngles( );
	//target->GetPoseParameters( backup_poses );
	//target->GetAnimLayers( backup_layers );
	//
	//// compute transform from raw data.
	//matrix3x4_t transform;
	//math::AngleMatrix( record->m_abs_ang, record->m_pred_origin, transform );
	//
	//// set non interpolated data.
	//target->AddEffect( EF_NOINTERP );
	//target->SetAbsOrigin( record->m_pred_origin );
	//target->SetAbsAngles( record->m_abs_ang );
	//target->SetPoseParameters( record->m_poses );
	//target->SetAnimLayers( record->m_layers );
	//
	//// force game to call AccumulateLayers - pvs fix.
	//m_running = true;
	//
	//// set bone array for write.
	//accessor->m_pBones = out;
	//
	//// compute and build bones.
	//target->StandardBlendingRules( hdr, pos, q, record->m_pred_time, mask );
	//
	//uint8_t computed[ 0x100 ];
	//std::memset( computed, 0, 0x100 );
	//target->BuildTransformations( hdr, pos, q, transform, mask, computed );
	//
	//// restore old matrix.
	//accessor->m_pBones = backup_matrix;
	//
	//// restore original interpolated entity data.
	//target->SetAbsOrigin( backup_origin );
	//target->SetAbsAngles( backup_angles );
	//target->SetPoseParameters( backup_poses );
	//target->SetAnimLayers( backup_layers );
	//
	//// revert to old game behavior.
	//m_running = false;
	//
	//// allow the game to call ShouldSkipAnimationFrame.
	//*reinterpret_cast< int* >( uintptr_t( target ) + 0x260) = bSkipAnimationFrame;
	//
	//return true;










	vec3_t		     pos[128];
	quaternion_t     q[128];
	vec3_t           backup_origin;
	ang_t            backup_angles;
	float            backup_poses[24];
	C_AnimationLayer backup_layers[13];

	// get hdr.
	CStudioHdr* hdr = target->GetModelPtr();
	if (!hdr)
		return false;

	// get ptr to bone accessor.
	CBoneAccessor* accessor = &target->m_BoneAccessor();
	if (!accessor)
		return false;


	// store origial output matrix.
	// likely cachedbonedata.
	BoneArray* backup_matrix = accessor->m_pBones;
	if (!backup_matrix)
		return false;
	//0x260 = 0x268
	// prevent the game from calling ShouldSkipAnimationFrame.
	auto bSkipAnimationFrame = *reinterpret_cast<int*>(uintptr_t(target) + 0x260);
	*reinterpret_cast<int*>(uintptr_t(target) + 0x260) = NULL;

	// backup original.
	backup_origin = target->GetAbsOrigin();
	backup_angles = target->GetAbsAngles();
	target->GetPoseParameters(backup_poses);
	target->GetAnimLayers(backup_layers);

	// compute transform from raw data.
	matrix3x4_t transform;
	math::AngleMatrix(record->m_abs_ang, record->m_pred_origin, transform);

	// set non interpolated data.
	target->AddEffect(EF_NOINTERP);
	target->SetAbsOrigin(record->m_pred_origin);
	target->SetAbsAngles(record->m_abs_ang);
	target->SetPoseParameters(record->m_poses);
	target->SetAnimLayers(record->m_layers);

	// force game to call AccumulateLayers - pvs fix.
	m_running = true;

	// set bone array for write.
	accessor->m_pBones = out;

	// compute and build bones.
	// keep track of old occlusion values
	const auto backup_occlusion_flags = target->m_iOcclusionFlags();
	const auto backup_occlusion_framecount = target->m_iOcclusionFramecount();

	// skip occlusion checks in c_cs_player::setup_bones.
	if (!target->m_bIsLocalPlayer())
	{
		target->m_iOcclusionFlags() = 0;
		target->m_iOcclusionFramecount() = 0;
	}

	// clear bone accessor.
	target->GetReadableBones() = target->GetWriteableBones() = 0;

	// invalidate bone cache.
	target->m_iMostRecentModelBoneCounter() = 0;
	target->m_flLastBoneSetupTime() = -FLT_MAX;

	// stop interpolation.
	target->m_fEffects() |= 8;

	// build bones.
	target->SetupBones(nullptr, -1, 0x7FF00, record->m_sim_time);

	// restore original occlusion.
	if (!target->m_bIsLocalPlayer())
	{
		target->m_iOcclusionFlags() = backup_occlusion_flags;
		target->m_iOcclusionFramecount() = backup_occlusion_framecount;
	}

	// start interpolation again.
	target->m_fEffects() &= ~8;

	// save bones.
	const auto bone_matrix = target->GetBoneArrayForWrite();
	memcpy(accessor->m_pBones, bone_matrix, target->m_BoneCacheCount() * sizeof(float) * 12);


	// restore old matrix.
	accessor->m_pBones = backup_matrix;

	// restore original interpolated entity data.
	target->SetAbsOrigin(backup_origin);
	target->SetAbsAngles(backup_angles);
	target->SetPoseParameters(backup_poses);
	target->SetAnimLayers(backup_layers);

	// revert to old game behavior.
	m_running = false;

	// allow the game to call ShouldSkipAnimationFrame.
	*reinterpret_cast<int*>(uintptr_t(target) + 0x260) = bSkipAnimationFrame;

	return true;


}
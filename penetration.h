#pragma once

namespace penetration {
	struct PenetrationInput_t
	{
		Player* m_from;
		Player* m_target;
		vec3_t  m_pos;
		float	m_damage;
		float   m_damage_pen;
		bool	m_can_pen;
	};

	struct FireBulletData_t
	{
		vec3_t m_start;
		vec3_t m_end;
		vec3_t m_current_position;
		vec3_t m_direction;

		CTraceFilter* m_filter;
		CGameTrace m_enter_trace;

		float m_thickness;
		float m_current_damage;
		int m_penetration_count;
	};

	struct PenetrationOutput_t {
		Player* m_target;
		float   m_damage;
		int     m_hitgroup;
		bool    m_pen;

		__forceinline PenetrationOutput_t() : m_target{ nullptr }, m_damage{ 0.f }, m_hitgroup{ -1 }, m_pen{ false } {}
	};

	bool HandleBulletPenetration(WeaponInfo* info, FireBulletData_t& data, bool extracheck, vec3_t point);
	void TraceLine(vec3_t& start, vec3_t& end, unsigned int mask, Entity* ignore, CGameTrace* trace);
	bool CanHitFloatingPoint(const vec3_t& point, const vec3_t& source);

	float scale(Player* player, float damage, float armor_ratio, int hitgroup);
	bool  TraceToExit(const vec3_t& start, const vec3_t& dir, vec3_t& out, CGameTrace* enter_trace, CGameTrace* exit_trace);
	void  ClipTraceToPlayer(const vec3_t& start, const vec3_t& end, uint32_t mask, CGameTrace* tr, Player* player, float min);
	bool  run(PenetrationInput_t* in, PenetrationOutput_t* out);
}
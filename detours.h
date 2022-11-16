#pragma once

class Detours {
public:
	void init( );

public:
	static void __cdecl CL_Move(float accumulated_extra_samples, bool bFinalTick);
	decltype(&CL_Move) oCL_Move;
};

extern Detours g_detours;
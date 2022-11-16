#pragma once
#include <d3d9.h>
#include <d3dx9.h>

#include "../imgui.h"
#include "../imgui_freetype.h"

#pragma comment(lib, "freetype.lib")

#include "../imgui_impl_dx9.h"
#include "../imgui_impl_win32.h"

class c_menu  {
public:
	void initialize(IDirect3DDevice9* pDevice);
	void draw_begin();
	void draw();
	void draw_end();

	bool is_menu_initialized();
	bool is_menu_opened();
	void set_menu_opened(bool v);
	void cjec();
	ImColor get_accent_color();
	IDirect3DTexture9* skinImage = nullptr;
private:
	bool m_bInitialized;
	bool m_bIsOpened;
	int m_nCurrentTab;
	/* user-defined configuration filename in miscellaneous tab */
	std::string	szConfigFile = "";
	/* current selected configuration in miscellaneous tab */
	int	iSelectedConfig = 0;
};





extern c_menu skeet_menu;



class adding_menu
{
public:
	std::string link;
	std::string oldlink;
	IDirect3DTexture9* saveImage = nullptr;
	int het;
	void write(int, int);
};

extern adding_menu skin_menu;
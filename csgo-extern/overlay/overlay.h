#pragma once

#include "d3d.h"

#ifndef OVERLAY_NAME
#define OVERLAY_NAME "GAMERFOOD MEMES"
#endif // OVERLAY_NAME

typedef void( *OnFrameFn )( );

class Overlay
{
public:
	Overlay();
	~Overlay();

	bool					Attach( HWND hWnd );

	int						OnFrame();

	void					AddOnFrame( const OnFrameFn& pFunction );
	void					GetScreenSize( int* width, int* height );
	void					GetCenterScreen( int* x, int* y );

	IDirect3DDevice9*		GetDevice() const;

private:
	bool					InitDirectX();

private:
	static LRESULT WINAPI	WndProcedure( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

private:
	HWND					m_hWnd;
	HWND					m_hGame;
	int						m_nSize[ 2 ];

	IDirect3D9*				m_pDirect3D;
	IDirect3DDevice9*		m_pDevice;
	D3DPRESENT_PARAMETERS	m_Present;

	std::vector<OnFrameFn>	m_pOnFrameList;
};

extern Overlay* g_pOverlay;
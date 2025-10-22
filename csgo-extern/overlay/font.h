#pragma once

#include "color.h"

#undef DrawText

typedef unsigned long HFont;
extern HFont hFont;
extern HFont hFontMenuHeader;
extern HFont hFontMenu;



enum FontCreateFlag_t
{
	FONT_CREATE_NONE = 0,
	FONT_CREATE_BOLD = 1,
	FONT_CREATE_ITALIC = 2,
	FONT_CREATE_OUTLINE = 4,
	FONT_CREATE_SPRITE = 8,
	FONT_CREATE_ANTIALIASED = 16
};

class Font
{
public:
	Font( IDirect3DDevice9* pDevice, const std::string& pFontName, int nFontSize, DWORD dwFontFlags = FONT_CREATE_NONE );
	~Font();

	void				PreFrame();
	void				PostFrame();

	void				OnReset();

	void				DrawText( int x, int y, Color color, const char* text );
	void				GetTextSize( const char* text, int& width, int& height );
	int					GetFontSize() const;

private:
	IDirect3DDevice9*	m_pDevice;
	ID3DXFont*			m_pFont;
	ID3DXSprite*		m_pSprite;

	std::string			m_pFontName;
	int					m_nFontSize;
	DWORD				m_dwFontFlags;
};
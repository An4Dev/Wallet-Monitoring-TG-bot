#pragma once

#include "color.h"

typedef unsigned long HSprite;
extern HSprite hSprite1;
extern HSprite hSprite2;

class Sprite
{
public:
	Sprite( IDirect3DDevice9* pDevice, std::string file, int width, int height, Color transparentColorKey );
	~Sprite();

	void				PreFrame();
	void				PostFrame();

	void				OnReset();

	void				DrawSprite( int x, int y, float scale );

private:
	IDirect3DDevice9*	m_pDevice;
	ID3DXSprite*		m_pSprite;
	IDirect3DTexture9*  m_pTexture;

	std::string			m_TextureFile;
	int					m_nSpriteHeight;
	int					m_nSpriteWidth;
	Color				m_TransparentColorKey;
};
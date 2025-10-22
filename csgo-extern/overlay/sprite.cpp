#include "..\include.h"

HSprite hSprite1 = NULL;
HSprite hSprite2 = NULL;

Sprite::Sprite( IDirect3DDevice9* pDevice, std::string file, int width, int height, Color transparentColorKey )
{
	m_pDevice				= pDevice;
	m_pSprite				= nullptr;
	m_pTexture				= nullptr;
	m_TextureFile			= file;
	m_nSpriteWidth			= width;
	m_nSpriteHeight			= height;
	m_TransparentColorKey	= transparentColorKey;
}

Sprite::~Sprite()
{
	if( m_pSprite )
	{
		m_pSprite->Release();
		m_pSprite = nullptr;
	}

	if( m_pTexture )
	{
		m_pTexture->Release();
		m_pTexture = nullptr;
	}
}

void Sprite::PreFrame()
{
	if( !m_pDevice )
	{
		return;
	}

	if( !m_pSprite )
	{
		D3DXCreateSprite( m_pDevice, &m_pSprite );
	}

	if( m_pSprite )
	{
		m_pSprite->Begin( D3DXSPRITE_ALPHABLEND );
	}

	if( !m_pTexture )
	{
		D3DXCreateTextureFromFileEx( m_pDevice,
									 m_TextureFile.c_str(),
									 m_nSpriteWidth,
									 m_nSpriteHeight,
									 D3DX_DEFAULT,
									 NULL,
									 D3DFMT_A8R8G8B8,
									 D3DPOOL_MANAGED,
									 D3DX_DEFAULT,
									 D3DX_DEFAULT,
									 NULL,
									 NULL,
									 NULL,
									 &m_pTexture
									 );
	}
}

void Sprite::PostFrame()
{
	if( !m_pDevice )
	{
		return;
	}
	
	if( m_pSprite )
	{
		m_pSprite->End();
	}
}

void Sprite::OnReset()
{
	if( m_pSprite )
	{
		m_pSprite->OnLostDevice();
		m_pSprite->OnResetDevice();
	}
}

void Sprite::DrawSprite( int x, int y, float scale )
{
	if( !m_pDevice )
		return;

	if( !m_pTexture )
		return;

	D3DXVECTOR2 spriteCentre = D3DXVECTOR2( ( m_nSpriteWidth * scale ) / 2, ( m_nSpriteHeight * scale ) / 2 );
	D3DXVECTOR2 trans = D3DXVECTOR2( x - spriteCentre.x, y - spriteCentre.y );
	D3DXMATRIX mat;
	D3DXVECTOR2 scaling( scale, scale );
	D3DXMatrixTransformation2D( &mat, NULL, 0.0, &scaling, &spriteCentre, NULL, &trans );

	m_pSprite->SetTransform( &mat );
	
	m_pSprite->Draw( m_pTexture, NULL, NULL, NULL, Color::White().Code() );
}



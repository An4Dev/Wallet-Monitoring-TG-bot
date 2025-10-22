#pragma once

struct Vertex_t
{
	XMFLOAT4 xyzrhw;
	D3DCOLOR color;

	enum
	{
		FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE
	};
};

enum FontRenderFlag_t
{
	FONT_RENDER_LEFT = 0,
	FONT_RENDER_RIGHT = 1,
	FONT_RENDER_CENTER_V = 2,
	FONT_RENDER_CENTER_H = 4
};

class Renderer
{
public:
	Renderer();
	~Renderer();

	void					OnSetup( IDirect3DDevice9* pDevice );

	void					PreFrame();
	void					PostFrame();

	void					OnReset();

	HFont					CreateFont( const std::string& pFontName, int nFontSize, DWORD dwFontFlags = FONT_CREATE_NONE );
	HSprite					CreateSprite( std::string file, int width, int height, Color transparentColorKey );

	void					DrawRect( int x, int y, int w, int h, Color color );
	void					DrawRectOut( int x, int y, int w, int h, Color color, Color out );
	void					DrawRectGradV( int x, int y, int w, int h, Color from, Color to );
	void					DrawRectGradH( int x, int y, int w, int h, Color from, Color to );
	void					DrawBorderBox( int x, int y, int w, int h, int t, Color color );
	void					DrawBorderBoxOut( int x, int y, int w, int h, int t, Color color, Color out );
	void					DrawLine( int x0, int y0, int x1, int y1, Color color );
	void					DrawCircle( float x, float y, float r, float s, Color color );
	void					DrawTriangle( int x, int y, int w, int h, Color color );
	void					DrawText( HFont hFont, int x, int y, DWORD dwAlign, Color color, const char* text, ... );
	void					DrawSprite( HSprite hSprite, int x, int y, float scale );
	void					DrawHealthBar( int x, int y, int h, int hp );
	void                    Draw3DBox( Vector3 org, Vector3 vMin, Vector3 vMax, Color color );
	void					DrawHitBox( Vector3* hitBoxVectors, Color color );
	void					DrawBone( Vector3 start, Vector3 end, Color color );
	void					DrawSkeletion( Entity entity, Color color );
	void					DrawCrosshair( int iSize );


	Font*					GetFont( HFont hFont );

private:
	IDirect3DDevice9*		m_pDevice;
	std::vector<Font*>		m_pFontList;
	std::vector<Sprite*>	m_pSpriteList;
};

extern Renderer* g_pRenderer;

inline void BuildVertex( XMFLOAT4 xyzrhw, D3DCOLOR color, Vertex_t* vertexList, int index )
{
	vertexList[ index ].xyzrhw = xyzrhw;
	vertexList[ index ].color = color;
}
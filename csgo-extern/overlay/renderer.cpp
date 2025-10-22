#include "..\include.h"

Renderer* g_pRenderer = new Renderer();

Renderer::Renderer()
{
	m_pDevice = nullptr;
}

Renderer::~Renderer()
{
	if( !m_pFontList.empty() )
	{
		for( auto& pFont : m_pFontList )
		{
			delete pFont;
			pFont = nullptr;
		}
	}

	if( !m_pSpriteList.empty() )
	{
		for( auto& pSprite : m_pSpriteList )
		{
			delete pSprite;
			pSprite = nullptr;
		}
	}
}

void Renderer::OnSetup( IDirect3DDevice9* pDevice )
{
	m_pDevice = pDevice;
}

void Renderer::PreFrame()
{
	if( !m_pFontList.empty() )
	{
		for( auto& pFont : m_pFontList )
		{
			if( pFont )
			{
				pFont->PreFrame();
			}
		}
	}

	if( !m_pSpriteList.empty() )
	{
		for( auto& pSprite : m_pSpriteList )
		{
			if( pSprite )
			{
				pSprite->PreFrame();
			}
		}
	}

	m_pDevice->SetFVF( Vertex_t::FVF );
}

void Renderer::PostFrame()
{
	if( !m_pFontList.empty() )
	{
		for( auto& pFont : m_pFontList )
		{
			if( pFont )
			{
				pFont->PostFrame();
			}
		}
	}

	if( !m_pSpriteList.empty() )
	{
		for( auto& pSprite : m_pSpriteList )
		{
			if( pSprite )
			{
				pSprite->PostFrame();
			}
		}
	}
}

void Renderer::OnReset()
{
	if( !m_pFontList.empty() )
	{
		for( auto& pFont : m_pFontList )
		{
			if( pFont )
			{
				pFont->OnReset();
			}
		}
	}

	if( !m_pSpriteList.empty() )
	{
		for( auto& pSprite : m_pSpriteList )
		{
			if( pSprite )
			{
				pSprite->OnReset();
			}
		}
	}
}

HFont Renderer::CreateFont( const std::string& pFontName, int nFontSize, DWORD dwFontFlags )
{
	Font* pFont = new Font( m_pDevice, pFontName, nFontSize, dwFontFlags );

	m_pFontList.push_back( pFont );

	return ( m_pFontList.size() - 1 );
}

HSprite Renderer::CreateSprite( std::string file, int width, int height, Color transparentColorKey )
{
	Sprite* pSprite = new Sprite( m_pDevice, file, width, height, transparentColorKey );

	m_pSpriteList.push_back( pSprite );

	return ( m_pSpriteList.size() - 1 );
}

void Renderer::DrawRect( int x, int y, int w, int h, Color color )
{
	Vertex_t vertexList[ 4 ];

	BuildVertex( XMFLOAT4( x, y + h, 0, 1 ), color.Code(), vertexList, 0 );
	BuildVertex( XMFLOAT4( x, y, 0, 1 ), color.Code(), vertexList, 1 );
	BuildVertex( XMFLOAT4( x + w, y + h, 0, 1 ), color.Code(), vertexList, 2 );
	BuildVertex( XMFLOAT4( x + w, y, 0, 1 ), color.Code(), vertexList, 3 );

	m_pDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, vertexList, sizeof( Vertex_t ) );
}

void Renderer::DrawRectOut( int x, int y, int w, int h, Color color, Color out )
{
	DrawRect( x, y, w, h, color );
	DrawBorderBox( x, y, w, h, 1, out );
}

void Renderer::DrawRectGradV( int x, int y, int w, int h, Color from, Color to )
{
	int r = ( to.R() - from.R() ) / h;
	int g = ( to.G() - from.G() ) / h;
	int b = ( to.B() - from.B() ) / h;
	int a = ( to.A() - from.A() ) / h;

	for( int i = 0; i < h; i++ )
	{
		int R = from.R() + r * i;
		int G = from.G() + g * i;
		int B = from.B() + b * i;
		int A = from.A() + a * i;

		DrawRect( x, y + i, w, 1, Color( R, G, B, A ) );
	}
}

void Renderer::DrawRectGradH( int x, int y, int w, int h, Color from, Color to )
{
	int r = ( to.R() - from.R() ) / w;
	int g = ( to.G() - from.G() ) / w;
	int b = ( to.B() - from.B() ) / w;
	int a = ( to.A() - from.A() ) / w;

	for( int i = 0; i < h; i++ )
	{
		int R = from.R() + r * i;
		int G = from.G() + g * i;
		int B = from.B() + b * i;
		int A = from.A() + a * i;

		DrawRect( x + i, y, 1, h, Color( R, G, B, A ) );
	}
}

void Renderer::DrawBorderBox( int x, int y, int w, int h, int t, Color color )
{
	DrawRect( x, y, w, t, color );
	DrawRect( x, y, t, h, color );
	DrawRect( x + w, y, t, h, color );
	DrawRect( x, y + h, w + t, t, color );
}

void Renderer::DrawBorderBoxOut( int x, int y, int w, int h, int t, Color color, Color out )
{
	DrawBorderBox( x, y, w, h, t, color );
	DrawBorderBox( x - t, y - t, w + t * 2, h + t * 2, 1, out );
	DrawBorderBox( x + t, y + t, w - t * 2, h - t * 2, 1, out );
}

void Renderer::DrawLine( int x0, int y0, int x1, int y1, Color color )
{
	Vertex_t vertexList[ 2 ];

	BuildVertex( XMFLOAT4( x0, y0, 0, 0 ), color.Code(), vertexList, 0 );
	BuildVertex( XMFLOAT4( x1, y1, 0, 0 ), color.Code(), vertexList, 1 );

	m_pDevice->DrawPrimitiveUP( D3DPT_LINELIST, 2, vertexList, sizeof( Vertex_t ) );
}

void Renderer::DrawCircle( float x, float y, float r, float s, Color color )
{
	float Step = M_PI * 2.0 / s;
	for( float a = 0; a < ( M_PI*2.0 ); a += Step )
	{
		float x1 = r * cos( a ) + x;
		float y1 = r * sin( a ) + y;
		float x2 = r * cos( a + Step ) + x;
		float y2 = r * sin( a + Step ) + y;

		DrawLine( x1, y1, x2, y2, color );
	}
}

void Renderer::DrawTriangle( int x, int y, int w, int h, Color color )
{
	Vertex_t vertexList[ 3 ];

	BuildVertex( XMFLOAT4( x + (w / 2), y, 0, 1 ), color.Code(), vertexList, 0 );
	BuildVertex( XMFLOAT4( x, y + h, 0, 1 ), color.Code(), vertexList, 1 );
	BuildVertex( XMFLOAT4( x + w, y + h, 0, 1 ), color.Code(), vertexList, 2 );

	m_pDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 1, vertexList, sizeof( Vertex_t ) );
}

void Renderer::DrawText( HFont hFont, int x, int y, DWORD dwAlign, Color color, const char* text, ... )
{
	Font* pFont = m_pFontList[ hFont ];

	if( !pFont )
		return;

	char buffer[ 1024 ] = { NULL };

	va_list args;
	va_start( args, text );
	vsprintf( buffer, text, args );
	va_end( args );

	int w = 0, h = 0;

	pFont->GetTextSize( buffer, w, h );

	if( dwAlign & FONT_RENDER_RIGHT )
		x -= w;
	else if( dwAlign & FONT_RENDER_CENTER_H )
		x -= w / 2;

	if( dwAlign & FONT_RENDER_CENTER_V )
		y -= h / 2;

	pFont->DrawText( x, y, color, buffer );
}

void Renderer::DrawSprite( HSprite hSprite, int x, int y, float scale )
{
	Sprite* pSprite = m_pSpriteList[ hSprite ];

	if( !pSprite )
		return;
	
	pSprite->DrawSprite( x, y, scale );
}


void Renderer::DrawHealthBar( int x, int y, int h, int hp )
{
	int R = 255 - ( hp*2.55 );
	int G = hp*2.55;
	hp = (h - ( ( h * hp ) / 100 ));

	DrawRect( x - 4, y + hp, 2, h - hp + 1, Color( R, G, 0 ) );
	DrawBorderBox( x - 5, y - 1, 3, h + 2, 1, Color::Black() );
}

void Renderer::DrawHitBox( Vector3* hitBoxVectors, Color color )
{
	Vector3 hitBoxVectors0, hitBoxVectors1, hitBoxVectors2, hitBoxVectors3,
		hitBoxVectors4, hitBoxVectors5, hitBoxVectors6, hitBoxVectors7;


	if( g_pEngine->WorldToScreen( hitBoxVectors[ 0 ], hitBoxVectors0 ) &&
		g_pEngine->WorldToScreen( hitBoxVectors[ 1 ], hitBoxVectors1 ) &&
		g_pEngine->WorldToScreen( hitBoxVectors[ 2 ], hitBoxVectors2 ) &&
		g_pEngine->WorldToScreen( hitBoxVectors[ 3 ], hitBoxVectors3 ) &&
		g_pEngine->WorldToScreen( hitBoxVectors[ 4 ], hitBoxVectors4 ) &&
		g_pEngine->WorldToScreen( hitBoxVectors[ 5 ], hitBoxVectors5 ) &&
		g_pEngine->WorldToScreen( hitBoxVectors[ 6 ], hitBoxVectors6 ) &&
		g_pEngine->WorldToScreen( hitBoxVectors[ 7 ], hitBoxVectors7 ) )
	{

		/*									  							
								.+--5---+     
							  .8 4    6'|  
							 +--7---+'  11   
							 9   |  10  |   
							 |  ,+--|3--+   
							 |.0    | 2'   
							 +--1---+'     					
	    */

		D3DXVECTOR2 lines[ 12 ][ 2 ];
		//bottom of box
		lines[ 0 ][ 0 ] = { hitBoxVectors0.x, hitBoxVectors0.y };
		lines[ 0 ][ 1 ] = { hitBoxVectors1.x, hitBoxVectors1.y };
		lines[ 1 ][ 0 ] = { hitBoxVectors1.x, hitBoxVectors1.y };
		lines[ 1 ][ 1 ] = { hitBoxVectors2.x, hitBoxVectors2.y };
		lines[ 2 ][ 0 ] = { hitBoxVectors2.x, hitBoxVectors2.y };
		lines[ 2 ][ 1 ] = { hitBoxVectors3.x, hitBoxVectors3.y };
		lines[ 3 ][ 0 ] = { hitBoxVectors3.x, hitBoxVectors3.y };
		lines[ 3 ][ 1 ] = { hitBoxVectors0.x, hitBoxVectors0.y };

		lines[ 4 ][ 0 ] = { hitBoxVectors0.x, hitBoxVectors0.y };
		lines[ 4 ][ 1 ] = { hitBoxVectors6.x, hitBoxVectors6.y };

		// top of box
		lines[ 5 ][ 0 ] = { hitBoxVectors6.x, hitBoxVectors6.y };
		lines[ 5 ][ 1 ] = { hitBoxVectors5.x, hitBoxVectors5.y };
		lines[ 6 ][ 0 ] = { hitBoxVectors5.x, hitBoxVectors5.y };
		lines[ 6 ][ 1 ] = { hitBoxVectors4.x, hitBoxVectors4.y };
		lines[ 7 ][ 0 ] = { hitBoxVectors4.x, hitBoxVectors4.y };
		lines[ 7 ][ 1 ] = { hitBoxVectors7.x, hitBoxVectors7.y };
		lines[ 8 ][ 0 ] = { hitBoxVectors7.x, hitBoxVectors7.y };
		lines[ 8 ][ 1 ] = { hitBoxVectors6.x, hitBoxVectors6.y };


		lines[ 9 ][ 0 ] = { hitBoxVectors5.x, hitBoxVectors5.y };
		lines[ 9 ][ 1 ] = { hitBoxVectors1.x, hitBoxVectors1.y };

		lines[ 10 ][ 0 ] = { hitBoxVectors4.x, hitBoxVectors4.y };
		lines[ 10 ][ 1 ] = { hitBoxVectors2.x, hitBoxVectors2.y };

		lines[ 11 ][ 0 ] = { hitBoxVectors7.x, hitBoxVectors7.y };
		lines[ 11 ][ 1 ] = { hitBoxVectors3.x, hitBoxVectors3.y };

		for( int i = 0; i < 12; i++ )
		{
			DrawLine( lines[ i ][ 0 ].x, lines[ i ][ 0 ].y, lines[ i ][ 1 ].x, lines[ i ][ 1 ].y, color );
		}
	}
}

Font* Renderer::GetFont( HFont hFont )
{
	return m_pFontList[ hFont ];
}

void Renderer::DrawBone( Vector3 start, Vector3 end, Color color )
{
	Vector3 vStart, vEnd;
	if( (!g_pEngine->WorldToScreen( start, vStart )) || (!g_pEngine->WorldToScreen( end, vEnd )) )
		return;

	DrawLine( vStart.x, vStart.y, vEnd.x, vEnd.y, color );
}
void Renderer::DrawSkeletion( Entity entity, Color color )
{
	static const std::vector<std::vector<int>> bones =
	{
		{  8,  9, 10 }, { 36, 37, 38 },			// arms
		{ 63, 64, 65 }, { 69, 70, 71 },			// Legs
		{  0,  1,  2,  3,  4,  5,  6 }			// Body
	};

	for( auto& skel : bones )
	{
		for( auto bone = skel.begin(); bone != skel.end() - 1; bone++ )
		{
			DrawBone( entity.GetBonePosition( *bone ), entity.GetBonePosition( *(bone + 1) ), color );
		}
	}
}

void Renderer::DrawCrosshair( int iSize )
{
	int x, y;
	g_pOverlay->GetScreenSize(&x, &y);

	x /= 2;
    y /= 2;
 
        

	DrawRect( x - iSize, y, iSize * 2 + 1, 1, Color( 255, 0, 0, 255 ) );
	DrawRect( x, y - iSize, 1, iSize * 2 + 1, Color( 255, 0, 0, 255 ) );

	DrawRect( x - iSize, y - iSize, 1, iSize, Color( 255, 0, 0, 255 ) ); // 1
	DrawRect( x, y - iSize, iSize + 1, 1, Color( 255, 0, 0, 255 ) ); // 2
	DrawRect( x + iSize + 1, y, 1, iSize + 1, Color( 255, 0, 0, 255 ) ); // 3
	DrawRect( x - iSize, y + iSize, iSize + 1, 1, Color( 255, 0, 0, 255 ) ); // 4
 
}
#include "..\include.h"

Overlay*  g_pOverlay = new Overlay();

Overlay::Overlay()
{
	m_hWnd = NULL;
	m_hGame = NULL;

	m_pDirect3D = nullptr;
	m_pDevice = nullptr;

	memset( &m_Present, NULL, sizeof( D3DPRESENT_PARAMETERS ) );
}

Overlay::~Overlay()
{
	if( m_pDevice )
	{
		m_pDevice->Release();
		m_pDevice = nullptr;
	}

	if( m_pDirect3D )
	{
		m_pDirect3D->Release();
		m_pDirect3D = nullptr;
	}
}

bool Overlay::Attach( HWND hWnd )
{
	m_hGame = hWnd;

	if( !m_hGame )
	{
		return false;
	}

	RECT client;

	GetClientRect( m_hGame, &client );

	m_nSize[ 0 ] = client.right;
	m_nSize[ 1 ] = client.bottom;

	WNDCLASSEX wc = { NULL };

	wc.cbSize = sizeof( WNDCLASSEX );
	wc.style = CS_VREDRAW | CS_HREDRAW;
	wc.lpfnWndProc = WndProcedure;
	wc.cbClsExtra = NULL;
	wc.cbWndExtra = NULL;
	wc.hInstance = NULL;
	wc.hIcon = LoadIcon( NULL, IDI_APPLICATION );
	wc.hCursor = LoadCursor( NULL, IDC_ARROW );
	wc.hbrBackground = ( HBRUSH )COLOR_WINDOW;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = OVERLAY_NAME;
	wc.hIconSm = LoadIcon( NULL, IDI_APPLICATION );

	if( !RegisterClassEx( &wc ) )
	{
		return false;
	}

	m_hWnd = CreateWindowEx( WS_EX_TOPMOST | WS_EX_COMPOSITED | WS_EX_TRANSPARENT | WS_EX_LAYERED, OVERLAY_NAME, OVERLAY_NAME, WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT, m_nSize[ 0 ], m_nSize[ 1 ], NULL, NULL, NULL, NULL );

	if( !m_hWnd )
	{
		return false;
	}

	MARGINS margin = { -1, -1, -1, -1 };

	DwmExtendFrameIntoClientArea( m_hWnd, &margin );

	ShowWindow( m_hWnd, SW_SHOWDEFAULT );

	UpdateWindow( m_hWnd );

	RECT game;

	GetWindowRect( m_hGame, &game );

	int w = game.right - game.left;
	int h = game.bottom - game.top;

	LONG_PTR dwStyle = GetWindowLongPtr( m_hGame, GWL_STYLE );

	if( dwStyle & WS_BORDER )
	{
		int x = GetSystemMetrics( SM_CXBORDER );
		int y = GetSystemMetrics( SM_CYCAPTION ) + GetSystemMetrics( SM_CYBORDER );

		game.left += x;
		game.top += y;

		w -= x;
		h -= y;
	}

	MoveWindow( m_hWnd, game.left, game.top, w, h, TRUE );

	m_nSize[ 0 ] = w;
	m_nSize[ 1 ] = h;

	return InitDirectX();
}

int Overlay::OnFrame()
{
	MSG msg;

	for( ;; )
	{
		Sleep( 1 );

		if( PeekMessage( &msg, NULL, NULL, NULL, PM_REMOVE ) )
		{
			if( msg.message == WM_QUIT )
			{
				break;
			}

			TranslateMessage( &msg );
			DispatchMessageA( &msg );
		}
		else
		{
			m_pDevice->Clear( NULL, NULL, D3DCLEAR_TARGET, NULL, 1.0f, NULL );
			m_pDevice->BeginScene();

			if( !m_pOnFrameList.empty() )
			{
				for( auto& pOnFrame : m_pOnFrameList )
				{
					pOnFrame();
				}
			}

			m_pDevice->EndScene();
			m_pDevice->Present( NULL, NULL, NULL, NULL );
		}
	}

	return ( int )msg.wParam;
}

void Overlay::AddOnFrame( const OnFrameFn& pFunction )
{
	m_pOnFrameList.push_back( pFunction );
}

void Overlay::GetScreenSize( int* width, int* height )
{
	if( width )
		*width = m_nSize[ 0 ];

	if( height )
		*height = m_nSize[ 1 ];
}

void Overlay::GetCenterScreen( int* x, int* y )
{
	if( x )
		*x = m_nSize[ 0 ] / 2;

	if( y )
		*y = m_nSize[ 1 ] / 2;
}

IDirect3DDevice9* Overlay::GetDevice() const
{
	return m_pDevice;
}

bool Overlay::InitDirectX()
{
	m_Present.EnableAutoDepthStencil	= TRUE;
	m_Present.AutoDepthStencilFormat	= D3DFMT_D16;
	m_Present.Windowed					= TRUE;
	m_Present.BackBufferCount			= 1;
	m_Present.BackBufferFormat			= D3DFMT_A8R8G8B8;
	m_Present.BackBufferWidth			= m_nSize[ 0 ];
	m_Present.BackBufferHeight			= m_nSize[ 1 ];
	m_Present.MultiSampleType			= D3DMULTISAMPLE_NONE;
	m_Present.SwapEffect				= D3DSWAPEFFECT_DISCARD;
	m_Present.PresentationInterval		= D3DPRESENT_INTERVAL_IMMEDIATE;
	m_Present.hDeviceWindow				= m_hWnd;

	m_pDirect3D = Direct3DCreate9( D3D_SDK_VERSION );

	if( !m_pDirect3D )
	{
		return false;
	}

	if( FAILED( m_pDirect3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &m_Present, &m_pDevice ) ) )
	{
		return false;
	}

	if( !m_pDevice )
	{
		return false;
	}

	return true;
}

LRESULT WINAPI Overlay::WndProcedure( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
	case WM_DESTROY:
		PostQuitMessage( NULL );
		break;

	default:
		return DefWindowProc( hWnd, message, wParam, lParam );
	}

	return NULL;
}
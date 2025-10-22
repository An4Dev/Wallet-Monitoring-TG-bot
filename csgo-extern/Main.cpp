#include "include.h"

using std::cout;
using std::endl;
using std::cin;

Utils utils;
WebShit* g_pWebShit = new WebShit();

D3DMenu*		d3dmenu		= new D3DMenu();
AimbotMenu*		aimbotMenu	= new AimbotMenu();
TriggerMenu*	triggerMenu	= new TriggerMenu();
VisualsMenu*	visualsMenu	= new VisualsMenu();
MiscMenu*		miscMenu	= new MiscMenu();
ConfigMenu*		configMenu	= new ConfigMenu();

void Render()
{
	int iLocalIndex = g_pEngine->GetLocalPlayer();
	LocalEntity.Update( iLocalIndex );

	for( int iIndex = 0; iIndex <= g_pGlobals->GetMaxClients(); iIndex++ )
	{
		EntityList[ iIndex ].Update( iIndex );

		if( EntityList[ iIndex ].IsValid() && ( iLocalIndex != iIndex ) )
			g_pESP->DrawESP( iIndex );
	}

	//g_pRenderer->DrawCrosshair( 10 ); // uncomment to meme :)
}

void OnFrame()
{
	g_pRenderer->PreFrame();

	if( g_pEngine->IsInGame() )
	{
		Render();
	}
		
	if( !d3dmenu->IsCreated() )
		d3dmenu->Create( { aimbotMenu, triggerMenu, visualsMenu, miscMenu, configMenu } );

	d3dmenu->Render( 50, 300 );
	d3dmenu->Watermark( "gamerfood memes" );
	d3dmenu->PollInput();

	g_pRenderer->PostFrame();
}

void Aimbot()
{
	for( ;; )
	{
		if( g_pEngine->IsInGame() )
		{
			g_pAimbot->FindTarget();
			g_pAimbot->GotoTarget();
			g_pAimbot->AlwaysOnRCS();
		}
		std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) ); // Better sleep method for C++11 multi-threading
	}
}

void Misc()
{
	for( ;; )
	{
		if( g_pEngine->IsInGame() )
		{
			g_pMisc->BunnyHop();
			g_pMisc->NoFlash();
			g_pMisc->AutoStrafe();
			g_pMisc->NoHands();
			g_pMisc->SkinChanger();
			g_pMisc->Derank();
		}
		std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) ); // Better sleep method for C++11 multi-threading
	}
}

void ClientCmdHandler()
{
	for( ;; )
	{
		g_pMisc->InsultGen();
		g_pMisc->IrcChatter();
		g_pMisc->Yiff();
		std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) ); // Better sleep method for C++11 multi-threading
	}
}

void FakeLag()
{
	for( ;; )
	{
		if( g_pEngine->IsInGame() )
		{
			g_pMisc->FakeLag();
		}
		std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) ); // Better sleep method for C++11 multi-threading
	}
}

void BSPHandler()
{
	for( ;; )
	{
		if( g_pEngine->IsInGame() )
		{
			if( g_pBSP->getName() == NULL || ( strcmp( g_pBSP->getName(), g_pEngine->GetMapName() ) != 0 ) ) // check if the map loaded in memory is the same as ingame
			{
				g_pBSP->unload(); // unload map stored in memory before we load the new one
				g_pBSP->load( g_pStatic->GameDirectory, g_pEngine->GetMapName() ); // load the correct map
			}
		}
		std::this_thread::sleep_for( std::chrono::milliseconds( 5000 ) ); // Better sleep method for C++11 multi-threading
	}
}

void VisCheckHandler()
{
	for( ;; )
	{
		if( g_pEngine->IsInGame() )
		{
			for( int iIndex = 0; iIndex <= g_pGlobals->GetMaxClients(); iIndex++ )
			{
				EntityList[ iIndex ].CheckVisible( g_pAimbot->GetHitbox() ); // Decreases the number of calls by x150, no lag with this now :^)
			}
		}
		std::this_thread::sleep_for( std::chrono::milliseconds( 50 ) ); // Better sleep method for C++11 multi-threading
	}
}

int main()
{
	g_pConFuncs->SetConsoleCursor( false );

	HWND game = NULL;

	g_pConFuncs->GotoXY( 32, 11 );
	g_pConFuncs->SetConsoleColor( aqua, black );
	std::cout << "Looking for CSGO\n\n";

	while( !game )
	{
		game = FindWindow( NULL, "Counter-Strike: Global Offensive" );
		Sleep( 1000 );
	}

	process->SetWindow( game );

	if( !process->Attach( "csgo.exe" ) )
		return 1;

	client   = process->GetModule( "client.dll" );
	engine   = process->GetModule( "engine.dll" );
	csgobase = process->GetModule( "csgo.exe" );

	ShowWindow( GetConsoleWindow(), SW_HIDE );

	g_pInterface->OnSetup();
	g_pStatic	->OnSetup();
	g_pDynamic	->OnSetup();
	g_pHitbox	->OnSetup();
	g_pFiles	->OnSetup( "JewsDid.911", utils.CurrentPath().c_str() );
	g_pConfig	->OnSetup();
	g_pMisc		->ReadYiff();
	g_pInterface->OnShutdown(); // forgot to unload interfaces :^) saves 80mb

	std::thread tBSPHandler( BSPHandler );
	tBSPHandler.detach();

	std::thread tVisCheckHandler( VisCheckHandler );
	tVisCheckHandler.detach();

	std::thread tClientCmdHandler( ClientCmdHandler );
	tClientCmdHandler.detach();
				 
	std::thread tFakeLag( FakeLag );
	tFakeLag.detach();

	std::thread tMisc( Misc );

	std::thread tAimbot( Aimbot );

	if( !g_pOverlay->Attach( game ) )
		return 2;

	g_pRenderer->OnSetup( g_pOverlay->GetDevice() );
	hFont			= g_pRenderer->CreateFont( "HaxrCorp 4088", 12, FONT_CREATE_SPRITE | FONT_CREATE_OUTLINE | FONT_CREATE_BOLD | FONT_CREATE_ANTIALIASED );
	hFontMenuHeader = g_pRenderer->CreateFont( "Calibri", 20, FONT_CREATE_SPRITE | FONT_CREATE_BOLD );
	hFontMenu		= g_pRenderer->CreateFont( "Calibri", 15, FONT_CREATE_SPRITE | FONT_CREATE_OUTLINE );

	hSprite1 = g_pRenderer->CreateSprite( "enemy.png", 300, 300, Color::White().Code() );
	hSprite2 = g_pRenderer->CreateSprite( "friendly.png", 300, 300, Color::White().Code() );

	g_pOverlay->AddOnFrame( OnFrame );

	std::thread tOverlay( &Overlay::OnFrame, g_pOverlay );
	tOverlay.join();

	return 0;
}


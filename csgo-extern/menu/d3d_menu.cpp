#include "..\include.h"

void D3DMenu::Create( std::initializer_list<IMenuElement*> elements )
{
	bIsCreated = true;
	Elements.clear();
	Elements.assign( elements.begin(), elements.end() );

	for( auto element : Elements )
	{
		MenuEntity ent;
		ent.Header = element->Header();
		ent.Items = element->Items();
		MenuEntities.push_back( ent );
	}

	if( MenuEntities.size() > 0 )
		MenuEntities[ 0 ].IsHighlighted = 0;

	HeaderColor			= Color( 233, 223, 92 );
	BackgroundColor		= Color( 55, 55, 55 );
	HightlightedColor	= Color( 100, 100, 100 );
	BorderColor			= Color( 100, 100, 100 );
	TextColor			= Color::White();
	TextColorHeader		= Color( 70, 70, 70 );
	ValueColor			= Color( 233, 223, 92 );
	waterMarkColor		= Color::White();
}

bool D3DMenu::IsCreated()
{
	return bIsCreated;
}

void D3DMenu::Watermark( char* cheatName )
{
	g_pRenderer->DrawText( hFont, 5, 0, FONT_RENDER_LEFT, waterMarkColor, cheatName );
}

void D3DMenu::Render( int X, int Y )
{
	if( !Enabled )
		return;

	int y = Y;

	for( int curEnt = 0; curEnt < MenuEntities.size(); curEnt++ )
	{
		if( MenuEntities[ curEnt ].IsCollapsed )
		{
			if( MenuEntities[ curEnt ].IsHighlighted != -1 )
			{
				g_pRenderer->DrawText( hFontMenuHeader, X + TextLeftMargin, y + TextTopMargin, FONT_RENDER_LEFT, HeaderColor, ( char* )MenuEntities[ curEnt ].Header.c_str() );
				g_pRenderer->DrawRectOut( X, y, rectWidth, rectHeaderHeight, HightlightedColor, BorderColor );
			}
			else
			{
				g_pRenderer->DrawText( hFontMenuHeader, X + TextLeftMargin, y + TextTopMargin, FONT_RENDER_LEFT, TextColorHeader, ( char* )MenuEntities[ curEnt ].Header.c_str() );
				g_pRenderer->DrawRectOut( X, y, rectWidth, rectHeaderHeight, HeaderColor, BorderColor );
			}
			y += rectHeaderHeight + rectBottomMargin;
		}
		else
		{
			if( MenuEntities[ curEnt ].IsHighlighted == 0 )
			{
				g_pRenderer->DrawText( hFontMenuHeader, X + TextLeftMargin, y + TextTopMargin, FONT_RENDER_LEFT, HeaderColor, ( char* )( MenuEntities[ curEnt ].Header + " ›››" ).c_str() );
				g_pRenderer->DrawRectOut( X, y, rectWidth, rectHeaderHeight, HightlightedColor, BorderColor );
			}
			else
			{
				g_pRenderer->DrawText( hFontMenuHeader, X + TextLeftMargin, y + TextTopMargin, FONT_RENDER_LEFT, TextColorHeader, ( char* )( MenuEntities[ curEnt ].Header + " ›››" ).c_str() );
				g_pRenderer->DrawRectOut( X, y, rectWidth, rectHeaderHeight, HeaderColor, BorderColor );
			}


			y += rectHeaderHeight + rectBottomMargin;

			for( int i = 0; i < MenuEntities[ curEnt ].Items.size(); i++ )
			{
				if( i + 1 == MenuEntities[ curEnt ].IsHighlighted )
				{
					g_pRenderer->DrawRectOut( X, y, rectWidth, rectHeight, HightlightedColor, BorderColor );
					g_pRenderer->DrawText( hFontMenu, X + TextLeftMargin, y + TextTopMargin, FONT_RENDER_LEFT, HeaderColor, ( char* )MenuEntities[ curEnt ].Items[ i ].c_str() );

				}
				else
				{
					g_pRenderer->DrawRectOut( X, y, rectWidth, rectHeight, BackgroundColor, BorderColor );
					g_pRenderer->DrawText( hFontMenu, X + TextLeftMargin, y + TextTopMargin, FONT_RENDER_LEFT, TextColor, ( char* )MenuEntities[ curEnt ].Items[ i ].c_str() );
				}
				g_pRenderer->DrawText( hFontMenu, X + rectWidth - TextRightMargin, y + TextTopMargin, FONT_RENDER_LEFT, ValueColor, Elements[ curEnt ]->Value( i ) );
				
				y += rectHeight + rectBottomMargin;
			}
		}
	}
}

void D3DMenu::PollInput()
{
	if( GetAsyncKeyState( VK_INSERT ) & 1 )
		Enabled = !Enabled;

	if( !Enabled )
		return;

	int Header = 0;
	int ItemOption = 0;

	int newHeader = 0;
	int newItemOption = 0;

	for( int i = 0; i < MenuEntities.size(); i++ )
	{
		if( MenuEntities[ i ].IsHighlighted != -1 )
		{
			Header = i;
			ItemOption = MenuEntities[ i ].IsHighlighted;
		}
	}

	if( GetAsyncKeyState( VK_LEFT ) & 1 )
	{
		if( ItemOption > 0 )
			Elements[ Header ]->OnLeftKey( ItemOption - 1 );

		return;
	}

	//only right key collapses headers 
	if( GetAsyncKeyState( VK_RIGHT ) & 1 )
	{
		//collapse header 
		if( ItemOption == 0 )
			MenuEntities[ Header ].IsCollapsed = !MenuEntities[ Header ].IsCollapsed;
		else
			Elements[ Header ]->OnRightKey( ItemOption - 1 );

		return;
	}

	if( GetAsyncKeyState( VK_UP ) & 1 )
	{
		if( Header == 0 && ItemOption == 0 )
		{
			newHeader = Header;
			newItemOption = ItemOption;
		}
		else
		{
			if( ItemOption > 0 )
			{
				newHeader = Header;
				newItemOption = ItemOption - 1;
			}
			else //Cheat Option is 0 
			{
				if( MenuEntities[ Header - 1 ].IsCollapsed )
				{
					newHeader = Header - 1;
					newItemOption = 0;
				}
				else
				{
					newHeader = Header - 1;
					newItemOption = MenuEntities[ Header - 1 ].Items.size();
				}
			}
		}

		MenuEntities[ Header ].IsHighlighted = -1;
		MenuEntities[ newHeader ].IsHighlighted = newItemOption;
		return;
	}

	if( GetAsyncKeyState( VK_DOWN ) & 1 )
	{
		if( Header == MenuEntities.size() - 1 &&
			( ( ItemOption == MenuEntities[ Header ].Items.size() && !MenuEntities[ Header ].IsCollapsed ) ||
			ItemOption == 0 && MenuEntities[ Header ].IsCollapsed ) )
		{
			newHeader = Header;
			newItemOption = ItemOption;
		}
		else
		{
			if( MenuEntities[ Header ].IsCollapsed )
			{
				//Cheat options must be 0 
				newHeader = Header + 1;
				newItemOption = 0;
			}
			else
			{
				if( ItemOption == MenuEntities[ Header ].Items.size() )
				{
					newHeader = Header + 1;
					newItemOption = 0;
				}
				else
				{
					newHeader = Header;
					newItemOption = ItemOption + 1;
				}
			}
		}

		MenuEntities[ Header ].IsHighlighted = -1;
		MenuEntities[ newHeader ].IsHighlighted = newItemOption;
		return;
	}
}

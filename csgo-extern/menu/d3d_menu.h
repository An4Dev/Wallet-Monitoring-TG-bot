#pragma once 

#undef DrawText

class D3DMenu
{
public:
	void Create( std::initializer_list<IMenuElement*> cheats );
	bool IsCreated();
	void Watermark( char* cheatName );
	void Render( int X, int Y );
	void PollInput();

private:
	struct MenuEntity
	{
		std::string Header;
		std::vector<std::string> Items;
		int IsHighlighted = -1; 		//-1 = nothing is highlighted, 0 = the header is highlighted, then first cheatOption is 1 
		bool IsCollapsed  = true;
	};

	std::vector<IMenuElement*>	Elements;
	std::vector<MenuEntity>		MenuEntities;

	bool		Enabled = false;
	bool		bIsCreated = false;

	Color		HeaderColor;
	Color		BackgroundColor;
	Color		HightlightedColor;
	Color		BorderColor;
	Color		TextColor;
	Color		TextColorHeader;
	Color		ValueColor;
	Color		waterMarkColor;

	const int	rectWidth			= 150;
	const int	rectHeight			= 22;
	const int	rectHeaderHeight	= 28;
	const int	rectBottomMargin	= 0;
	const int	TextLeftMargin		= 10;
	const int	TextTopMargin		= 4;
	const int	TextRightMargin		= 30;
};
#pragma once

class ConfigMenu : public IMenuElement
{
private:

	enum EItems
	{
		eLoad,
		eSave
	};

public:

	std::string Header()
	{
		return "CONFIG";
	}

	std::vector<std::string> Items()
	{
		return std::vector<std::string>
		{
			"Load Config",
			"Save Config"
		};
	}

	char* Value( int index )
	{
		static std::string str;

		switch( index )
		{
		case eLoad:
			return " ››";
		case eSave:
			return " ››";
		default:
			return "N/A";
		}
	}

	void OnRightKey( int index )
	{
		switch( index )
		{
		case eLoad:
			g_pConfig->Load();
			break;
		case eSave:
			g_pConfig->Save();
			break;
		default:
			break;
		}
	}

	void OnLeftKey( int index )
	{
		switch( index )
		{
		case eLoad:
			break;
		case eSave:
			break;
		default:
			break;
		}
	}

};
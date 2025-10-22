#pragma once 

class IMenuElement
{
public:

	virtual std::string Header() = 0;

	virtual std::vector<std::string> Items() = 0;
	virtual char* Value( int index ) = 0;
	virtual void OnRightKey( int index ) = 0;
	virtual void OnLeftKey( int index ) = 0;
};
#pragma once

#define KEYDOWN 0x8000

class Input
{
public:
	int move_mouse( int x, int y );
	int send_key( unsigned short int key_code, bool key_down );
};

extern Input* g_pInput;

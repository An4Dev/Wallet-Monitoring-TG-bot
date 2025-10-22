#include "..\include.h"

Input* g_pInput;

int Input::move_mouse( int x, int y )
{
	INPUT input = { 0 };
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
	input.mi.dx += x;
	input.mi.dy += y;
	if( GetForegroundWindow() == process->GetWindow() )
		return SendInput( 1, &input, sizeof( INPUT ) );

	return 0;
}

int Input::send_key( unsigned short int key_code, bool key_down )
{
	INPUT input;
	input.type = INPUT_KEYBOARD;
	input.ki.wScan = key_code;
	input.ki.time = time( NULL );
	input.ki.dwExtraInfo = 0;
	if( key_down )
		input.ki.dwFlags = KEYEVENTF_SCANCODE;
	
	else
		input.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;

	if( GetForegroundWindow() == process->GetWindow() )
		return SendInput( 1, &input, sizeof( INPUT ) );

	return 0;
}
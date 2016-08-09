#include "input.h"
#include "scene.h"
#include "monster.h"
#include "objects.h"

bool Input::oldLeftMouseButtonState( true );

bool Input::oldRightMouseButtonState( true );

hgeVector Input::getMousePosition( )
{
	float x, y;
	hge->Input_GetMousePos( &x, &y );
	return hgeVector( x, y );
}

void Input::handleLeftMouse( )
{
	if( oldLeftMouseButtonState == false && hge->Input_GetKeyState( HGEK_LBUTTON ) )
	{
		objects->prepareMonsterToMove( );
	}
	oldLeftMouseButtonState = hge->Input_GetKeyState( HGEK_LBUTTON );
}
 
void Input::handleRightMouse( )
{
	if( oldRightMouseButtonState == false && hge->Input_GetKeyState( HGEK_RBUTTON ) )
	{
		scene->makeObstacle( );
	}
	oldRightMouseButtonState = hge->Input_GetKeyState( HGEK_RBUTTON );
}

bool Input::handleEsc( )
{
	return hge->Input_GetKeyState( HGEK_ESCAPE );
}

bool Input::handle1( )
{	
	bool isPressed = hge->Input_GetKeyState( HGEK_1 ); 
	if ( isPressed )
	{
		toExit = false;
		toNextMap = 1;
	}
	return isPressed;
}

bool Input::handle2( )
{
	bool isPressed = hge->Input_GetKeyState( HGEK_2 );
	if ( isPressed )
	{
		toExit = false;
		toNextMap = 2;
	}
	return isPressed;
}

bool Input::handle3( )
{
	bool isPressed = hge->Input_GetKeyState( HGEK_3 );
	if ( isPressed )
	{
		toExit = false;
		toNextMap = 3;
	}
	return isPressed;
}

bool Input::handle4( )
{
	bool isPressed = hge->Input_GetKeyState( HGEK_4 );
	if ( isPressed )
	{
		toExit = false;
		toNextMap = 4;
	}
	return isPressed;
}
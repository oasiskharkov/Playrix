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
	char* centers = scene->getCellCenters( );

	if( oldLeftMouseButtonState == false && hge->Input_GetKeyState( HGEK_LBUTTON ) )
	{
		hgeVector pos = getMousePosition( );
		int i = static_cast<int>( pos.y / TILE_STEP );
		int j = static_cast<int>( pos.x / TILE_STEP );

		pos.x = TILE_STEP * i + TILE_STEP / 2;
		pos.y = TILE_STEP * j + TILE_STEP / 2;

		if( objects->getMonster( ).get( ) == nullptr || !objects->getMonster( )->isMoving( ) )
		{
			if( objects->getMonster( ).get( ) == nullptr )
			{
				objects->initMonster( ); 
			}
			if( centers[ i * MRC + j ] != 'X' )
			{
				objects->getMonster( )->setPosition( pos );
				objects->getMonster( )->setReadinessToMove( );
			}
		}
		else if( objects->getMonster( ).get( ) != nullptr && objects->getMonster( )->isReadyToMove( ) )
		{
			if( centers[ i * MRC + j ] != 'X' )
			{
				objects->getMonster( )->setDestination( pos );
				objects->getMonster( )->setMoving( ); 
				objects->getMonster( )->setReadinessToMove( false );
			}
		}	
	}
	oldLeftMouseButtonState = hge->Input_GetKeyState( HGEK_LBUTTON);
}
 
void Input::handleRightMouse( )
{
	if( oldRightMouseButtonState == false && hge->Input_GetKeyState( HGEK_RBUTTON ) )
	{
		hgeVector pos = getMousePosition( );
		int i = static_cast<int>( pos.y / TILE_STEP );
		int j = static_cast<int>( pos.x / TILE_STEP );

		char* center = scene->getCellCenters( );
		if( scene->canSetupObstacle( ) )
		{
			if(  center[ i * MRC + j ]  == 'X' )
			{
				center[ i * MRC + j ] = '0';
			}
			else if( center[ i * MRC + j ] == '0' )
			{
				center[ i * MRC + j ] = 'X';
			}
		}
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



#include "service.h"
#include "input.h"
#include "objects.h"
#include "scene.h"
#include "monster.h"

HGE* hge = 0;
Objects* objects = 0;
Scene* scene = 0;
float dt;
int toNextMap = 1;
bool toExit = true;

void ShowErrorMessageIfAnyAndSafeExit( const std::string& error = "" );
void ReleaseGameSources( );

bool FrameFunc( )
{
	// Get timer delta
	dt = hge->Timer_GetDelta( );

	// Press ESC to exit
	if( Input::handleEsc( ) )
	{
		return true;
	}

	// Right mouse single click
	Input::handleRightMouse( );

	// Left mouse single click
	Input::handleLeftMouse( );

	// Press 1, 2, 3 or 4 to choose the map
	if( ( Input::handle1( ) || Input::handle2( ) || Input::handle3( ) || Input::handle4( ) ) 
		&& ( objects->getMonster( ).get( ) == nullptr || 
		( objects->getMonster( ).get( ) != nullptr && !objects->getMonster( )->isMoving( ) ) ) )
	{
		return true;
	}

	objects->frameObjects( );

	return false;	
}

bool RenderFunc( )
{
	hge->Gfx_BeginScene( );
	
	scene->renderScene( );
	
	objects->renderObjects( );

	scene->renderCursor( );

	hge->Gfx_EndScene( );

	return false;
}

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{	
	// Check for memory leak if debug build
	#if defined( DEBUG ) | defined( _DEBUG )
		_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );	
	#endif
	
	hge = hgeCreate( HGE_VERSION );
		
	hge->System_SetState( HGE_FRAMEFUNC, FrameFunc );
	hge->System_SetState( HGE_RENDERFUNC, RenderFunc );
	hge->System_SetState( HGE_WINDOWED, true );	
	hge->System_SetState( HGE_SCREENWIDTH, GAME_WIDTH );
	hge->System_SetState( HGE_SCREENHEIGHT, GAME_HEIGHT );
	hge->System_SetState( HGE_SCREENBPP, 32 );
	hge->System_SetState( HGE_TITLE, "Maze" );
	
	if( hge->System_Initiate( ) )
	{
		while( true )
		{
			try
			{
				scene = Scene::getInstance( toNextMap );
				objects = Objects::getInstance( );
			}
			catch( const game_errors& gerr )
			{
				switch( gerr )
				{
				case game_errors::LOAD_SCENE_SOURCES:
					ShowErrorMessageIfAnyAndSafeExit( "Can't load scene sources." );
					return 0;
				case game_errors::LOAD_MONSTER_SOURCES:
					ShowErrorMessageIfAnyAndSafeExit( "Can't load monster sources." );
					return 0;
				case game_errors::OPEN_FILE:
					ShowErrorMessageIfAnyAndSafeExit( "Can't open source file to read data." );
					return 0;
				case game_errors::UNKNOWN_ERROR: 
					ShowErrorMessageIfAnyAndSafeExit( "Unknown error." );
					return 0;
				}
			}
			catch(...)
			{
				ShowErrorMessageIfAnyAndSafeExit( "Something goes wrong." );
				return 0;
			}	

			hge->System_Start();

			if( toExit )
			{
				break;
			}
			toExit = true;
			ReleaseGameSources();
		}
	}
	else
	{
		MessageBox( NULL, hge->System_GetErrorMessage( ), "Error", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL );
	}

	ShowErrorMessageIfAnyAndSafeExit( ); 
	return 0;
}


void ReleaseGameSources( )
{
	delete objects;
	delete scene;
}

void ShowErrorMessageIfAnyAndSafeExit( const std::string& error )
{
	if( !error.empty( ) )
	{
		MessageBox( NULL, error.c_str( ), "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL );
	}
	ReleaseGameSources( );
	hge->System_Shutdown( );
	hge->Release( );
}

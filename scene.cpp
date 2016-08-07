#include "scene.h"
#include "input.h"

Scene* Scene::instance( 0 );

Scene* Scene::getInstance( int fieldNumber )
{
   if( instance == 0 )
	{
		instance = new Scene( fieldNumber );
	}
	return instance;
}

Scene::Scene( int fieldNumber ) : m_nFieldNumber( fieldNumber ), cellCenters( 0 ) 
{
	m_hFloorTileTex = hge->Texture_Load( "Resources\\square.png" );				

	m_hBlockTex = hge->Texture_Load( "Resources\\block.png" );

	m_hCursorTex = hge->Texture_Load( "Resources\\cursor.png" );

	m_hCellSelectionTex = hge->Texture_Load( "Resources\\green_cross.png" );

	fillCellCenters( );

	prepareSources( );

	// If one of the source files is not found, free all scene sources and throw an error message.
	if( !m_upFloorTile.get( ) || !m_upBlock.get( ) || !m_upCursor.get( ) 
		|| !m_upCellSelectionAni.get( ) || !m_upFont.get( ) )
	{		
		release( );
		throw game_errors::LOAD_SCENE_SOURCES;
	}		
}

Scene::~Scene(void)
{
	release( );
}

void Scene::release( )
{
	hge->Texture_Free( m_hFloorTileTex );
	hge->Texture_Free( m_hBlockTex );
	hge->Texture_Free( m_hCursorTex );
	hge->Texture_Free( m_hCellSelectionTex );
	
	delete [] cellCenters;

	instance = 0;
}

bool Scene::prepareSources( )
{
	try
	{
		m_upFloorTile.reset( new hgeSprite( m_hFloorTileTex, 0.0f, 0.0f, 128.0f, 128.0f ) );

		m_upBlock.reset( new hgeSprite( m_hBlockTex, 0.0f, 0.0f, 128.0f, 128.0f ) );

		m_upCursor.reset( new hgeSprite( m_hCursorTex, 0.0f, 0.0f, 16.0f, 16.0f ) );
		
		m_upCellSelectionAni.reset( new hgeAnimation( m_hCellSelectionTex, 4, 5.0f, 0.0f, 0.0f, 32.0f, 32.0f ) );

		m_upFont.reset( new hgeFont( "Resources\\font1.fnt" ) );
	}
	catch(...)
	{
		return false;
	}
	return true;
}

void Scene::readCentersFromFile( char* centers, const char* filename )
{
	char directory[ 0x400 ];
	HANDLE hFile;
	DWORD dwBytes;
	GetCurrentDirectory( 0x400, directory );
	strcat( directory, "\\Resources" );
	SetCurrentDirectory( directory );

	if( INVALID_HANDLE_VALUE != ( hFile = CreateFile( filename, GENERIC_READ, 
		FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL ) ) )
	{
		ReadFile( hFile, centers, MRC * MRC + 2 * MRC, &dwBytes, NULL );
	}
	else
	{
		throw game_errors::OPEN_FILE;
	}

	char* begin = centers;
	char* start = begin;
	while( *centers != '\0' )
	{
		if( *centers == '0' || *centers == 'X' )
		{
			*start = *centers;
			start++;
		}
		centers++;
	}
	*start = '\0';
	centers = begin;

	CloseHandle( hFile ); 
}

void Scene::fillCellCenters( )
{
	cellCenters = new char[ MRC * MRC + 2 * MRC ];
	
	if( canSetupObstacle( ) )
	{
		memset( cellCenters, '0', MRC * MRC );
		cellCenters[ MRC * MRC + 1 ] = '\0'; 
	}
	else
	{
		switch( m_nFieldNumber )
		{
		case 1:
			readCentersFromFile( cellCenters, "field1.txt" );
			break;
		case 2:
			readCentersFromFile( cellCenters, "field2.txt" );
			break;
		case 3:
			readCentersFromFile( cellCenters, "field3.txt" );
			break;
		}
	}
}

void Scene::renderFloorTiles( )
{
	float tile_x = 0, tile_y = 0;
	for( size_t i = 0; i < MRC; i++ )
	{
		for( size_t j = 0; j < MRC; j++ )
		{
			if( cellCenters[ i * MRC + j ] == '0' )
			{
				m_upFloorTile->RenderStretch( tile_x, tile_y, tile_x + TILE_STEP, tile_y + TILE_STEP );
			}
			else
			{
				m_upBlock->RenderStretch( tile_x, tile_y, tile_x + TILE_STEP, tile_y + TILE_STEP );
			}
			tile_x += TILE_STEP;
		}
		tile_y += TILE_STEP;
		tile_x = 0.0f;
	}
}

void Scene::renderPath( )
{

}

void Scene::renderCellSelection( )
{

}

void Scene::renderScene( )
{
	renderFloorTiles( );
	renderPath( );
	renderCellSelection( );
}

void Scene::renderCursor( )
{
	hgeVector cursor = Input::getMousePosition( );
	m_upCursor->Render( cursor.x , cursor.y );
}

#include "scene.h"
#include "input.h"
#include "objects.h"
#include "monster.h"

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

	m_hCircleTex = hge->Texture_Load( "Resources\\circle.png" );

	fillCellCenters( );

	prepareSources( );

	// If one of the source files is not found, free all scene sources and throw an error message.
	if( !m_upFloorTile || !m_upBlock || !m_upCursor || !m_upCellSelectionAni || !m_upCircle || !m_upFont )
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
	hge->Texture_Free( m_hCircleTex );
	
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

		m_upCircle.reset( new hgeSprite( m_hCircleTex, 0.0f, 0.0f, 88.0f, 88.0f ) );

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
	if( objects->getMonster( ) != nullptr && objects->getMonster( )->isMoving( ) )
	{
		auto path = objects->getMonster( )->getPath( );

		for( size_t i = path.size( ) - 1; i > 0; i-- )
		{
			m_upCircle->SetHotSpot( 44.0f, 44.0f );

			auto current = getCenterByIndices( path[ i ] );
			auto dest = getCenterByIndices( path[ i - 1 ] );
			auto middle = hgeVector( ( current.x + dest.x ) / 2.0f, ( current.y + dest.y ) / 2.0f );

			m_upCircle->RenderEx( current.x, current.y, 0.0f, 0.1f );
			m_upCircle->RenderEx( middle.x, middle.y, 0.0f, 0.1f ); 
		}
	}
}

void Scene::renderCellSelection( )
{
	if( objects->getMonster( ) != nullptr && objects->getMonster( )->isMoving( ) )
	{
		auto dest = objects->getMonster( )->getPath( ).front( );
		auto destination = getCenterByIndices( dest );

		if( !m_upCellSelectionAni->IsPlaying( ) )
		{
			m_upCellSelectionAni->Play( );
		}
		else
		{
			m_upCellSelectionAni->Update( dt );
		}

		m_upCellSelectionAni->SetHotSpot( 16.0f, 16.0f );
		m_upCellSelectionAni->RenderEx( destination.x, destination.y, 0.0f, 0.8f );
	}
}

void Scene::renderScene( )
{
	renderFloorTiles( );
	renderPath( );
	renderCellSelection( );
}

void Scene::renderCursor( )
{
	auto cursor = Input::getMousePosition( );
	m_upCursor->Render( cursor.x , cursor.y );
}

void Scene::makeObstacle( ) 
{
	auto pos = Input::getMousePosition( );
	auto indices = getCellIndices( pos );

	int i = indices.first;
	int j = indices.second;

	char* center = scene->getCellCenters( );
	
	if( !scene->canSetupObstacle( ) || ( objects->getMonster( ) != nullptr 
		&& (indices == getCellIndices( objects->getMonster( )->getPosition( ) ) 
			|| objects->getMonster( )->isMoving( ) ) ) )
	{
		return;
	}

	if(  center[ i * MRC + j ]  == 'X' )
	{
		center[ i * MRC + j ] = '0';
	}
	else if( center[ i * MRC + j ] == '0' )
	{
		center[ i * MRC + j ] = 'X';
	}
}

hgeVector Scene::getSelectedCellCenter( )
{
	hgeVector center = Input::getMousePosition( );

	auto indices = getCellIndices( center );

	center.x = TILE_STEP * indices.second + TILE_STEP / 2.0f;
	center.y = TILE_STEP * indices.first + TILE_STEP / 2.0f;

	return center;
}

std::pair<int,int> Scene::getCellIndices( hgeVector pos )
{
	int i = static_cast<int>( pos.y / TILE_STEP );
	int j = static_cast<int>( pos.x / TILE_STEP );

	return std::pair<int,int>( i, j );
}

hgeVector Scene::getCenterByIndices( std::pair<int, int> indices )
{
	float x = indices.second * TILE_STEP + TILE_STEP / 2.0f;
	float y = indices.first * TILE_STEP + TILE_STEP / 2.0f;

	return hgeVector( x, y );
}

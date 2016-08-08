#include "objects.h"
#include "monster.h"
#include "input.h"
#include "scene.h"

Objects* Objects::instance( 0 );

Objects* Objects::getInstance( )
{
   if( instance == 0 )
	{
		instance = new Objects( );
	}
	return instance;
}

Objects::Objects( )
{
	prepareObjects( );
}

Objects::~Objects( )
{
	release( );
}

void Objects::release( )
{
	instance = 0;
}

void Objects::prepareObjects( )
{
	try
	{
		m_upMonster.reset( nullptr );
	}
	catch(const game_errors& error)
	{
		release( );
		throw error;
	}
	catch(...)
	{
		release( );
		throw game_errors::UNKNOWN_ERROR;
	}
}

void Objects::initMonster( )
{
	m_upMonster.reset( new Monster( "Resources\\monster.png" ) );
}

void Objects::frameMonster( )
{
	if( m_upMonster.get( ) != nullptr )
	{
		m_upMonster->frame( );
	}
}

void Objects::frameObjects( )
{
	frameMonster( );
}

void Objects::renderMonster( )
{
	if( m_upMonster.get( ) != nullptr )
	{
		m_upMonster->render( );
	}
}

void Objects::renderObjects( )
{
	renderMonster( );
}

void Objects::prepareMonsterToMove( )
{
	char* centers = scene->getCellCenters( );
	hgeVector center = Scene::getSelectedCellCenter( );
	
	auto indices = Scene::getCellIndices( center );
	int i = indices.first;
	int j = indices.second;

	if( objects->getMonster( ).get( ) == nullptr 
		|| ( !objects->getMonster( )->isMoving( ) 
		&& !objects->getMonster( )->isReadyToMove( ) ) )
	{
		if( centers[ i * MRC + j ] != 'X' )
		{
			if( objects->getMonster( ).get( ) == nullptr )
			{
				objects->initMonster( ); 
			}
			objects->getMonster( )->setPosition( center );
			objects->getMonster( )->setReadinessToMove( );
		}
	}
	else if( objects->getMonster( ).get( ) != nullptr 
		&& objects->getMonster( )->isReadyToMove( ) 
		&& !objects->getMonster( )->isMoving( ) )
	{
		if( centers[ i * MRC + j ] != 'X' && indices != Scene::getCellIndices( objects->getMonster( )->getPosition( ) ) )
		{
			if( !objects->getMonster( )->calculatePath( center ) )
			{
				return;
			}
			objects->getMonster( )->setMoving( ); 
			objects->getMonster( )->calculateNextStep( );
		}
	}	
}

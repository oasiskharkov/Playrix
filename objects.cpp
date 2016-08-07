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
	m_upMonster.reset( new Monster( 0.0f, 0.0f, directions::NORTH_EAST, "Recources\\monster.png" ) );
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

#include "monster.h"
#include "objects.h"

Monster::Monster( float xPos, float yPos, directions dir, const char* monster, bool moving ) : m_vPos( xPos, yPos ), 
					  m_bIsMoving( moving ), m_bReadyToMove( false ),  m_pMonsterAnimated( 0 ), m_pMonsterStopped( 0 )
{
	m_hMonsterTex = hge->Texture_Load( monster );

	if( !prepareResources( ) )
	{
		release( );
		throw game_errors::LOAD_MONSTER_SOURCES;
	}
}

Monster::~Monster(void)
{
	release( );
}

bool Monster::prepareResources( )
{
	try
	{
		m_pMonsterAnimated = new hgeAnimation( m_hMonsterTex, 8, 7.0f, 0.0f, 0.0f, 32.0f, 32.0f );
		m_pMonsterStopped = new hgeSprite ( m_hMonsterTex, 0.0f, 0.0f, 32.0f, 32.0f ); 
	}
	catch(...)
	{
		return false;
	}
	return true;
}

void Monster::release( )
{
	hge->Texture_Free( m_hMonsterTex );

	delete m_pMonsterAnimated;
}

void Monster::frame( ) 
{
	if( m_bIsMoving )
	{
		m_vPos += m_vDir * MONSTER_SPEED * dt;
	}
}

void Monster::render( )
{
	if( m_pMonsterAnimated != nullptr )
	{
		if( !m_pMonsterAnimated->IsPlaying( ) )
		{
			m_pMonsterAnimated->Play( );
		}
		else
		{
			m_pMonsterAnimated->Update( dt );
		}

		m_pMonsterAnimated->SetHotSpot( 128.0f, 128.0f );
		m_pMonsterAnimated->RenderEx( m_vPos.x, m_vPos.y, m_vDir.Angle( ) + M_PI_2, 0.2f );
	}
	else
	{
		throw game_errors::NULL_POINTER;
	}
}

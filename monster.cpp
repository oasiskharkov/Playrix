#include "monster.h"
#include "objects.h"
#include "scene.h"

Monster::Monster( const char* monster, bool moving ) : m_bIsMoving( moving ),
	m_pMonsterAnimated( 0 ), m_pMonsterStopped( 0 )
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
		m_pMonsterAnimated = new hgeAnimation( m_hMonsterTex, 8, 7.0f, 0.0f, 0.0f, 48.0f, 48.0f );
		m_pMonsterStopped = new hgeSprite ( m_hMonsterTex, 0.0f, 0.0f, 48.0f, 48.0f ); 
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
	delete m_pMonsterStopped;
}

void Monster::frame( ) 
{
	if( m_bIsMoving )
	{
		m_vPos += m_vDir * MONSTER_SPEED * dt;
		
		if( ( m_vDir == hgeVector( -1.0f, 0.0f ) && m_vPos.x < m_vDest.x )
			|| ( m_vDir == hgeVector( 1.0f, 0.0f ) && m_vPos.x > m_vDest.x )
			|| ( m_vDir == hgeVector( 0.0f, -1.0f ) && m_vPos.y < m_vDest.y ) 
			|| ( m_vDir == hgeVector( 0.0f, 1.0f ) && m_vPos.y > m_vDest.y ) )
		{
			m_vPos = m_vDest;
			calculateNextStep( );
		}
	}
}

void Monster::render( )
{
	m_bIsMoving ? move( ) : stop( );
}

void Monster::move( )
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

		m_pMonsterAnimated->SetHotSpot( 24.0f, 24.0f );
		m_pMonsterAnimated->RenderEx( m_vPos.x, m_vPos.y, m_vDir.Angle( ) - M_PI_2, 0.7f );
	}
	else
	{
		throw game_errors::NULL_POINTER;
	}
}

void Monster::stop( )
{
	if( m_pMonsterStopped != nullptr )
	{
		m_pMonsterStopped->SetHotSpot( 24.0f, 24.0f );
		m_pMonsterStopped->RenderEx( m_vPos.x, m_vPos.y, m_vDir.Angle( ) - M_PI_2, 0.7f );
	}
	else
	{
		throw game_errors::NULL_POINTER;
	}
}

bool Monster::calculatePath( const hgeVector& destination )
{
	// start position
	auto start = Scene::getCellIndices( m_vPos );

	// destination
	auto dest = Scene::getCellIndices( destination );

	// field
	auto cellCenters = scene->getCellCenters( );

	// declare dictionary with visited cells of the field
	std::map<std::pair<int, int>, bool> visited;

	// set flags for all cells as false
	for( size_t i = 0; i < MRC; i++ )
	{
		for( size_t j = 0; j < MRC; j++ )
		{
			visited[ std::pair<int, int>( i, j ) ]  = false;
		}
	}

	// mark start position as visited
	visited[ start ] = true;

	// declare queue to store analyzing cells
	std::queue< std::pair<int, int>> analyzing;

	// enqueue the start position
	analyzing.push( start );

	// show if destination was found
	bool found = false;

	// show from which cell we got to current
	std::map<std::pair<int, int>, std::pair<int, int>> cameFrom;

	// operate until all nodes are not analyzed
	while( analyzing.size( ) )
	{
		// get front cell
		std::pair<int, int> frontOfQueue = analyzing.front( );
		
		// dequeue front element
		analyzing.pop( );

		// neighbors cells
		std::vector<std::pair<int, int>> neighbors;
		
		// fill neighbor cells
		fillNeighborCells( neighbors, frontOfQueue );

		// Process all neighbor cells
		for( auto neighbor : neighbors )
		{
			// check is analyzing cell connected with other and other cell is not visited
			if( cellCenters[ neighbor.first * MRC + neighbor.second ] != 'X' && !visited[ neighbor ] )
			{
				// mark the current cell as visited
				visited[ neighbor ] = true;
				// save the link to the cell from which we came to current
				cameFrom[ neighbor ] = frontOfQueue;
				// if the destination was reached set the flag as true and break the loop
				if( neighbor == dest )
				{
					found = true;		
					break;
				}
				// enqueue analyzing cell
				analyzing.push( neighbor );
			}
		}	
		
		// if destination cell was found break main loop
		if( found )
		{
			break;
		}
	}
	
	// fill in the path from the start position to the destination
	if( found )
	{
		// begin from last cell
		std::pair<int, int> lastCell = dest;

		// start endless cycle
		while( true )
		{
			// add last cell
			m_vPath.push_back( lastCell );
			// break the loop if the destination was reached
			if( lastCell == start )
			{
				break;
			}
			// get the cell from which was reached the last
			lastCell = cameFrom[ lastCell ];
		}
	}
	
	return found;
}

void Monster::fillNeighborCells( std::vector<std::pair<int, int>>& neighbors, std::pair<int, int> point )
{
	auto p1 = std::pair<int, int>( point.first + 1, point.second );
	if( isPointInsideField( p1 ) )
	{
		neighbors.push_back( p1 );
	}

	auto p2 = std::pair<int, int>( point.first - 1, point.second );
	if( isPointInsideField( p2 ) )
	{
		neighbors.push_back( p2 );
	}

	auto p3 = std::pair<int, int>( point.first, point.second - 1 );
	if( isPointInsideField( p3 ) )
	{
		neighbors.push_back( p3 );
	}

	auto p4 = std::pair<int, int>( point.first, point.second + 1 );
	if( isPointInsideField( p4 ) )
	{
		neighbors.push_back( p4 );
	}
}

bool Monster::isPointInsideField( std::pair<int, int> point )
{
	return point.first >= 0 && point.first < MRC && point.second >= 0 && point.second < MRC;
}

void Monster::calculateNextStep( )
{
	if( !m_vPath.empty( ) )
	{
		auto currentCell = m_vPath.back( );
		m_vPath.pop_back( );
		if( !m_vPath.size( ) )
		{
			setMoving( false );
			return;
		}  
		auto nextCell = m_vPath.back( );
		setDestination( Scene::getCenterByIndices( nextCell ) );
		determineDirection( currentCell, nextCell );
	}
}

void Monster::determineDirection( std::pair<int, int> current, std::pair<int, int> next )
{
	if( current.first - 1 == next.first && current.second == next.second )
	{
		setDirection( hgeVector( 0.0f, -1.0f ) );
	}
	else if( current.first + 1 == next.first && current.second == next.second )
	{
		setDirection( hgeVector( 0.0f, 1.0f ) );
	}
	else if( current.first == next.first && current.second + 1 == next.second )
	{
		setDirection( hgeVector( 1.0f, 0.0f ) );
	}
	else if( current.first == next.first && current.second - 1 == next.second )
	{
		setDirection( hgeVector( -1.0f, 0.0f ) );
	}
}
#ifndef _MONSTER_H_
#define _MONSTER_H_

#include "service.h"

class Monster
{
private:
	// Current monster's position
	hgeVector m_vPos;

	// Monster's direction
	hgeVector m_vDir;

	// Monster's destination;
	hgeVector m_vDest;

	// Is monster moving
	bool m_bIsMoving;

	// Monster's texture
	HTEXTURE m_hMonsterTex;
	
	// Monster's animation
	hgeAnimation* m_pMonsterAnimated;

	// Monster's stopped sprite
	hgeSprite* m_pMonsterStopped;

	// Monsters's path
	std::vector< std::pair< int, int > > m_vPath;
		
	// Determine monster's direction
	void Monster::determineDirection( std::pair<int, int> current, std::pair<int, int> next );

	// Prepeare monster's resources
	bool prepareResources( );

	// Release all allocated memory
	void release( );

	// Monster moves
	void move( );

	// Monster stops
	void stop( );

	// Private copy constructor
	Monster( const Monster& );

	// Private copy assignment operator
	Monster& operator = ( const Monster& );
public:
	// Constructor
	Monster( const char* monster, bool moving = false );

	// Destructor
	virtual ~Monster( );
	
	// Monster frame calculations
	void frame( );

	// Render monster
	void render( );

	// Calculate path
	bool calculatePath( const hgeVector& destination );

	// Calculate next step
	void calculateNextStep( );

	// Get monster's position
	hgeVector getPosition( ) const;

	// Get monster's direction
	hgeVector getDirection( ) const;

	// Get monster's destination
	hgeVector getDestination( ) const;

	// Get monster's path
	std::vector<std::pair<int, int>>& getPath( );

	// Determine neighbor cells
	void fillNeighborCells( std::vector<std::pair<int, int>>& neighbors, std::pair<int, int> point );

	// Is cell valid
	bool isPointInsideField( std::pair<int, int> point );

	// Is monster moving
	bool isMoving( ) const;

	// Set monster's position
	void setPosition( hgeVector position );
	
	// Set monster's direction
	void setDirection( hgeVector direction );
	
	// Set monster's destination
	void setDestination( hgeVector destination );

	// Set monster's movement
	void setMoving( bool moving = true );	
};

inline hgeVector Monster::getPosition( ) const
{
	return m_vPos;
}

inline hgeVector Monster::getDirection( ) const
{
	return m_vDir;
}

inline hgeVector Monster::getDestination( ) const
{
	return m_vDest;
}

inline bool Monster::isMoving( ) const
{
	return m_bIsMoving;
}

inline void Monster::setPosition( hgeVector position )
{
	m_vPos = position;
}

inline void Monster::setDirection( hgeVector direction )
{
	m_vDir = direction;
}

inline void Monster::setDestination( hgeVector destination )
{
	m_vDest = destination;
}

inline void Monster::setMoving ( bool moving )
{
	m_bIsMoving = moving;
}

inline std::vector<std::pair<int, int>>& Monster::getPath( )
{
	return m_vPath;
}

#endif
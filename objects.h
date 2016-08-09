#ifndef _OBJECTS_H_
#define _OBJECTS_H_

#include "service.h"

class Monster;

class Objects
{
private:
	// Unique pointer to the monster
	std::unique_ptr<Monster> m_upMonster;
	
	// Init monster
	void initMonster( );

	// Render monster
	void renderMonster( );

	// Monster's frame valculations
	void frameMonster( );

	// Prepare game objects
	void prepareObjects( );

	// Release all allocated memory
	void release( );

	// Private constructor
	Objects( );

	// Scene instance
	static Objects* instance;
public:
	// Destructor
	~Objects( );

	// Get Objects instance
	static Objects* getInstance( );

	// Frame calculations
	void frameObjects( );

	// Render objects
	void renderObjects( );
	
	// Prepare monster to move
	void prepareMonsterToMove( );

	// Get monster
	std::unique_ptr<Monster>& getMonster( );
};

inline std::unique_ptr<Monster>& Objects::getMonster( )
{
	return m_upMonster;
}

#endif

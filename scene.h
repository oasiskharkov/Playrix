#ifndef _SCENE_H_
#define _SCENE_H_

#include "service.h"

class Scene
{
	// Field number 
	int m_nFieldNumber;

	// Floor tile texture
	HTEXTURE m_hFloorTileTex;

	// Block texture
	HTEXTURE m_hBlockTex;

	// Cursor texture
	HTEXTURE m_hCursorTex;

	// Cell selection texture
	HTEXTURE m_hCellSelectionTex;

	// Path circle texture
	HTEXTURE m_hCircleTex;
	
	// Unique pointer to the floor tile sprite
	std::unique_ptr<hgeSprite> m_upFloorTile;

	// Unique pointer to the block sprite
	std::unique_ptr<hgeSprite> m_upBlock;

	// Unique pointer to the cursor sprite
	std::unique_ptr<hgeSprite> m_upCursor;

	// Unique pointer to the cell selection animation
	std::unique_ptr<hgeAnimation> m_upCellSelectionAni;

	// Unique pointer to the path circle
	std::unique_ptr<hgeSprite> m_upCircle;

	// Unique pointer to the font 
	std::unique_ptr<hgeFont> m_upFont;

	// Cell centers
	char* cellCenters;

	// Prepare all scene sources
	bool prepareSources( );

	// Release all sources
	void release( );

	// Render floor tiles
	void renderFloorTiles( );

	// Render monster trajectory
	void renderPath( );

	// Render cell selection animation
	void renderCellSelection( );

	// Fill array with cell centers
	void fillCellCenters( );

	// Read data from source *.txt file
	void readCellsFromFile( char* centers, const char* filename ); 

	// Posibility to setup an obstacle
	bool canSetupObstacle( );

	// Private constructor
	Scene( int fieldNumber );

	// Scene instance
	static Scene* instance;
public:
	// Destructor
	~Scene( );
	
	// Get scene instance
	static Scene* getInstance( int fieldNumber );

	// Render scene
	void renderScene( );

	// Render curcor
	void renderCursor( );

	// Make an obstacle
	void makeObstacle( );

	// Get cell centers
	char* getCellCenters( );

	// Get center of the selected cell
	static hgeVector getSelectedCellCenter( );

	// Get indices of the selected cell
	static std::pair<int, int> getCellIndices( hgeVector pos );

	// Get cell's center coordinates by indices
	static hgeVector getCenterByIndices( std::pair<int, int> indices );
};

inline bool Scene::canSetupObstacle( )
{
	return m_nFieldNumber == 4;
}

inline char* Scene::getCellCenters( )
{
	return cellCenters;
}

#endif


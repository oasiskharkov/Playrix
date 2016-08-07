#ifndef _INPUT_H_
#define _INPUT_H_

#include "Service.h"

class Input
{
private:
	// Old left mouse button state
	static bool oldLeftMouseButtonState;

	// Old right mouse button state
	static bool oldRightMouseButtonState;
public:
	// Handle all tank control keys
	static void handleTankControlKeys( );

	// Get current mouse position 
	static hgeVector getMousePosition( );

	// Handle left mouse button click
	static void handleLeftMouse( );

	// Handle right mouse button click
	static void handleRightMouse( );

	// Hangle '1' key
	static bool handle1( );

	// Handle '2' key
	static bool handle2( );

	// Handle '3' key
	static bool handle3( );

	// Handle '4' key
	static bool handle4( );

	// Handle 'Esc' key
	static bool handleEsc( );
};

#endif

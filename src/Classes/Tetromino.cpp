#include "Tetromino.h"

using namespace std;
USING_NS_CC;

Tetromino::Tetromino(vector<Vec2> tileCoords) :
	m_Tiles(tileCoords),
	m_RotationPointPos(0, 0),
	m_Color( 1, 1, 1, 1 ),
	m_RotationTypeX(RotationType_NONE),
	m_RotationTypeY(RotationType_NONE),
	m_RotationState(1)
{
}


Tetromino::~Tetromino()
{
}

void Tetromino::Rotate()
{
	//loop through all of the points surrounding rotation point
	for (size_t i = 0; i < m_Tiles.size(); ++i)
	{
		//get original coords of tile
		float originalX = m_Tiles[i].x;
		float originalY = m_Tiles[i].y;
		float rotatedX = originalX;
		float rotatedY = originalY;

		//rotation formula for (0,X) or (X,0) tiles
		if (originalX == 0 || originalY == 0)
		{
			//swap the x and y coordinates
			rotatedX = originalY;
			rotatedY = originalX;

			//if swapping from x to y, switch positive/negative value
			if (originalX != 0)
				rotatedY *= -1;
		}
		//rotation formula for (1,1) tiles
		else if (originalX != 0 && originalY != 0)
		{
			//rotation types of coordinate determines when the coordinate switches their positive/negative value
			//if rotation type is even, it switches when rotation state is 2 or 4
			//if rotation type is odd, it switches when rotation state is 1 or 3

			if (m_RotationTypeX == RotationType_Even)
			{
				if ((m_RotationState % 2) == 0)
					rotatedX = originalX * -1;
			}
			else if (m_RotationTypeX == RotationType_Odd)
			{
				if ((m_RotationState % 2) != 0)
					rotatedX = originalX * -1;
			}

			if (m_RotationTypeY == RotationType_Even)
			{
				if ((m_RotationState % 2) == 0)
					rotatedY = originalY * -1;
			}
			else if (m_RotationTypeY == RotationType_Odd)
			{
				if ((m_RotationState % 2) != 0)
					rotatedY = originalY * -1;
			}
		}

		//set new rotated coordinates
		m_Tiles[i].x = rotatedX;
		m_Tiles[i].y = rotatedY;
	}

	//increment rotation state and reset back to 1 if past max
	m_RotationState++;

	if (m_RotationState > MAX_ROTATION_STATE)
		m_RotationState = 1;
}

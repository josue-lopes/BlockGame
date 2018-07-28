#pragma once

#include "cocos2d.h"

enum RotationType
{
	RotationType_Even,	//(1,1) tiles switch from negative/positive after 2nd/4th states
	RotationType_Odd,   //(1,1) tiles switch from negative/positive after 1st/3rd states
	RotationType_NONE = -1
};

class Board;

class Tetromino
{
public:
	Tetromino(std::vector<cocos2d::Vec2> tileCoords);
	~Tetromino();

	//methods
	void Rotate();											//rotates all tiles around rotation point clockwise

	//inline methods
	cocos2d::Vec2 GetRotationPointPos();					//get/set rotation tile position
	void SetRotationPointPos(cocos2d::Vec2 pos);
	cocos2d::Color4F GetColor();							//get/set piece color
	void SetColor(float r, float g, float b);
	std::vector<cocos2d::Vec2> GetOtherTiles();				//get vector with all surrounding tile offsets in relation to rotation tile
	void SetOtherTiles(std::vector<cocos2d::Vec2> &tiles);	//set other tile offsets
	void SetRotationType(RotationType x, RotationType y);	//set the rotation type which determines when an offset switches from positive/negative


private:
	//const members
	const unsigned int MAX_ROTATION_STATE = 4;

	//const members
	std::vector<cocos2d::Vec2> m_Tiles; //stores coordinates in relation to rotation point
	cocos2d::Vec2 m_RotationPointPos;   //rotation point position
	cocos2d::Color4F m_Color;			//color of the block
	RotationType m_RotationTypeX;		//rotation type for x/y coordinates determines how (1,1) tiles move
	RotationType m_RotationTypeY;
	unsigned int m_RotationState;		//stores which rotation state out of 4 the block is in
};

inline cocos2d::Vec2 Tetromino::GetRotationPointPos()
{
	return m_RotationPointPos;
}

inline void Tetromino::SetRotationPointPos(cocos2d::Vec2 pos)
{
	m_RotationPointPos = pos;
}

inline cocos2d::Color4F Tetromino::GetColor()
{
	return m_Color;
}

inline void Tetromino::SetColor(float r, float g, float b)
{
	cocos2d::Color4F color = { r, g, b, 1 };
	m_Color = color;
}

inline std::vector<cocos2d::Vec2> Tetromino::GetOtherTiles()
{
	return m_Tiles;
}

inline void Tetromino::SetOtherTiles(std::vector<cocos2d::Vec2> &tiles)
{
	m_Tiles = tiles;
}

inline void Tetromino::SetRotationType(RotationType x, RotationType y)
{
	m_RotationTypeX = x;
	m_RotationTypeY = y;
}
#pragma once

#include "cocos2d.h"

struct BoardTile
{
	int xPos = 0;			//position of tile on screen		
	int yPos = 0;	
	bool isSolid = false;	//whether tile is empty or not
	cocos2d::Color4F tileColor = { 1, 1, 1, 1 };
};

class Board
{
public:
	Board();
	~Board();

	//methods
	cocos2d::DrawNode* DrawTiles(cocos2d::DrawNode* boardDrawNode);		//loop through tiles on board and draw
	void SetTileSolid(float posX, float posY, bool flag);				//set tile at position or coord to be solid/empty
	void SetTileSolid(int coordX, int coordY, bool flag);
	void ClearLines();													//check for any full rows, and clear them
	bool IsGameOver();													//check if board tiles have reached spawn row to determine game over state
	void ResetBoard();													//set all tiles on board to empty

	//inline methods
	cocos2d::Vec2 GetCoordsFromPosition(int x, int y);					//return tile coordinates from position
	BoardTile GetTileFromPosition(int x, int y);						//return tile at position or coordinates
	BoardTile GetTileFromCoords(int x, int y);
	cocos2d::Vec2 GetSpawnLocation();									//return spawn location for new blocks
	int GetNumberOfColumns();											//return # of columns
	void SetTileColor(float posX, float posY, cocos2d::Color4F color);	//set tile color at position or coordinates
	void SetTileColor(int coordX, int coordY, cocos2d::Color4F color);

private:
	//const members
	const int TILE_SIZE = 40;
	const int BOARD_WIDTH = 400;
	const int BOARD_HEIGHT = 800;
	const int NUM_ROWS = BOARD_HEIGHT / TILE_SIZE;
	const int NUM_COLUMNS = BOARD_WIDTH / TILE_SIZE;
	const int NUM_TILES = NUM_ROWS * NUM_COLUMNS;
	const int BOARD_OFFSET = 250; //TODO: get rid of hard offset

	//members
	std::vector<BoardTile> m_Board;		//stores all board info and whether board is filled or not	
	cocos2d::Vec2 m_SpawnLocation;		//spawn location where the rotate point of the new block will start
	int m_NumberOfSolidBlocks;			//number of solid blocks total on the board

	//methods
	void Init();
	void ShiftRowsDown(std::vector<int> &clearedRows);	//shift all tiles above cleared rows down
	
	//inline methods
	int GetIndexFromPosition(float x, float y);		//return vector index from position or coordinates
	int GetIndexFromCoords(int x, int y);
};

inline int Board::GetIndexFromPosition(float x, float y)
{
	cocos2d::Vec2 coords = GetCoordsFromPosition(x, y);
	return GetIndexFromCoords(coords.x, coords.y);
}

inline int Board::GetIndexFromCoords(int x, int y)
{
	return y * NUM_COLUMNS + x;
}

inline cocos2d::Vec2 Board::GetCoordsFromPosition(int x, int y)
{
	int xCoord = (x - BOARD_OFFSET) / TILE_SIZE;
	int yCoord = y / TILE_SIZE;

	return cocos2d::Vec2(xCoord, yCoord);
}

inline BoardTile Board::GetTileFromPosition(int x, int y)
{
	int index = GetIndexFromPosition(x, y);
	return m_Board[index];
}

inline BoardTile Board::GetTileFromCoords(int x, int y)
{
	int index = y * NUM_COLUMNS + x;
	return m_Board[index];
}

inline cocos2d::Vec2 Board::GetSpawnLocation()
{
	return m_SpawnLocation;
}

inline int Board::GetNumberOfColumns()
{
	return NUM_COLUMNS;
}

inline void Board::SetTileColor(float posX, float posY, cocos2d::Color4F color)
{
	int index = GetIndexFromPosition(posX, posY);
	m_Board[index].tileColor = color;
}

inline void Board::SetTileColor(int coordX, int coordY, cocos2d::Color4F color)
{
	int index = GetIndexFromCoords(coordX, coordY);
	m_Board[index].tileColor = color;
}

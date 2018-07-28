#include "Board.h"

using namespace std;
USING_NS_CC;

Board::Board() :
	m_SpawnLocation(0, 0),
	m_NumberOfSolidBlocks(0)
{
	Init();
}


Board::~Board()
{
}

void Board::Init()
{
	//fill the vector with instances of tiles
	for (int i = 0; i < NUM_ROWS; ++i)
	{
		for (int j = 0; j < NUM_COLUMNS; ++j)
		{
			BoardTile t;
			t.xPos = j * TILE_SIZE + BOARD_OFFSET;
			t.yPos = i * TILE_SIZE;
			m_Board.push_back(t);
		}
	}

	//set up spawn location
	int index = m_Board.size() - 1;
	index -= (NUM_COLUMNS * 4);
	index -= (NUM_COLUMNS / 2);

	Vec2 spawnPos;
	spawnPos.x = m_Board[index].xPos;
	spawnPos.y = m_Board[index].yPos;
	m_SpawnLocation = spawnPos;
}

DrawNode* Board::DrawTiles(DrawNode* boardDrawNode)
{
	//clear draw node first
	auto drawNode = boardDrawNode;
	drawNode->clear();

	//loop through all tiles and draw filled/empty rectangles
	for (size_t i = 0; i < m_Board.size(); ++i)
	{
		float x = static_cast<float>(m_Board[i].xPos);
		float y = static_cast<float>(m_Board[i].yPos);

		Vec2 origin = { x, y };
		Vec2 dst = { x + TILE_SIZE, y + TILE_SIZE };
		
		if (!m_Board[i].isSolid)
			drawNode->drawRect(origin, dst, m_Board[i].tileColor);
		else 
			drawNode->drawSolidRect(origin, dst, m_Board[i].tileColor);
	}

	return drawNode;
}

void Board::SetTileSolid(float posX, float posY, bool flag)
{
	int index = GetIndexFromPosition(posX, posY);
	m_Board[index].isSolid = flag;

	if (flag)
		++m_NumberOfSolidBlocks;
	else
		--m_NumberOfSolidBlocks;
}

void Board::SetTileSolid(int coordX, int coordY, bool flag)
{
	int index = GetIndexFromCoords(coordX, coordY);
	m_Board[index].isSolid = flag;

	if (flag)
		++m_NumberOfSolidBlocks;
	else
		--m_NumberOfSolidBlocks;
}

void Board::ClearLines()
{
	//if number of solid blocks not equal to a full row, return
	if (m_NumberOfSolidBlocks < NUM_COLUMNS)
		return;

	//set up of variables used to iterate through board and find solid rows
	int xCoord = 0;
	int yCoord = 0;
	int solidBlocksFoundTotal = 0;
	int solidBlocksFoundRow = 0;
	vector<int> clearedRows;		//stores y coordinate of cleared rows

	for (int i = 0; i < NUM_TILES; ++i)
	{
		if (GetTileFromCoords(xCoord, yCoord).isSolid)
		{
			//if solid tile found, incrememnt solid row and total values
			++solidBlocksFoundTotal;
			++solidBlocksFoundRow;

			if (solidBlocksFoundRow == NUM_COLUMNS)
			{
				int rowIndex = 0;

				//if all tiles in row are solid, loop through and set row to be empty
				while (rowIndex != NUM_COLUMNS)
				{
					SetTileSolid(rowIndex, yCoord, false);
					SetTileColor(rowIndex, yCoord, Color4F::WHITE);
					++rowIndex;
					--solidBlocksFoundTotal;	//important to decrement total when blocks are removed
				}

				//push y coordinate of cleared row onto vector
				clearedRows.push_back(yCoord);
			}
		}

		//return early if no more solid blocks to check
		if (solidBlocksFoundTotal == m_NumberOfSolidBlocks)
		{
			//if rows were cleared, shift the remaining board down
			if (!clearedRows.empty())
				ShiftRowsDown(clearedRows);
			
			return;
		}

		//incrememnt to next tile
		++xCoord;

		//if at end of row, increment up to next row and reset row value
		if (xCoord == NUM_COLUMNS)
		{
			xCoord = 0;
			solidBlocksFoundRow = 0;
			++yCoord;
		}

		//if no more rows, return
		if (yCoord == NUM_ROWS)
			return;
	}
}

bool Board::IsGameOver()
{
	int yCoord = GetCoordsFromPosition(m_SpawnLocation.x, m_SpawnLocation.y).y;
	int xCoord = 0;

	//loop through all tiles in spawn row to check for solid tiles
	while (xCoord != NUM_COLUMNS)
	{
		//if any solid tiles, return game over
		if (GetTileFromCoords(xCoord, yCoord).isSolid)
		{
			return true;
		}

		++xCoord;
	}

	return false;
}

void Board::ResetBoard()
{
	//set all tiles to be empty
	for (auto it = m_Board.begin(); it != m_Board.end(); ++it)
	{
		it->isSolid = false;
		it->tileColor = Color4F::WHITE;
	}

	//reset the number of solid blocks
	m_NumberOfSolidBlocks = 0;
}


void Board::ShiftRowsDown(std::vector<int> &clearedRows)
{
	//loop through cleared rows from the top-down
	for (auto it = clearedRows.rbegin(); it != clearedRows.rend(); ++it )
	{
		//start at row above cleared row
		int startIndex = GetIndexFromCoords(0, *it + 1);
		
		//loop through all rows above cleared row and move tiles down
		for (size_t i = startIndex; i < m_Board.size(); ++i)
		{
			int tileBelowIndex = i - NUM_COLUMNS;
			
			m_Board[tileBelowIndex].isSolid = m_Board[i].isSolid;
			m_Board[i].isSolid = false;
			
			m_Board[tileBelowIndex].tileColor = m_Board[i].tileColor;
			m_Board[i].tileColor = Color4F::WHITE;
		}
	}
}


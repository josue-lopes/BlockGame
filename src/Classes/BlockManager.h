#pragma once

#include "Tetromino.h"

class Board;

enum TetrominoType
{
	I_PIECE,
	J_PIECE,
	L_PIECE,
	BOX_PIECE,
	S_PIECE,
	Z_PIECE,
	T_PIECE
};

class BlockManager
{
public:
	BlockManager();
	~BlockManager();

	//methods
	void MovePiece(int direction);							 //moves piece left/right
	void RotatePiece();										 //rotates piece clockwise

	//draw/update functions
	void Update(float deltaTime);
	cocos2d::DrawNode* Draw(cocos2d::DrawNode* blockDrawNode);

	//inline methods
	void DropNewPiece(cocos2d::Vec2 startLoc);				 //grab a new random block and spawn at location
	void ResetPiece();										 //resets pointer to main piece
	void ToggleFastFall(bool flag);							 //toggle variable for block to fast fall
	void SetBoardReference(std::shared_ptr<Board> boardRef); //set reference to the board to access certain function

private:
	//const members
	const int NUM_UNIQUE_PIECES = 7;
	const int BLOCK_SIZE = 40;
	const float MOVEMENT_TIME = 2;
	const float FAST_MOVEMENT_TIME = MOVEMENT_TIME / 10;

	//members
	std::map<TetrominoType, std::shared_ptr<Tetromino>> m_UniquePieces;		//map which stores each unique tetromino shape
	std::shared_ptr<Tetromino> m_CurrentPiece;								//current tetromino which is being controlled
	float m_DropTimer;														//timer which increments up to a certain point before the main block drops another row
	bool m_IsFastFalling;													//if true, piece moves faster downwards
	std::shared_ptr<Board> m_BoardRef;										//reference to instance of board class

	//methods
	bool CanBlockMove(cocos2d::Vec2 pos);					//checks whether all tiles of block can move to empty tiles at location
	void AddBlockToBoard();									//adds block to board when it can no longer move
	void CreateInitialUniqueBlocks();						//initial function called in constructor to fill map with unique tetrominos

	//inline methods
	std::shared_ptr<Tetromino> GetRandomTetrominoBlock();	//initialize new random tetromino block to begin falling
};

inline void BlockManager::DropNewPiece(cocos2d::Vec2 startLoc)
{
	m_CurrentPiece = GetRandomTetrominoBlock();
	m_CurrentPiece->SetRotationPointPos(startLoc);
}

inline void BlockManager::ResetPiece()
{
	m_CurrentPiece = nullptr;
}

inline void BlockManager::ToggleFastFall(bool flag)
{
	m_IsFastFalling = flag;
}

inline void BlockManager::SetBoardReference(std::shared_ptr<Board> boardRef)
{
	m_BoardRef = boardRef;
}

inline std::shared_ptr<Tetromino> BlockManager::GetRandomTetrominoBlock()
{
	//get random block type
	TetrominoType randomType = static_cast<TetrominoType>(cocos2d::RandomHelper::random_int(0, 6));

	//return pointer to random block
	return m_UniquePieces.at(randomType);
}


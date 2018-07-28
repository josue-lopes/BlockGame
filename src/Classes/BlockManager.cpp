#include "BlockManager.h"
#include "Board.h"

using namespace std;
USING_NS_CC;

BlockManager::BlockManager() :
	m_CurrentPiece(nullptr),
	m_DropTimer(0.0f),
	m_IsFastFalling(false),
	m_BoardRef(nullptr)
{
	CreateInitialUniqueBlocks();
}


BlockManager::~BlockManager()
{
}

void BlockManager::MovePiece(int direction)
{
	Vec2 newPos = m_CurrentPiece->GetRotationPointPos();
	newPos.x += BLOCK_SIZE * direction;

	if (CanBlockMove(newPos))
		m_CurrentPiece->SetRotationPointPos(newPos);
}

void BlockManager::RotatePiece()
{
	//return if box piece
	if (m_CurrentPiece->GetColor() == Color4F::YELLOW)
		return;

	//store old positions of tiles before rotation
	vector<Vec2> oldRotation = m_CurrentPiece->GetOtherTiles();

	m_CurrentPiece->Rotate();

	//if not able to rotate to empty tiles, revert to old rotation
	if (!CanBlockMove(m_CurrentPiece->GetRotationPointPos()))
		m_CurrentPiece->SetOtherTiles(oldRotation);
}

void BlockManager::Update(float deltaTime)
{
	//return if no current piece is set
	if (!m_CurrentPiece)
		return;

	//drop block down a row after a certain amount of time has past
	m_DropTimer += deltaTime;

	if (m_DropTimer >= MOVEMENT_TIME ||
		(m_DropTimer >= FAST_MOVEMENT_TIME && m_IsFastFalling))
	{
		Vec2 newPos = m_CurrentPiece->GetRotationPointPos();
		newPos.y -= BLOCK_SIZE;

		//add block to board once piece can't move anymore downwards
		if (CanBlockMove(newPos))
			m_CurrentPiece->SetRotationPointPos(newPos);
		else
			AddBlockToBoard();

		m_DropTimer = 0;
	}
}

DrawNode* BlockManager::Draw(DrawNode* blockDrawNode)
{
	//clear draw node first  
	auto drawNode = blockDrawNode;
	drawNode->clear();

	if (!m_CurrentPiece)
		return drawNode;

	//draw main tile first
	Vec2 rotationBlockOrigin = m_CurrentPiece->GetRotationPointPos();
	Vec2 rotationBlockDst = Vec2(rotationBlockOrigin.x + BLOCK_SIZE, rotationBlockOrigin.y + BLOCK_SIZE);
	drawNode->drawSolidRect(rotationBlockOrigin, rotationBlockDst, m_CurrentPiece->GetColor());

	vector<Vec2> otherBlocks = m_CurrentPiece->GetOtherTiles();

	//loop through and draw other tiles
	for (size_t i = 0; i < otherBlocks.size(); ++i)
	{
		Vec2 otherBlockOrigin;
		otherBlockOrigin.x = rotationBlockOrigin.x + otherBlocks[i].x * BLOCK_SIZE;
		otherBlockOrigin.y = rotationBlockOrigin.y + otherBlocks[i].y * BLOCK_SIZE;

		Vec2 otherBlockDst = Vec2(otherBlockOrigin.x + BLOCK_SIZE, otherBlockOrigin.y + BLOCK_SIZE);

		drawNode->drawSolidRect(otherBlockOrigin, otherBlockDst, m_CurrentPiece->GetColor());
	}

	return drawNode;
}

bool BlockManager::CanBlockMove(Vec2 pos)
{
	//check for main rotation point first
	Vec2 mainTileCoords = m_BoardRef->GetCoordsFromPosition(pos.x, pos.y);

	if (mainTileCoords.x >= m_BoardRef->GetNumberOfColumns() || mainTileCoords.x < 0 || mainTileCoords.y < 0)
		return false;

	BoardTile tileAtLocation = m_BoardRef->GetTileFromCoords(mainTileCoords.x, mainTileCoords.y);

	if (tileAtLocation.isSolid)
		return false;

	//loop through other tiles if main piece can move
	vector<Vec2> otherTiles = m_CurrentPiece->GetOtherTiles();

	for (size_t i = 0; i < otherTiles.size(); ++i)
	{
		float otherTileX = pos.x + otherTiles[i].x * BLOCK_SIZE;
		float otherTileY = pos.y + otherTiles[i].y * BLOCK_SIZE;

		Vec2 otherTileCoords = m_BoardRef->GetCoordsFromPosition(otherTileX, otherTileY);

		if (otherTileCoords.x >= m_BoardRef->GetNumberOfColumns() || otherTileCoords.x < 0 || otherTileCoords.y < 0)
			return false;

		BoardTile otherTileLocation = m_BoardRef->GetTileFromCoords(otherTileCoords.x, otherTileCoords.y);

		if (otherTileLocation.isSolid)
			return false;
	}

	//if no tiles move beyond board limits or into occupied spaces, return true
	return true;
}

void BlockManager::AddBlockToBoard()
{
	//Add rotation tile first
	Vec2 rotBlockPos = m_CurrentPiece->GetRotationPointPos();
	m_BoardRef->SetTileSolid(rotBlockPos.x, rotBlockPos.y, true);
	m_BoardRef->SetTileColor(rotBlockPos.x, rotBlockPos.y, m_CurrentPiece->GetColor());

	vector<Vec2> otherBlocks = m_CurrentPiece->GetOtherTiles();

	//loop and add other tiles
	for (size_t i = 0; i < otherBlocks.size(); ++i)
	{
		Vec2 otherBlockPos;
		otherBlockPos.x = rotBlockPos.x + otherBlocks[i].x * BLOCK_SIZE;
		otherBlockPos.y = rotBlockPos.y + otherBlocks[i].y * BLOCK_SIZE;

		m_BoardRef->SetTileSolid(otherBlockPos.x, otherBlockPos.y, true);
		m_BoardRef->SetTileColor(otherBlockPos.x, otherBlockPos.y, m_CurrentPiece->GetColor());
	}

	//reset the piece and drop new piece
	ResetPiece();

	//check for game over condition
	//if false, check for any cleared rows. if true, clear entire board
	bool gameOver = m_BoardRef->IsGameOver();
	if (!gameOver)
		m_BoardRef->ClearLines();
	else
		m_BoardRef->ResetBoard();

	DropNewPiece(m_BoardRef->GetSpawnLocation());
}

void BlockManager::CreateInitialUniqueBlocks()
{
	for (int i = 0; i < NUM_UNIQUE_PIECES; ++i)
	{
		vector<Vec2> coords;
		Color4F color;
		RotationType rotationTypeX = RotationType_NONE;
		RotationType rotationTypeY = RotationType_NONE;

		//add unique coordinates, color and rotation types for each piece in relation to rotation point
		if (i == I_PIECE)
		{
			coords = { Vec2(-1,0), Vec2(1,0), Vec2(2,0) };
			color = { 0, 1, 1, 1 };
		}
		else if (i == J_PIECE)
		{
			coords = { Vec2(-1,0), Vec2(1,0), Vec2(1,-1) };
			color = color.BLUE;
			rotationTypeX = RotationType_Odd;
			rotationTypeY = RotationType_Even;
		}
		else if (i == L_PIECE)
		{
			coords = { Vec2(1,0), Vec2(-1,0), Vec2(-1,-1) };
			color = color.ORANGE;
			rotationTypeX = RotationType_Even;
			rotationTypeY = RotationType_Odd;
		}
		else if (i == BOX_PIECE)
		{
			coords = { Vec2(1,0), Vec2(0,-1), Vec2(1,-1) };
			color = color.YELLOW;
		}
		else if (i == S_PIECE)
		{
			coords = { Vec2(1,0), Vec2(0,-1), Vec2(-1,-1) };
			color = color.GREEN;
			rotationTypeX = RotationType_Even;
			rotationTypeY = RotationType_Odd;
		}
		else if (i == Z_PIECE)
		{
			coords = { Vec2(-1,0), Vec2(0,-1), Vec2(1,-1) };
			color = color.RED;
			rotationTypeX = RotationType_Odd;
			rotationTypeY = RotationType_Even;
		}
		else if (i == T_PIECE)
		{
			coords = { Vec2(-1,0), Vec2(0,-1), Vec2(1,0) };
			color = color.MAGENTA;
		}

		TetrominoType key = static_cast<TetrominoType>(i);									 //cast iterator into enum key
		shared_ptr<Tetromino> uniqueBlock = make_shared<Tetromino>(coords);					 //create pointer to new unique tetromino block and input coords
		uniqueBlock->SetColor(color.r, color.g, color.b);									 //set unique color of block
		uniqueBlock->SetRotationType(rotationTypeX, rotationTypeY);							 //set rotation type for x and y coordinates
		m_UniquePieces.insert(pair<TetrominoType, shared_ptr<Tetromino>>(key, uniqueBlock)); //insert piece into map
	}
}

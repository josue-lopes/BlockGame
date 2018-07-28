#include "GameScene.h"

using namespace std;
USING_NS_CC;

GameScene::GameScene() :
	m_BlockManager(new BlockManager()),
	m_MainBlock(nullptr),
	m_BoardDrawNode(nullptr)
{
	m_Board = make_shared<Board>();
	m_BlockManager->SetBoardReference(m_Board);
}


GameScene::~GameScene()
{
}

Scene* GameScene::createScene()
{
	//Our GameScene class inherits from Layer which is a node class that can recieve input events
	//This function creates a scene with our class
	auto scene = Scene::create();
	auto layer = GameScene::create();

	scene->addChild(layer);

	return scene;
}

bool GameScene::init()
{
	//first we call base class init, this is important
	if (!Layer::init())
	{
		return false;
	}

	//create keyboard event listener
	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(GameScene::OnKeyPressed, this);
	keyboardListener->onKeyReleased = CC_CALLBACK_2(GameScene::OnKeyReleased, this);

	//add keyboard listener to event dispatcher
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener, this);

	//draw empty board
	m_BoardDrawNode = DrawNode::create();
	m_Board->DrawTiles(m_BoardDrawNode);
	this->addChild(m_BoardDrawNode);
	
	//spawn new player block
	m_BlockManager->DropNewPiece(m_Board->GetSpawnLocation());

	//draw player block
	m_MainBlock = DrawNode::create();
	m_BlockManager->Draw(m_MainBlock);
	this->addChild(m_MainBlock);

	//start update
	this->scheduleUpdate();

	return true;
}

void GameScene::update(float delta)
{
	m_BlockManager->Update(delta);
	DrawLogic();
}

void GameScene::DrawLogic()
{
	m_Board->DrawTiles(m_BoardDrawNode);
	m_BlockManager->Draw(m_MainBlock);
}

void GameScene::OnKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	int direction = 0;

	//KEYBOARD CONTROLS
	//left and right arrows to move block
	//up arrow to rotate, down arrow to fast fall

	if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
		direction = -1;
	else if (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
		direction = 1;
	else if (keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW)
		m_BlockManager->ToggleFastFall(true);
	else if (keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW)
		m_BlockManager->RotatePiece();
		
	if (direction != 0)
		m_BlockManager->MovePiece(direction);
}

void GameScene::OnKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW)
		m_BlockManager->ToggleFastFall(false);
}

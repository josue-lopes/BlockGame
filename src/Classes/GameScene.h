#pragma once

#include "cocos2d.h"
#include "Board.h"
#include "BlockManager.h"

class GameScene : cocos2d::Layer
{
public:
	GameScene();
	~GameScene();

	static cocos2d::Scene* createScene();
	bool init();

	//this macro creates an instance of our class, calls init and disposes of the memory so there are no leaks
	CREATE_FUNC(GameScene);

	void update(float delta);	
	void DrawLogic();			//all draw calls in update stored here

	//Input events
	void OnKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void OnKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

private:
	std::shared_ptr<Board> m_Board;					//instance which handles all board-related tasks
	std::unique_ptr<BlockManager> m_BlockManager;	//instance which handles all tasks related to player-controlled block
	cocos2d::DrawNode* m_MainBlock;					//nodes which update all draw information
	cocos2d::DrawNode* m_BoardDrawNode;				
};


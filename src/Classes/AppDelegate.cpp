#include "AppDelegate.h"
#include "GameScene.h"

USING_NS_CC;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching()
{
	auto director = Director::getInstance();
	auto glView = director->getOpenGLView();

	if (!glView)
	{
		glView = GLViewImpl::create("BlockGame");
		director->setOpenGLView(glView);
	}

	auto scene = GameScene::createScene();
	director->runWithScene(scene);

	return true;
}

void AppDelegate::applicationDidEnterBackground()
{
}

void AppDelegate::applicationWillEnterForeground()
{
}

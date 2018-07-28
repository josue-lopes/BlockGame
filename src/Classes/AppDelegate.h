#pragma once

#include "cocos2d.h"

class AppDelegate : cocos2d::Application
{
public:
	AppDelegate();
	~AppDelegate();

	bool applicationDidFinishLaunching();
	void applicationDidEnterBackground();
    void applicationWillEnterForeground();
};

#pragma once

#include "GameObject.h"

class Platform : public GameObject
{
	public: 
	Ogre::SceneNode* CubeNode;

	Platform(Ogre::SceneManager* scnMgr, Ogre::SceneNode* platformNode);
	bool frameStarted(const Ogre::FrameEvent& evt);
};


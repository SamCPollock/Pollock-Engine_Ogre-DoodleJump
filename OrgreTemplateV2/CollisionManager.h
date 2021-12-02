#pragma once

#include "Ogre.h"
#include "GameObject.h"

class CollisionManager
{
	public: 
	CollisionManager();
	static bool CheckForIntersect(Ogre::SceneNode* objectA, Ogre::SceneNode* objectB);
};


#include "CollisionManager.h"



CollisionManager::CollisionManager()
{

}

/// This is the CheckForIntersect function. 
/// This takes two SceneNodes and returns true if they overlap. 
bool CollisionManager::CheckForIntersect(Ogre::SceneNode* objectA, Ogre::SceneNode* objectB)
{
// TESTING COLLISION USING INTERSECTS
	if (objectA->_getWorldAABB().intersects(objectB->_getWorldAABB()))
	{
		return true;
	}

}

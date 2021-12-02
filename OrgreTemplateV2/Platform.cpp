#include "Platform.h"
#include <iostream>


///  This is the platform constructor.
/// It uses Height and Width variables to set the size of a 3d Rectangle.

Platform::Platform(Ogre::SceneManager* scnMgr, Ogre::SceneNode* platformNode)
{
	height = 0.005;
	width = 0.015;
	
	SetSceneNode(platformNode);

	Ogre::Entity* CubeEntity = scnMgr->createEntity(Ogre::SceneManager::PT_CUBE);
	CubeNode = scnMgr->getRootSceneNode()->createChildSceneNode();
	CubeNode->setPosition(0, 0, 0);
	CubeNode->setScale(width, height, width);
	CubeNode->attachObject(CubeEntity);

}


/// This is the frameStarted function. 
/// This could be used for moving platforms, but for the time being these platforms are stationary. 

bool Platform::frameStarted(const Ogre::FrameEvent& evt)
{
	CubeNode->translate(velocity * evt.timeSinceLastFrame);

	velocity = Ogre::Vector3(0, 0, 0);



	return true;
}


#include "DoodlePlayer.h"
#include <iostream>


/// <summary>
/// This is the DoodlePlayer constructor
/// </summary>
/// <param name="scnMgr"> It takes the SceneManager as a parameter</param>
/// <param name="playerNode">It also takes the playerNode as parameter</param>
/// Inside, use the height and width to set the dimensions of the player cube. 
/// Also, set the initial velocity for the player. 
DoodlePlayer::DoodlePlayer(Ogre::SceneManager* scnMgr, Ogre::SceneNode* playerNode)
{
	height = 0.01;
	width = 0.005;

	jumpForce = 10.0f;
	moveSpeed = 15.0f;

	Ogre::Entity* PlayerEntity = scnMgr->createEntity(Ogre::SceneManager::PT_CUBE);
	PlayerNode = scnMgr->getRootSceneNode()->createChildSceneNode();
	PlayerNode->setPosition(3, -3, 0);
	PlayerNode->setScale(width, height, width);
	PlayerNode->attachObject(PlayerEntity);

	velocity = Ogre::Vector3(0, 10.0f, 0);
}

/// <summary>
/// The frame started function/
/// 
/// This calls simulate physics every frame to determine the effects of gravity and velocity. 
/// It also translates the player according to their velocity. 
/// </summary>
bool DoodlePlayer::frameStarted(const Ogre::FrameEvent& evt)
{

	PlayerNode->translate(velocity * evt.timeSinceLastFrame);

	PhysicsSimulate(evt);

	// make sure player and player node are at the same position
	position.y = PlayerNode->getPosition().y;

	return true;
}

/// <summary>
///  This is the PhysicsSimulate function
/// This uses a simple physics formula to determine the velocity and acceleration of the player. 
/// @note it is using gravity of -9.8 (earth's gravity)
void DoodlePlayer::PhysicsSimulate(const Ogre::FrameEvent& evt)
{
	Ogre::Vector3 acceleration = Ogre::Vector3(0, -9.8f, 0); 

	position += evt.timeSinceLastFrame * (velocity + evt.timeSinceLastFrame * acceleration / 2);
	velocity += evt.timeSinceLastFrame * acceleration;
	velocity.x = 0;
}

/// <summary>
/// This is the OnPlayerEnter function
/// This takes in the SceneNode of the other object colliding 
/// It also takes in the soundmanager, which it uses to play sound effects on collision.
/// @note it checks to see if the player's y velocity is less than zero, to ensure that the player only bounces off platforms when they are falling.
void DoodlePlayer::OnCollisionEnter(Ogre::SceneNode* other, SoundManager* sm)
{
	if (other->getPosition().y < position.y)
	{
		if (velocity.y < 0)
		{
			std::cout << "--BOUNCE!-- \n";
			Bounce();
			sm->playSoundEffect(1, false);
		}
	}
}
/// <summary>
/// This is the bounce function. 
/// It changes the player's y velocity to be equal to their jumpForce variable. 
/// </summary>
void DoodlePlayer::Bounce()
{
	velocity.y = jumpForce;
}
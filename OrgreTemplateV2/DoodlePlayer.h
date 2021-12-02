#pragma once

#include "GameObject.h"
#include "SoundManager.h"

class DoodlePlayer : public GameObject
{
	public:
	Ogre::SceneNode* PlayerNode;

	float jumpForce;
	float moveSpeed;


	DoodlePlayer(Ogre::SceneManager* scnMgr, Ogre::SceneNode* playerNode);
	bool frameStarted(const Ogre::FrameEvent& evt);

	void PhysicsSimulate(const Ogre::FrameEvent& evt);

	void OnCollisionEnter(Ogre::SceneNode* other, SoundManager* sm);

	void Bounce();
};


#pragma once
#include "Ogre.h"
class GameObject : public Ogre::FrameListener	// make this pure virtual --- inherit from FrameListener? 
{
	public: 
	//GameObject();
	Ogre::Vector3 position;
	Ogre::Vector3 velocity;
	Ogre::SceneNode* sceneNode;
	float height;
	float width;


	// getters
	Ogre::Vector3 GetVelocity();
	Ogre::SceneNode* GetSceneNode();
	float GetHeight();
	float GetWidth();

	// setters
	void SetSceneNode(Ogre::SceneNode* node);
	void SetVelocity(Ogre::Vector3 newVelocity);
	void SetHeight(float newHeight);
	void SetWidth(float newWidth);


	// Collisions
	void HitBottom();
	void ReboundPaddle();
	void ReboundSides();

	void Update();	/// make this an Frameupdate function 
};


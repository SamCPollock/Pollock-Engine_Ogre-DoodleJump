#include "GameObject.h"

/// This is the GetVelocity function.
/// 
/// This simply returns the GameObject's velocity value.
Ogre::Vector3 GameObject::GetVelocity()
{
	return Ogre::Vector3();
}

/// This is the GetSceneNode function.
/// 
/// This simply returns the GameObject's sceneNode.
Ogre::SceneNode* GameObject::GetSceneNode()
{
	return sceneNode;
}

/// This is the GetHeight function.
/// 
/// This simply returns the GameObject's height value.
float GameObject::GetHeight()
{
	return height;
}

/// This is the GetWidth function.
/// 
/// This simply returns the GameObject's width value.
float GameObject::GetWidth()
{
	return width;
}

/// This is the SetSceneNode function.
/// 
/// This simply assings the GameObject's sceneNode.
void GameObject::SetSceneNode(Ogre::SceneNode* node)
{
	sceneNode = node;
}

/// This is the SetVelocity function.
/// 
/// This simply assigns the GameObject's velocity value.
void GameObject::SetVelocity(Ogre::Vector3 newVelocity)
{
	velocity = Ogre::Vector3(newVelocity);
}

void GameObject::SetHeight(float newHeight)
{
}

void GameObject::SetWidth(float newWidth)
{
}

void GameObject::HitBottom()
{
}

void GameObject::ReboundPaddle()
{
}

void GameObject::ReboundSides()
{
}

void GameObject::Update()
{
}

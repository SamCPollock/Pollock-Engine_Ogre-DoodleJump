/// DoodleJump in OGRE
///@note HOW TO PLAY: Move Player using A and D. 
/// Bounce on platforms to keep climbing!
/// Lose lives by falling off screen.
/// Made by Sam Pollock for Game Engine Development (Taught by Hooman Salamat) at George Brown College, fall 2021.

#include "OgreApplicationContext.h"
#include "OgreInput.h"
#include "OgreRTShaderSystem.h"
#include "OgreTrays.h"

#include "Platform.h"
#include "DoodlePlayer.h"
#include "CollisionManager.h"

#include "SoundManager.h"

#include <time.h>
#include <iostream>
#include <vector>

using namespace Ogre;
using namespace OgreBites;

Platform* m_Platform;
DoodlePlayer* m_Player;
SceneNode* camNode;

time_t start = time(0);

SoundManager soundManager;
CollisionManager collisionManager;

const int numberOfPlatforms = 8;	// TODO: figure out why this crashes if number of platforms is higher than 8.
std::vector<Platform*> platformsVector;

int lives = 3;
int score = 0;


OgreBites::TrayManager* mTrayMgr;


/// This is a LabelFrameListener class
/// 
/// this goes every frame to update labels.
class LabelsFrameListener : public Ogre::FrameListener
{
private:
	OgreBites::Label* _livesLabel;
	OgreBites::Label* _scoreLabel;
	OgreBites::Label* _timeLabel;


public:

	/// This is a LabelFrameListener constructor
	/// 
	/// It takes three labels as parameters: lives, score, and time per update. 
	LabelsFrameListener(OgreBites::Label* livesLabel, OgreBites::Label* scoreLabel, OgreBites::Label* tpuLabel)
	{
		_livesLabel = livesLabel;
		_scoreLabel = scoreLabel;
		_timeLabel = tpuLabel;

	}
	/// FrameStarted function
	/// 
	/// This updates the captions for lives and score, as well as the time since last frame.
	/// 
	bool frameStarted(const Ogre::FrameEvent& evt)
	{
		_livesLabel->setCaption("Lives: " + std::to_string(lives));
		_scoreLabel->setCaption("Height: " + std::to_string(int(m_Player->position.y)));
	
		if (time(0) - start >= 1)
		{ 
			_timeLabel->setCaption(std::to_string(evt.timeSinceLastFrame));
			start = start + 1;
		}

		return true;

	}
};

/// This is the GameLogicFrameListener class. 
/// 
/// This 
class GameLogicFrameListener : public Ogre::FrameListener
{
	protected: 

		/// Check for collisions between player and platforms
		/// This loops through each platform in the platform vector to check them against the player for collision. 
		/// If it finds a collision, it calls the OnCollisionEnter function in the player, which is used for physics.
		void CollisionCheck()
		{
			for (int i = 0; i < platformsVector.size(); i++)
			{
				if (collisionManager.CheckForIntersect(m_Player->PlayerNode, platformsVector[i]->CubeNode))
				{
					m_Player->OnCollisionEnter(platformsVector[i]->CubeNode, &soundManager);
				}
			}
		}

		/// Move platforms that go off screen
		/// This function checks each platform to see if they are at the bottom of the camera view. 
		/// If they are at the bottom of the camera view, it moves them up to the top. 
		/// When it moves them to the top, it also sets a new random x position for them.
		void MovePlatforms()
		{
			for (int i = 0; i < platformsVector.size(); i++)
			{

				if (platformsVector[i]->CubeNode->getPosition().y < camNode->getPosition().y - 5)
				{
					platformsVector[i]->CubeNode->setPosition(Ogre::Math::RangeRandom(-10, 10), camNode->getPosition().y + 7, platformsVector[i]->CubeNode->getPosition().z);
				}
			}
		};

		/// Move camera up to follow player
		/// As the player position gets above the middle of the camera, this moves the camera up to follow. 
		/// @note it does not move the camera down, so as to keep moving the gameplay upwards. 
		void CameraFollow()
		{
			if (m_Player->position.y - 1 > camNode->getPosition().y)
			{
				camNode->setPosition(camNode->getPosition().x, m_Player->position.y - 1, camNode->getPosition().z);
			}
		};

		/// Player fall off screen.
		/// This checks if the player y position is off the bottom of the camera. 
		/// If they are off the screen, it removes a life and plays the death sound. 
		/// If the player has lives left, it moves them up to the top of the screen again. 
		/// If they don't have lives left, it closes the game. 
		void CheckForPlayerFall()
		{
			if (m_Player->PlayerNode->getPosition().y < camNode->getPosition().y - 8)
			{
				lives--;
				soundManager.playSoundEffect(2, false);
				m_Player->PlayerNode->setPosition(0, m_Player->PlayerNode->getPosition().y + 10, m_Player->PlayerNode->getPosition().z);
				if (lives <= 0)
				{
					std::cout << "GAME OVER";
					exit(3);
				}
			}
		};

		/// Player wins! 
		/// This checks if the player has reached the top of the stage. 
		/// If they have, it repeatedly plays a "You win!" sound effect
		void CheckForPlayerWins()
		{
			if (m_Player->PlayerNode->getPosition().y > 50)
			{
				soundManager.playSoundEffect(3, false);
				m_Player->SetVelocity(Ogre::Vector3(0, 0, 0));
			}
		}
		
		/// This is used to call each of the other functions.
		void GameLogic()
		{
			CollisionCheck();

			MovePlatforms();

			CameraFollow();

			CheckForPlayerFall();

			CheckForPlayerWins();
		}

	public: 
		/// Every frame, this runs the Game Logic. 

		bool frameStarted(const Ogre::FrameEvent& evt)

		{
			GameLogic();
			return true;
		}

};



/// This is a DoodleAssignment class
/// 
///
class DoodleAssignment
	: public ApplicationContext
	, public InputListener
{
private:
	SceneManager* scnMgr;
	Root* root;
public:
	DoodleAssignment();
	virtual ~DoodleAssignment() {}

	void setup();
	void createScene();
	void createCamera();
	bool keyPressed(const KeyboardEvent& evt);
	void createFrameListener();
	void SpawnPlatforms(int numberOfPlatforms);  //

	OgreBites::TrayListener mTrayListener;

	OgreBites::Label* mInfoLabel;
	OgreBites::Label* mTpuLabel;
	OgreBites::Label* mTpu;
	OgreBites::Label* mScoreLabel;
	OgreBites::Label* mLivesLabel;
};


DoodleAssignment::DoodleAssignment()
	: ApplicationContext("Pollock-DoodleAssignment")
{
}

/// This is the SpawnPlatforms function.
/// This takes in an integer for number of platforms and spawns that many platforms. 
/// The variance variable is used to determine the range of y displacements between platforms. 
/// Each platform's position starts at 0,0,0 then is adjusted according to how many were made. 
/// Each platform has a random x value set, between a range determined in the setPosition. 
/// @note After each platform is created, it is added to the platformsVector for use in GameLogic. 
void DoodleAssignment::SpawnPlatforms(int numberOfPlatforms)
{
	for (int i = 0; i < numberOfPlatforms; i++)
	{
		Platform* m_newPlatform;
		String name = "Platform" + i;
		float variance = Ogre::Math::RangeRandom(0.3, 0.8);
		m_newPlatform = new Platform(scnMgr, scnMgr->createSceneNode(name));
		m_newPlatform->CubeNode->setPosition(i * variance, Ogre::Math::RangeRandom(-10, 10), m_newPlatform->CubeNode->getPosition().z);
		platformsVector.push_back(m_newPlatform);
	}
	
}

/// This is a setup function
/// 
/// This calls the functions to setup the game, including the base setup(), adding scene manager, creating the scene, creating the camera, and creating the frame listeners. 
void DoodleAssignment::setup()
{
	// do not forget to call the base first
	ApplicationContext::setup();
	addInputListener(this);


	// get a pointer to the already created root
	root = getRoot();
	scnMgr = root->createSceneManager();

	// register our scene with the RTSS
	RTShader::ShaderGenerator* shadergen = RTShader::ShaderGenerator::getSingletonPtr();
	shadergen->addSceneManager(scnMgr);
	createScene();
	createCamera();
	createFrameListener();


}

/// This is the createScene function.
/// 
/// This creates the lights and any other objects and UI in the scene. 
void DoodleAssignment::createScene()
{	
	// Music
	soundManager.addSoundEffect("C:/Users/samcp/Documents/Github Stuff/GBC Year 3/_Ogre-Assignment2-DoodleJump/OrgreTemplateV2/Assets/SoundEffects/Space Song.wav");
	
	// Sound Effects
	soundManager.addSoundEffect("C:/Users/samcp/Documents/Github Stuff/GBC Year 3/_Ogre-Assignment2-DoodleJump/OrgreTemplateV2/Assets/SoundEffects/jump.wav");
	soundManager.addSoundEffect("C:/Users/samcp/Documents/Github Stuff/GBC Year 3/_Ogre-Assignment2-DoodleJump/OrgreTemplateV2/Assets/SoundEffects/explode.wav");
	soundManager.addSoundEffect("C:/Users/samcp/Documents/Github Stuff/GBC Year 3/_Ogre-Assignment2-DoodleJump/OrgreTemplateV2/Assets/SoundEffects/YouWin.wav");

	// Play music
	soundManager.playSoundEffect(0, true);


	//! [turnlights]
	scnMgr->setAmbientLight(ColourValue(0.5, 0.5, 0.5));
	//! [turnlights]

	//! [newlight]
	//
	Light* light1 = scnMgr->createLight("Light1");
	light1->setType(Ogre::Light::LT_POINT);
	// Set Light Color
	light1->setDiffuseColour(1.0f, 1.0f, 1.0f);
	// Set Light Reflective Color
	light1->setSpecularColour(1.0f, 0.0f, 0.0f);
	// Set Light (Range, Brightness, Fade Speed, Rapid Fade Speed)
	light1->setAttenuation(10, 0.5, 0.045, 0.0);

	//
	Entity* lightEnt = scnMgr->createEntity("LightEntity", "sphere.mesh");
	SceneNode* lightNode = scnMgr->createSceneNode("LightNode");
	lightNode->attachObject(lightEnt);
	lightNode->attachObject(light1);
	lightNode->setScale(0.01f, 0.01f, 0.01f);


	scnMgr->getRootSceneNode()->addChild(lightNode);
	//! [newlight]

	//! [lightpos]
	lightNode->setPosition(0, 4, 10);
	//! [lightpos]

	m_Platform = new Platform(scnMgr, scnMgr->createSceneNode("Platform1"));
	m_Player = new DoodlePlayer(scnMgr, scnMgr->createSceneNode("Player1"));

	SpawnPlatforms(numberOfPlatforms);

	OgreBites::TrayManager* mTrayMgr = new OgreBites::TrayManager("InterfaceName", getRenderWindow());
	scnMgr->addRenderQueueListener(mOverlaySystem);
	addInputListener(mTrayMgr);

	mTrayMgr->showFrameStats(TL_BOTTOMLEFT);
	mTrayMgr->toggleAdvancedFrameStats();

	mTpuLabel = mTrayMgr->createLabel(TL_TOPRIGHT, "Time/Update", "Time/Update:", 150);
	mTpu = mTrayMgr->createLabel(TL_TOPRIGHT, "tpu", "0", 150);
	mScoreLabel = mTrayMgr->createLabel(TL_TOPLEFT, "Score", "Score: " + std::to_string(score), 100);
	mLivesLabel = mTrayMgr->createLabel(TL_TOP, "Lives", "Lives: " + std::to_string(lives), 100);


}

/// This is the createCamera function.
/// 
/// It creates the camera in the scene and sets its initial position and characteristics. 
void DoodleAssignment::createCamera()
{

	//! [camera]
	camNode = scnMgr->getRootSceneNode()->createChildSceneNode();

	// create the camera
	Camera* cam = scnMgr->createCamera("myCam");
	cam->setNearClipDistance(5); // specific to this sample
	cam->setAutoAspectRatio(true);
	camNode->attachObject(cam);
	camNode->setPosition(0, 0, 15);
	camNode->lookAt(Ogre::Vector3(0, 0, 0), Node::TS_WORLD);

	// and tell it to render into the main window
	getRenderWindow()->addViewport(cam);

	//! [camera]
}

/// This is a keyPressed function.
/// 
/// It checks for keyboard inputs and sets the velocity of the Player accordingly. 
/// @note more cases can be added to introduce additional inputs for the player. 
bool DoodleAssignment::keyPressed(const KeyboardEvent& evt)
{
	switch (evt.keysym.sym)
	{
	case SDLK_ESCAPE:
		getRoot()->queueEndRendering();
		break;
	case 'a':
		m_Player->SetVelocity(Ogre::Vector3(-m_Player->moveSpeed, m_Player->velocity.y, 0));
		break;
	case 'd':
		m_Player->SetVelocity(Ogre::Vector3(m_Player->moveSpeed, m_Player->velocity.y, 0));
		break;
	default:
		break;
	}
	return true;
}

/// This is the createFrameListener function
/// 
/// It adds each a frame listener for each game object. It also adds a frame listenr for the UI. 
/// Finally, it adds the GameLogicFrameListener, which process gameplay logic every frame.
void DoodleAssignment::createFrameListener()
{


	Ogre::FrameListener* FrameListener3 = new LabelsFrameListener(mLivesLabel, mScoreLabel, mTpu);
	mRoot->addFrameListener(FrameListener3);

	Ogre::FrameListener* GameLogicListener = new GameLogicFrameListener();
	mRoot->addFrameListener(GameLogicListener);

	Ogre::FrameListener* PlatformFrameListener = m_Platform;
	mRoot->addFrameListener(PlatformFrameListener);

	Ogre::FrameListener* PlayerFrameListener = m_Player;
	mRoot->addFrameListener(PlayerFrameListener);



}

/// This is the main function.
/// 
/// This runs at compile to instantiate the game application. 
int main(int argc, char** argv)
{


	try
	{
		DoodleAssignment app;
		app.initApp();
		app.getRoot()->startRendering();
		app.closeApp();
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error occurred during execution: " << e.what() << '\n';
		return 1;
	}

	return 0;
}

//! [fullsource]

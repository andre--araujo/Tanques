
#ifndef __MyGame_h_
#define __MyGame_h_

#include <Terrain/OgreTerrain.h>
#include <Terrain/OgreTerrainGroup.h>
#include "BaseApplication.h"
#include <vector>
#include <string>
#include "PhysicsManager.h"
#include "gameObject.h"

#pragma once

using namespace std;

class MyGame : public BaseApplication
{
public:
    Ogre::TerrainGlobalOptions* mTerrainGlobals;
    Ogre::TerrainGroup* mTerrainGroup;
	OgreBites::Label* mInfoLabel;
    bool mTerrainsImported;
 
    void defineTerrain(long x, long y);
    void initBlendMaps(Ogre::Terrain* terrain);
    void configureTerrainDefaults(Ogre::Light* light);
	std::vector<int> posY;


    MyGame(void);
    virtual ~MyGame(void);
	virtual bool keyPressed( const OIS::KeyEvent &arg );



    virtual void createScene(void);
	
	virtual void createViewports(void);
	virtual void createFrameListener(void);
    virtual void destroyScene(void);

	void createCamera(void);
	void freeCamera();
	void changeCamera();
	void overCamera();
	Ogre::Camera* mP1Camera;
	Ogre::Camera* mP2Camera;
	Ogre::Camera* m2P1Camera;
	Ogre::Camera* m2P2Camera;
	Ogre::Camera* mOverCamera;
	void updateCameraPosition();
		
	void checkCollision();
	void updateHUD();

	void passTheTurn();
	GameObject* getObjectofTurn(); //retorna um ponteiro para o objeto (tanque) que esta em seu turno

	PhysicsManager physicsManager;
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	std::vector<GameObject *> myObjects;

	private:
    bool processUnbufferedInput(const Ogre::FrameEvent& evt);
	int currentTurn; //	0 = tank1   1 = tank2
	int cannonVelX;
	int cannonVelY;

};

#endif // #ifndef __MyGame_h_

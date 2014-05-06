
#ifndef __MyGame_h_
#define __MyGame_h_

#include <Terrain/OgreTerrain.h>
#include <Terrain/OgreTerrainGroup.h>
#include "BaseApplication.h"
#include <vector>
#include "PhysicsManager.h"
#include "gameObject.h"

#pragma once

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
	void changeCamera();
	Ogre::Camera* mP1Camera;
	void updateCameraPosition();

	void shoot(GameObject * tank);
	void checkProjectileCollision();

	void passTheTurn();
	GameObject* getObjectofTurn(); //retorna um ponteiro para o objeto (tanque) que esta em seu turno

	PhysicsManager physicsManager;
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	std::vector<GameObject *> myObjects;

	private:
    bool processUnbufferedInput(const Ogre::FrameEvent& evt);
	int currentTurn; //	0 = tank1   1 = tank2

};

#endif // #ifndef __MyGame_h_

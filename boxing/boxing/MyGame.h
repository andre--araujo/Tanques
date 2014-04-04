
#ifndef __MyGame_h_
#define __MyGame_h_

#include <Terrain/OgreTerrain.h>
#include <Terrain/OgreTerrainGroup.h>
#include "BaseApplication.h"
#include <Vector>
#include "PhysicsManager.h"

class MyGame : public BaseApplication
{
private:
    Ogre::TerrainGlobalOptions* mTerrainGlobals;
    Ogre::TerrainGroup* mTerrainGroup;
	OgreBites::Label* mInfoLabel;
    bool mTerrainsImported;
 
    void defineTerrain(long x, long y);
    void initBlendMaps(Ogre::Terrain* terrain);
    void configureTerrainDefaults(Ogre::Light* light);
	std::vector<int> posY;

public:
    MyGame(void);
    virtual ~MyGame(void);
	virtual bool keyPressed( const OIS::KeyEvent &arg );


protected:
    virtual void createScene(void);
	virtual void createCamera(void);
	virtual void createViewports(void);
	virtual void createFrameListener(void);
    virtual void destroyScene(void);


	PhysicsManager physicsManager;
    bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	void newSphere();
};

#endif // #ifndef __MyGame_h_

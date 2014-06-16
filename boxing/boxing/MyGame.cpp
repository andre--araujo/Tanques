#include "btBulletDynamicsCommon.h"
#include <vector>
#include <String>
#include "MyGame.h"
#include "gameObject.h"
#include "BulletCollision\CollisionShapes\btHeightfieldTerrainShape.h"

static bool flag = false;
//-------------------------------------------------------------------------------------
MyGame::MyGame(void)
{

}

//-------------------------------------------------------------------------------------
MyGame::~MyGame(void)
{
}

//-------------------------------------------------------------------------------------

void getTerrainImage(bool flipX, bool flipY, Ogre::Image& img)
{
  img.load("ground.png", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    if (flipX)
        img.flipAroundY();
    if (flipY)
        img.flipAroundX();
}

void MyGame::defineTerrain(long x, long y)
{
 Ogre::String filename = mTerrainGroup->generateFilename(x, y);
    if (Ogre::ResourceGroupManager::getSingleton().resourceExists(mTerrainGroup->getResourceGroup(), filename))
    {
        mTerrainGroup->defineTerrain(x, y);
    }
    else
    {
        Ogre::Image img;
        getTerrainImage(x % 2 != 0, y % 2 != 0, img);
        mTerrainGroup->defineTerrain(x, y, &img);
        mTerrainsImported = true;
    }
}

void MyGame::initBlendMaps(Ogre::Terrain* terrain)
{
 Ogre::TerrainLayerBlendMap* blendMap0 = terrain->getLayerBlendMap(1);
    Ogre::TerrainLayerBlendMap* blendMap1 = terrain->getLayerBlendMap(2);
    Ogre::Real minHeight0 = 70;
    Ogre::Real fadeDist0 = 40;
    Ogre::Real minHeight1 = 70;
    Ogre::Real fadeDist1 = 15;
    float* pBlend0 = blendMap0->getBlendPointer();
    float* pBlend1 = blendMap1->getBlendPointer();
    for (Ogre::uint16 y = 0; y < terrain->getLayerBlendMapSize(); ++y)
    {
        for (Ogre::uint16 x = 0; x < terrain->getLayerBlendMapSize(); ++x)
        {
            Ogre::Real tx, ty;
 
            blendMap0->convertImageToTerrainSpace(x, y, &tx, &ty);
            Ogre::Real height = terrain->getHeightAtTerrainPosition(tx, ty);
            Ogre::Real val = (height - minHeight0) / fadeDist0;
            val = Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);
            *pBlend0++ = val;
 
            val = (height - minHeight1) / fadeDist1;
            val = Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);
            *pBlend1++ = val;
        }
    }
    blendMap0->dirty();
    blendMap1->dirty();
    blendMap0->update();
    blendMap1->update();
}

bool MyGame::processUnbufferedInput(const Ogre::FrameEvent& evt)
{
	static bool mMouseDown = false;     // If a mouse button is depressed
    static Ogre::Real mToggle = 0.0;    // The time left until next toggle
    static Ogre::Real mRotate = 0.23;   // The rotate constant
    static Ogre::Real mMove = 25;      // The movement constant
	//btVector3 run = btVector3(10,0,0);
   /* bool currMouse = mMouse->getMouseState().buttonDown(OIS::MB_Left);
 
    if (currMouse && ! mMouseDown)
    {
        Ogre::Light* light = mSceneMgr->getLight("pointLight");
        light->setVisible(! light->isVisible());
    }
 
    mMouseDown = currMouse;
 
    mToggle -= evt.timeSinceLastFrame;
 
    if ((mToggle < 0.0f ) && mKeyboard->isKeyDown(OIS::KC_1))
    {
        mToggle  = 0.5;
        Ogre::Light* light = mSceneMgr->getLight("pointLight");
        light->setVisible(! light->isVisible());
    }*/
 
    Ogre::Vector3 transVector = Ogre::Vector3::ZERO;
 
    if (mKeyboard->isKeyDown(OIS::KC_I)) // Forward
    {
		physicsManager.move(myObjects[currentTurn], btVector3(10,0,0));       
    }
    if (mKeyboard->isKeyDown(OIS::KC_K)) // Backward
    {
		physicsManager.move(myObjects[currentTurn], btVector3(-10,0,0));       
    }
    if (mKeyboard->isKeyDown(OIS::KC_J)) // rotaciona pra esquerda
    {        
			physicsManager.rotate(myObjects[currentTurn], btVector3(0,0.4,0));        
        
    }
    if (mKeyboard->isKeyDown(OIS::KC_L)) // rotaciona pra direita
    {        
		physicsManager.rotate(myObjects[currentTurn], btVector3(0,-0.4,0));             
    }
  //  if (mKeyboard->isKeyDown(OIS::KC_U)) // Up
  //  {
		//physicsManager.move(myObjects[currentTurn], btVector3(0,10,0));        
  //  }
  //  if (mKeyboard->isKeyDown(OIS::KC_O)) // Down
  //  {
		//physicsManager.move(myObjects[currentTurn], btVector3(0,-10,0));     
		//
  //  }

    return true;
}

bool MyGame::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    bool ret = BaseApplication::frameRenderingQueued(evt);
	checkCollision();
	Ogre::Vector3 pos;
	if (flag){
		for (int i = 0; i < myObjects.size(); i++){ // pra cada corpo solido que eu criei la na bullet
			physicsManager.fall(myObjects[i],evt); // eu rodo um frame, calculo colisoes e atualizo as posicoes e rotacoes dos nos
		}
	}
	updateCameraPosition();
    if (mTerrainGroup->isDerivedDataUpdateInProgress())
	{
        mTrayMgr->moveWidgetToTray(mInfoLabel, OgreBites::TL_TOP, 0);
        mInfoLabel->show();
        if (mTerrainsImported)
        {
			updateHUD();
           // mInfoLabel->setCaption("Fazendo o terreno, sossega ai...");
			mTrayMgr->destroyAllWidgetsInTray(OgreBites::TL_BOTTOMLEFT); //desliga aquele monte de coisa na tela
			mTrayMgr->destroyAllWidgetsInTray(OgreBites::TL_BOTTOMRIGHT);
        }
        else
        {
			updateHUD();
            //mInfoLabel->setCaption("Atualizando as texturas, sossega ai...");	
			mTrayMgr->destroyAllWidgetsInTray(OgreBites::TL_BOTTOMLEFT); //desliga aquele monte de coisa na tela
			mTrayMgr->destroyAllWidgetsInTray(OgreBites::TL_BOTTOMRIGHT);
        }
    }
    else
    {
        mTrayMgr->destroyAllWidgetsInTray(OgreBites::TL_BOTTOMLEFT); //desliga aquele monte de coisa na tela
		mTrayMgr->destroyAllWidgetsInTray(OgreBites::TL_BOTTOMRIGHT);
        if (mTerrainsImported)
        {
            mTerrainGroup->saveAllTerrains(true);
            mTerrainsImported = false;
        }
    } 
	
	if(!processUnbufferedInput(evt)) return false;

    return ret;
}

void MyGame::configureTerrainDefaults(Ogre::Light* light)
{
  // Configure global
    mTerrainGlobals->setMaxPixelError(8);
    // testing composite map
    mTerrainGlobals->setCompositeMapDistance(3000);
 
    // Important to set these so that the terrain knows what to use for derived (non-realtime) data
    mTerrainGlobals->setLightMapDirection(light->getDerivedDirection());
    mTerrainGlobals->setCompositeMapAmbient(mSceneMgr->getAmbientLight());
    mTerrainGlobals->setCompositeMapDiffuse(light->getDiffuseColour());
 
    // Configure default import settings for if we use imported image
    Ogre::Terrain::ImportData& defaultimp = mTerrainGroup->getDefaultImportSettings();
    defaultimp.terrainSize = 129;
    defaultimp.worldSize = 12000.0f;
    defaultimp.inputScale = 1600;
	defaultimp.inputBias = 0;
    defaultimp.minBatchSize = 33;
    defaultimp.maxBatchSize = 65;
    // textures
    defaultimp.layerList.resize(3);
    defaultimp.layerList[0].worldSize = 100;
    defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_diffusespecular.dds");
    defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_normalheight.dds");
    defaultimp.layerList[1].worldSize = 30;
    defaultimp.layerList[1].textureNames.push_back("grass_green-01_diffusespecular.dds");
    defaultimp.layerList[1].textureNames.push_back("grass_green-01_normalheight.dds");
    defaultimp.layerList[2].worldSize = 200;
    defaultimp.layerList[2].textureNames.push_back("growth_weirdfungus-03_diffusespecular.dds");
    defaultimp.layerList[2].textureNames.push_back("growth_weirdfungus-03_normalheight.dds");
}

void MyGame::createFrameListener(void)
{
    BaseApplication::createFrameListener();
 
    mInfoLabel = mTrayMgr->createLabel(OgreBites::TL_TOP, "TInfo", "", 700);
}

void MyGame::destroyScene(void)
{
    OGRE_DELETE mTerrainGroup;
    OGRE_DELETE mTerrainGlobals;
}

void MyGame::createCamera(void)
{
	mCamera = mSceneMgr->createCamera("globalCamera");
	mCameraMan = new OgreBites::SdkCameraMan(mCamera);

	mCamera->setPosition(Ogre::Vector3(100, 600, 100));
    mCamera->lookAt(Ogre::Vector3(10, 400, 0));

	mCamera->setNearClipDistance(0.1);
    mCamera->setFarClipDistance(50000);

	if (mRoot->getRenderSystem()->getCapabilities()->hasCapability(Ogre::RSC_INFINITE_FAR_PLANE))
    {
        mCamera->setFarClipDistance(0);   // enable infinite far clip distance if we can
    }

	/////////////////////////////////////////////////////////////////////////
	////////////// my new camera ////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////


	mP1Camera = mSceneMgr->createCamera("p1Camera");

	mP1Camera->setNearClipDistance(0.1);
    mP1Camera->setFarClipDistance(50000);

 
    if (mRoot->getRenderSystem()->getCapabilities()->hasCapability(Ogre::RSC_INFINITE_FAR_PLANE))
    {
        mP1Camera->setFarClipDistance(0);   // enable infinite far clip distance if we can
    }

	/////////////////////////////////////////////////////////////////////////
	////////////// player 2 camera //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////


	mP2Camera = mSceneMgr->createCamera("p2Camera");

	mP2Camera->setNearClipDistance(0.1);
    mP2Camera->setFarClipDistance(50000);
 
    if (mRoot->getRenderSystem()->getCapabilities()->hasCapability(Ogre::RSC_INFINITE_FAR_PLANE))
    {
        mP2Camera->setFarClipDistance(0);   // enable infinite far clip distance if we can
    }

	/////////////////////////////////////////////////////////////////////////
	////////////// third-person p1 camera ///////////////////////////////////
	/////////////////////////////////////////////////////////////////////////


	m2P1Camera = mSceneMgr->createCamera("tp1Camera");

	//m2P1Camera->setPosition(Ogre::Vector3(100, 300, 100));

	m2P1Camera->setNearClipDistance(0.1);
    m2P1Camera->setFarClipDistance(50000);
 
    if (mRoot->getRenderSystem()->getCapabilities()->hasCapability(Ogre::RSC_INFINITE_FAR_PLANE))
    {
        m2P1Camera->setFarClipDistance(0);   // enable infinite far clip distance if we can
    }

	/////////////////////////////////////////////////////////////////////////
	////////////// third-person p2 camera ///////////////////////////////////
	/////////////////////////////////////////////////////////////////////////


	m2P2Camera = mSceneMgr->createCamera("tp2Camera");

	//m2P2Camera->setPosition(Ogre::Vector3(100, 300, 100));

	m2P2Camera->setNearClipDistance(0.1);
    m2P2Camera->setFarClipDistance(50000);
 
    if (mRoot->getRenderSystem()->getCapabilities()->hasCapability(Ogre::RSC_INFINITE_FAR_PLANE))
    {
        m2P2Camera->setFarClipDistance(0);   // enable infinite far clip distance if we can
    }

	/////////////////////////////////////////////////////////////////////////
	////////////// over world camera ///////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////


	mOverCamera = mSceneMgr->createCamera("overCamera");

	mOverCamera->setPosition(Ogre::Vector3(1000, 1600, 1000));
    mOverCamera->lookAt(Ogre::Vector3(0,0, 0));

	mOverCamera->setNearClipDistance(0.1);
    mOverCamera->setFarClipDistance(50000);
 
    if (mRoot->getRenderSystem()->getCapabilities()->hasCapability(Ogre::RSC_INFINITE_FAR_PLANE))
    {
		mOverCamera->setFarClipDistance(0);
    }

}
void MyGame::updateCameraPosition(){
	//Ogre::Vector3 objPos = mSceneMgr->getSceneNode("node_tank1")->getPosition();
	Ogre::Vector3 objPos = myObjects[currentTurn]->sceneNode->getPosition();

	btMatrix3x3& boxRot = myObjects[currentTurn]->rigidBody->getWorldTransform().getBasis();
	btVector3 aux = btVector3(100,0,0)* boxRot;
	
	Ogre::Vector3 camPos = objPos + Ogre::Vector3(-30,50,0);
	Ogre::Vector3 cam2Pos = objPos + Ogre::Vector3(-aux.getX(),20,aux.getZ());
	//tank olhando sempre pra frente
	Ogre::Vector3 camTargetPos = objPos + Ogre::Vector3(aux.getX(),aux.getY(),-aux.getZ());

	if (currentTurn == 0){
		mP1Camera->setPosition(camPos);
		mP1Camera->lookAt(camTargetPos);
		m2P1Camera->setPosition(cam2Pos);
		m2P1Camera->lookAt(objPos);
	}

	else if (currentTurn == 1){
		mP2Camera->setPosition(camPos);
		mP2Camera->lookAt(camTargetPos);
		m2P2Camera->setPosition(cam2Pos);
		m2P2Camera->lookAt(objPos);
	}

	mOverCamera->setPosition(Ogre::Vector3(1000, 1600, 1000));
}


void MyGame::updateHUD() // gera a strng q aparece naquela telinha de cima
{
		string turn;
		if (currentTurn == 0) { turn = "Turno: P1";}
		else if (currentTurn == 1) { turn = "Turno: P2";}
		
		std::ostringstream ss;

		ss << turn << "  " << "P1 hp: " << "P2 hp: " << "  " << "canhao X: " << cannonVelX <<  "  "
			<< "canhao Y: " << cannonVelY << "  "<< "forca: " << shotForce  ;

		std::string ret(ss.str());
		mInfoLabel->setCaption(ret);
		
}

void MyGame::checkCollision(){

	btTransform trans;
	trans.setOrigin(btVector3(btScalar(0),  btScalar(-1000), btScalar(0)));
	
	
	GameObject * gameObjA;
	GameObject * gameObjB;
	int numManifolds = physicsManager.mWorld->getDispatcher()->getNumManifolds();
    for (int i=0;i<numManifolds;i++){
        //2
		btPersistentManifold* contactManifold =  physicsManager.mWorld->getDispatcher()->getManifoldByIndexInternal(i);
 
        //3
		int numContacts = contactManifold->getNumContacts();
		if (numContacts > 0){
           
            //5
            const btCollisionObject* collisionObjA = contactManifold->getBody0();
            const btCollisionObject* collisionObjB = contactManifold->getBody1();

			for(int j = 0; j < myObjects.size(); j++ ){
				//collisionObjB->getCollisionShape()->getName();
				if((myObjects[j]->tag=="BULLET")){
					if(collisionObjA==myObjects[j]->rigidBody){//if(collisionObjA->getUserPointer()==myObjects[j]->rigidBody->getUserPointer()){// descobrir se o btCollisionObject está relacinado ao objeto A
						gameObjA = myObjects[j];
						if(((GameObject*)collisionObjB->getUserPointer())->tag=="TANK")//gameObjA->rigidBody->setWorldTransform(trans);//destuir
							myObjects[j]->rigidBody->setWorldTransform(trans);
					}	
					if(collisionObjB==myObjects[j]->rigidBody){					
						gameObjB = myObjects[j];
						if(((GameObject*)collisionObjA->getUserPointer())->tag=="TANK")//gameObjB->rigidBody->setWorldTransform(trans);//destuir
							myObjects[j]->rigidBody->setWorldTransform(trans);
						
					}
				}
			}	
		}
	}
}


void MyGame::freeCamera(){
	
	try{
		if(mCameraMan->getCamera() != mCamera){
			mCameraMan->setCamera(mCamera);
			mWindow->getViewport(0)->setCamera(mCamera);
		}	
	

	}catch(Ogre::Exception e){
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
    std::cerr << "An exception has occured: " <<
            e.getFullDescription().c_str() << std::endl;
#endif
	}

	//if (mCameraMan->getCamera()->getPosition().x == (mSceneMgr->getSceneNode("node_box")->getPosition().x-30))
	//	MessageBox( NULL, , "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
}


void MyGame::changeCamera(){
	
	try{

	if(currentTurn == 0){
		if(mCameraMan->getCamera() != m2P1Camera){
			mCameraMan->setCamera(m2P1Camera);
			mWindow->getViewport(0)->setCamera(m2P1Camera);
		}

	}

	else{
		if(mCameraMan->getCamera() != m2P2Camera){
			mCameraMan->setCamera(m2P2Camera);
			mWindow->getViewport(0)->setCamera(m2P2Camera);
		}

	}
	

	}catch(Ogre::Exception e){
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
    std::cerr << "An exception has occured: " <<
            e.getFullDescription().c_str() << std::endl;
#endif
	}

	//if (mCameraMan->getCamera()->getPosition().x == (mSceneMgr->getSceneNode("node_box")->getPosition().x-30))
	//	MessageBox( NULL, , "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
}

void MyGame::overCamera(){
	
	try{
		if(mCameraMan->getCamera() != mOverCamera){
			mCameraMan->setCamera(mOverCamera);
			mWindow->getViewport(0)->setCamera(mOverCamera);
		}	
	

	}catch(Ogre::Exception e){
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
    std::cerr << "An exception has occured: " <<
            e.getFullDescription().c_str() << std::endl;
#endif
	}

	//if (mCameraMan->getCamera()->getPosition().x == (mSceneMgr->getSceneNode("node_box")->getPosition().x-30))
	//	MessageBox( NULL, , "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
}


void MyGame::createViewports(void)
{
	Ogre::Viewport* vp = mWindow->addViewport(mCamera);
	vp->setBackgroundColour(Ogre::ColourValue(0,0,0)); // cor de fundo = preto
	mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight())); // ajusta o aspect ratio
	mP1Camera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
	mP2Camera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
	m2P1Camera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
	m2P2Camera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
	mOverCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

}

void MyGame::createScene(void)
{   
	

	//tutorial 3
	mSceneMgr->setSkyDome(true, "Examples/CloudySky", 5, 8); // um coubo em volta da camera com textura

	//Ogre::Vector3 lightdir(0.55, -0.3, 0.75);
	Ogre::Vector3 lightdir(0.05, -0.1, 0.05);
    lightdir.normalise();
 
    Ogre::Light* light = mSceneMgr->createLight("tstLight");
    light->setType(Ogre::Light::LT_DIRECTIONAL);
    light->setDirection(lightdir);
    light->setDiffuseColour(Ogre::ColourValue::White);
    light->setSpecularColour(Ogre::ColourValue(0.4, 0.4, 0.4));
 
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.2, 0.2, 0.2));
	mTerrainGlobals = OGRE_NEW Ogre::TerrainGlobalOptions();
	mTerrainGroup = OGRE_NEW Ogre::TerrainGroup(mSceneMgr, Ogre::Terrain::ALIGN_X_Z, 129, 1500.0f);
    mTerrainGroup->setFilenameConvention(Ogre::String("BasicTutorial3Terrain"), Ogre::String("dat"));
    mTerrainGroup->setOrigin(Ogre::Vector3::ZERO);

	configureTerrainDefaults(light);
	for (long x = 0; x <= 0; ++x)
        for (long y = 0; y <= 0; ++y)
            defineTerrain(x, y);
 
    // sync load since we want everything in place when we start
    mTerrainGroup->loadAllTerrains(true);

	if (mTerrainsImported)
    {
        Ogre::TerrainGroup::TerrainIterator ti = mTerrainGroup->getTerrainIterator();
        while(ti.hasMoreElements())
        {
            Ogre::Terrain* t = ti.getNext()->instance;
            initBlendMaps(t);
        }
    }

	mTerrainGroup->freeTemporaryResources();
	
	///create a few basic rigid bodies
	// start with ground plane, 1500, 1500
	Ogre::Terrain * pTerrain=mTerrainGroup->getTerrain(0,0);
	float* terrainHeightData = pTerrain->getHeightData();
	Ogre::Vector3 terrainPosition = pTerrain->getPosition();
	float * pDataConvert= new float[pTerrain->getSize() *pTerrain->getSize()];
	for(int i=0;i<pTerrain->getSize();i++)
		memcpy(
			pDataConvert+pTerrain->getSize() * i, // source
			terrainHeightData + pTerrain->getSize() * (pTerrain->getSize()-i-1), // target
			sizeof(float)*(pTerrain->getSize()) // size
		);

	// TERRAIN PHYSICS BEGIN
	float metersBetweenVertices = pTerrain->getWorldSize()/(pTerrain->getSize()-1); 
	btVector3 localScaling(metersBetweenVertices, 1, metersBetweenVertices);
	btHeightfieldTerrainShape* groundShape = new btHeightfieldTerrainShape(
		 pTerrain->getSize(),
		 pTerrain->getSize(),
		 pDataConvert,
		 1/*ignore*/,
		 pTerrain->getMinHeight(),
		 pTerrain->getMaxHeight(),
		 1,
		 PHY_FLOAT,
		 true);
	groundShape->setUseDiamondSubdivision(true);
	groundShape->setLocalScaling(localScaling);
	btRigidBody * mGroundBody = new btRigidBody(0, new btDefaultMotionState(), groundShape);
	mGroundBody->getWorldTransform().setOrigin(
		 btVector3( terrainPosition.x, 
		 terrainPosition.y + (pTerrain->getMaxHeight()-pTerrain->getMinHeight())/2,
		 terrainPosition.z));
	mGroundBody->getWorldTransform().setRotation(
		btQuaternion(Ogre::Quaternion::IDENTITY.x,
		 Ogre::Quaternion::IDENTITY.y,
		 Ogre::Quaternion::IDENTITY.z,
		 Ogre::Quaternion::IDENTITY.w));
	physicsManager.mWorld->addRigidBody(mGroundBody);
	physicsManager.mCollisionShapes.push_back(groundShape);
	mGroundBody->setUserPointer(this);
	//TERRAIN PHYSICS END



	GameObject * tank1 = new GameObject("tank1","cube.mesh","node_tank1",mSceneMgr->getRootSceneNode(),Ogre::Vector3(310,420,220),
										10,mSceneMgr,physicsManager.mWorld, new btVector3(310,420,220),40,10,30);
	tank1->turn = 1;
	myObjects.push_back(tank1);


	GameObject * tank2 = new GameObject(1, "tank2","tanque.mesh","node_tank2",mSceneMgr->getRootSceneNode(),Ogre::Vector3(-470,420,-220),
										40,mSceneMgr,physicsManager.mWorld, new btVector3(-470,430,-220),40,10,30);
	
	//GameObject * tank2 = new GameObject("tank2","cube.mesh","node_tank2",mSceneMgr->getRootSceneNode(),Ogre::Vector3(-13,550,13),
	//									40,mSceneMgr,physicsManager.mWorld, new btVector3(-13,550,13),20,15,60);

	myObjects.push_back(tank2);
	GameObject * proj = new GameObject( "proj",
										"sphere.mesh",
										"node_proj",
										mSceneMgr->getRootSceneNode(), 
										Ogre::Vector3(-200,450,300),
										1,
										mSceneMgr,
										physicsManager.mWorld, 
										new btVector3(-200,450,300),
										5);
	myObjects.push_back(proj);

	tank1->tag="TANK";
	tank2->tag="TANK";
	proj->tag="BULLET";

	tank2->turn = -1;
	currentTurn = 0;	 // turno inicial
	cannonVelX = 100; // velocidade de disparos iniciais
	cannonVelY = 100;
	shotForce = 1;

	stateAI = "Fire";
}

void MyGame::runAI()
{
	if(currentTurn == 1)
	{
		if (stateAI == "Fire")
		{
		//inserir código que mira no jogador
			physicsManager.shootTheProjectil(myObjects[currentTurn],myObjects[2], btVector3(cannonVelX*shotForce,cannonVelY*shotForce,0)); 
			passTheTurn();
		}

		// Já que a função checkcollision verifica quando o projétil acertou um tanque, basta setar o stateAI nesse caso para "Hit"

		else // if(stateAI == "Hit")
		{
		//inserir código que move o tanque numa direção aleatória
				stateAI = "Fire";
		//inserir código que mira no jogador
			physicsManager.shootTheProjectil(myObjects[currentTurn],myObjects[2], btVector3(cannonVelX*shotForce,cannonVelY*shotForce,0));
			passTheTurn();
		}
	}
}

void MyGame::passTheTurn()
{
	if (currentTurn == 0)
	{
		currentTurn = 1;
		return;
	}
	else if(currentTurn == 1)
	{
		currentTurn = 0;
		return;
	}
	
}

GameObject * MyGame::getObjectofTurn()
{
	return myObjects[currentTurn];
}

void MyGame::setupSound()
	{
		soundMgr = SoundManager::createManager();
 
		std::cout << soundMgr->listAvailableDevices();
 
		soundMgr->init();
		soundMgr->setAudioPath( (char*) ".\\" );
 
                // Just for testing
		unsigned int audioId;
		soundMgr->loadAudio( "Explosion.wav", &audioId, true);
                soundMgr->playAudio( audioId, true );
	}


bool MyGame::keyPressed( const OIS::KeyEvent &arg )
{
	

    if (mTrayMgr->isDialogVisible()) return true;   // don't process any more keys if dialog is up

	if (arg.key == OIS::KC_H) //botao troca de turno
	{
			overCamera();
			passTheTurn();		
			updateHUD();
	}

	//cameras
	if (arg.key == OIS::KC_1)freeCamera();
	if (arg.key == OIS::KC_2)changeCamera();
	if (arg.key == OIS::KC_3)overCamera();

	if (arg.key == OIS::KC_U) //aumentar inclinacao do canhao
	{
		if (cannonVelX >= 20)
		{
		cannonVelY+=10;
		cannonVelX-=10;
		updateHUD();
		}
	}

	if (arg.key == OIS::KC_Y)  //diminuir inclinacao do canhao
	{		
		if (cannonVelY >= 20)
		{
			cannonVelY -=10;
			cannonVelX +=10;
			updateHUD();	
		}
	}

	if (arg.key == OIS::KC_P) // aumentar forca de disparo
	{
		shotForce+=0.1;
		updateHUD();
	}

	if (arg.key == OIS::KC_O) // diminuir forca de disparo
	{		
		if (shotForce == 1)
		{
			shotForce = 1;
			updateHUD();			
		}
		else if (shotForce >=1.1 )
		{
			shotForce -= 0.1;
			updateHUD();
		}

	}

	if (arg.key == OIS::KC_X) // atirar
    {

		physicsManager.shootTheProjectil(myObjects[currentTurn],myObjects[2], btVector3(cannonVelX*shotForce,cannonVelY*shotForce,0)); 
	}
	if (arg.key == OIS::KC_V)   // faz a fisica correr
    {
		flag=!flag;
		setupSound();
		
		
    }
    //if (arg.key == OIS::KC_F)   // toggle visibility of advanced frame stats FPS
    //{
    //    mTrayMgr->toggleAdvancedFrameStats();
    //}

    //else if (arg.key == OIS::KC_G)   // toggle visibility of even rarer debugging details
    //{
    //    if (mDetailsPanel->getTrayLocation() == OgreBites::TL_NONE)
    //    {
    //        mTrayMgr->moveWidgetToTray(mDetailsPanel, OgreBites::TL_TOPRIGHT, 0);
    //        mDetailsPanel->show();
    //    }
    //    else
    //    {
    //        mTrayMgr->removeWidgetFromTray(mDetailsPanel);
    //        mDetailsPanel->hide();
    //    }
    //}
   
    else if(arg.key == OIS::KC_F5)   // refresh all textures
    {
        Ogre::TextureManager::getSingleton().reloadAll();
    }
    else if (arg.key == OIS::KC_SYSRQ)   // take a screenshot
    {
        mWindow->writeContentsToTimestampedFile("screenshot", ".jpg");
    }
    else if (arg.key == OIS::KC_ESCAPE)
    {
        mShutDown = true;
    }

    mCameraMan->injectKeyDown(arg);
    return true;
}


#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
    int main(int argc, char *argv[])
#endif
    {
        // Create application object
        MyGame app;

        try {
            app.go();
        } catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occured: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }

        return 0;
    }

#ifdef __cplusplus
}
#endif

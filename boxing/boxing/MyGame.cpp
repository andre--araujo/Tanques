#include "btBulletDynamicsCommon.h"
#include <vector>
#include "MyGame.h"


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
  img.load("terrain.png", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
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

bool MyGame::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    bool ret = BaseApplication::frameRenderingQueued(evt);
	Ogre::Vector3 pos;
	if (flag){
		physicsManager.mWorld->stepSimulation(1/120.f,10); //(quando tempo dura um "step" , quantos steps a bullet pode demorar pra calcular as coisas)
		for (int i = 0; i < physicsManager.mBodies.size(); i++){ // pra cada corpo solido que eu criei la na bullet eu pego o as varieveis de posicao e coloco no nó
			
			///////////
			btTransform trans;
			physicsManager.mBodies[i]->getMotionState()->getWorldTransform(trans);
				
			//return trans.getOrigin().getY();
			//////////			
			//pos = mSceneMgr->getSceneNode("node_sphere")->getPosition();
			if (i == 0){
			mSceneMgr->getSceneNode("node_sphere")->setPosition( trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ());
			}
			if (i == 1){
			mSceneMgr->getSceneNode("node_sphere2")->setPosition( trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ());
			}

		}
	}


    if (mTerrainGroup->isDerivedDataUpdateInProgress())
	{
        mTrayMgr->moveWidgetToTray(mInfoLabel, OgreBites::TL_TOP, 0);
        mInfoLabel->show();
        if (mTerrainsImported)
        {
            mInfoLabel->setCaption("Fazendo o terreno, sossega ai...");
        }
        else
        {
            mInfoLabel->setCaption("Atualizando as texturas, sossega ai...");
        }
    }
    else
    {
        mTrayMgr->removeWidgetFromTray(mInfoLabel);
        mInfoLabel->hide();
        if (mTerrainsImported)
        {
            mTerrainGroup->saveAllTerrains(true);
            mTerrainsImported = false;
        }
    }
 
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
    defaultimp.terrainSize = 513;
    defaultimp.worldSize = 12000.0f;
    defaultimp.inputScale = 600;
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
 
    mInfoLabel = mTrayMgr->createLabel(OgreBites::TL_TOP, "TInfo", "", 350);
}

void MyGame::destroyScene(void)
{
    OGRE_DELETE mTerrainGroup;
    OGRE_DELETE mTerrainGlobals;
}

void MyGame::createCamera(void)
{
	mCamera = mSceneMgr->createCamera("camera_1");
	mCamera->setPosition(0,10,300);
	mCamera->lookAt(0,0,0);
	mCamera->setNearClipDistance(5); // o quao proximo estou de um objeto pra ele sumir (e nao tampar a tela toda)
	mCameraMan = new OgreBites::SdkCameraMan(mCamera);
}

void MyGame::createViewports(void)
{
	Ogre::Viewport* vp = mWindow->addViewport(mCamera);
	vp->setBackgroundColour(Ogre::ColourValue(0,0,0)); // cor de fundo = preto
	mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight())); // ajusta o aspect ratio
}

void MyGame::createScene(void)
{   
	//tutorial 3
	mSceneMgr->setSkyDome(true, "Examples/CloudySky", 5, 8); // um coubo em volta da camera com textura

	mCamera->setPosition(Ogre::Vector3(100, 400, 100));
    mCamera->lookAt(Ogre::Vector3(10, 400, 0));
    mCamera->setNearClipDistance(0.1);
    mCamera->setFarClipDistance(50000);
 
    if (mRoot->getRenderSystem()->getCapabilities()->hasCapability(Ogre::RSC_INFINITE_FAR_PLANE))
    {
        mCamera->setFarClipDistance(0);   // enable infinite far clip distance if we can
    }

	Ogre::Vector3 lightdir(0.55, -0.3, 0.75);
    lightdir.normalise();
 
    Ogre::Light* light = mSceneMgr->createLight("tstLight");
    light->setType(Ogre::Light::LT_DIRECTIONAL);
    light->setDirection(lightdir);
    light->setDiffuseColour(Ogre::ColourValue::White);
    light->setSpecularColour(Ogre::ColourValue(0.4, 0.4, 0.4));
 
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.2, 0.2, 0.2));
	mTerrainGlobals = OGRE_NEW Ogre::TerrainGlobalOptions();
	mTerrainGroup = OGRE_NEW Ogre::TerrainGroup(mSceneMgr, Ogre::Terrain::ALIGN_X_Z, 513, 12000.0f);
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

	//ate o tutorial 2
//	//== faz o chao =================================================
//	Ogre::Plane plane(Ogre::Vector3::UNIT_Y, -10);
//	Ogre::MeshManager::getSingleton().createPlane("plane", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane,
//1500,1500,20,20,true,1,5,5,Ogre::Vector3::UNIT_Z);// até aqui é como se eu criasse a .mesh do plano
//	Ogre::Entity* floor = mSceneMgr->createEntity("floor", "plane");
//	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(floor);
//	floor->setMaterialName("Examples/BeachStones"); //põe uma textura no plano/chão
//
/*	//== coloca um boneco na cena  ==================================
	Ogre::Entity* someGuy = mSceneMgr->createEntity("someGuy","Sinbad.mesh");
	Ogre::SceneNode* node_someGuy = mSceneMgr->createSceneNode("node_someGuy"); //criei um nó para o someGuy
	mSceneMgr->getRootSceneNode()->addChild(node_someGuy);// colocoquei o nó do someGuy no nó raiz
	node_someGuy->attachObject(someGuy); //coloquei o someGuy (entidade) no nó correspondente
	node_someGuy->setPosition(10,450,0);
	node_someGuy->scale(8,8,8);
*/
	
	Ogre::Entity *sphere = mSceneMgr->createEntity("sphere", "sphere.mesh");//Ogre::SceneManager::PT_SPHERE);//"Sinbad.mesh");
	Ogre::SceneNode* node_sphere = mSceneMgr->createSceneNode("node_sphere"); //criei um nó para o someGuy
	
	mSceneMgr->getRootSceneNode()->addChild(node_sphere);// colocoquei o nó do someGuy no nó raiz
	node_sphere->attachObject(sphere); //coloquei o someGuy (entidade) no nó correspondente
	node_sphere->setPosition(-10,450,0);
	node_sphere->scale(0.10,0.10,0.10);	 // sphere.mesh tem raio 100 por padrao, regular o raio pela escala!
	
	physicsManager.createGround();
	physicsManager.createSphere();


	//andre

	Ogre::Entity *sphere2 = mSceneMgr->createEntity("sphere2", "sphere.mesh");//Ogre::SceneManager::PT_SPHERE);//"Sinbad.mesh");
	Ogre::SceneNode* node_sphere2 = mSceneMgr->createSceneNode("node_sphere2"); //criei um nó para o someGuy
	
	mSceneMgr->getRootSceneNode()->addChild(node_sphere2);// colocoquei o nó do someGuy no nó raiz
	node_sphere2->attachObject(sphere2); //coloquei o someGuy (entidade) no nó correspondente
	node_sphere2->setPosition(-18,550,0);
	node_sphere2->scale(0.15,0.15,0.15);	
	physicsManager.createSphere2();

	
//	Ogre::SceneNode* node1 = mSceneMgr->getRootSceneNode()->createChildSceneNode(name);
     
//    btRigidBody &body = mPhysics.createBody(btTransform(btQuaternion::getIdentity(), btVector3(pos.x, pos.y, pos.z)), mass, shape);
     
//    mObjects.push_back(new SceneObject(*node1, body));
     
//    Ogre::Entity *entity = mSceneMgr->createEntity(name, "Prefab_Cube");
//    node1->attachObject(entity);
 
//    node1->setScale(size.x / 100.0f, size.y / 100.0f, size.z / 100.0f);
	
//	//== outro boneco na cena  =======================================
//	Ogre::Entity* anotherGuy = mSceneMgr->createEntity("anotherGuy","ninja.mesh");
//	Ogre::SceneNode* node_anotherGuy = mSceneMgr->createSceneNode("node_anotherGuy"); //criei um nó para o anotherGuy
//	mSceneMgr->getRootSceneNode()->addChild(node_anotherGuy);// colocoquei o nó do anotherGuy no nó raiz
//	node_anotherGuy->attachObject(anotherGuy); //coloquei o anotherGuy (entidade) no nó correspondente
//	node_anotherGuy->setPosition(10,-10,20);
//	node_anotherGuy->scale(0.15,0.15,0.15);
//	
//	//== luz =========================================================
//	Ogre::SceneNode* node_light = mSceneMgr->createSceneNode("node_light");
//	mSceneMgr->getRootSceneNode()->addChild(node_light);
//	Ogre::Light* light1 = mSceneMgr->createLight("light1");
//	light1->setType(Ogre::Light::LT_DIRECTIONAL);//
//	//light1->setPosition(0,20,0);
//    light1->setDiffuseColour(1.0f,1.0f,1.0f);// cor em RGB de 0 a 1
//	light1->setDirection(Ogre::Vector3(1,-1,0));
//	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.7, 0.7, 0.7)); // alterea aqui pra clarear ou escurecer a tela
//	mSceneMgr->setShadowTechnique(Ogre:: SHADOWTYPE_STENCIL_ADDITIVE);// ativa sombra dos 3d no plano
	//btBroadphaseInterface* broadphase = new btDbvtBroadphase();
}

void MyGame::newSphere(){
	char * name;
	itoa(physicsManager.createSphere(),name,10);
	
	Ogre::Entity *sphere = mSceneMgr->createEntity(strcat("sphere",name), "Sinbad.mesh");
	Ogre::SceneNode* node_sphere = mSceneMgr->createSceneNode(strcat("node_sphere",name)); //criei um nó para o someGuy
	
	mSceneMgr->getRootSceneNode()->addChild(node_sphere);// colocoquei o nó do someGuy no nó raiz
	node_sphere->attachObject(sphere); //coloquei o someGuy (entidade) no nó correspondente
	node_sphere->setPosition(-10,450,0);
	node_sphere->scale(5,5,5);	
	

}
bool MyGame::keyPressed( const OIS::KeyEvent &arg )
{
    if (mTrayMgr->isDialogVisible()) return true;   // don't process any more keys if dialog is up
	if (arg.key == OIS::KC_X)   // trava o jogo todo
    {
		
		newSphere();
		
    }
	if (arg.key == OIS::KC_V)   // dispara a fisica?
    {
		flag=!flag;

		
    }
    if (arg.key == OIS::KC_F)   // toggle visibility of advanced frame stats
    {
        mTrayMgr->toggleAdvancedFrameStats();
    }
    else if (arg.key == OIS::KC_G)   // toggle visibility of even rarer debugging details
    {
        if (mDetailsPanel->getTrayLocation() == OgreBites::TL_NONE)
        {
            mTrayMgr->moveWidgetToTray(mDetailsPanel, OgreBites::TL_TOPRIGHT, 0);
            mDetailsPanel->show();
        }
        else
        {
            mTrayMgr->removeWidgetFromTray(mDetailsPanel);
            mDetailsPanel->hide();
        }
    }
    else if (arg.key == OIS::KC_T)   // cycle polygon rendering mode
    {
        Ogre::String newVal;
        Ogre::TextureFilterOptions tfo;
        unsigned int aniso;

        switch (mDetailsPanel->getParamValue(9).asUTF8()[0])
        {
        case 'B':
            newVal = "Trilinear";
            tfo = Ogre::TFO_TRILINEAR;
            aniso = 1;
            break;
        case 'T':
            newVal = "Anisotropic";
            tfo = Ogre::TFO_ANISOTROPIC;
            aniso = 8;
            break;
        case 'A':
            newVal = "None";
            tfo = Ogre::TFO_NONE;
            aniso = 1;
            break;
        default:
            newVal = "Bilinear";
            tfo = Ogre::TFO_BILINEAR;
            aniso = 1;
        }

        Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(tfo);
        Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(aniso);
        mDetailsPanel->setParamValue(9, newVal);
    }
    else if (arg.key == OIS::KC_R)   // cycle polygon rendering mode
    {
        Ogre::String newVal;
        Ogre::PolygonMode pm;

        switch (mCamera->getPolygonMode())
        {
        case Ogre::PM_SOLID:
            newVal = "Wireframe";
            pm = Ogre::PM_WIREFRAME;
            break;
        case Ogre::PM_WIREFRAME:
            newVal = "Points";
            pm = Ogre::PM_POINTS;
            break;
        default:
            newVal = "Solid";
            pm = Ogre::PM_SOLID;
        }

        mCamera->setPolygonMode(pm);
        mDetailsPanel->setParamValue(10, newVal);
    }
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

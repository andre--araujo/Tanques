
#include "MyGame.h"

//-------------------------------------------------------------------------------------
MyGame::MyGame(void)
{
}
//-------------------------------------------------------------------------------------
MyGame::~MyGame(void)
{
}

//-------------------------------------------------------------------------------------
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
	//== faz o chao =================================================
	Ogre::Plane plane(Ogre::Vector3::UNIT_Y, -10);
	Ogre::MeshManager::getSingleton().createPlane("plane", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane,
1500,1500,20,20,true,1,5,5,Ogre::Vector3::UNIT_Z);// até aqui é como se eu criasse a .mesh do plano
	Ogre::Entity* floor = mSceneMgr->createEntity("floor", "plane");
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(floor);
	floor->setMaterialName("Examples/BeachStones"); //põe uma textura no plano/chão

	//== coloca um boneco na cena  ==================================
	Ogre::Entity* someGuy = mSceneMgr->createEntity("someGuy","Sinbad.mesh");
	Ogre::SceneNode* node_someGuy = mSceneMgr->createSceneNode("node_someGuy"); //criei um nó para o someGuy
	mSceneMgr->getRootSceneNode()->addChild(node_someGuy);// colocoquei o nó do someGuy no nó raiz
	node_someGuy->attachObject(someGuy); //coloquei o someGuy (entidade) no nó correspondente
	//node_someGuy->setPosition(10,0,0);
	node_someGuy->scale(2,2,2);

	//== outro boneco na cena  =======================================
	Ogre::Entity* anotherGuy = mSceneMgr->createEntity("anotherGuy","ninja.mesh");
	Ogre::SceneNode* node_anotherGuy = mSceneMgr->createSceneNode("node_anotherGuy"); //criei um nó para o anotherGuy
	mSceneMgr->getRootSceneNode()->addChild(node_anotherGuy);// colocoquei o nó do anotherGuy no nó raiz
	node_anotherGuy->attachObject(anotherGuy); //coloquei o anotherGuy (entidade) no nó correspondente
	node_anotherGuy->setPosition(10,-10,20);
	node_anotherGuy->scale(0.15,0.15,0.15);
	
	//== luz =========================================================
	Ogre::SceneNode* node_light = mSceneMgr->createSceneNode("node_light");
	mSceneMgr->getRootSceneNode()->addChild(node_light);
	Ogre::Light* light1 = mSceneMgr->createLight("light1");
	light1->setType(Ogre::Light::LT_DIRECTIONAL);//
	//light1->setPosition(0,20,0);
    light1->setDiffuseColour(1.0f,1.0f,1.0f);// cor em RGB de 0 a 1
	light1->setDirection(Ogre::Vector3(1,-1,0));
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.7, 0.7, 0.7)); // alterea aqui pra clarear ou escurecer a tela
	mSceneMgr->setShadowTechnique(Ogre:: SHADOWTYPE_STENCIL_ADDITIVE);// ativa sombra dos 3d no plano
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

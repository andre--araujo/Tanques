#include "GameObject.h"

GameObject::GameObject(	Ogre::String entityName, 
						char * mesh, 
						Ogre::String nodeName, 
						Ogre::SceneNode* parentNode, 
						Ogre::Vector3 relativePosition,//posicao do no da mesh
						int radius,
						btScalar mass, 
						Ogre::SceneManager * sceneMgr,
						btDynamicsWorld * dWld,
						btVector3 * iPos) //posicao da figura fisica

{
	sceneManager = sceneMgr;
	dWorld = dWld;
	initGraphics(	entityName, 
					mesh, 
					nodeName, 
					parentNode, 
					relativePosition,
					radius
					);
	
	initPhysics(mass,iPos,radius);
}

void GameObject::initGraphics(	Ogre::String entityName, 
								char* mesh,
								Ogre::String nodeName, 
								Ogre::SceneNode* parentNode, 
								Ogre::Vector3 relativePosition,
								float radius
								)
{
	//Ogre::Entity *sphere = mSceneMgr->createEntity("sphere", "sphere.mesh");//Sinbad
	sceneEntity = sceneManager->createEntity(entityName, mesh);//createEntity(entityName, "sinbad.mesh");//criando entidade no gerenciador de cena
	sceneNode = sceneManager->createSceneNode(nodeName); //criando node no gerenciador de cena
	parentNode->addChild(sceneNode); // adiciona o nó na arvore de cena como filho de parent node
	sceneNode->attachObject(sceneEntity); // associa nó à entidade
	sceneNode->setPosition(relativePosition); // define a posicao do nó em relação a seu parentNode
	sceneNode->scale(radius/100, radius/100, radius/100);
	
}


void GameObject::initPhysics(btScalar mass, btVector3 * iPos, int radius)
{

	collisionShape = new btSphereShape(radius); // cria uma esfera de colisao

	motionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),*iPos));
	bodyMass = mass;
	btVector3 bodyInertia = btVector3(0,0,0);

	collisionShape->calculateLocalInertia(mass,bodyInertia);
	btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass,motionState,collisionShape,bodyInertia);
	rigidBody = new btRigidBody(rigidBodyCI);
	dWorld->addRigidBody(rigidBody);
}









GameObject::~GameObject(void)
{


}
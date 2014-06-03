#include <btBulletDynamicsCommon.h>
#include <vector>
#include <Ogre.h>
#include <string>
#pragma once

class GameObject
{
	public:
	int turn; //uso pra saber quem eu pego na hora de trocar o turno
	Ogre::Entity * sceneEntity;
	Ogre::SceneNode* sceneNode;
	btCollisionShape* collisionShape;
	btRigidBody* rigidBody;
	btDefaultMotionState* motionState;
	btScalar bodyMass;
	
	Ogre::SceneManager * sceneManager;
	btDynamicsWorld * dWorld; 
	
	std::string tag;

	void setVelocity(GameObject * parentObj, btVector3& velocity);

	void initPhysics(btScalar mass, btVector3 * iPos, int radius);
	//void initPhysics(btScalar mass, btVector3 & iPos, btVector3 & collisionBoxDim);
	//void initPhysics(btScalar mass, btVector3 & iPos, btCollisionShape & collisionShape);

	//void initGraphics(	Ogre::String entityName, Ogre::Mesh & mesh, Ogre::String nodeName, 
	//					Ogre::SceneNode& parentNode, Ogre::Vector3 relativePosition);

	/*GameObject(	Ogre::String entityName, Ogre::Mesh & mesh, Ogre::String nodeName, Ogre::SceneNode& parentNode, 
				Ogre::Vector3 relativePosition, int collisionSphereRadius, btScalar mass, 
				const Ogre::SceneManager * sceneMgr, const btDynamicsWorld * dWld, btVector3 & iPos);
*/
	GameObject(	Ogre::String entityName, char* mesh, Ogre::String nodeName, Ogre::SceneNode* parentNode, 
				Ogre::Vector3 relativePosition, btScalar mass, 
				Ogre::SceneManager * sceneMgr, btDynamicsWorld * dWld, btVector3 * iPos, int radius);

	GameObject(	Ogre::String entityName, char* mesh, Ogre::String nodeName, Ogre::SceneNode* parentNode, 
				Ogre::Vector3 relativePosition, btScalar mass, 
				Ogre::SceneManager * sceneMgr, btDynamicsWorld * dWld, btVector3 * iPos, float x, float y, float z);

	//pro tanque
	GameObject(	int a, Ogre::String entityName, char* mesh, Ogre::String nodeName, Ogre::SceneNode* parentNode, 
				Ogre::Vector3 relativePosition, btScalar mass, 
				Ogre::SceneManager * sceneMgr, btDynamicsWorld * dWld, btVector3 * iPos, float x, float y, float z);
	
	void initGraphics(	Ogre::String entityName, 
								char* mesh,
								Ogre::String nodeName, 
								Ogre::SceneNode* parentNode, 
								Ogre::Vector3 relativePosition,
								float radius
								);

	void startPhysicsSim();
	~GameObject(void);

	void initGraphicsBox(	Ogre::String entityName, 
								char* mesh,
								Ogre::String nodeName, 
								Ogre::SceneNode* parentNode, 
								Ogre::Vector3 relativePosition,
								float x, float y, float z
								);
	void initPhysicsBox(btScalar mass, btVector3 * iPos, float x, float y, float z);
	
	//tanque
	void initGraphicsTank(	Ogre::String entityName, 
								char* mesh,
								Ogre::String nodeName, 
								Ogre::SceneNode* parentNode, 
								Ogre::Vector3 relativePosition,
								float x, float y, float z
								);
	void initPhysicsTank(btScalar mass, btVector3 * iPos, float x, float y, float z);

};


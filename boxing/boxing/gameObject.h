#include <btBulletDynamicsCommon.h>
#include <vector>
#include <Ogre.h>
#pragma once

class GameObject
{
	public:
	Ogre::Entity * sceneEntity;
	Ogre::SceneNode* sceneNode;
	btCollisionShape* collisionShape;
	btRigidBody* rigidBody;
	btDefaultMotionState* motionState;
	btScalar bodyMass;
	
	Ogre::SceneManager * sceneManager;
	btDynamicsWorld * dWorld; 


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
				Ogre::Vector3 relativePosition, int radius, btScalar mass, 
				Ogre::SceneManager * sceneMgr, btDynamicsWorld * dWld, btVector3 * iPos);
	
	void initGraphics(	Ogre::String entityName, 
								char* mesh,
								Ogre::String nodeName, 
								Ogre::SceneNode* parentNode, 
								Ogre::Vector3 relativePosition,
								float radius
								);

	void startPhysicsSim();
	~GameObject(void);

};


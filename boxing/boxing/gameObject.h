#include <btBulletDynamicsCommon.h>
#include <vector>
#include <Ogre.h>
#pragma once

class gameObject
{
	Ogre::Entity * entity;
	Ogre::SceneNode* sceneNode;
	btCollisionShape* cShape;
	btRigidBody* rBody;
	btDefaultMotionState* mState;

public:
	void initPhysics();
	void initGraphics();
	gameObject(void);
	~gameObject(void);
};


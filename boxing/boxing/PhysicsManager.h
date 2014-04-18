#pragma once
#include <btBulletDynamicsCommon.h>
#include <vector>
#include <Ogre.h>
#include "gameObject.h"

class PhysicsManager
{
    public:
        btDiscreteDynamicsWorld *mWorld;
        btCollisionDispatcher *mCollisionDispatcher;
 
        btDefaultCollisionConfiguration *mCollisionConfig;        
        btDbvtBroadphase *mBroadphase;
        btSequentialImpulseConstraintSolver *mConstraintSolver;

		

   
        PhysicsManager(); 
        ~PhysicsManager();
		btCollisionShape &createBoxShape(float x, float y, float z);
		btRigidBody &createBody(const btTransform &transform, float mass, btCollisionShape &shape);
		void update(float ticks=1.f);
		int createSphere();
		void createGround();
		void PhysicsManager::fall(GameObject *& obj, const Ogre::FrameEvent & evt);
		void PhysicsManager::move(GameObject*& obj, btVector3& velocity);
		std::vector<btCollisionShape*> mCollisionShapes;
		//std::vector<btRigidBody*> mBodies;
};
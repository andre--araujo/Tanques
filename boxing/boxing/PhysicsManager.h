#pragma once
#include <btBulletDynamicsCommon.h>
#include <vector>
#include <Ogre.h>

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
		btVector3 PhysicsManager::fall(int i, const Ogre::FrameEvent & evt);
		std::vector<btCollisionShape*> mCollisionShapes;
		std::vector<btRigidBody*> mBodies;
};
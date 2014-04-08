#pragma once
#include <btBulletDynamicsCommon.h>
#include <vector>

class PhysicsManager
{
	public:
   // private:
        btDiscreteDynamicsWorld *mWorld;
        btCollisionDispatcher *mCollisionDispatcher;
 
        btDefaultCollisionConfiguration *mCollisionConfig;        
        btDbvtBroadphase *mBroadphase;
        btSequentialImpulseConstraintSolver *mConstraintSolver;

		

   // public:
        PhysicsManager(); 
        ~PhysicsManager();
		btCollisionShape &createBoxShape(float x, float y, float z);
		btRigidBody &createBody(const btTransform &transform, float mass, btCollisionShape &shape);
		void update(float ticks=1.f);
		int createSphere();
		int createSphere2();
		void createGround();
		float fall(int i);
		std::vector<btCollisionShape*> mCollisionShapes;
		std::vector<btRigidBody*> mBodies;
};
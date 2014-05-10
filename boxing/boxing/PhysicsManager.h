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

		void PhysicsManager::shootTheProjectil(GameObject*& obj,  GameObject*& projectil,btVector3& velocity); //dispara o projetil a partir das coordenadas do tank
		void PhysicsManager::fall(GameObject *& obj, const Ogre::FrameEvent & evt);
		void PhysicsManager::move(GameObject*& obj, btVector3& velocity); // move o tank para frente/traz de acordo pra onde esta apontado
		void PhysicsManager::rotate(GameObject*& obj, btVector3& velocity); // gira o tank pra esquerda/direita
		std::vector<btCollisionShape*> mCollisionShapes;
		//std::vector<btRigidBody*> mBodies;
};
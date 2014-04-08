#include "PhysicsManager.h"
#include <Ogre.h>

PhysicsManager::PhysicsManager(void)
{
	
        mBroadphase = new btDbvtBroadphase();

        mCollisionConfig = new btDefaultCollisionConfiguration();
        mCollisionDispatcher = new btCollisionDispatcher(mCollisionConfig);

        mConstraintSolver = new btSequentialImpulseConstraintSolver;

        mWorld = new btDiscreteDynamicsWorld(mCollisionDispatcher,mBroadphase, mConstraintSolver,mCollisionConfig);

        mWorld->setGravity(btVector3(0,-100,0));

}
void PhysicsManager::createGround(){
	
		btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0,1,0),250);
        btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,-1,0)));
        btRigidBody::btRigidBodyConstructionInfo
                groundRigidBodyCI(0,groundMotionState,groundShape,btVector3(0,0,0));
        btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
        mWorld->addRigidBody(groundRigidBody);

}
int PhysicsManager::createSphere(){
		btCollisionShape* fallShape = new btSphereShape(28);
        btDefaultMotionState* fallMotionState =
                new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(-10,450,0)));
        btScalar mass = 10;
        btVector3 fallInertia(0,0,0);
        fallShape->calculateLocalInertia(mass,fallInertia);
        btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass,fallMotionState,fallShape,fallInertia);
        btRigidBody* fallRigidBody = new btRigidBody(fallRigidBodyCI);
        mWorld->addRigidBody(fallRigidBody);
		mCollisionShapes.push_back(fallShape);
		mBodies.push_back(fallRigidBody);
		return mBodies.size()-1;
}
btVector3 PhysicsManager::fall(int i, const Ogre::FrameEvent &evt){
    //mWorld->stepSimulation(1/200.f,10);

	mWorld->stepSimulation(evt.timeSinceLastFrame);

    btTransform trans;
	mBodies[i]->getMotionState()->getWorldTransform(trans);
	
	btVector3 pos = trans.getOrigin();
	return pos;

}
PhysicsManager::~PhysicsManager()
{
    for(int i = 0, len = mCollisionShapes.size();i < len; ++i)
        delete mCollisionShapes[i];

	for (int i=mWorld->getNumCollisionObjects()-1; i>=0 ;i--)
	{
		btCollisionObject* obj = mWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		mWorld->removeCollisionObject( obj );
		delete obj;
	}
    delete mWorld;    
    delete mCollisionDispatcher;
}
btCollisionShape & PhysicsManager::createBoxShape(float x, float y, float z)
{
    btCollisionShape *shape = new btBoxShape(btVector3(x / 2, y / 2, z / 2));
    mCollisionShapes.push_back(shape);
 
    return *shape;
}

btRigidBody & PhysicsManager::createBody(const btTransform &transform, float mass, btCollisionShape &shape)
{
    bool isDynamic = (mass != 0.0f);
 
    btVector3 localInertia(0,0,0);
 
    if (isDynamic)
        shape.calculateLocalInertia(mass,localInertia);
 
    btDefaultMotionState* myMotionState = new btDefaultMotionState(transform);
 
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,&shape,localInertia);
    btRigidBody* body = new btRigidBody(rbInfo);
     
    mWorld->addRigidBody(body);
 
    return *body;
}
/*
void PhysicsManager::update(float ticks=1.f)
{
    mWorld->stepSimulation(ticks);
}
*/
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

void PhysicsManager::fall(GameObject*& obj, const Ogre::FrameEvent &evt){
    //mWorld->stepSimulation(1/200.f,10);

	mWorld->stepSimulation(evt.timeSinceLastFrame);

    btTransform trans;
	obj->rigidBody->getMotionState()->getWorldTransform(trans);	
	btVector3 pos = trans.getOrigin();
	btQuaternion rotation(trans.getRotation());
	obj->sceneNode->setPosition(pos.getX(),pos.getY(),pos.getZ());
	obj->sceneNode->setOrientation(rotation.getW(), rotation.getX(), rotation.getY(), rotation.getZ());
}

void PhysicsManager::shootTheProjectil(GameObject*& obj, GameObject*& projectil, btVector3& velocity)
{	
	btVector3 relativeForce = btVector3(0,10,0); //esse bloco pega a posição e rotacao do tank pra ser usado no tiro
	btMatrix3x3& boxRot = obj->rigidBody->getWorldTransform().getBasis();
    btVector3 correctedForce = boxRot * velocity;
	
	btTransform trans;// aplica o q pegou ali de cima no projetil, levantando o y dele
	obj->rigidBody->getMotionState()->getWorldTransform(trans);
    trans.setOrigin(btVector3(btScalar(trans.getOrigin().getX()), btScalar(trans.getOrigin().getY() + 60), btScalar(trans.getOrigin().getZ())));

	projectil->rigidBody->setWorldTransform(trans);
	projectil->rigidBody->setLinearVelocity(correctedForce);
}

void PhysicsManager::move(GameObject*& obj, btVector3& velocity)
{    	
	btVector3 relativeForce = btVector3(0,10,0); //isso aqui tudo é pra converter a direcao da velocidade pra direcao que seu objeto ta virado
	btMatrix3x3& boxRot = obj->rigidBody->getWorldTransform().getBasis();
    btVector3 correctedForce = boxRot * velocity;
   	obj->rigidBody->setLinearVelocity(correctedForce);		
}

void PhysicsManager::rotate(GameObject*& obj, btVector3& velocity)
{	
	obj->rigidBody->setAngularVelocity(velocity);	
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
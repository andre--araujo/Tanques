#include "PhysicsManager.h"


PhysicsManager::PhysicsManager(void)
{
	
        mBroadphase = new btDbvtBroadphase(); //detecta objetos que estao colidindi/prestes a colidir

        mCollisionConfig = new btDefaultCollisionConfiguration();  //configura a dispatcher, dando tamanho de buffer e etc
        mCollisionDispatcher = new btCollisionDispatcher(mCollisionConfig); //cuida de que algoritmos de colisao serao usados

        mConstraintSolver = new btSequentialImpulseConstraintSolver;

        mWorld = new btDiscreteDynamicsWorld(mCollisionDispatcher,mBroadphase, mConstraintSolver,mCollisionConfig); //cria um mundo na bullet

        mWorld->setGravity(btVector3(0,-10,0)); // seta a gravidade desse mundo

}
void PhysicsManager::createGround(){
	
		btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0,1,0),1);
        btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,-1,0)));//(0,-1,0)));
        btRigidBody::btRigidBodyConstructionInfo
                groundRigidBodyCI(0,groundMotionState,groundShape,btVector3(0,0,0));
        btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
        mWorld->addRigidBody(groundRigidBody);

}


float PhysicsManager::fall(int i){  // "rodar!"
    mWorld->stepSimulation(1/60.f,10); //(quando tempo dura um "step" , quantos steps a bullet pode demorar pra calcular as coisas)

    btTransform trans;
	mBodies[i]->getMotionState()->getWorldTransform(trans);
				
	return trans.getOrigin().getY();

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

int PhysicsManager::createSphere(){  //creteBody + createboxShape
		btCollisionShape* fallShape = new btSphereShape(10);
        btDefaultMotionState* fallMotionState =
                new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(-10,450,0)));//(-10,450,0)));
        btScalar mass = 20;
        btVector3 fallInertia(0,0,0);//
        fallShape->calculateLocalInertia(mass,fallInertia);//
        btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass,fallMotionState,fallShape,fallInertia);//
        btRigidBody* fallRigidBody = new btRigidBody(fallRigidBodyCI);//
        mWorld->addRigidBody(fallRigidBody);//
		mCollisionShapes.push_back(fallShape);
		mBodies.push_back(fallRigidBody);  //???
		return mBodies.size()-1;
}

int PhysicsManager::createSphere2(){  //creteBody + createboxShape
		btCollisionShape* fallShape = new btSphereShape(15);
        btDefaultMotionState* fallMotionState =
                new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(-18,550,0)));//(-10,450,0)));
        btScalar mass = 200;
        btVector3 fallInertia(0,0,0);//
        fallShape->calculateLocalInertia(mass,fallInertia);//
        btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass,fallMotionState,fallShape,fallInertia);//
        btRigidBody* fallRigidBody = new btRigidBody(fallRigidBodyCI);//
        mWorld->addRigidBody(fallRigidBody);//
		mCollisionShapes.push_back(fallShape);
		mBodies.push_back(fallRigidBody);  //???
		return mBodies.size()-1;
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
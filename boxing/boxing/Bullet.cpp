#include "btBulletDynamicsCommon.h"



class Bullet
{
	private:

		bool visable;
		int weight;
		int size;
		int damage;
		int type;
		int x;  //posicionamento do n�
		int y;
		int z;
		btBoxShape * box;

		Bullet(){
			btBroadphaseInterface* broadphase = new btDbvtBroadphase();
		}

};
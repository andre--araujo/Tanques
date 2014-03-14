class Tank
{
	private:

		bool alive;
		//string entityName; 
		int color;
		int status;
		int hp;
		int x; //posicionamente do nó
		int y;
		int z;
		int camera_x; //posicionamento do nó da camera
		int camera_y;
		int camera_z;
		int selectedCamera; //pra alternar entre as cameras fixas
				
	public:

		Tank(int Color, int handicap, int X, int Y, int Z) //construtor
		{
			alive = true;
			hp = 100 * handicap;
			setPosition(X, Y, Z);
			setColor(Color);
		}

		void setPosition(int X, int Y, int Z)
		{
			x = X;
			y = Y;
			z = Z;
			//mudar tambem o node
			//usar translate()?
		}

		void setCamera(int X, int Y, int Z)
		{
			camera_x = X;
			camera_y = Y;
			camera_z = Z;
			//mudar tambem o node
		}

		int setDamage(int damageReceived)
		{
			hp = hp - damageReceived;
			//if (hp < 0).. 
		}

		void setColor(int Color)
		{
			color = Color;
			// alterar cor/textura da mesh
		}

		int getHp()
		{
			return hp;
		}

		
		
};
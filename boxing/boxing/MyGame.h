/*
-----------------------------------------------------------------------------
Filename:    MyGame.h
-----------------------------------------------------------------------------

This source file is part of the
   ___                 __    __ _ _    _ 
  /___\__ _ _ __ ___  / / /\ \ (_) | _(_)
 //  // _` | '__/ _ \ \ \/  \/ / | |/ / |
/ \_// (_| | | |  __/  \  /\  /| |   <| |
\___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
      |___/                              
      Tutorial Framework
      http://www.ogre3d.org/tikiwiki/
-----------------------------------------------------------------------------
*/
#ifndef __MyGame_h_
#define __MyGame_h_

#include "BaseApplication.h"

class MyGame : public BaseApplication
{
public:
    MyGame(void);
    virtual ~MyGame(void);

protected:
    virtual void createScene(void);
	//virtual void createCamera(void); //tive que adicionar isso aqui
};

#endif // #ifndef __MyGame_h_

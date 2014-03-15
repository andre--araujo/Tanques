
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
	virtual void createCamera(void);
	virtual void createViewports(void);
};

#endif // #ifndef __MyGame_h_

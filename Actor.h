#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

//forward declaration
class StudentWorld;


// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp


class Actor : public GraphObject	//base object class
{
public:
	Actor(int imageID, int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 0) 
		: GraphObject(imageID, startX, startY, dir, size, depth) 
	{
		setVisible(true);
	};

	virtual ~Actor() {
		setVisible(false);
	};


	virtual void doSomething() = 0;

private:
};

class Ice : public Actor
{
public:
	Ice(int x = 0, int y = 0) : Actor(IID_ICE, x, y, right, 0.25, 3) {};

	~Ice() {
		getGraphObjects(3).erase(this);
		//setVisible(false);
	};

	virtual void doSomething() {};

private:

};

class Iceman : public Actor
{
public:
	Iceman(int x = 30, int y = 60) : Actor(IID_PLAYER, x, y) {};

	~Iceman() {};

	void setWorld(StudentWorld* w) {
		sWorld = w;
	}

	StudentWorld* getWorld() {
		return sWorld;
	}

	virtual void doSomething();

private:
	StudentWorld* sWorld;

};

#endif // ACTOR_H_

#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;


//*********Base class

class Actor : public GraphObject	//base object class
{
public:
	Actor(StudentWorld* world, int imageID, int startX, int startY, Direction dir, double size = 1.0, unsigned int depth = 0) 
		: GraphObject(imageID, startX, startY, dir, size, depth) 
	{
		m_world = world;
		m_alive = true;
		setVisible(true);
	};
	virtual ~Actor() {
		//setVisible(false);
	};

	virtual void doSomething() = 0;

	bool isAlive() const {
		return m_alive;
	}
	void setDead() {
		m_alive = false;
	}

	virtual bool annoy(unsigned int amt) {
		return false;
	}

	StudentWorld* getWorld() const {
		return m_world;
	}


	virtual bool canActorPassThroughMe() const {
		return false;
	};

	virtual bool canDigThroughIce() const {
		return false;
	}


	bool moveToIfPossible(int x, int y);



private:
	StudentWorld* m_world;
	bool m_alive;
};



//*********Obstacle

class Ice : public Actor
{
public:
	Ice(int x = 0, int y = 0) 
		: Actor(nullptr, IID_ICE, x, y, right, 0.25, 3) {};
	~Ice() {
		//getGraphObjects(3).erase(this);
		//setVisible(false);
	};

	virtual void doSomething() {};
};


class Boulder : public Actor
{
public:
	enum State { stable, waiting, falling, dead };

	Boulder(StudentWorld* world, int startX, int startY)
		: Actor(world, IID_BOULDER, startX, startY, down, 1.0, 1) {
		setVisible(true);
		m_state = stable;
	};


	virtual void doSomething();

private:
	State m_state;
	unsigned int waitTime;
};


class Squirt : public Actor
{


};


//********************Object


//********************Agents

class Agent : public Actor
{
public:
	Agent(StudentWorld* world, int imageID, int startX, int startY, Direction startDir, unsigned int hitPoints)
		: Actor(world, imageID, startX, startY, startDir) 
	{
		m_hitPoints = hitPoints;
	};

	virtual void addGold() = 0;

	unsigned int getHitPoints() const {
		return m_hitPoints;
	}

	virtual bool annoy(unsigned int amt) {
		m_hitPoints -= amt;
		if (m_hitPoints == 0)
			setDead();
		return true;
	}
	//virtual bool canPickThingsUp() const;


private:
	unsigned int m_hitPoints;
};


class Iceman : public Agent
{
public:
	Iceman(StudentWorld* world = nullptr, int x = 30, int y = 60) 
		: Agent(world, IID_PLAYER, x, y, right, 100) {};
	~Iceman() {};

	virtual void doSomething();

	virtual bool canDigThroughIce() const {
		return true;
	}

	virtual void addGold() {}



private:

};

#endif // ACTOR_H_

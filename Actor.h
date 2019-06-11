#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <algorithm>

class StudentWorld;


//*********Base class

class Actor : public GraphObject	//base object class
{
public:
	Actor(StudentWorld* world, int imageID, int startX, int startY, 
		Direction dir, double size = 1.0, unsigned int depth = 0) 
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

	virtual bool canActorsPassThroughMe() const {
		return false;
	};

	virtual bool canDigThroughIce() const {
		return false;
	}

	virtual bool canPickThingsUp() const {
		return false;
	}

	virtual bool huntsIceMan() const {
		return true;
	}

	virtual bool isAnnoyable() const {
		return false;
	}

	virtual void addGold() {}
	virtual void addSonar() {}
	virtual void addWater() {}

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
	~Boulder() {}


	virtual void doSomething();

private:
	State m_state;
	unsigned int waitTime;
};


class Squirt : public Actor
{
public:
	Squirt(StudentWorld* world, int startX, int startY, Direction dir)
		: Actor(world, IID_WATER_SPURT, startX, startY, dir, 1.0, 1) {
		m_travelDistance = 4;
		setVisible(false);
	}
	~Squirt() {}

	virtual bool huntsIceMan() const  override {
		return false;
	}

	virtual void doSomething();

private:
	int m_travelDistance;

};


//********************Activating Object
class ActivatingObject : public Actor
{
public:

	ActivatingObject(StudentWorld* world, int imageID, int startX, int startY,
		bool activateOnPlayer, bool isPermanent, bool isVisible)
		: Actor(world, imageID, startX, startY, right, 1.0, 2) {
		m_pickable = activateOnPlayer;
		setVisible(isVisible);
		m_state = isPermanent;
		waitTime = 100;
	}
	

	virtual void doSomething() {};

	virtual bool canActorsPassThroughMe() const {
		return true;
	}

	void setWaitTime(int t) {
		waitTime = t;
	}

	void setTicksToLive() {
		if (waitTime > 0)	
			--waitTime;
		else
			setDead();
	}

	bool isPickableByIceman() const{
		return m_pickable;
	}

	bool isPermanent() const {
		return m_state;
	}


private:
	bool m_pickable;
	bool m_state;
	unsigned int waitTime;
};

class OilBarrel : public ActivatingObject 
{
public:

	OilBarrel(StudentWorld* world, int startX, int startY)
		: ActivatingObject(world, IID_BARREL, startX, startY, true, true, true) {
		}
	~OilBarrel() {}

	virtual void doSomething();
};

class GoldNugget : public ActivatingObject
{
public:
	
	GoldNugget(StudentWorld* world, int startX, int startY, 
		bool pickableByIceman, bool isPermanent, bool isVisible )
		: ActivatingObject(world, IID_GOLD, startX, startY, 
			pickableByIceman, isPermanent, isVisible) {
	}
	~GoldNugget() {}

	virtual void doSomething();


private:
};


class SonarKit : public ActivatingObject
{
public:

	SonarKit(StudentWorld* world, int startX, int startY, int cLevel)
		: ActivatingObject(world, IID_SONAR, startX, startY,
			true, false, true) {
		setWaitTime(std::max(100, 300 - 10*cLevel));
	}
	~SonarKit() {}

	virtual void doSomething();

};


class WaterPool : public ActivatingObject
{
public:

	WaterPool(StudentWorld* world, int startX, int startY, int cLevel)
		: ActivatingObject(world, IID_WATER_POOL, startX, startY, 
			true, false, true) {
		setWaitTime(std::max(100, 300 - 10 * cLevel));
	}
	~WaterPool() {}

	virtual void doSomething();


};


//********************Agents
class Agent : public Actor
{
public:
	Agent(StudentWorld* world, int imageID, int startX, int startY, Direction startDir, unsigned int hitPoints)
		: Actor(world, imageID, startX, startY, startDir) 
	{
		m_hitPoints = hitPoints;
	};

	unsigned int getHitPoints() const {
		return m_hitPoints;
	}

	virtual bool annoy(unsigned int amt) {
		m_hitPoints -= amt;
		if (m_hitPoints <= 0)
			setDead();
		return true;
	}
	
	
	virtual bool canPickThingsUp() const override{
		return true;
	}

	virtual bool isAnnoyable() const override {
		return true;
	}


private:
	unsigned int m_hitPoints;
};


class Iceman : public Agent
{
public:
	Iceman(StudentWorld* world = nullptr, int x = 30, int y = 60) 
		: Agent(world, IID_PLAYER, x, y, right, 100) {
		m_water = 5;
		m_gold = 0;
		m_sonar = 1;
	};
	~Iceman() {};

	virtual void doSomething() override;

	virtual bool canDigThroughIce() const override {
		return true;
	}

	virtual void addGold() override{
		++m_gold;
	}
	void decGold() {
		--m_gold;
	}
	unsigned int getGold() const {
		return m_gold;
	}
	
	virtual void addSonar() override{
		++m_sonar;
	}
	void decSonar() {
		--m_sonar;
	}
	unsigned int getSonar() const {
		return m_sonar;
	}

	virtual void addWater() override {
		m_water += 5;
	}
	void decWater() {
		--m_water;
	}
	unsigned int getWater() const {
		return m_water;
	}


private:
	unsigned int m_water;
	unsigned int m_gold;
	unsigned int m_sonar;
};

#endif // ACTOR_H_

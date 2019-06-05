#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
#include <vector>
#include <algorithm>

#include "Actor.h"

//class Actor;
//class Ice;
//class Iceman;

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
		: GameWorld(assetDir){}

	virtual ~StudentWorld() {}


	void addActor(std::vector<Actor*> actors);

	void clearIce(int x, int y);


	virtual int init()
	{

		//add Ice
		sw_ice = new Ice**[VIEW_WIDTH];
		for (int i = 0; i < VIEW_WIDTH; ++i) {

			sw_ice[i] = new Ice*[VIEW_HEIGHT - 4];
			for (int j = 0; j < VIEW_HEIGHT-4; ++j) {
				if (i >= 30 && i <= 33 && j >= 4 && j <= 59) {
					sw_ice[i][j] = nullptr;
					continue;
				}

				sw_ice[i][j] = new Ice(i, j);
			}
		}


		//add iceman
		sw_iceman = new Iceman(this);

		
		addActor(sw_actors);


		return GWSTATUS_CONTINUE_GAME;
	}

	virtual int move()
	{
		// This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
		// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
		

		sw_iceman->doSomething();
		
		return GWSTATUS_CONTINUE_GAME;


		decLives();
		return GWSTATUS_PLAYER_DIED;
	}

	virtual void cleanUp()
	{
		for (int i = 0; i <= VIEW_WIDTH; ++i) {
			for (int j = 0; j < VIEW_HEIGHT-4; ++j) {
				delete sw_ice[i][j];
				sw_ice[i][j] = nullptr;
			}
			delete[] *sw_ice[i];
		}
		delete[] **sw_ice; //messed up


		delete sw_iceman;
	}


	bool boulderSupport(int x, int y) {
		for (int i = 0; i <= 3; ++i) {
			if (sw_ice[x+i][y] != nullptr)
				return true;
		}
		return false;
	}

	Ice*** getIce() {
		return sw_ice;
	}

private:
	//const int sw_y = 59;
	//tracking Ice and Iceman
	//std::unique_ptr<Ice**> sw_ice;
	Ice*** sw_ice;
	Iceman* sw_iceman;
	std::vector<Actor*> sw_actors;
};

#endif // STUDENTWORLD_H_

#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
#include <vector>

#include "Actor.h"


// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
		: GameWorld(assetDir)
	{
	}

	//~StudentWorld();

	virtual int init()
	{

		sw_iceman = new Iceman;
		sw_iceman->setWorld(this);

		
		sw_ice = new Ice**[sw_x];
		for (int i = 0; i <= sw_x; ++i) {

			sw_ice[i] = new Ice*[sw_y];
			for (int j = 0; j <= sw_y; ++j) {
				if (i >= 30 && i <= 33 && j >= 4 && j <= 59) {
					sw_ice[i][j] = nullptr;
					continue;
				}

				sw_ice[i][j] = new Ice(i, j);
			}
		}


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
		for (int i = 0; i <= sw_x; ++i) {
			for (int j = 0; j <= sw_y; ++j) {
				delete sw_ice[i][j];
				sw_ice[i][j] = nullptr;
			}
			delete[] *sw_ice[i];
		}
		delete[] **sw_ice; //messed up


		delete sw_iceman;
	}

	Ice*** getIce() {
		return sw_ice;
	}

private:
	const int sw_x = 64;
	const int sw_y = 59;
	//tracking Ice and Iceman
	Ice*** sw_ice;
	Iceman* sw_iceman;
};

#endif // STUDENTWORLD_H_
